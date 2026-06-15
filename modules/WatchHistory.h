#pragma once

#include "../entities/Video.h"
#include <stack>
#include <string>

// =============================================================
//  WatchHistory — Tracks videos a user has recently watched.
//
//  DSA: Stack (LIFO)
//  Backed by: std::stack<Video>
//
//  Why Stack?
//    The most recently watched video is always the most
//    relevant. LIFO ordering surfaces it instantly at O(1).
//    This mirrors how YouTube's "Continue Watching" works.
//
//  Owned by: ConsoleUI (one instance per active user session)
//  Depends on: Video (entities/)
// =============================================================

class WatchHistory {
private:
    std::stack<Video> history;   // Internal LIFO container
    int               maxSize;   // Cap to prevent unbounded growth
                                 // Default: 50 entries

public:
    // ── Constructor ───────────────────────────────────────────
    explicit WatchHistory(int maxSize = 50);

    // ── Core Stack Operations ─────────────────────────────────

    // Push a video to the top of the history.
    // If maxSize is reached, the oldest entry (bottom) is
    // dropped by rebuilding without it.
    // Called by ConsoleUI immediately after a user "watches" a video.
    void push(const Video& video);

    // Remove and return the most recently watched video.
    // Caller must check isEmpty() before calling pop().
    Video pop();

    // Return the most recently watched video WITHOUT removing it.
    // Caller must check isEmpty() before calling peek().
    const Video& peek() const;

    // ── Utility ───────────────────────────────────────────────
    bool isEmpty()  const;  // True if no videos in history
    int  getSize()  const;  // Number of entries currently stored
    void clear();           // Remove all entries (user clears history)

    // ── Display ───────────────────────────────────────────────
    // Print all entries from most-recent (top) to oldest (bottom).
    // Does NOT modify the stack — uses an internal copy to iterate.
    void display() const;
};
