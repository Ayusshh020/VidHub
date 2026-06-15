#pragma once

#include "../entities/Video.h"
#include <string>
#include <vector>

// =============================================================
//  VideoCatalog — Sorted video storage using a Binary Search Tree
//
//  DSA: Binary Search Tree (hand-implemented, NOT std::map)
//  Ordering key: Video::title (alphabetical, case-sensitive)
//
//  Why BST?
//    Videos are uploaded in arbitrary order but must be
//    browsed/searched in sorted order. BST maintains sort
//    automatically on every insert — no re-sorting needed.
//    In-order traversal gives alphabetical listing in O(n).
//    Exact title search runs in O(log n) average.
//
//  Operations summary:
//    insert()          O(log n) average
//    searchByTitle()   O(log n) average
//    searchByKeyword() O(n)     — requires full traversal
//    searchByGenre()   O(n)     — requires full traversal
//    remove()          O(log n) average
//    inOrderDisplay()  O(n)
//
//  Owned by: ConsoleUI (one global instance for the platform)
//  Depends on: Video (entities/)
// =============================================================

class VideoCatalog {
private:

    // ── Internal BST Node ────────────────────────────────────
    struct BSTNode {
        Video    data;
        BSTNode* left;
        BSTNode* right;

        explicit BSTNode(const Video& v)
            : data(v), left(nullptr), right(nullptr) {}
    };

    BSTNode* root;      // Root of the BST
    int      nodeCount; // Tracks total videos in catalog

    // ── Private Recursive Helpers ────────────────────────────
    BSTNode* insertHelper  (BSTNode* node, const Video& video);
    BSTNode* removeHelper  (BSTNode* node, const std::string& videoId);
    BSTNode* getMinNode    (BSTNode* node) const;         // In-order successor
    BSTNode* searchByTitle (BSTNode* node,
                            const std::string& title) const;

    void inOrderHelper    (BSTNode* node,
                           std::vector<Video>& result) const;
    void keywordHelper    (BSTNode* node,
                           const std::string& keyword,
                           std::vector<Video>& result) const;
    void genreHelper      (BSTNode* node,
                           const std::string& genre,
                           std::vector<Video>& result) const;
    void clearHelper      (BSTNode* node);                // Free all nodes

public:
    // ── Constructor / Destructor ─────────────────────────────
    VideoCatalog();
    ~VideoCatalog();

    // ── Core BST Operations ──────────────────────────────────

    // Insert a video into the BST (ordered by title).
    // If a video with the same videoId already exists, it is
    // NOT inserted again (prevents duplicates).
    void insert(const Video& video);

    // Remove a video by its videoId.
    // Handles all 3 BST deletion cases internally.
    // Returns true if found and removed, false if not found.
    bool remove(const std::string& videoId);

    // ── Search Operations ────────────────────────────────────

    // Exact title match — O(log n) average using BST property.
    // Returns pointer to the Video if found, nullptr otherwise.
    const Video* searchExact(const std::string& title) const;

    // Keyword search — O(n) full traversal.
    // Returns all videos whose title CONTAINS the keyword.
    // Case-insensitive matching.
    std::vector<Video> searchByKeyword(const std::string& keyword) const;

    // Genre filter — O(n) full traversal.
    // Returns all videos matching the given genre exactly.
    std::vector<Video> searchByGenre(const std::string& genre) const;

    // ── Traversal / Access ───────────────────────────────────

    // In-order traversal → returns all videos alphabetically sorted.
    std::vector<Video> getAllSorted() const;

    // ── Utility ──────────────────────────────────────────────
    int  getCount()  const;   // Total videos in catalog
    bool isEmpty()   const;   // True if catalog has no videos
    bool contains(const std::string& videoId) const; // Existence check by ID

    // ── Display ─────────────────────────────────────────────
    void displayAll()          const; // Alphabetical list of all videos
    void displaySearchResults  (const std::vector<Video>& results,
                                const std::string& query)  const;
};
