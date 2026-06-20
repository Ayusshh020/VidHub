// modules/RecommendationQueue.cpp
// Purpose: Implementation of RecommendationQueue queue methods, handling enqueueing, dequeueing, and recommendation population.

#include "RecommendationQueue.h"

#include <iostream>
#include <iomanip>
#include <queue>

// Helper to print a uniform divider line for the Recommendation Queue output
static void printRQDivider() {
    std::cout << "  " << std::string(62, '-') << "\n";
}

// Constructor setting maximum size constraint
RecommendationQueue::RecommendationQueue(int maxSize)
    : maxSize(maxSize)
{}

// Enqueues a video if the queue size is within capacity limits
void RecommendationQueue::enqueue(const Video& video) {
    if (static_cast<int>(recQueue.size()) >= maxSize)
        return;
    recQueue.push(video);
}

// Dequeues and returns the front video element (FIFO)
Video RecommendationQueue::dequeue() {
    Video next = recQueue.front();
    recQueue.pop();
    return next;
}

// Returns a reference to the front video element without modifications
const Video& RecommendationQueue::front() const {
    return recQueue.front();
}

// Refuels queue with videos from videoPool matching currentGenre (and skipping excludeId)
void RecommendationQueue::populate(const std::string&        currentGenre,
                                   const std::string&        excludeId,
                                   const std::vector<Video>& videoPool) {
    clear(); // Clear existing recommendations to refresh

    for (const auto& v : videoPool) {
        if (recQueue.size() >= static_cast<size_t>(maxSize))
            break;
        if (v.videoId == excludeId)
            continue; // Skip the active video
        if (v.genre == currentGenre)
            recQueue.push(v); // Queue up similar genre video
    }
}

// Checks if queue is empty
bool RecommendationQueue::isEmpty() const {
    return recQueue.empty();
}

// Returns current count of enqueued items
int RecommendationQueue::getSize() const {
    return static_cast<int>(recQueue.size());
}

// Clears queue
void RecommendationQueue::clear() {
    while (!recQueue.empty())
        recQueue.pop();
}

// Displays queue from front (next to play) to back
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

    // Copy queue to traverse elements print-friendly without destroying the original queue
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
