#pragma once

#include <string>
#include <unordered_map>
#include <unordered_set>
#include <vector>
#include <queue>

class SharingNetwork {
private:
    std::unordered_map<std::string, std::vector<std::string>> adjList;
    std::unordered_set<std::string> nodes;

    void dfsHelper(const std::string& node,
                   std::unordered_set<std::string>& visited,
                   std::vector<std::string>& result) const;

    std::string getExactNodeName(const std::string& name) const;

public:
    SharingNetwork() = default;

    void addNode(const std::string& userId);
    void addShare(const std::string& fromUser,
                  const std::string& toUser);

    std::vector<std::string> BFS(const std::string& source) const;
    std::vector<std::vector<std::string>> BFSLevels(const std::string& source) const;
    std::vector<std::string> DFS(const std::string& source) const;

    int  getReachCount(const std::string& source) const;
    int  getNodeCount()  const;
    int  getEdgeCount()  const;
    bool hasNode(const std::string& userId)  const;
    bool hasEdge(const std::string& from,
                 const std::string& to)      const;

    void displayNetwork()               const;
    void displayBFSTraversal(const std::string& source) const;
    void displayDFSTraversal(const std::string& source) const;
    void displayReachAnalysis(const std::string& source) const;
};
