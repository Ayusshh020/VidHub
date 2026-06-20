// modules/RecommendationQueue.h
// Purpose: Header file defining the RecommendationQueue class, representing the "Up Next" queue using a Queue.

#pragma once

#include "../entities/Video.h"
#include <queue>
#include <vector>
#include <string>

/**
 * @class RecommendationQueue
 * @brief Manages the viewer's recommendation feed or playback queue using a FIFO Queue.
 *
 * This queue holds videos queued up for playback. It provides features to automatically
 * populate the queue with videos matching a specific genre from a platform pool.
 */
class RecommendationQueue {
private:
    std::queue<Video> recQueue; // Internal queue storing Video objects
    int maxSize;                // Maximum allowed size constraint

public:
    /**
     * @brief Constructs a RecommendationQueue with a specified capacity limit.
     */
    explicit RecommendationQueue(int maxSize = 10);

    /**
     * @brief Enqueues a video to the back of the queue (if capacity permits).
     * @param video The Video object to enqueue.
     */
    void enqueue(const Video& video);

    /**
     * @brief Dequeues and returns the front video (first in, first out).
     */
    Video dequeue();

    /**
     * @brief Accesses the front video without removing it from the queue.
     */
    const Video& front() const;

    /**
     * @brief Clears the queue and refuels it with videos matching currentGenre from a pool.
     * Excludes a specific video ID (typically the active video).
     * @param currentGenre The genre to filter by.
     * @param excludeId The video ID to skip (active video).
     * @param videoPool The list of all platform videos to scan.
     */
    void populate(const std::string&        currentGenre,
                  const std::string&        excludeId,
                  const std::vector<Video>& videoPool);

    /**
     * @brief Returns true if queue is empty.
     */
    bool isEmpty() const;

    /**
     * @brief Returns current queue size.
     */
    int getSize() const;

    /**
     * @brief Clears all entries from the queue.
     */
    void clear();

    /**
     * @brief Prints the queue contents in order from front to back.
     */
    void display() const;
};
