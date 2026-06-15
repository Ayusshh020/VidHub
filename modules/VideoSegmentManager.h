#pragma once

#include <string>
#include <deque>

// =============================================================
//  VideoSegmentManager — Manages video chunks for streaming.
//
//  DSA: Deque (std::deque)
//
//  Why Deque?
//    Streaming requires:
//      - O(1) indexed access  : jump to any segment by position
//      - O(1) front removal   : consume played segments
//      - O(1) back append     : buffer newly downloaded segments
//      - O(1) front prepend   : insert a priority/repair segment
//    No other structure gives all four. A vector is slow at
//    front insertions; a linked list is slow at indexed access.
//
//  Real-world analogy:
//    This models how HLS/DASH adaptive streaming works —
//    a video is pre-divided into .ts or .m4s chunks.
//    The player maintains a sliding window of buffered segments.
//
//  Lifecycle:
//    1. Creator uploads video → segmentVideo() divides it.
//    2. ConsoleUI calls displaySegments() to show the plan.
//    3. On "Watch", ConsoleUI simulates playback:
//         - consumeFront() removes played segments
//         - jumpToSegment(i) simulates seeking
//    4. For live content, appendSegment() adds new chunks.
//
//  Owned by: ConsoleUI manages one instance per video watch
//  Depends on: nothing (standalone module)
// =============================================================

// ── A single video segment ───────────────────────────────────
struct Segment {
    int         index;       // 1-based position in the video
    int         startSec;    // Segment start time (seconds)
    int         endSec;      // Segment end time   (seconds)
    std::string quality;     // "1080p", "720p", "480p", "360p"
    int         sizeKB;      // Simulated file size in kilobytes

    // Default constructor
    Segment();

    Segment(int index, int startSec, int endSec,
            std::string quality, int sizeKB);

    int  getDuration() const;   // endSec - startSec
    void display()     const;   // One-line segment info
};

// ─────────────────────────────────────────────────────────────

class VideoSegmentManager {
private:
    std::deque<Segment> segments;  // The segment buffer
    std::string         videoId;   // Which video this manages
    int                 totalDurationSec; // Full video duration

public:
    // ── Constructor ───────────────────────────────────────────
    VideoSegmentManager();
    explicit VideoSegmentManager(const std::string& videoId,
                                 int totalDurationSec);

    // ── Segmentation (primary operation) ─────────────────────
    // Divide the video into equal-sized chunks.
    //   segmentSizeSec : duration of each chunk in seconds
    //   quality        : bitrate label for all segments
    // Last segment may be shorter if duration isn't divisible.
    // Clears any existing segments before creating new ones.
    void segmentVideo(int segmentSizeSec,
                      const std::string& quality = "720p");

    // ── Deque Operations ─────────────────────────────────────

    // O(1) — Add a segment to the back (buffering / live append)
    void appendSegment(const Segment& seg);

    // O(1) — Add a segment to the front (priority / repair)
    void prependSegment(const Segment& seg);

    // O(1) — Remove and return the front segment (played)
    Segment consumeFront();

    // O(1) — Peek at the front segment (what plays next)
    const Segment& peekFront() const;

    // O(1) — Access any segment by 0-based index (seeking)
    const Segment& getSegment(int index) const;

    // ── Utility ──────────────────────────────────────────────
    int  getTotalSegments()    const;  // Total chunks in deque
    bool isEmpty()             const;
    int  getTotalBufferedSec() const;  // Sum of all segment durations
    void clear();                      // Empty the buffer

    // ── Display ──────────────────────────────────────────────
    void displaySegments()               const; // Show all segments
    void displaySegmentRange(int from,
                             int to)     const; // Show a slice [from, to]
    void displayPlaybackStatus(int currentSegIdx) const; // Playback progress bar
};
