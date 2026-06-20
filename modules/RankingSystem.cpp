// modules/RankingSystem.cpp
// Purpose: Implementation of the RankingSystem class, maintaining the Max Heap for dynamic popular content rankings.

#include "RankingSystem.h"

#include <iostream>
#include <iomanip>

// Helper to print a uniform divider line for rankings display
static void printRSDivider(char c = '-') {
    std::cout << "  " << std::string(66, c) << "\n";
}

// Returns a medal icon or blank space depending on the rank
static std::string medal(int rank) {
    if (rank == 1) return "\xF0\x9F\xA5\x87"; // Gold
    if (rank == 2) return "\xF0\x9F\xA5\x88"; // Silver
    if (rank == 3) return "\xF0\x9F\xA5\x89"; // Bronze
    return "  ";
}

// Constructor setting heap dirty initially to false
RankingSystem::RankingSystem()
    : heapDirty(false)
{}

// Rebuilds the Max Heap from scratch using items in videoStore
void RankingSystem::rebuildHeap() const {
    while (!heap.empty())
        heap.pop();

    for (const auto& pair : videoStore)
        heap.push(pair.second);

    heapDirty = false; // Reset dirty flag
}

// Registers a video. Marks heap as dirty so it's rebuilt on the next query.
void RankingSystem::addVideo(const Video& video) {
    videoStore[video.videoId] = video;
    heapDirty = true;
}

// Updates an existing video metrics (views/likes) and flags heap rebuild
bool RankingSystem::updateVideo(const Video& updatedVideo) {
    auto it = videoStore.find(updatedVideo.videoId);
    if (it == videoStore.end())
        return false;
    it->second = updatedVideo;
    heapDirty  = true;
    return true;
}

// Erases a video from the database and flags heap rebuild
bool RankingSystem::removeVideo(const std::string& videoId) {
    auto erased = videoStore.erase(videoId);
    if (erased > 0) {
        heapDirty = true;
        return true;
    }
    return false;
}

// Returns the top item of the heap (highest engagement score). Rebuilds heap first if dirty.
const Video& RankingSystem::peekTop() const {
    if (heapDirty) rebuildHeap();
    return heap.top();
}

// Returns the top-K elements by copying the heap and popping K times (O(k log n))
std::vector<Video> RankingSystem::getTopK(int k) const {
    if (heapDirty) rebuildHeap();

    std::vector<Video> result;
    MaxHeap copy = heap; // Create a temporary copy to destructively extract top elements

    int extracted = 0;
    while (!copy.empty() && extracted < k) {
        result.push_back(copy.top());
        copy.pop();
        extracted++;
    }
    return result;
}

int RankingSystem::getCount() const {
    return static_cast<int>(videoStore.size());
}

bool RankingSystem::isEmpty() const {
    return videoStore.empty();
}

bool RankingSystem::contains(const std::string& videoId) const {
    return videoStore.count(videoId) > 0;
}

// Displays formatted platform rankings list
void RankingSystem::displayRankings(int topK) const {
    std::cout << "\n";
    printRSDivider('=');
    std::cout << "  \xF0\x9F\x8F\x86  VidHub Rankings — Top " << topK
              << "  (" << getCount() << " total videos)\n";
    printRSDivider('=');

    if (isEmpty()) {
        std::cout << "  No videos in the ranking system yet.\n";
        printRSDivider('=');
        std::cout << "\n";
        return;
    }

    std::vector<Video> topVideos = getTopK(topK);

    for (int i = 0; i < static_cast<int>(topVideos.size()); ++i) {
        const Video& v = topVideos[i];
        int rank = i + 1;

        std::cout << "  " << medal(rank) << " "
                  << std::right << std::setw(2) << rank << ". "
                  << std::left  << std::setw(36) << v.title
                  << " | Score: " << std::right << std::setw(8)
                  << std::fixed << std::setprecision(1) << v.engagementScore
                  << "  | Views: " << std::setw(6) << v.viewCount
                  << "  | Likes: " << v.likeCount
                  << "\n";
    }

    printRSDivider('=');
    std::cout << "\n";
}
