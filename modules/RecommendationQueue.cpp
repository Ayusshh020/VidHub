// Note: Comments are only for understanding—do not modify the code itself.
#include "RecommendationQueue.h"

#include <iostream>
#include <iomanip>
#include <queue>

static void printRQDivider() {
    std::cout << "  " << std::string(62, '-') << "\n";
}

// =============================================================
//  CONSTRUCTOR
// =============================================================

RecommendationQueue::RecommendationQueue(int maxSize)
    : maxSize(maxSize)
{}

// =============================================================
//  CORE QUEUE OPERATIONS
// =============================================================

// Add a video to the back of the queue.
// If the queue is already at maxSize, the new video is ignored.
// This prevents unbounded growth during long viewing sessions.
void RecommendationQueue::enqueue(const Video& video) {
    if (static_cast<int>(recQueue.size()) >= maxSize)
        return;
    recQueue.push(video);
}

// Remove and return the front (next-up) video.
// PRECONDITION: caller must verify isEmpty() == false.
Video RecommendationQueue::dequeue() {
    Video next = recQueue.front();
    recQueue.pop();
    return next;
}

// Return the front video without removing it.
// PRECONDITION: caller must verify isEmpty() == false.
const Video& RecommendationQueue::front() const {
    return recQueue.front();
}

// Populates queue with up to maxSize videos sharing currentGenre (excluding excludeId).

void RecommendationQueue::populate(const std::string&        currentGenre,
                                   const std::string&        excludeId,
                                   const std::vector<Video>& videoPool) {
    // Clear stale recommendations before refilling
    clear();

    for (const auto& v : videoPool) {
        if (recQueue.size() >= static_cast<size_t>(maxSize))
            break;
        if (v.videoId == excludeId)
            continue;                  // Skip the video just watched
        if (v.genre == currentGenre)
            recQueue.push(v);          // Genre match → enqueue
    }
}

// =============================================================
//  UTILITY
// =============================================================

bool RecommendationQueue::isEmpty() const {
    return recQueue.empty();
}

int RecommendationQueue::getSize() const {
    return static_cast<int>(recQueue.size());
}

// Clear all pending recommendations.
// Called on: user logs out, user manually clears queue,
//            or populate() resets before refilling.
void RecommendationQueue::clear() {
    while (!recQueue.empty())
        recQueue.pop();
}

// =============================================================
//  DISPLAY
//
//  std::queue has no iterator. To show all items without
//  destroying the queue, we copy it and drain the copy.
//  The original recQueue is untouched.
// =============================================================

void RecommendationQueue::display() const {
    if (recQueue.empty()) {
        std::cout << "\n  No recommendations in queue.\n\n";
        return;
    }

    std::cout << "\n";
    printRQDivider();
    std::cout << "  \xF0\x9F\x8E\xAF  Up Next — Recommended Queue  ("
              << recQueue.size() << " video"
              << (recQueue.size() == 1 ? "" : "s") << ")\n";
    printRQDivider();

    // Drain a copy — never mutate the real queue in display()
    std::queue<Video> copy = recQueue;
    int position = 1;

    while (!copy.empty()) {
        const Video& v = copy.front();
        std::cout << "  "
                  << std::right << std::setw(2) << position++ << ". "
                  << std::left  << std::setw(35) << v.title
                  << " [" << v.videoId << "]"
                  << "  Genre: " << v.genre
                  << "\n";
        copy.pop();
    }

    printRQDivider();
    std::cout << "\n";
}
