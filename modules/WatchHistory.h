#pragma once

#include "../entities/Video.h"
#include <stack>
#include <string>

class WatchHistory {
private:
    std::stack<Video> history;
    int maxSize;

public:
    explicit WatchHistory(int maxSize = 50);

    void push(const Video& video);
    Video pop();
    const Video& peek() const;

    bool isEmpty() const;
    int getSize() const;
    void clear();

    void display() const;
};
