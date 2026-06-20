// modules/SharingNetwork.h
// Purpose: Header file defining the SharingNetwork class, modeling social sharing paths using a Directed Graph.

#pragma once

#include <string>
#include <unordered_map>
#include <unordered_set>
#include <vector>
#include <queue>

/**
 * @class SharingNetwork
 * @brief Represents a sharing network modeled as a Directed Graph.
 *
 * Users/nodes represent platforms or viewers, and directed edges represent shares
 * (e.g. from user A to user B). Supports BFS (reach analysis/level tracking) and DFS traversal.
 */
class SharingNetwork {
private:
    std::unordered_map<std::string, std::vector<std::string>> adjList; // Adjacency list representing directed sharing edges
    std::unordered_set<std::string> nodes;                            // Set of all unique node/user IDs registered in the network

    /**
     * @brief Recursive helper method to perform Depth First Search (DFS).
     */
    void dfsHelper(const std::string& node,
                   std::unordered_set<std::string>& visited,
                   std::vector<std::string>& result) const;

    /**
     * @brief Resolves case-insensitivity or partial user names to exact node keys.
     */
    std::string getExactNodeName(const std::string& name) const;

public:
    SharingNetwork() = default;

    /**
     * @brief Explicitly registers a user/node in the network graph.
     */
    void addNode(const std::string& userId);

    /**
     * @brief Creates a directed edge representing content sharing from fromUser to toUser.
     */
    void addShare(const std::string& fromUser,
                  const std::string& toUser);

    /**
     * @brief Performs Breadth First Search (BFS) starting from a source node.
     */
    std::vector<std::string> BFS(const std::string& source) const;

    /**
     * @brief Groups BFS traversal nodes by hop level distance from the source.
     */
    std::vector<std::vector<std::string>> BFSLevels(const std::string& source) const;

    /**
     * @brief Performs Depth First Search (DFS) starting from a source node.
     */
    std::vector<std::string> DFS(const std::string& source) const;

    // Analytics and query checks
    int  getReachCount(const std::string& source) const;
    int  getNodeCount()  const;
    int  getEdgeCount()  const;
    bool hasNode(const std::string& userId)  const;
    bool hasEdge(const std::string& from,
                 const std::string& to)      const;

    /**
     * @brief Prints the adjacency list representation of the sharing graph.
     */
    void displayNetwork()               const;

    /**
     * @brief Prints the step-by-step BFS traversal showing reach tiers.
     */
    void displayBFSTraversal(const std::string& source) const;

    /**
     * @brief Prints the DFS traversal path.
     */
    void displayDFSTraversal(const std::string& source) const;

    /**
     * @brief Displays side-by-side BFS and DFS analyses for standard network audits.
     */
    void displayReachAnalysis(const std::string& source) const;
};
