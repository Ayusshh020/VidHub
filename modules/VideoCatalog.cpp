// Note: Comments are only for understanding—do not modify the code itself.
#include "VideoCatalog.h"
#include "../utils/Helpers.h"

#include <iostream>
#include <iomanip>
#include <algorithm>
#include <cctype>

static void printVCDivider(char c = '-') {
    std::cout << "  " << std::string(66, c) << "\n";
}

// =============================================================
//  CONSTRUCTOR / DESTRUCTOR
// =============================================================

VideoCatalog::VideoCatalog()
    : root(nullptr), nodeCount(0)
{}

VideoCatalog::~VideoCatalog() {
    clearHelper(root);
}

// Recursively delete all nodes — called by destructor.
// Post-order: free children before the parent.
void VideoCatalog::clearHelper(BSTNode* node) {
    if (node == nullptr) return;
    clearHelper(node->left);
    clearHelper(node->right);
    delete node;
}

// BST insert ordered alphabetically by video title. Duplicates are ignored.

VideoCatalog::BSTNode*
VideoCatalog::insertHelper(BSTNode* node, const Video& video) {
    if (node == nullptr) {
        // Found the correct empty slot — create node here
        nodeCount++;
        return new BSTNode(video);
    }

    if (video.videoId == node->data.videoId)
        return node;   // Duplicate videoId — reject silently

    if (video.title < node->data.title)
        node->left  = insertHelper(node->left,  video);  // Go left
    else
        node->right = insertHelper(node->right, video);  // Go right

    return node;
}

void VideoCatalog::insert(const Video& video) {
    root = insertHelper(root, video);
}

// BST deletion matched by videoId. Requires O(n) traversal since BST is keyed by title.

// Find the leftmost (minimum-title) node in a subtree.
// Used to locate the in-order successor during deletion.
VideoCatalog::BSTNode*
VideoCatalog::getMinNode(BSTNode* node) const {
    while (node->left != nullptr)
        node = node->left;
    return node;
}

// Recursive removal: traverses the whole tree to find by videoId.
// Returns the updated subtree root after deletion.
VideoCatalog::BSTNode*
VideoCatalog::removeHelper(BSTNode* node, const std::string& videoId) {
    if (node == nullptr)
        return nullptr;   // videoId not found in this subtree

    // Search left and right subtrees (can't use BST shortcut
    // because we're matching on videoId, not title)
    if (toLower(videoId) == toLower(node->data.videoId)) {
        // ── Found the node to delete ──────────────────────────
        nodeCount--;

        // Case 1: Leaf node
        if (node->left == nullptr && node->right == nullptr) {
            delete node;
            return nullptr;
        }

        // Case 2a: Only right child
        if (node->left == nullptr) {
            BSTNode* temp = node->right;
            delete node;
            return temp;
        }

        // Case 2b: Only left child
        if (node->right == nullptr) {
            BSTNode* temp = node->left;
            delete node;
            return temp;
        }

        // Case 3: Two children
        //   Find in-order successor (smallest in right subtree)
        //   Copy its data into current node
        //   Delete the successor from the right subtree
        BSTNode* successor    = getMinNode(node->right);
        node->data            = successor->data;           // Copy data
        node->right = removeHelper(node->right,
                                   successor->data.videoId); // Delete successor
        nodeCount++;   // removeHelper decremented — undo since we copied, not deleted
        return node;
    }

    // Not this node — recurse into both subtrees
    node->left  = removeHelper(node->left,  videoId);
    node->right = removeHelper(node->right, videoId);
    return node;
}

bool VideoCatalog::remove(const std::string& videoId) {
    int before = nodeCount;
    root = removeHelper(root, videoId);
    return nodeCount < before;   // true if a node was actually removed
}

// Exact title match search using BST ordering (O(log n)).
VideoCatalog::BSTNode*
VideoCatalog::searchByTitle(BSTNode* node, const std::string& title) const {
    if (node == nullptr)              return nullptr;
    if (title == node->data.title)    return node;
    if (title  < node->data.title)    return searchByTitle(node->left,  title);
    return                                   searchByTitle(node->right, title);
}

const Video* VideoCatalog::searchExact(const std::string& title) const {
    BSTNode* found = searchByTitle(root, title);
    if (found == nullptr) return nullptr;
    return &found->data;
}

// Keyword search — O(n) in-order traversal with contains check.
// Case-insensitive: both the video title and keyword are lowercased.
void VideoCatalog::keywordHelper(BSTNode* node,
                                  const std::string& keyword,
                                  std::vector<Video>& result) const {
    if (node == nullptr) return;
    keywordHelper(node->left, keyword, result);
    if (toLower(node->data.title).find(keyword) != std::string::npos)
        result.push_back(node->data);
    keywordHelper(node->right, keyword, result);
}

std::vector<Video> VideoCatalog::searchByKeyword(const std::string& keyword) const {
    std::vector<Video> result;
    keywordHelper(root, toLower(keyword), result);
    return result;   // Already in alphabetical order (in-order traversal)
}

// Genre filter — O(n) in-order traversal with genre match.
void VideoCatalog::genreHelper(BSTNode* node,
                                const std::string& genre,
                                std::vector<Video>& result) const {
    if (node == nullptr) return;
    genreHelper(node->left, genre, result);
    if (toLower(node->data.genre) == genre)
        result.push_back(node->data);
    genreHelper(node->right, genre, result);
}

std::vector<Video> VideoCatalog::searchByGenre(const std::string& genre) const {
    std::vector<Video> result;
    genreHelper(root, toLower(genre), result);
    return result;
}

// =============================================================
//  TRAVERSAL / ACCESS
// =============================================================

// In-order traversal — visits Left, Root, Right.
// Because BST is keyed on title, this returns videos in
// alphabetical order automatically. No sorting step needed.
void VideoCatalog::inOrderHelper(BSTNode* node,
                                  std::vector<Video>& result) const {
    if (node == nullptr) return;
    inOrderHelper(node->left,  result);
    result.push_back(node->data);
    inOrderHelper(node->right, result);
}

std::vector<Video> VideoCatalog::getAllSorted() const {
    std::vector<Video> result;
    inOrderHelper(root, result);
    return result;
}

// =============================================================
//  UTILITY
// =============================================================

int  VideoCatalog::getCount()  const { return nodeCount;    }
bool VideoCatalog::isEmpty()   const { return root == nullptr; }

// Existence check by videoId — O(n) traversal since we index by title.
// Acceptable for a university project; in production use a parallel hash set.
bool VideoCatalog::contains(const std::string& videoId) const {
    std::vector<Video> all = getAllSorted();
    for (const auto& v : all)
        if (toLower(v.videoId) == toLower(videoId)) return true;
    return false;
}

// =============================================================
//  DISPLAY
// =============================================================

void VideoCatalog::displayAll() const {
    std::cout << "\n";
    printVCDivider('=');
    std::cout << "  \xF0\x9F\x93\xBC  Video Catalog  ("
              << nodeCount << " video"
              << (nodeCount == 1 ? "" : "s")
              << " | alphabetical order)\n";
    printVCDivider('=');

    if (root == nullptr) {
        std::cout << "  Catalog is empty.\n";
        printVCDivider('=');
        std::cout << "\n";
        return;
    }

    std::vector<Video> sorted = getAllSorted();
    int rank = 1;
    for (const auto& v : sorted) {
        std::cout << "  "
                  << std::right << std::setw(3) << rank++ << ". "
                  << std::left  << std::setw(36) << v.title
                  << " [" << v.videoId << "]"
                  << "  " << std::setw(12) << v.genre
                  << "  Score: " << std::fixed
                  << std::setprecision(1) << v.engagementScore
                  << "\n";
    }

    printVCDivider('=');
    std::cout << "\n";
}

void VideoCatalog::displaySearchResults(const std::vector<Video>& results,
                                         const std::string& query) const {
    std::cout << "\n";
    printVCDivider();
    std::cout << "  \xF0\x9F\x94\x8D  Search: \"" << query
              << "\"  (" << results.size() << " result"
              << (results.size() == 1 ? "" : "s") << ")\n";
    printVCDivider();

    if (results.empty()) {
        std::cout << "  No videos found matching \"" << query << "\".\n";
    } else {
        int rank = 1;
        for (const auto& v : results) {
            std::cout << "  "
                      << std::right << std::setw(2) << rank++ << ". "
                      << std::left  << std::setw(36) << v.title
                      << " [" << v.videoId << "]"
                      << "  Genre: " << v.genre
                      << "\n";
        }
    }

    printVCDivider();
    std::cout << "\n";
}
