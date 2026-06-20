// modules/VideoCatalog.h
// Purpose: Header file defining the VideoCatalog class, representing the library of platform videos in a BST.

#pragma once

#include "../entities/Video.h"
#include <string>
#include <vector>

/**
 * @class VideoCatalog
 * @brief Manages platform videos using a Binary Search Tree (BST).
 *
 * Keeps all platform videos sorted alphabetically (A-Z) by title.
 * Allows logarithmic searching, keyword searching, and insertion.
 */
class VideoCatalog {
private:
    struct BSTNode {
        Video    data;       // The Video object stored in the node
        BSTNode* left;       // Pointer to left child subtree
        BSTNode* right;      // Pointer to right child subtree

        explicit BSTNode(const Video& v)
            : data(v), left(nullptr), right(nullptr) {}
    };

    BSTNode* root;           // Root node of the BST
    int      nodeCount;      // Count of nodes/videos in the tree

    // Recursive helper functions for tree operations
    BSTNode* insertHelper  (BSTNode* node, const Video& video);
    BSTNode* removeHelper  (BSTNode* node, const std::string& videoId);
    BSTNode* getMinNode    (BSTNode* node) const;
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
    void clearHelper      (BSTNode* node);

public:
    /**
     * @brief Constructs an empty VideoCatalog.
     */
    VideoCatalog();

    /**
     * @brief Destructor that deallocates all nodes in the tree.
     */
    ~VideoCatalog();

    /**
     * @brief Inserts a video into the BST alphabetically by title.
     */
    void insert(const Video& video);

    /**
     * @brief Removes a video by its unique video ID.
     * @return True if deletion was successful.
     */
    bool remove(const std::string& videoId);

    /**
     * @brief Finds a video with an exact title match (O(log n) lookup).
     */
    const Video* searchExact(const std::string& title) const;

    /**
     * @brief Finds all videos whose titles contain the query string (case-insensitive, O(n)).
     */
    std::vector<Video> searchByKeyword(const std::string& keyword) const;

    /**
     * @brief Filters and returns all videos of a specific genre (O(n)).
     */
    std::vector<Video> searchByGenre(const std::string& genre) const;

    /**
     * @brief Returns all videos sorted alphabetically via an in-order traversal.
     */
    std::vector<Video> getAllSorted() const;

    // Attribute getters
    int  getCount()  const;
    bool isEmpty()   const;
    bool contains(const std::string& videoId) const;

    /**
     * @brief Displays the catalog in alphabetical order.
     */
    void displayAll()          const;

    /**
     * @brief Formats and prints keyword/genre search results.
     */
    void displaySearchResults  (const std::vector<Video>& results,
                                const std::string& query)  const;
};
