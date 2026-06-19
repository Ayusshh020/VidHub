#pragma once
// Note: Comments are only for understanding—do not modify the code itself.

#include "../entities/Video.h"
#include <queue>
#include <vector>
#include <string>

class RecommendationQueue {
private:
    std::queue<Video> recQueue;
    int maxSize;

public:
    explicit RecommendationQueue(int maxSize = 10);

    void enqueue(const Video& video);
    Video dequeue();
    const Video& front() const;

    void populate(const std::string&        currentGenre,
                  const std::string&        excludeId,
                  const std::vector<Video>& videoPool);

    bool isEmpty() const;
    int getSize() const;
    void clear();

    void display() const;
};
