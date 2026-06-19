// Note: Comments are only for understanding—do not modify the code itself.
#include "RankingSystem.h"

#include <iostream>
#include <iomanip>

static void printRSDivider(char c = '-') {
    std::cout << "  " << std::string(66, c) << "\n";
}

static std::string medal(int rank) {
    if (rank == 1) return "\xF0\x9F\xA5\x87";
    if (rank == 2) return "\xF0\x9F\xA5\x88";
    if (rank == 3) return "\xF0\x9F\xA5\x89";
    return "  ";
}

RankingSystem::RankingSystem()
    : heapDirty(false)
{}

void RankingSystem::rebuildHeap() const {
    while (!heap.empty())
        heap.pop();

    for (const auto& pair : videoStore)
        heap.push(pair.second);

    heapDirty = false;
}

void RankingSystem::addVideo(const Video& video) {
    videoStore[video.videoId] = video;
    heapDirty = true;
}

bool RankingSystem::updateVideo(const Video& updatedVideo) {
    auto it = videoStore.find(updatedVideo.videoId);
    if (it == videoStore.end())
        return false;
    it->second = updatedVideo;
    heapDirty  = true;
    return true;
}

bool RankingSystem::removeVideo(const std::string& videoId) {
    auto erased = videoStore.erase(videoId);
    if (erased > 0) {
        heapDirty = true;
        return true;
    }
    return false;
}

const Video& RankingSystem::peekTop() const {
    if (heapDirty) rebuildHeap();
    return heap.top();
}

std::vector<Video> RankingSystem::getTopK(int k) const {
    if (heapDirty) rebuildHeap();

    std::vector<Video> result;
    MaxHeap copy = heap;

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
