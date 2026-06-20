// entities/Video.h
// Purpose: Header file defining the Video class, representing video metadata and engagement scoring.

#pragma once

#include <string>

/**
 * @class Video
 * @brief Represents a video asset with metadata and engagement scoring logic.
 *
 * This class tracks video attributes (ID, title, genre, creator, upload date) and
 * user engagement metrics (views, likes, watch time). It calculates a dynamic
 * popularity/engagement score used by the RankingSystem.
 */
class Video {
public:
    // Core Metadata
    std::string videoId;            // Unique identifier for the video (e.g., VID-001)
    std::string title;              // Video title displayed to viewers
    std::string creatorId;          // Reference to the Creator who uploaded the video
    std::string genre;              // Genre classification (e.g., Education, Tech)
    std::string uploadDate;         // ISO date format string for upload tracking
    std::string licenseKey;         // Key referenced in the LicenseRegistry to verify copyrights

    int durationSec;                // Video length in seconds

    // Engagement Metrics (updated during playback simulation)
    long viewCount;                 // Total views accumulated by the video
    long likeCount;                 // Total likes received from viewers
    long totalWatchTimeSec;         // Cumulative playback time across all viewers

    double engagementScore;         // Calculated popular score for max heap sorting

    /**
     * @brief Constructs an empty/default Video entity.
     */
    Video();

    /**
     * @brief Constructs a Video entity initialized with core metadata attributes.
     */
    Video(std::string id,
          std::string title,
          std::string creatorId,
          std::string genre,
          int durationSec,
          std::string licenseKey,
          std::string uploadDate);

    /**
     * @brief Increments views and triggers score recalculation.
     */
    void incrementViews();

    /**
     * @brief Increments likes and triggers score recalculation.
     */
    void addLike();

    /**
     * @brief Adds playback seconds to watch time and triggers score recalculation.
     * @param seconds Playback duration to log.
     */
    void addWatchTime(int seconds);

    /**
     * @brief Computes popularity rating based on views, likes, and watch time ratio.
     * Formula: (views * 0.5) + (likes * 2.0) + ((watchTime / duration) * 1000.0)
     */
    void recalculateScore();

    /**
     * @brief Returns the calculated engagement score.
     */
    double getEngagementScore() const;

    /**
     * @brief Prints a single-line summary of video details in catalog views.
     */
    void displaySummary() const;

    /**
     * @brief Prints full video metadata and metrics for detailed views.
     */
    void displayFull() const;

    /**
     * @brief Equality operator comparing video IDs to detect duplicate video assets.
     */
    bool operator==(const Video& other) const;
};
