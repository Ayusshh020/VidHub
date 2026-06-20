// modules/DeliveryOptimizer.h
// Purpose: Header file defining the DeliveryOptimizer class, computing optimal server paths using Dijkstra.

#pragma once

#include <string>
#include <vector>
#include <unordered_map>
#include <limits>

/**
 * @class DeliveryOptimizer
 * @brief Computes optimal network paths between origin, PoPs, Edges, and clients.
 *
 * Models the CDN topology as a Weighted Graph. It implements Dijkstra's algorithm
 * to find the lowest-latency path (in milliseconds) from media sources to users,
 * supporting edge updates to simulate dynamic traffic congestion.
 */
class DeliveryOptimizer {
public:
    static constexpr int INF = std::numeric_limits<int>::max(); // Infinity representation for unreachable paths

private:
    int V; // Number of nodes in the graph

    std::vector<std::vector<std::pair<int, int>>> adjList; // Adjacency list storing pairs: {neighborNodeId, costMs}
    std::unordered_map<int, std::string> nodeNames;        // Helper mapping node ID to human-readable names

    /**
     * @brief Traverses the prev predecessor vector to reconstruct route path array.
     */
    std::vector<int> reconstructPath(const std::vector<int>& prev,
                                     int src, int dest) const;

public:
    /**
     * @brief Constructs DeliveryOptimizer for a network of nodeCount vertices.
     */
    explicit DeliveryOptimizer(int nodeCount);

    /**
     * @brief Maps a descriptive name to a node ID.
     */
    void setNodeName(int nodeId, const std::string& name);

    /**
     * @brief Adds a bidirectional link (edge) between two network nodes with latency.
     */
    void addEdge(int u, int v, int costMs);

    /**
     * @brief Updates the latency edge cost, or adds link if it doesn't exist.
     */
    void updateEdgeCost(int u, int v, int newCostMs);

    /**
     * @brief Computes the shortest latency path from src to dest using Dijkstra's algorithm.
     * @return Pair: {totalCostMs, vectorOfNodeIDsPath}
     */
    std::pair<int, std::vector<int>> dijkstra(int src, int dest) const;

    // Attribute getters
    std::string getNodeName(int nodeId) const;
    int         getNodeCount()          const;

    /**
     * @brief Prints the entire CDN latency matrix topology.
     */
    void displayGraph()                        const;

    /**
     * @brief Computes and prints optimal route details from source to destination.
     */
    void displayRoute(int src, int dest)       const;

    /**
     * @brief Computes and prints paths to all reachable destinations from a source.
     */
    void displayAllRoutes(int src)             const;
};
