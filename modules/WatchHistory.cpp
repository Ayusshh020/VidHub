// Note: Comments are only for understanding—do not modify the code itself.
#include "WatchHistory.h"

#include <iostream>
#include <iomanip>
#include <stack>

static void printWHDivider() {
    std::cout << "  " << std::string(62, '-') << "\n";
}

WatchHistory::WatchHistory(int maxSize)
    : maxSize(maxSize)
{}

void WatchHistory::push(const Video& video) {
    if (static_cast<int>(history.size()) >= maxSize) {
        // Remove oldest element if over cap
        std::stack<Video> temp;

        while (!history.empty()) {
            temp.push(history.top());
            history.pop();
        }
        temp.pop();

        while (!temp.empty()) {
            history.push(temp.top());
            temp.pop();
        }
    }
    history.push(video);
}

Video WatchHistory::pop() {
    Video top = history.top();
    history.pop();
    return top;
}

const Video& WatchHistory::peek() const {
    return history.top();
}

bool WatchHistory::isEmpty() const {
    return history.empty();
}

int WatchHistory::getSize() const {
    return static_cast<int>(history.size());
}

void WatchHistory::clear() {
    while (!history.empty())
        history.pop();
}

void WatchHistory::display() const {
    if (history.empty()) {
        std::cout << "\n  Your watch history is empty.\n\n";
        return;
    }

    std::cout << "\n";
    printWHDivider();
    std::cout << "  \xF0\x9F\x93\x8B  Watch History  ("
              << history.size() << " video"
              << (history.size() == 1 ? "" : "s")
              << " | most recent first)\n";
    printWHDivider();

    std::stack<Video> copy = history;
    int rank = 1;

    while (!copy.empty()) {
        const Video& v = copy.top();
        std::cout << "  "
                  << std::right << std::setw(2) << rank++ << ". "
                  << std::left  << std::setw(35) << v.title
                  << " [" << v.videoId << "]"
                  << "  Views: " << v.viewCount
                  << "\n";
        copy.pop();
    }

    printWHDivider();
    std::cout << "\n";
}
