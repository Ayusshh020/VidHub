#pragma once

#include "../entities/Video.h"
#include <queue>
#include <vector>
#include <string>
#include <unordered_map>

struct VideoScoreComparator {
    bool operator()(const Video& a, const Video& b) const {
        return a.engagementScore < b.engagementScore;
    }
};

using MaxHeap = std::priority_queue<Video,
                                    std::vector<Video>,
                                    VideoScoreComparator>;

class RankingSystem {
private:
    std::unordered_map<std::string, Video> videoStore;

    mutable MaxHeap heap;
    mutable bool    heapDirty;

    void rebuildHeap() const;

public:
    RankingSystem();

    void addVideo(const Video& video);
    bool updateVideo(const Video& updatedVideo);
    bool removeVideo(const std::string& videoId);

    const Video& peekTop() const;
    std::vector<Video> getTopK(int k) const;

    int  getCount()                          const;
    bool isEmpty()                           const;
    bool contains(const std::string& videoId)const;

    void displayRankings(int topK = 10) const;
};
