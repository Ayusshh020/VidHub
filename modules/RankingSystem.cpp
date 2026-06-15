#include "RankingSystem.h"

#include <iostream>
#include <iomanip>

// ─────────────────────────────────────────────────────────────
//  File-local helpers
// ─────────────────────────────────────────────────────────────

static void printRSDivider(char c = '-') {
    std::cout << "  " << std::string(66, c) << "\n";
}

// Rank medal labels for top 3 positions
static std::string medal(int rank) {
    if (rank == 1) return "\xF0\x9F\xA5\x87"; // 🥇
    if (rank == 2) return "\xF0\x9F\xA5\x88"; // 🥈
    if (rank == 3) return "\xF0\x9F\xA5\x89"; // 🥉
    return "  ";
}

// =============================================================
//  CONSTRUCTOR
// =============================================================

RankingSystem::RankingSystem()
    : heapDirty(false)
{}

// =============================================================
//  PRIVATE: rebuildHeap()
//
//  Clears the current heap and rebuilds it from videoStore.
//  Called automatically before any heap-reading operation when
//  heapDirty == true.
//
//  Why not just keep the heap always in sync?
//    priority_queue has no update-in-place operation.
//    If we insert a "new" entry for every score update without
//    removing the old one, the heap becomes stale.
//    Rebuild ensures heap always reflects the current store.
//
//  Time: O(n log n) — insert each of n videos into heap.
// =============================================================

void RankingSystem::rebuildHeap() const {
    // Drain existing heap
    while (!heap.empty())
        heap.pop();

    // Re-insert every video from the source of truth
    for (const auto& pair : videoStore)
        heap.push(pair.second);

    heapDirty = false;
}

// =============================================================
//  WRITE OPERATIONS
// =============================================================

// Add a video. If videoId already exists, update it.
// Marks heap dirty — next read will trigger a rebuild.
void RankingSystem::addVideo(const Video& video) {
    videoStore[video.videoId] = video;
    heapDirty = true;
}

// Update an existing video (called after engagement changes).
// Returns false if the videoId isn't in the system.
bool RankingSystem::updateVideo(const Video& updatedVideo) {
    auto it = videoStore.find(updatedVideo.videoId);
    if (it == videoStore.end())
        return false;
    it->second = updatedVideo;
    heapDirty  = true;
    return true;
}

// Remove a video from rankings entirely.
bool RankingSystem::removeVideo(const std::string& videoId) {
    auto erased = videoStore.erase(videoId);
    if (erased > 0) {
        heapDirty = true;
        return true;
    }
    return false;
}

// =============================================================
//  READ OPERATIONS
// =============================================================

// Return the top-ranked video without removing it.
// O(1) — reads the heap root directly.
// PRECONDITION: caller must ensure getCount() > 0.
const Video& RankingSystem::peekTop() const {
    if (heapDirty) rebuildHeap();
    return heap.top();
}

// Extract the top-K videos in descending score order.
//
// Strategy: copy the heap (so the original is untouched),
// then pop k times from the copy — each pop gives the
// next highest-ranked video.
//
// Time: O(k log n) — k pops, each O(log n) re-heapify.
std::vector<Video> RankingSystem::getTopK(int k) const {
    if (heapDirty) rebuildHeap();

    std::vector<Video> result;
    MaxHeap copy = heap;    // Copy the entire heap — O(n)

    int extracted = 0;
    while (!copy.empty() && extracted < k) {
        result.push_back(copy.top());
        copy.pop();
        extracted++;
    }
    return result;
}

// =============================================================
//  UTILITY
// =============================================================

int RankingSystem::getCount() const {
    return static_cast<int>(videoStore.size());
}

bool RankingSystem::isEmpty() const {
    return videoStore.empty();
}

bool RankingSystem::contains(const std::string& videoId) const {
    return videoStore.count(videoId) > 0;
}

// =============================================================
//  DISPLAY
//
//  Shows the top-K videos ranked by engagementScore.
//  Uses getTopK() which works on a copy — original heap safe.
// =============================================================

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
