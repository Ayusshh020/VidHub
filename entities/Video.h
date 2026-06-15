#pragma once

#include <string>

// =============================================================
//  Video — Core entity representing a single video on VidHub
//
//  This is a pure data class. It holds all video metadata
//  and provides helper methods to update engagement metrics.
//
//  Used by: VideoCatalog (BST), RankingSystem (Heap),
//           WatchHistory (Stack), RecommendationQueue (Queue),
//           VideoSegmentManager (Deque), SharingNetwork (Graph)
// =============================================================

class Video {
public:
    // ── Identity ─────────────────────────────────────────────
    std::string videoId;       // Unique identifier  e.g. "VID-001"
    std::string title;         // Display title
    std::string creatorId;     // Uploader's userId
    std::string genre;         // e.g. "Education", "Tech", "Health"
    std::string uploadDate;    // e.g. "2024-01-15"
    std::string licenseKey;    // e.g. "MIT-AJ-002"

    // ── Playback ─────────────────────────────────────────────
    int durationSec;           // Total duration in seconds

    // ── Engagement Metrics ───────────────────────────────────
    long viewCount;            // Total number of views
    long likeCount;            // Total number of likes
    long totalWatchTimeSec;    // Sum of all watch durations (seconds)

    // ── Computed Score ───────────────────────────────────────
    double engagementScore;    // Derived from views, likes, watch time
                               // Used by RankingSystem (Max-Heap)

    // ── Constructors ─────────────────────────────────────────
    Video();                   // Default constructor (all fields zeroed)

    Video(std::string id,
          std::string title,
          std::string creatorId,
          std::string genre,
          int durationSec,
          std::string licenseKey,
          std::string uploadDate);

    // ── Engagement Mutators ──────────────────────────────────
    void incrementViews();                    // +1 to viewCount
    void addLike();                           // +1 to likeCount
    void addWatchTime(int seconds);           // Add seconds to totalWatchTimeSec

    // ── Score Computation ────────────────────────────────────
    // Formula: (viewCount * 0.5) + (likeCount * 2.0)
    //        + (totalWatchTimeSec / durationSec) * 1000.0
    // Call this after any engagement update to keep score fresh.
    void recalculateScore();

    double getEngagementScore() const;        // Returns current engagementScore

    // ── Display ──────────────────────────────────────────────
    void displaySummary() const;              // One-line: [ID] Title | Creator | Views
    void displayFull()    const;              // All fields in a formatted block

    // ── Equality Operator ────────────────────────────────────
    // Compares by videoId — used to check if a video already
    // exists in a structure (e.g., WatchHistory, Catalog).
    // BST and Heap use external comparator structs (defined in
    // their own headers) instead of intrusive operators.
    bool operator==(const Video& other) const;
};
