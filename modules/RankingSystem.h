// modules/RankingSystem.h
// Purpose: Header file defining the RankingSystem class, ranking videos using a Priority Queue Max Heap.

#pragma once

#include "../entities/Video.h"
#include <queue>
#include <vector>
#include <string>
#include <unordered_map>

/**
 * @struct VideoScoreComparator
 * @brief Comparator functor for comparing Video engagement scores.
 * Uses < operator to yield a Max Heap (highest engagement score at top).
 */
struct VideoScoreComparator {
    bool operator()(const Video& a, const Video& b) const {
        return a.engagementScore < b.engagementScore;
    }
};

// Priority Queue configuration utilizing std::priority_queue acting as a Max Heap
using MaxHeap = std::priority_queue<Video,
                                    std::vector<Video>,
                                    VideoScoreComparator>;

/**
 * @class RankingSystem
 * @brief Handles real-time video rankings using a Max Heap.
 *
 * It stores videos in a hash map (`videoStore`) and updates/rebuilds a priority queue Max Heap
 * to easily retrieve the top-K highest-ranked videos.
 */
class RankingSystem {
private:
    std::unordered_map<std::string, Video> videoStore; // Map for fast O(1) video retrieval/updates

    mutable MaxHeap heap;                              // Max Heap priority queue
    mutable bool    heapDirty;                         // Flag indicating if heap needs rebuilding

    /**
     * @brief Clears and repopulates the Max Heap from the videoStore.
     */
    void rebuildHeap() const;

public:
    /**
     * @brief Constructs an empty RankingSystem.
     */
    RankingSystem();

    /**
     * @brief Registers a new video or adds it to the ranking store.
     */
    void addVideo(const Video& video);

    /**
     * @brief Updates an existing video record's metrics and marks heap as dirty.
     * @return True if update succeeded.
     */
    bool updateVideo(const Video& updatedVideo);

    /**
     * @brief Removes a video from the ranking database.
     * @return True if video existed and was removed.
     */
    bool removeVideo(const std::string& videoId);

    /**
     * @brief Peeks at the highest-ranked video on the platform (Max Heap top).
     */
    const Video& peekTop() const;

    /**
     * @brief Extracts and returns the top K ranked videos in descending order of score.
     */
    std::vector<Video> getTopK(int k) const;

    // Accessors
    int  getCount()                          const;
    bool isEmpty()                           const;
    bool contains(const std::string& videoId)const;

    /**
     * @brief Prints the leader-board rankings to console.
     */
    void displayRankings(int topK = 10) const;
};
