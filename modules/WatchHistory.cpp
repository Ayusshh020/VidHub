// modules/WatchHistory.cpp
// Purpose: Implementation of WatchHistory stack operations, including overflow eviction and formatted display.

#include "WatchHistory.h"

#include <iostream>
#include <iomanip>
#include <stack>

// Helper function to print a uniform divider line for the Watch History display
static void printWHDivider() {
    std::cout << "  " << std::string(62, '-') << "\n";
}

// Constructor setting max capacity constraint
WatchHistory::WatchHistory(int maxSize)
    : maxSize(maxSize)
{}

// Adds video onto the top of the stack. If stack size exceeds maxSize, the bottom/oldest element is evicted.
void WatchHistory::push(const Video& video) {
    if (static_cast<int>(history.size()) >= maxSize) {
        // Eviction logic: To access and remove the bottom element,
        // we dump the stack contents to a temporary stack, pop the bottom element, and restore the stack.
        std::stack<Video> temp;

        while (!history.empty()) {
            temp.push(history.top());
            history.pop();
        }
        // This is the bottom element of history stack, which is now the top of temp stack
        temp.pop();

        while (!temp.empty()) {
            history.push(temp.top());
            temp.pop();
        }
    }
    history.push(video);
}

// Pops and returns the top (most recent) video from stack
Video WatchHistory::pop() {
    Video top = history.top();
    history.pop();
    return top;
}

// Returns a reference to the top (most recent) video without modifying the stack
const Video& WatchHistory::peek() const {
    return history.top();
}

// Checks if stack is empty
bool WatchHistory::isEmpty() const {
    return history.empty();
}

// Returns stack size
int WatchHistory::getSize() const {
    return static_cast<int>(history.size());
}

// Clears all elements from stack
void WatchHistory::clear() {
    while (!history.empty())
        history.pop();
}

// Displays all elements in history starting from top (most recent) to bottom (least recent)
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

    // Copy stack to traverse it without destroying the original watch history
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
