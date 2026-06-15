#include "VideoSegmentManager.h"

#include <iostream>
#include <iomanip>
#include <stdexcept>
#include <string>

// ─────────────────────────────────────────────────────────────
//  File-local helpers
// ─────────────────────────────────────────────────────────────

static void printSMDivider(char c = '-') {
    std::cout << "  " << std::string(66, c) << "\n";
}

// Estimate segment file size based on quality and duration
static int estimateSizeKB(const std::string& quality, int durationSec) {
    // Rough bitrates: 1080p≈4Mbps, 720p≈2.5Mbps, 480p≈1Mbps, 360p≈0.5Mbps
    int bitsPerSec = 2500000;  // Default: 720p
    if      (quality == "1080p") bitsPerSec = 4000000;
    else if (quality == "720p")  bitsPerSec = 2500000;
    else if (quality == "480p")  bitsPerSec = 1000000;
    else if (quality == "360p")  bitsPerSec =  500000;

    return (bitsPerSec / 8 / 1024) * durationSec;  // bytes → KB
}

// Format seconds as MM:SS
static std::string toMMSS(int totalSec) {
    int m = totalSec / 60;
    int s = totalSec % 60;
    std::string mm = (m < 10 ? "0" : "") + std::to_string(m);
    std::string ss = (s < 10 ? "0" : "") + std::to_string(s);
    return mm + ":" + ss;
}

// =============================================================
//  SEGMENT — STRUCT IMPLEMENTATION
// =============================================================

Segment::Segment()
    : index(0), startSec(0), endSec(0), quality("720p"), sizeKB(0)
{}

Segment::Segment(int idx, int start, int end,
                 std::string quality, int sizeKB)
    : index(idx), startSec(start), endSec(end),
      quality(quality), sizeKB(sizeKB)
{}

int Segment::getDuration() const {
    return endSec - startSec;
}

// Output: SEG-001 | 00:00 → 00:10 | 720p | 3052 KB
void Segment::display() const {
    std::cout << "  SEG-" << std::setfill('0') << std::setw(3) << index
              << std::setfill(' ')
              << " | " << toMMSS(startSec) << " \xE2\x86\x92 " << toMMSS(endSec)
              << " | " << std::setw(5) << quality
              << " | " << std::setw(6) << sizeKB << " KB\n";
}

// =============================================================
//  CONSTRUCTOR
// =============================================================

VideoSegmentManager::VideoSegmentManager()
    : videoId(""), totalDurationSec(0)
{}

VideoSegmentManager::VideoSegmentManager(const std::string& videoId,
                                         int totalDurationSec)
    : videoId(videoId), totalDurationSec(totalDurationSec)
{}

// =============================================================
//  SEGMENTATION
//
//  Divides totalDurationSec into chunks of segmentSizeSec.
//  The last chunk is sized to cover the remainder.
//
//  Example: 2700s video, 10s segments → 270 segments
//    SEG-001: 0s   → 10s
//    SEG-002: 10s  → 20s
//    ...
//    SEG-270: 2690s→ 2700s
// =============================================================

void VideoSegmentManager::segmentVideo(int segmentSizeSec,
                                        const std::string& quality) {
    if (segmentSizeSec <= 0) return;
    segments.clear();

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

// =============================================================
//  DEQUE OPERATIONS
// =============================================================

// Append to back — O(1)
// Used for: buffering the next chunk, live stream new segments
void VideoSegmentManager::appendSegment(const Segment& seg) {
    segments.push_back(seg);
}

// Prepend to front — O(1)
// Used for: inserting a repair/priority segment at the head
void VideoSegmentManager::prependSegment(const Segment& seg) {
    segments.push_front(seg);
}

// Remove and return the front segment — O(1)
// Called during simulated playback after a segment finishes
// PRECONDITION: caller must check isEmpty() first
Segment VideoSegmentManager::consumeFront() {
    Segment front = segments.front();
    segments.pop_front();
    return front;
}

// Peek at front without removing — O(1)
// Used by player to preview what loads next
const Segment& VideoSegmentManager::peekFront() const {
    return segments.front();
}

// Random access by 0-based index — O(1)
// This is the key deque advantage over list.
// Used for seek operations (user scrubs to minute 30).
const Segment& VideoSegmentManager::getSegment(int index) const {
    if (index < 0 || index >= static_cast<int>(segments.size()))
        throw std::out_of_range("Segment index out of range");
    return segments[index];
}

// =============================================================
//  UTILITY
// =============================================================

int VideoSegmentManager::getTotalSegments() const {
    return static_cast<int>(segments.size());
}

bool VideoSegmentManager::isEmpty() const {
    return segments.empty();
}

// Total seconds of video currently buffered
int VideoSegmentManager::getTotalBufferedSec() const {
    int total = 0;
    for (const auto& seg : segments)
        total += seg.getDuration();
    return total;
}

void VideoSegmentManager::clear() {
    segments.clear();
}

// =============================================================
//  DISPLAY
// =============================================================

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

    // Show first 5 and last 3 if large; otherwise show all
    int total = getTotalSegments();
    if (total <= 10) {
        for (const auto& seg : segments)
            seg.display();
    } else {
        // First 5
        for (int i = 0; i < 5; ++i) segments[i].display();
        std::cout << "  ... (" << (total - 8) << " segments) ...\n";
        // Last 3
        for (int i = total - 3; i < total; ++i) segments[i].display();
    }

    printSMDivider('=');
    std::cout << "\n";
}

// Display a specific range [from, to] (1-based)
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

// Visual progress bar for simulated playback
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
