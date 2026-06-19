// Note: Comments are only for understanding—do not modify the code itself.
#include "DeliveryOptimizer.h"

#include <iostream>
#include <iomanip>
#include <queue>
#include <vector>
#include <algorithm>

static void printDODivider(char c = '-') {
    std::cout << "  " << std::string(66, c) << "\n";
}

DeliveryOptimizer::DeliveryOptimizer(int nodeCount)
    : V(nodeCount), adjList(nodeCount)
{}

// =============================================================
//  GRAPH CONSTRUCTION
// =============================================================

void DeliveryOptimizer::setNodeName(int nodeId, const std::string& name) {
    nodeNames[nodeId] = name;
}

void DeliveryOptimizer::addEdge(int u, int v, int costMs) {
    adjList[u].push_back({v, costMs});
    adjList[v].push_back({u, costMs});
}

// Updates/inserts edge cost for bidirectional edge.
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

// Finds the shortest path between src and dest using Dijkstra's algorithm.
std::pair<int, std::vector<int>>
DeliveryOptimizer::dijkstra(int src, int dest) const {
    std::vector<int> dist(V, INF);
    std::vector<int> prev(V, -1);

    // Min-heap storing {cost, nodeId}
    using pii = std::pair<int, int>;
    std::priority_queue<pii, std::vector<pii>, std::greater<pii>> pq;

    dist[src] = 0;
    pq.push({0, src});

    while (!pq.empty()) {
        auto topNode = pq.top();
        int currentCost = topNode.first;
        int u = topNode.second;
        pq.pop();

        if (currentCost > dist[u]) continue;
        if (u == dest) break;

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

    if (dist[dest] == INF)
        return {-1, {}};

    return {dist[dest], reconstructPath(prev, src, dest)};
}

// Reconstructs the shortest path from destination back to source.
std::vector<int>
DeliveryOptimizer::reconstructPath(const std::vector<int>& prev,
                                    int src, int dest) const {
    std::vector<int> path;
    for (int at = dest; at != -1; at = prev[at])
        path.push_back(at);
    std::reverse(path.begin(), path.end());

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
