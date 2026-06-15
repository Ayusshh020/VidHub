#pragma once

#include <string>
#include <vector>
#include <unordered_map>
#include <limits>

// =============================================================
//  DeliveryOptimizer — Finds the cheapest CDN delivery route.
//
//  DSA: Weighted Undirected Graph + Dijkstra's Algorithm
//
//  Why Dijkstra's?
//    CDN delivery is a classic shortest-path problem:
//      Nodes = CDN servers / Points of Presence (PoPs)
//      Edges = network connections with cost (latency in ms)
//    Dijkstra's is optimal for non-negative weighted graphs —
//    which all CDN latency graphs are. It guarantees the
//    minimum cost path from any source to any destination.
//
//  Representation:
//    Adjacency list of pairs: vector<pair<int nodeId, int cost>>
//    Node names stored separately in a name map.
//
//  Algorithm:
//    Uses a min-heap (priority_queue) for the greedy selection.
//    Relaxes edges: if cost(u→v) < known cost(v), update.
//    Tracks 'prev' array to reconstruct the actual path.
//
//  Time: O((V + E) log V) with binary heap.
//
//  Owned by: ConsoleUI (one global instance for the platform)
// =============================================================

class DeliveryOptimizer {
public:
    // Constant for "no path" / infinity
    // Defined inside the class as static constexpr for pre-C++17 compatibility.
    static constexpr int INF = std::numeric_limits<int>::max();
private:
    int V;  // Number of CDN nodes

    // Adjacency list: adjList[u] = { {v, cost}, {w, cost}, ... }
    std::vector<std::vector<std::pair<int, int>>> adjList;

    // Human-readable node names (e.g. "Mumbai-Origin")
    std::unordered_map<int, std::string> nodeNames;

    // Helper: reconstruct path from prev[] array
    std::vector<int> reconstructPath(const std::vector<int>& prev,
                                     int src, int dest) const;

public:
    // ── Constructor ───────────────────────────────────────────
    // Pass the total number of CDN nodes upfront.
    explicit DeliveryOptimizer(int nodeCount);

    // ── Graph Construction ────────────────────────────────────
    // Assign a human-readable name to a node ID (0-based).
    void setNodeName(int nodeId, const std::string& name);

    // Add a bidirectional edge (undirected: u↔v) with given cost.
    void addEdge(int u, int v, int costMs);

    // Update the cost of an existing edge (e.g., on congestion).
    // Both directions are updated (undirected graph).
    void updateEdgeCost(int u, int v, int newCostMs);

    // ── Dijkstra's Algorithm ──────────────────────────────────
    // Find shortest path from src to dest.
    // Returns:
    //   .first  = total cost (-1 if no path exists)
    //   .second = sequence of node IDs from src to dest
    std::pair<int, std::vector<int>>
        dijkstra(int src, int dest) const;

    // ── Utility ──────────────────────────────────────────────
    std::string getNodeName(int nodeId) const;
    int         getNodeCount()          const;

    // ── Display ──────────────────────────────────────────────
    void displayGraph()                        const; // Print all edges
    void displayRoute(int src, int dest)       const; // Run + show result
    void displayAllRoutes(int src)             const; // src → all nodes
};
