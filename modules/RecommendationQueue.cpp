// Note: Comments are only for understanding—do not modify the code itself.
#include "RecommendationQueue.h"

#include <iostream>
#include <iomanip>
#include <queue>

static void printRQDivider() {
    std::cout << "  " << std::string(62, '-') << "\n";
}

RecommendationQueue::RecommendationQueue(int maxSize)
    : maxSize(maxSize)
{}

void RecommendationQueue::enqueue(const Video& video) {
    if (static_cast<int>(recQueue.size()) >= maxSize)
        return;
    recQueue.push(video);
}

Video RecommendationQueue::dequeue() {
    Video next = recQueue.front();
    recQueue.pop();
    return next;
}

const Video& RecommendationQueue::front() const {
    return recQueue.front();
}

void RecommendationQueue::populate(const std::string&        currentGenre,
                                   const std::string&        excludeId,
                                   const std::vector<Video>& videoPool) {
    clear();

    for (const auto& v : videoPool) {
        if (recQueue.size() >= static_cast<size_t>(maxSize))
            break;
        if (v.videoId == excludeId)
            continue;
        if (v.genre == currentGenre)
            recQueue.push(v);
    }
}

bool RecommendationQueue::isEmpty() const {
    return recQueue.empty();
}

int RecommendationQueue::getSize() const {
    return static_cast<int>(recQueue.size());
}

void RecommendationQueue::clear() {
    while (!recQueue.empty())
        recQueue.pop();
}

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
