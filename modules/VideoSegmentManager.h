#pragma once
// Note: Comments are only for understanding—do not modify the code itself.

#include <string>
#include <deque>

struct Segment {
    int         index;
    int         startSec;
    int         endSec;
    std::string quality;
    int         sizeKB;

    Segment();

    Segment(int index, int startSec, int endSec,
            std::string quality, int sizeKB);

    int  getDuration() const;
    void display()     const;
};

class VideoSegmentManager {
private:
    std::deque<Segment> segments;
    std::string         videoId;
    int                 totalDurationSec;

public:
    VideoSegmentManager();
    explicit VideoSegmentManager(const std::string& videoId,
                                 int totalDurationSec);

    void segmentVideo(int segmentSizeSec,
                      const std::string& quality = "720p");

    void appendSegment(const Segment& seg);
    void prependSegment(const Segment& seg);
    Segment consumeFront();
    const Segment& peekFront() const;
    const Segment& getSegment(int index) const;

    int  getTotalSegments()    const;
    bool isEmpty()             const;
    int  getTotalBufferedSec() const;
    void clear();

    void displaySegments()               const;
    void displaySegmentRange(int from,
                             int to)     const;
    void displayPlaybackStatus(int currentSegIdx) const;
};
