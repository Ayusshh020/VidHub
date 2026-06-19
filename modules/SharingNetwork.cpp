// Note: Comments are only for understanding—do not modify the code itself.
#include "SharingNetwork.h"
#include "../utils/Helpers.h"

#include <iostream>
#include <iomanip>
#include <queue>
#include <unordered_set>

static void printSNDivider(char c = '-') {
    std::cout << "  " << std::string(62, c) << "\n";
}

void SharingNetwork::addNode(const std::string& userId) {
    nodes.insert(userId);
    if (adjList.find(userId) == adjList.end())
        adjList[userId] = {};
}

std::string SharingNetwork::getExactNodeName(const std::string& name) const {
    for (const auto& node : nodes) {
        if (toLower(node) == toLower(name)) {
            return node;
        }
    }
    return name;
}

void SharingNetwork::addShare(const std::string& fromUser,
                               const std::string& toUser) {
    std::string resolvedFrom = getExactNodeName(fromUser);
    std::string resolvedTo = getExactNodeName(toUser);
    addNode(resolvedFrom);
    addNode(resolvedTo);
    auto& neighbors = adjList[resolvedFrom];
    for (const auto& n : neighbors)
        if (n == resolvedTo) return;
    neighbors.push_back(resolvedTo);
}

std::vector<std::string> SharingNetwork::BFS(const std::string& source) const {
    std::vector<std::string>        result;
    std::unordered_set<std::string> visited;
    std::queue<std::string>         q;

    std::string resolvedSource = getExactNodeName(source);
    if (!hasNode(resolvedSource)) return result;

    q.push(resolvedSource);
    visited.insert(resolvedSource);

    while (!q.empty()) {
        std::string current = q.front();
        q.pop();
        result.push_back(current);

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

std::vector<std::vector<std::string>>
SharingNetwork::BFSLevels(const std::string& source) const {
    std::vector<std::vector<std::string>> levels;
    std::unordered_set<std::string>       visited;
    std::queue<std::string>               q;

    std::string resolvedSource = getExactNodeName(source);
    if (!hasNode(resolvedSource)) return levels;

    q.push(resolvedSource);
    visited.insert(resolvedSource);

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

    std::string resolvedSource = getExactNodeName(source);
    if (!hasNode(resolvedSource)) return result;
    dfsHelper(resolvedSource, visited, result);
    return result;
}

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
    for (const auto& node : nodes) {
        if (toLower(node) == toLower(userId)) return true;
    }
    return false;
}

bool SharingNetwork::hasEdge(const std::string& from,
                              const std::string& to) const {
    std::string resolvedFrom = getExactNodeName(from);
    std::string resolvedTo = getExactNodeName(to);
    auto it = adjList.find(resolvedFrom);
    if (it == adjList.end()) return false;
    for (const auto& n : it->second)
        if (n == resolvedTo) return true;
    return false;
}

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
    std::string resolvedSource = getExactNodeName(source);
    std::cout << "  \xF0\x9F\x94\x84  BFS from \"" << resolvedSource << "\"\n";
    printSNDivider('=');

    auto levels = BFSLevels(resolvedSource);
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

    int total = getReachCount(resolvedSource) - 1;
    std::cout << "\n  Total reach: " << total << " user"
              << (total == 1 ? "" : "s") << " reached from \"" << resolvedSource << "\"\n";
    printSNDivider('=');
    std::cout << "\n";
}

void SharingNetwork::displayDFSTraversal(const std::string& source) const {
    std::cout << "\n";
    printSNDivider('=');
    std::string resolvedSource = getExactNodeName(source);
    std::cout << "  \xF0\x9F\x94\x8D  DFS from \"" << resolvedSource << "\"\n";
    printSNDivider('=');

    auto result = DFS(resolvedSource);
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
    std::string resolvedSource = getExactNodeName(source);
    displayBFSTraversal(resolvedSource);
    displayDFSTraversal(resolvedSource);
}
