// Note: Comments are only for understanding—do not modify the code itself.
#include "SharingNetwork.h"

#include <iostream>
#include <iomanip>
#include <queue>
#include <unordered_set>

static void printSNDivider(char c = '-') {
    std::cout << "  " << std::string(62, c) << "\n";
}

// =============================================================
//  GRAPH CONSTRUCTION
// =============================================================

void SharingNetwork::addNode(const std::string& userId) {
    nodes.insert(userId);
    // Ensure the node has an entry in adjList (even if no edges)
    if (adjList.find(userId) == adjList.end())
        adjList[userId] = {};
}

// Record a directed share edge: fromUser → toUser.
// Both nodes are auto-registered if not present.
void SharingNetwork::addShare(const std::string& fromUser,
                               const std::string& toUser) {
    addNode(fromUser);
    addNode(toUser);
    // Prevent duplicate edges
    auto& neighbors = adjList[fromUser];
    for (const auto& n : neighbors)
        if (n == toUser) return;   // Edge already exists
    neighbors.push_back(toUser);
}

// Performs Breadth-First Search (BFS) starting from the given source node.

std::vector<std::string> SharingNetwork::BFS(const std::string& source) const {
    std::vector<std::string>        result;
    std::unordered_set<std::string> visited;
    std::queue<std::string>         q;

    if (!hasNode(source)) return result;

    q.push(source);
    visited.insert(source);

    while (!q.empty()) {
        std::string current = q.front();
        q.pop();
        result.push_back(current);

        // Enqueue all unvisited neighbours
        auto it = adjList.find(current);
        if (it != adjList.end()) {
            for (const auto& neighbor : it->second) {
                if (visited.find(neighbor) == visited.end()) {
                    visited.insert(neighbor);
                    q.push(neighbor);
                }
            }
        }
    }
    return result;
}

// BFS with level separation — each inner vector is one hop away.
// Level 0 = source, Level 1 = direct shares, Level 2 = reshares...
std::vector<std::vector<std::string>>
SharingNetwork::BFSLevels(const std::string& source) const {
    std::vector<std::vector<std::string>> levels;
    std::unordered_set<std::string>       visited;
    std::queue<std::string>               q;

    if (!hasNode(source)) return levels;

    q.push(source);
    visited.insert(source);

    while (!q.empty()) {
        int levelSize = static_cast<int>(q.size());
        std::vector<std::string> currentLevel;

        for (int i = 0; i < levelSize; ++i) {
            std::string node = q.front();
            q.pop();
            currentLevel.push_back(node);

            auto it = adjList.find(node);
            if (it != adjList.end()) {
                for (const auto& neighbor : it->second) {
                    if (visited.find(neighbor) == visited.end()) {
                        visited.insert(neighbor);
                        q.push(neighbor);
                    }
                }
            }
        }
        levels.push_back(currentLevel);
    }
    return levels;
}

// Recursively traverses the network using Depth-First Search (DFS).

void SharingNetwork::dfsHelper(const std::string& node,
                                std::unordered_set<std::string>& visited,
                                std::vector<std::string>& result) const {
    visited.insert(node);
    result.push_back(node);

    auto it = adjList.find(node);
    if (it != adjList.end()) {
        for (const auto& neighbor : it->second) {
            if (visited.find(neighbor) == visited.end())
                dfsHelper(neighbor, visited, result);
        }
    }
}

std::vector<std::string> SharingNetwork::DFS(const std::string& source) const {
    std::vector<std::string>        result;
    std::unordered_set<std::string> visited;

    if (!hasNode(source)) return result;
    dfsHelper(source, visited, result);
    return result;
}

// =============================================================
//  ANALYTICS
// =============================================================

// Total nodes reachable from source (including source itself)
int SharingNetwork::getReachCount(const std::string& source) const {
    return static_cast<int>(BFS(source).size());
}

int SharingNetwork::getNodeCount() const {
    return static_cast<int>(nodes.size());
}

int SharingNetwork::getEdgeCount() const {
    int total = 0;
    for (const auto& pair : adjList)
        total += static_cast<int>(pair.second.size());
    return total;
}

bool SharingNetwork::hasNode(const std::string& userId) const {
    return nodes.count(userId) > 0;
}

bool SharingNetwork::hasEdge(const std::string& from,
                              const std::string& to) const {
    auto it = adjList.find(from);
    if (it == adjList.end()) return false;
    for (const auto& n : it->second)
        if (n == to) return true;
    return false;
}

// =============================================================
//  DISPLAY
// =============================================================

void SharingNetwork::displayNetwork() const {
    std::cout << "\n";
    printSNDivider('=');
    std::cout << "  \xF0\x9F\x8C\x90  Sharing Network  ("
              << getNodeCount() << " users, "
              << getEdgeCount() << " shares)\n";
    printSNDivider('=');

    for (const auto& node : nodes) {
        std::cout << "  " << std::left << std::setw(16) << node << " \xE2\x86\x92  ";
        auto it = adjList.find(node);
        if (it == adjList.end() || it->second.empty()) {
            std::cout << "(no outgoing shares)\n";
        } else {
            for (int i = 0; i < static_cast<int>(it->second.size()); ++i) {
                if (i > 0) std::cout << ", ";
                std::cout << it->second[i];
            }
            std::cout << "\n";
        }
    }
    printSNDivider('=');
    std::cout << "\n";
}

void SharingNetwork::displayBFSTraversal(const std::string& source) const {
    std::cout << "\n";
    printSNDivider('=');
    std::cout << "  \xF0\x9F\x94\x84  BFS from \"" << source << "\"\n";
    printSNDivider('=');

    auto levels = BFSLevels(source);
    if (levels.empty()) {
        std::cout << "  Node \"" << source << "\" not found.\n";
        printSNDivider('=');
        return;
    }

    for (int lvl = 0; lvl < static_cast<int>(levels.size()); ++lvl) {
        std::cout << "  Level " << lvl
                  << (lvl == 0 ? " [Origin]" : " [Hop " + std::to_string(lvl) + "]")
                  << ": ";
        for (int i = 0; i < static_cast<int>(levels[lvl].size()); ++i) {
            if (i > 0) std::cout << " \xE2\x86\x92 ";
            std::cout << levels[lvl][i];
        }
        std::cout << "\n";
    }

    int total = getReachCount(source) - 1;  // Exclude source itself
    std::cout << "\n  Total reach: " << total << " user"
              << (total == 1 ? "" : "s") << " reached from \"" << source << "\"\n";
    printSNDivider('=');
    std::cout << "\n";
}

void SharingNetwork::displayDFSTraversal(const std::string& source) const {
    std::cout << "\n";
    printSNDivider('=');
    std::cout << "  \xF0\x9F\x94\x8D  DFS from \"" << source << "\"\n";
    printSNDivider('=');

    auto result = DFS(source);
    if (result.empty()) {
        std::cout << "  Node \"" << source << "\" not found.\n";
    } else {
        std::cout << "  Visit order: ";
        for (int i = 0; i < static_cast<int>(result.size()); ++i) {
            if (i > 0) std::cout << " \xE2\x86\x92 ";
            std::cout << result[i];
        }
        std::cout << "\n";
    }
    printSNDivider('=');
    std::cout << "\n";
}

void SharingNetwork::displayReachAnalysis(const std::string& source) const {
    displayBFSTraversal(source);
    displayDFSTraversal(source);
}
