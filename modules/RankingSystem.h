#pragma once

#include "../entities/Video.h"
#include <queue>
#include <vector>
#include <string>
#include <unordered_map>

// =============================================================
//  RankingSystem — Ranks videos by engagement score.
//
//  DSA: Max-Heap (std::priority_queue with custom comparator)
//  Priority key: Video::engagementScore (higher = better rank)
//
//  Why Max-Heap?
//    We always want the TOP video instantly — O(1) peek.
//    New uploads/engagement updates need to re-sort efficiently.
//    Max-Heap gives O(1) top-access and O(log n) insert,
//    which is optimal for a leaderboard-style feature.
//
//  Design: Two-layer architecture
//    Layer 1 — videoStore (unordered_map):
//              Source of truth. Holds one Video per videoId.
//              Score updates happen here first.
//
//    Layer 2 — Max-Heap (priority_queue):
//              Built on demand from videoStore.
//              Used for top-K extraction and display.
//              Rebuilt after score updates ("lazy rebuild").
//
//  Operations summary:
//    addVideo()        O(log n) — heap insert
//    updateScore()     O(n)     — rebuild heap after map update
//    getTopK(k)        O(k log n) — extract from heap copy
//    peekTop()         O(1)     — read heap root
//    displayRankings() O(n log n) — full sorted output
//
//  Owned by: ConsoleUI (one global instance for the platform)
//  Depends on: Video (entities/)
// =============================================================

// ── Comparator for Max-Heap ──────────────────────────────────
// priority_queue uses this to decide order.
// Returns true if 'a' has LOWER priority than 'b'.
// So: a.score < b.score → b stays above a → max-heap on score.
struct VideoScoreComparator {
    bool operator()(const Video& a, const Video& b) const {
        return a.engagementScore < b.engagementScore;
    }
};

// Alias for readability
using MaxHeap = std::priority_queue<Video,
                                    std::vector<Video>,
                                    VideoScoreComparator>;

// ─────────────────────────────────────────────────────────────

class RankingSystem {
private:
    // Layer 1: Source of truth — always up to date
    std::unordered_map<std::string, Video> videoStore;  // videoId → Video

    // Layer 2: Heap — built from videoStore on demand
    mutable MaxHeap heap;          // mutable: rebuilt in const methods
    mutable bool    heapDirty;     // true when store updated but heap not rebuilt

    // Rebuild the heap from videoStore.
    // Called automatically before any heap-reading operation
    // if heapDirty is true.
    void rebuildHeap() const;

public:
    // ── Constructor ───────────────────────────────────────────
    RankingSystem();

    // ── Write Operations ──────────────────────────────────────

    // Add a new video to the ranking system.
    // If videoId already exists, the video is updated instead.
    void addVideo(const Video& video);

    // Update the score of an existing video after an engagement
    // event (view, like, watch-time update).
    // The Video object passed in should have recalculateScore()
    // already called — RankingSystem just stores the new state.
    // Returns false if videoId not found.
    bool updateVideo(const Video& updatedVideo);

    // Remove a video from the ranking system entirely.
    // Returns false if videoId not found.
    bool removeVideo(const std::string& videoId);

    // ── Read Operations ───────────────────────────────────────

    // Return the single highest-ranked video.
    // O(1) — reads the heap root.
    // Caller must check getCount() > 0 first.
    const Video& peekTop() const;

    // Extract the top-K videos in descending score order.
    // Works on a COPY of the heap — original is untouched.
    // Returns min(k, getCount()) videos.
    std::vector<Video> getTopK(int k) const;

    // ── Utility ──────────────────────────────────────────────
    int  getCount()                          const;
    bool isEmpty()                           const;
    bool contains(const std::string& videoId)const;  // Check by videoId

    // ── Display ──────────────────────────────────────────────
    void displayRankings(int topK = 10) const;  // Show top-K by score
};
