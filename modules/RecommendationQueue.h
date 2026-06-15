#pragma once

#include "../entities/Video.h"
#include <queue>
#include <vector>
#include <string>

// =============================================================
//  RecommendationQueue — Manages the "Up Next" autoplay queue.
//
//  DSA: Queue (FIFO)
//  Backed by: std::queue<Video>
//
//  Why Queue?
//    Recommendations are planned in advance and consumed in
//    order. FIFO ensures the sequence is respected — video[0]
//    plays before video[1]. This mirrors YouTube's autoplay.
//
//  Lifecycle:
//    1. ConsoleUI calls populate() after a user watches a video.
//    2. The queue is pre-filled with related videos by genre.
//    3. As the user clicks "Next", getNext() dequeues one.
//    4. The user can peek at what's coming via display().
//
//  Owned by: ConsoleUI (one instance per active user session)
//  Depends on: Video (entities/)
// =============================================================

class RecommendationQueue {
private:
    std::queue<Video> recQueue;  // Internal FIFO container
    int               maxSize;  // Cap on how many recs to hold
                                // Default: 10 entries

public:
    // ── Constructor ───────────────────────────────────────────
    explicit RecommendationQueue(int maxSize = 10);

    // ── Core Queue Operations ─────────────────────────────────

    // Add a video to the back of the recommendation queue.
    // Silently ignores the request if maxSize is already reached.
    void enqueue(const Video& video);

    // Remove and return the next recommended video (front).
    // Caller must check isEmpty() before calling.
    Video dequeue();

    // Return the next video WITHOUT removing it.
    // Caller must check isEmpty() before calling.
    const Video& front() const;

    // ── Populate ─────────────────────────────────────────────
    // Fill the queue from a pool of candidate videos.
    // Picks videos that share the same genre as 'currentGenre'
    // and excludes 'excludeId' (the video just watched).
    // Enqueues up to maxSize matches.
    void populate(const std::string&        currentGenre,
                  const std::string&        excludeId,
                  const std::vector<Video>& videoPool);

    // ── Utility ──────────────────────────────────────────────
    bool isEmpty() const;   // True if no recommendations pending
    int  getSize() const;   // Number of videos currently queued
    void clear();           // Empty the queue

    // ── Display ──────────────────────────────────────────────
    // Show all queued videos in order (front → back).
    // Does NOT dequeue — uses a copy to iterate.
    void display() const;
};
