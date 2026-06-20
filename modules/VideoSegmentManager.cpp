// modules/VideoSegmentManager.cpp
// Purpose: Implementation of VideoSegmentManager double-ended queue operations for segment buffering.

#include "VideoSegmentManager.h"

#include <iostream>
#include <iomanip>
#include <stdexcept>
#include <string>

// Helper to print a uniform visual divider for console segment lists
static void printSMDivider(char c = '-') {
    std::cout << "  " << std::string(66, c) << "\n";
}

// Simulates estimating the storage size of a segment chunk based on video quality and duration
static int estimateSizeKB(const std::string& quality, int durationSec) {
    int bitsPerSec = 2500000; // default 720p
    if      (quality == "1080p") bitsPerSec = 4000000;
    else if (quality == "720p")  bitsPerSec = 2500000;
    else if (quality == "480p")  bitsPerSec = 1000000;
    else if (quality == "360p")  bitsPerSec =  500000;

    return (bitsPerSec / 8 / 1024) * durationSec;
}

// Formats an integer seconds duration to a readable MM:SS string
static std::string toMMSS(int totalSec) {
    int m = totalSec / 60;
    int s = totalSec % 60;
    std::string mm = (m < 10 ? "0" : "") + std::to_string(m);
    std::string ss = (s < 10 ? "0" : "") + std::to_string(s);
    return mm + ":" + ss;
}

// Segment default constructor
Segment::Segment()
    : index(0), startSec(0), endSec(0), quality("720p"), sizeKB(0)
{}

// Segment parameterized constructor
Segment::Segment(int idx, int start, int end,
                 std::string quality, int sizeKB)
    : index(idx), startSec(start), endSec(end),
      quality(quality), sizeKB(sizeKB)
{}

// Returns segment duration
int Segment::getDuration() const {
    return endSec - startSec;
}

// Prints segment metadata record
void Segment::display() const {
    std::cout << "  SEG-" << std::setfill('0') << std::setw(3) << index
              << std::setfill(' ')
              << " | " << toMMSS(startSec) << " \xE2\x86\x92 " << toMMSS(endSec)
              << " | " << std::setw(5) << quality
              << " | " << std::setw(6) << sizeKB << " KB\n";
}

// Default constructor
VideoSegmentManager::VideoSegmentManager()
    : videoId(""), totalDurationSec(0)
{}

// Parameterized constructor mapping video information
VideoSegmentManager::VideoSegmentManager(const std::string& videoId,
                                         int totalDurationSec)
    : videoId(videoId), totalDurationSec(totalDurationSec)
{}

// Partitions the total duration into fixed size chunks and enqueues them
void VideoSegmentManager::segmentVideo(int segmentSizeSec,
                                        const std::string& quality) {
    if (segmentSizeSec <= 0) return;
    segments.clear(); // Flush existing buffer

    int currentTime = 0;
    int idx         = 1;

    while (currentTime < totalDurationSec) {
        int endTime = std::min(currentTime + segmentSizeSec, totalDurationSec);
        int dur     = endTime - currentTime;
        int sz      = estimateSizeKB(quality, dur);

        segments.push_back(Segment(idx, currentTime, endTime, quality, sz));
        currentTime = endTime;
        idx++;
    }
}

// Appends segment to the back of the double-ended queue (simulating live streams)
void VideoSegmentManager::appendSegment(const Segment& seg) {
    segments.push_back(seg);
}

// Prepends segment to the front of the double-ended queue (simulating priority repair chunks)
void VideoSegmentManager::prependSegment(const Segment& seg) {
    segments.push_front(seg);
}

// Pops and returns the front segment (simulating player buffer consumption)
Segment VideoSegmentManager::consumeFront() {
    Segment front = segments.front();
    segments.pop_front();
    return front;
}

// Peeks at the front segment
const Segment& VideoSegmentManager::peekFront() const {
    return segments.front();
}

// Index accessor to simulate player seeking directly to a specific segment chunk in O(1) time
const Segment& VideoSegmentManager::getSegment(int index) const {
    if (index < 0 || index >= static_cast<int>(segments.size()))
        throw std::out_of_range("Segment index out of range");
    return segments[index];
}

int VideoSegmentManager::getTotalSegments() const {
    return static_cast<int>(segments.size());
}

bool VideoSegmentManager::isEmpty() const {
    return segments.empty();
}

// Sums and returns the total duration buffered in the queue
int VideoSegmentManager::getTotalBufferedSec() const {
    int total = 0;
    for (const auto& seg : segments)
        total += seg.getDuration();
    return total;
}

// Clears the buffer
void VideoSegmentManager::clear() {
    segments.clear();
}

// Displays all chunks. If total chunks > 10, displays first 5 and last 3 to keep output readable.
void VideoSegmentManager::displaySegments() const {
    std::cout << "\n";
    printSMDivider('=');
    std::cout << "  \xF0\x9F\x93\xB9  Segments for [" << videoId << "]"
              << "  (" << getTotalSegments() << " segments"
              << " | " << getTotalBufferedSec() << "s total)\n";
    printSMDivider('=');

    if (segments.empty()) {
        std::cout << "  No segments available.\n";
        printSMDivider('=');
        std::cout << "\n";
        return;
    }

    int total = getTotalSegments();
    if (total <= 10) {
        for (const auto& seg : segments)
            seg.display();
    } else {
        for (int i = 0; i < 5; ++i) segments[i].display();
        std::cout << "  ... (" << (total - 8) << " segments) ...\n";
        for (int i = total - 3; i < total; ++i) segments[i].display();
    }

    printSMDivider('=');
    std::cout << "\n";
}

// Prints segment metadata for a specific range of indices
void VideoSegmentManager::displaySegmentRange(int from, int to) const {
    std::cout << "\n";
    printSMDivider();
    std::cout << "  Segments " << from << " to " << to << ":\n";
    printSMDivider();

    for (int i = from - 1; i < to && i < static_cast<int>(segments.size()); ++i)
        segments[i].display();

    printSMDivider();
    std::cout << "\n";
}

// Renders a visual console progress bar indicating current segment index position relative to total segments
void VideoSegmentManager::displayPlaybackStatus(int currentSegIdx) const {
    int total    = getTotalSegments();
    if (total == 0) return;

    int barWidth = 40;
    int filled   = (currentSegIdx * barWidth) / total;

    std::cout << "\n  ▶  Playback Progress:\n  [";
    for (int i = 0; i < barWidth; ++i)
        std::cout << (i < filled ? "\xE2\x96\x88" : "\xE2\x96\x91");

    int pct = (currentSegIdx * 100) / total;
    std::cout << "] " << pct << "%"
              << "  Segment " << currentSegIdx << "/" << total;

    if (currentSegIdx < total) {
        std::cout << "  |  Next: " << toMMSS(segments[currentSegIdx].startSec);
    }
    std::cout << "\n\n";
}
