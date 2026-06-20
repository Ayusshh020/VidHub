// modules/VideoCatalog.cpp
// Purpose: Implementation of VideoCatalog BST operations, sorting by title and enabling search/filtering.

#include "VideoCatalog.h"
#include "../utils/Helpers.h"

#include <iostream>
#include <iomanip>
#include <cctype>

// Helper to print a visual divider line for the Video Catalog display
static void printVCDivider(char c = '-') {
    std::cout << "  " << std::string(66, c) << "\n";
}

// Constructor initializing root and node count
VideoCatalog::VideoCatalog()
    : root(nullptr), nodeCount(0)
{}

// Destructor trigger clearing the nodes in tree
VideoCatalog::~VideoCatalog() {
    clearHelper(root);
}

// Post-order traversal to delete all nodes recursively
void VideoCatalog::clearHelper(BSTNode* node) {
    if (node == nullptr) return;
    clearHelper(node->left);
    clearHelper(node->right);
    delete node;
}

// Recursive insert helper. Places node in BST alphabetically sorted by title.
VideoCatalog::BSTNode*
VideoCatalog::insertHelper(BSTNode* node, const Video& video) {
    if (node == nullptr) {
        nodeCount++;
        return new BSTNode(video);
    }

    // Skip duplicates (uniqueness enforcement by videoId)
    if (video.videoId == node->data.videoId)
        return node;

    // Alphabetical comparisons dictate tree placement
    if (video.title < node->data.title)
        node->left  = insertHelper(node->left,  video);
    else
        node->right = insertHelper(node->right, video);

    return node;
}

// Inserts a video into the catalog
void VideoCatalog::insert(const Video& video) {
    root = insertHelper(root, video);
}

// Returns the node with the minimum value (leftmost leaf in a subtree)
VideoCatalog::BSTNode*
VideoCatalog::getMinNode(BSTNode* node) const {
    while (node->left != nullptr)
        node = node->left;
    return node;
}

// Recursive delete helper to remove a node by videoId
VideoCatalog::BSTNode*
VideoCatalog::removeHelper(BSTNode* node, const std::string& videoId) {
    if (node == nullptr)
        return nullptr;

    // Node matches ID target
    if (toLower(videoId) == toLower(node->data.videoId)) {
        nodeCount--;

        // Case 1: Leaf node (no children)
        if (node->left == nullptr && node->right == nullptr) {
            delete node;
            return nullptr;
        }

        // Case 2: One child (right child only)
        if (node->left == nullptr) {
            BSTNode* temp = node->right;
            delete node;
            return temp;
        }

        // Case 2: One child (left child only)
        if (node->right == nullptr) {
            BSTNode* temp = node->left;
            delete node;
            return temp;
        }

        // Case 3: Two children
        // Find the in-order successor (minimum node in the right subtree)
        BSTNode* successor    = getMinNode(node->right);
        node->data            = successor->data; // Copy successor's value
        // Delete successor node
        node->right = removeHelper(node->right, successor->data.videoId);
        nodeCount++; // Adjust count since recursive call decremented it
        return node;
    }

    // Recursively search both subtrees since deletion is by videoId but tree is sorted by title
    node->left  = removeHelper(node->left,  videoId);
    node->right = removeHelper(node->right, videoId);
    return node;
}

// Deletes a video from the catalog
bool VideoCatalog::remove(const std::string& videoId) {
    int before = nodeCount;
    root = removeHelper(root, videoId);
    return nodeCount < before; // Returns true if a node was deleted
}

// Searches the tree by exact title using BST logic (O(log n))
VideoCatalog::BSTNode*
VideoCatalog::searchByTitle(BSTNode* node, const std::string& title) const {
    if (node == nullptr)              return nullptr;
    if (title == node->data.title)    return node;
    if (title  < node->data.title)    return searchByTitle(node->left,  title);
    return                                   searchByTitle(node->right, title);
}

// Returns exact matching video reference or nullptr
const Video* VideoCatalog::searchExact(const std::string& title) const {
    BSTNode* found = searchByTitle(root, title);
    if (found == nullptr) return nullptr;
    return &found->data;
}

// In-order traversal helper searching for titles matching keyword sub-strings
void VideoCatalog::keywordHelper(BSTNode* node,
                                  const std::string& keyword,
                                  std::vector<Video>& result) const {
    if (node == nullptr) return;
    keywordHelper(node->left, keyword, result);
    if (toLower(node->data.title).find(keyword) != std::string::npos)
        result.push_back(node->data);
    keywordHelper(node->right, keyword, result);
}

// Case-insensitive search by keyword
std::vector<Video> VideoCatalog::searchByKeyword(const std::string& keyword) const {
    std::vector<Video> result;
    keywordHelper(root, toLower(keyword), result);
    return result;
}

// In-order traversal helper searching for matching genre categories
void VideoCatalog::genreHelper(BSTNode* node,
                                const std::string& genre,
                                std::vector<Video>& result) const {
    if (node == nullptr) return;
    genreHelper(node->left, genre, result);
    if (toLower(node->data.genre) == genre)
        result.push_back(node->data);
    genreHelper(node->right, genre, result);
}

// Case-insensitive search by genre
std::vector<Video> VideoCatalog::searchByGenre(const std::string& genre) const {
    std::vector<Video> result;
    genreHelper(root, toLower(genre), result);
    return result;
}

// Traverse the BST in-order to construct a sorted list
void VideoCatalog::inOrderHelper(BSTNode* node,
                                  std::vector<Video>& result) const {
    if (node == nullptr) return;
    inOrderHelper(node->left,  result);
    result.push_back(node->data);
    inOrderHelper(node->right, result);
}

// Returns all catalog elements sorted alphabetically by title
std::vector<Video> VideoCatalog::getAllSorted() const {
    std::vector<Video> result;
    inOrderHelper(root, result);
    return result;
}

int  VideoCatalog::getCount()  const { return nodeCount;    }
bool VideoCatalog::isEmpty()   const { return root == nullptr; }

// Checks if catalog contains a specific video ID
bool VideoCatalog::contains(const std::string& videoId) const {
    std::vector<Video> all = getAllSorted();
    for (const auto& v : all)
        if (toLower(v.videoId) == toLower(videoId)) return true;
    return false;
}

// Displays all catalog items
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

// Prints catalog search results
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
