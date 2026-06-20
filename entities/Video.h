#pragma once

#include <string>

class Video {
public:
    std::string videoId;
    std::string title;
    std::string creatorId;
    std::string genre;
    std::string uploadDate;
    std::string licenseKey;

    int durationSec;

    long viewCount;
    long likeCount;
    long totalWatchTimeSec;

    double engagementScore;

    Video();

    Video(std::string id,
          std::string title,
          std::string creatorId,
          std::string genre,
          int durationSec,
          std::string licenseKey,
          std::string uploadDate);

    void incrementViews();
    void addLike();
    void addWatchTime(int seconds);

    void recalculateScore();

    double getEngagementScore() const;

    void displaySummary() const;
    void displayFull() const;

    bool operator==(const Video& other) const;
};
