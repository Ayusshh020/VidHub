#pragma once

#include <string>
#include <unordered_map>
#include <unordered_set>
#include <vector>
#include <queue>

// =============================================================
//  SharingNetwork — Models how videos spread across users.
//
//  DSA: Directed Graph (Adjacency List) + BFS + DFS
//
//  Why Graph?
//    Sharing is inherently a network: nodes = users/platforms,
//    edges = "A shared to B". No other structure models this.
//
//  Representation: Adjacency List
//    unordered_map<string, vector<string>>
//    Key   = userId (source node)
//    Value = list of userIds this node shared to
//
//  Why adjacency list over adjacency matrix?
//    Social graphs are SPARSE — most users share to only a few
//    others. Matrix would waste O(V²) space for near-empty rows.
//    List uses O(V + E) — far more efficient.
//
//  Algorithms:
//    BFS — Level-by-level spread (shortest share path)
//           Real use: "How many hops to reach this user?"
//    DFS — Deep exploration of the share tree
//           Real use: "Find all communities that got this video"
//
//  Owned by: ConsoleUI (one global instance for the platform)
// =============================================================

class SharingNetwork {
private:
    // Adjacency list: fromUser → list of toUsers
    std::unordered_map<std::string, std::vector<std::string>> adjList;

    // Track all node names (some may have no outgoing edges)
    std::unordered_set<std::string> nodes;

    // Private DFS helper (recursive)
    void dfsHelper(const std::string& node,
                   std::unordered_set<std::string>& visited,
                   std::vector<std::string>& result) const;

public:
    SharingNetwork() = default;

    // ── Graph Construction ────────────────────────────────────
    void addNode(const std::string& userId);   // Register a user

    // Record that 'fromUser' shared a video to 'toUser'.
    // Automatically adds both nodes if not already present.
    void addShare(const std::string& fromUser,
                  const std::string& toUser);

    // ── BFS ──────────────────────────────────────────────────
    // Breadth-First Search from 'source'.
    // Returns nodes in BFS visit order (level by level).
    // Time: O(V + E)
    std::vector<std::string> BFS(const std::string& source) const;

    // Returns BFS levels separately — each inner vector is one hop
    std::vector<std::vector<std::string>>
        BFSLevels(const std::string& source) const;

    // ── DFS ──────────────────────────────────────────────────
    // Depth-First Search from 'source' (iterative + recursive).
    // Returns nodes in DFS visit order.
    // Time: O(V + E)
    std::vector<std::string> DFS(const std::string& source) const;

    // ── Analytics ────────────────────────────────────────────
    int  getReachCount(const std::string& source) const; // Total reachable
    int  getNodeCount()  const;
    int  getEdgeCount()  const;
    bool hasNode(const std::string& userId)  const;
    bool hasEdge(const std::string& from,
                 const std::string& to)      const;

    // ── Display ──────────────────────────────────────────────
    void displayNetwork()               const; // Adjacency list dump
    void displayBFSTraversal(const std::string& source) const;
    void displayDFSTraversal(const std::string& source) const;
    void displayReachAnalysis(const std::string& source) const;
};
