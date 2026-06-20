// modules/VideoSegmentManager.h
// Purpose: Header file defining Segment and VideoSegmentManager classes, managing playback buffering using a Deque.

#pragma once

#include <string>
#include <deque>

/**
 * @struct Segment
 * @brief Represents a video playback segment/chunk.
 */
struct Segment {
    int         index;     // 1-based index representing chunk order
    int         startSec;  // Start timestamp of the chunk
    int         endSec;    // End timestamp of the chunk
    std::string quality;   // Video quality (e.g. 1080p, 720p)
    int         sizeKB;    // Size of segment file on disk in kilobytes

    /**
     * @brief Constructs a default empty Segment.
     */
    Segment();

    /**
     * @brief Constructs an initialized Segment.
     */
    Segment(int index, int startSec, int endSec,
            std::string quality, int sizeKB);

    /**
     * @brief Returns chunk duration in seconds.
     */
    int  getDuration() const;

    /**
     * @brief Prints details of the segment to the console.
     */
    void display()     const;
};

/**
 * @class VideoSegmentManager
 * @brief Manages segment buffering and playback simulation using a Deque (Double-Ended Queue).
 *
 * Implements buffering controls such as segment partitioning, pre-pending (repair chunks),
 * appending (live streaming chunks), and O(1) random access seeking.
 */
class VideoSegmentManager {
private:
    std::deque<Segment> segments;         // Double-ended queue storing buffered segment files
    std::string         videoId;          // Video ID of target asset
    int                 totalDurationSec; // Total length of video in seconds

public:
    VideoSegmentManager();

    /**
     * @brief Constructs a segment manager for a specific video ID and length.
     */
    explicit VideoSegmentManager(const std::string& videoId,
                                 int totalDurationSec);

    /**
     * @brief Partitions the total duration into fixed duration segment blocks.
     */
    void segmentVideo(int segmentSizeSec,
                      const std::string& quality = "720p");

    /**
     * @brief Appends a segment to the back of the queue (live streaming simulation).
     */
    void appendSegment(const Segment& seg);

    /**
     * @brief Prepends a segment to the front of the queue (priority repair simulation).
     */
    void prependSegment(const Segment& seg);

    /**
     * @brief Consumes and removes the front segment (represents player consuming buffer).
     */
    Segment consumeFront();

    /**
     * @brief Peeks at the front segment without removing it.
     */
    const Segment& peekFront() const;

    /**
     * @brief Returns segment at a specific index (seek simulation, O(1)).
     */
    const Segment& getSegment(int index) const;

    // Accessors
    int  getTotalSegments()    const;
    bool isEmpty()             const;
    int  getTotalBufferedSec() const;
    void clear();

    /**
     * @brief Prints list of all segments in the buffer.
     */
    void displaySegments()               const;

    /**
     * @brief Displays details for a range of segments.
     */
    void displaySegmentRange(int from,
                             int to)     const;

    /**
     * @brief Prints a visual playback status progress bar.
     */
    void displayPlaybackStatus(int currentSegIdx) const;
};
