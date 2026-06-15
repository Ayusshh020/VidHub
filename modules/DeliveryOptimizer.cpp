#include "DeliveryOptimizer.h"

#include <iostream>
#include <iomanip>
#include <queue>
#include <vector>
#include <algorithm>

// ─────────────────────────────────────────────────────────────
//  File-local helpers
// ─────────────────────────────────────────────────────────────

static void printDODivider(char c = '-') {
    std::cout << "  " << std::string(66, c) << "\n";
}

// =============================================================
//  CONSTRUCTOR
// =============================================================

DeliveryOptimizer::DeliveryOptimizer(int nodeCount)
    : V(nodeCount), adjList(nodeCount)
{}

// =============================================================
//  GRAPH CONSTRUCTION
// =============================================================

void DeliveryOptimizer::setNodeName(int nodeId, const std::string& name) {
    nodeNames[nodeId] = name;
}

// Add undirected edge: both u→v and v→u with the same cost
void DeliveryOptimizer::addEdge(int u, int v, int costMs) {
    adjList[u].push_back({v, costMs});
    adjList[v].push_back({u, costMs});
}

// Update cost of edge u↔v in both directions.
// If edge doesn't exist, adds it.
void DeliveryOptimizer::updateEdgeCost(int u, int v, int newCostMs) {
    bool updated = false;
    for (auto& edge : adjList[u]) {
        if (edge.first == v) { edge.second = newCostMs; updated = true; break; }
    }
    for (auto& edge : adjList[v]) {
        if (edge.first == u) { edge.second = newCostMs; break; }
    }
    if (!updated) addEdge(u, v, newCostMs);  // Add if not found
}

// =============================================================
//  DIJKSTRA'S ALGORITHM
//
//  Greedy shortest-path algorithm using a min-heap.
//
//  Data structures:
//    dist[]  : shortest known distance from src to each node
//    prev[]  : previous node on the shortest path (for reconstruction)
//    pq      : min-heap of {cost, nodeId} — always pops cheapest
//
//  Steps:
//    1. Initialize dist[src] = 0, all others = INF.
//    2. Push {0, src} into the min-heap.
//    3. Pop the node with the smallest current distance.
//    4. For each neighbour v of current node u:
//         If dist[u] + edge(u,v) < dist[v]:
//             Update dist[v] and prev[v].
//             Push {dist[v], v} into the heap.
//    5. Repeat until heap is empty or dest is reached.
//    6. Reconstruct path using prev[].
//
//  Time: O((V + E) log V)
// =============================================================

std::pair<int, std::vector<int>>
DeliveryOptimizer::dijkstra(int src, int dest) const {
    std::vector<int> dist(V, INF);
    std::vector<int> prev(V, -1);

    // Min-heap: {cost, nodeId}
    // priority_queue is max-heap by default → negate or use greater<>
    using pii = std::pair<int, int>;
    std::priority_queue<pii, std::vector<pii>, std::greater<pii>> pq;

    dist[src] = 0;
    pq.push({0, src});

    while (!pq.empty()) {
        auto topNode = pq.top();
        int currentCost = topNode.first;
        int u = topNode.second;
        pq.pop();

        // Skip stale entries (a shorter path was already found)
        if (currentCost > dist[u]) continue;

        // Early exit — we've settled the destination
        if (u == dest) break;

        // Relax all edges from u
        for (const auto& edge : adjList[u]) {
            int v = edge.first;
            int edgeCost = edge.second;
            int newDist = dist[u] + edgeCost;
            if (newDist < dist[v]) {
                dist[v] = newDist;
                prev[v] = u;
                pq.push({newDist, v});
            }
        }
    }

    // No path found
    if (dist[dest] == INF)
        return {-1, {}};

    // Reconstruct path: trace back from dest using prev[]
    std::vector<int> path = reconstructPath(prev, src, dest);
    return {dist[dest], path};
}

// Trace back from dest to src using prev[], then reverse.
std::vector<int>
DeliveryOptimizer::reconstructPath(const std::vector<int>& prev,
                                    int src, int dest) const {
    std::vector<int> path;
    for (int at = dest; at != -1; at = prev[at])
        path.push_back(at);
    std::reverse(path.begin(), path.end());

    // Verify path starts at src (sanity check)
    if (path.front() != src) return {};
    return path;
}

// =============================================================
//  UTILITY
// =============================================================

std::string DeliveryOptimizer::getNodeName(int nodeId) const {
    auto it = nodeNames.find(nodeId);
    if (it != nodeNames.end()) return it->second;
    return "Node-" + std::to_string(nodeId);
}

int DeliveryOptimizer::getNodeCount() const { return V; }

// =============================================================
//  DISPLAY
// =============================================================

void DeliveryOptimizer::displayGraph() const {
    std::cout << "\n";
    printDODivider('=');
    std::cout << "  \xF0\x9F\x8C\x90  CDN Network Graph  (" << V << " nodes)\n";
    printDODivider('=');

    for (int u = 0; u < V; ++u) {
        std::cout << "  " << std::left << std::setw(20) << getNodeName(u) << " \xE2\x86\x94  ";
        if (adjList[u].empty()) {
            std::cout << "(isolated)\n";
            continue;
        }
        for (int i = 0; i < static_cast<int>(adjList[u].size()); ++i) {
            if (i > 0) std::cout << ", ";
            std::cout << getNodeName(adjList[u][i].first)
                      << " [" << adjList[u][i].second << "ms]";
        }
        std::cout << "\n";
    }
    printDODivider('=');
    std::cout << "\n";
}

// Run Dijkstra and display the result beautifully
void DeliveryOptimizer::displayRoute(int src, int dest) const {
    std::cout << "\n";
    printDODivider('=');
    std::cout << "  \xF0\x9F\x9A\x80  Optimal Delivery Route\n";
    std::cout << "  From : " << getNodeName(src)  << "\n";
    std::cout << "  To   : " << getNodeName(dest) << "\n";
    printDODivider('=');

    auto result = dijkstra(src, dest);
    int cost = result.first;
    std::vector<int> path = result.second;

    if (cost == -1) {
        std::cout << "  \xE2\x9D\x8C  No route found between these nodes.\n";
        printDODivider('=');
        std::cout << "\n";
        return;
    }

    // Print path as a chain
    std::cout << "  Route: ";
    for (int i = 0; i < static_cast<int>(path.size()); ++i) {
        if (i > 0) std::cout << " \xE2\x86\x92 ";
        std::cout << getNodeName(path[i]);
    }
    std::cout << "\n";
    std::cout << "  Hops : " << (path.size() - 1) << "\n";
    std::cout << "  Cost : " << cost << " ms  \xE2\x9C\x94 Optimal\n";

    printDODivider('=');
    std::cout << "\n";
}

// Show cheapest route from src to every other node
void DeliveryOptimizer::displayAllRoutes(int src) const {
    std::cout << "\n";
    printDODivider('=');
    std::cout << "  \xF0\x9F\x9B\xA3  All Routes from: " << getNodeName(src) << "\n";
    printDODivider('=');
    std::cout << "  " << std::left << std::setw(22) << "Destination"
              << std::setw(10) << "Cost"
              << "Path\n";
    printDODivider();

    for (int dest = 0; dest < V; ++dest) {
        if (dest == src) continue;
        auto result = dijkstra(src, dest);
        int cost = result.first;
        std::vector<int> path = result.second;

        std::cout << "  " << std::left << std::setw(22) << getNodeName(dest);
        if (cost == -1) {
            std::cout << std::setw(10) << "N/A" << "No route\n";
        } else {
            std::cout << std::setw(8) << cost << "ms  ";
            for (int i = 0; i < static_cast<int>(path.size()); ++i) {
                if (i > 0) std::cout << "\xE2\x86\x92";
                std::cout << getNodeName(path[i]);
            }
            std::cout << "\n";
        }
    }
    printDODivider('=');
    std::cout << "\n";
}
