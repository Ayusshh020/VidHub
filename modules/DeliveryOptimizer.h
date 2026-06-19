#pragma once
// Note: Comments are only for understanding—do not modify the code itself.

#include <string>
#include <vector>
#include <unordered_map>
#include <limits>

class DeliveryOptimizer {
public:
    static constexpr int INF = std::numeric_limits<int>::max();
private:
    int V;

    std::vector<std::vector<std::pair<int, int>>> adjList;
    std::unordered_map<int, std::string> nodeNames;

    std::vector<int> reconstructPath(const std::vector<int>& prev,
                                     int src, int dest) const;

public:
    explicit DeliveryOptimizer(int nodeCount);

    void setNodeName(int nodeId, const std::string& name);
    void addEdge(int u, int v, int costMs);
    void updateEdgeCost(int u, int v, int newCostMs);

    std::pair<int, std::vector<int>> dijkstra(int src, int dest) const;

    std::string getNodeName(int nodeId) const;
    int         getNodeCount()          const;

    void displayGraph()                        const;
    void displayRoute(int src, int dest)       const;
    void displayAllRoutes(int src)             const;
};
