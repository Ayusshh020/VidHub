// modules/DeliveryOptimizer.cpp
// Purpose: Implementation of the DeliveryOptimizer weighted graph class, using Dijkstra's algorithm for low-latency routing.

#include "DeliveryOptimizer.h"

#include <iostream>
#include <iomanip>
#include <queue>
#include <vector>
#include <algorithm>

// Helper to print a visual divider line for routing outputs
static void printDODivider(char c = '-') {
    std::cout << "  " << std::string(66, c) << "\n";
}

// Constructor mapping number of nodes and sizing adjacency list
DeliveryOptimizer::DeliveryOptimizer(int nodeCount)
    : V(nodeCount), adjList(nodeCount)
{}

// Binds descriptive labels (e.g. "Mumbai-Origin") to numerical node index IDs
void DeliveryOptimizer::setNodeName(int nodeId, const std::string& name) {
    nodeNames[nodeId] = name;
}

// Registers a bidirectional edge connecting node u and v with a specific latency cost
void DeliveryOptimizer::addEdge(int u, int v, int costMs) {
    adjList[u].push_back({v, costMs});
    adjList[v].push_back({u, costMs});
}

// Updates cost of an existing edge. If link is not found, adds it.
void DeliveryOptimizer::updateEdgeCost(int u, int v, int newCostMs) {
    bool updated = false;
    for (auto& edge : adjList[u]) {
        if (edge.first == v) { edge.second = newCostMs; updated = true; break; }
    }
    for (auto& edge : adjList[v]) {
        if (edge.first == u) { edge.second = newCostMs; break; }
    }
    if (!updated) addEdge(u, v, newCostMs);
}

// Computes shortest latency paths from a single source node to dest using Dijkstra's Algorithm (O((V+E) log V))
std::pair<int, std::vector<int>>
DeliveryOptimizer::dijkstra(int src, int dest) const {
    std::vector<int> dist(V, INF); // Shortest distance array initialized to infinity
    std::vector<int> prev(V, -1);  // Path predecessor map tracking routing trail

    // Min-heap priority queue tracking pairs: {costSoFar, nodeId}
    using pii = std::pair<int, int>;
    std::priority_queue<pii, std::vector<pii>, std::greater<pii>> pq;

    dist[src] = 0;
    pq.push({0, src});

    while (!pq.empty()) {
        auto topNode = pq.top();
        int currentCost = topNode.first;
        int u = topNode.second;
        pq.pop();

        // Skip processing if a cheaper path to node u has already been registered
        if (currentCost > dist[u]) continue;
        if (u == dest) break; // Optimization: exit early when destination is resolved

        // Scan all outgoing/neighbor edges
        for (const auto& edge : adjList[u]) {
            int v = edge.first;
            int edgeCost = edge.second;
            int newDist = dist[u] + edgeCost;

            // Relaxation step: if a shorter route to neighbor v is found
            if (newDist < dist[v]) {
                dist[v] = newDist;
                prev[v] = u;
                pq.push({newDist, v});
            }
        }
    }

    if (dist[dest] == INF)
        return {-1, {}}; // Returns -1 and empty path if destination is unreachable

    return {dist[dest], reconstructPath(prev, src, dest)};
}

// Traces the predecessor array backward from dest to src to construct the path vector
std::vector<int>
DeliveryOptimizer::reconstructPath(const std::vector<int>& prev,
                                    int src, int dest) const {
    std::vector<int> path;
    for (int at = dest; at != -1; at = prev[at])
        path.push_back(at);
    std::reverse(path.begin(), path.end());

    if (path.front() != src) return {}; // Confirms path reaches the source node
    return path;
}

// Accesses node labels, returning placeholder names if labels are not found
std::string DeliveryOptimizer::getNodeName(int nodeId) const {
    auto it = nodeNames.find(nodeId);
    if (it != nodeNames.end()) return it->second;
    return "Node-" + std::to_string(nodeId);
}

int DeliveryOptimizer::getNodeCount() const { return V; }

// Prints the graph showing nodes, edge connections, and link latencies
void DeliveryOptimizer::displayGraph() const {
    std::cout << "\n";
    printDODivider('=');
    std::cout << "  \xF0\x9F\x8C\x90  CDN Network Graph  (" << V << " nodes)\n";
    printDODivider('=');

    for (int u = 0; u < V; ++u) {
        std::cout << "  " << std::left << std::setw(20) << getNodeName(u) << " \xE2\x86\x92  ";
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

// Solves and prints optimal path and latency cost details for a src → dest request
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

// Displays latency cost and path vectors for all possible node destinations from src node
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
