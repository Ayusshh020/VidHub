// Note: Comments are only for understanding—do not modify the code itself.
#include "WatchHistory.h"

#include <iostream>
#include <iomanip>
#include <stack>

static void printWHDivider() {
    std::cout << "  " << std::string(62, '-') << "\n";
}

// =============================================================
//  CONSTRUCTOR
// =============================================================

WatchHistory::WatchHistory(int maxSize)
    : maxSize(maxSize)
{}

// =============================================================
//  CORE STACK OPERATIONS
// =============================================================

// Pushes a video. If maxSize is reached, removes the oldest (bottom) entry by reversing elements.
void WatchHistory::push(const Video& video) {
    if (static_cast<int>(history.size()) >= maxSize) {
        // --- Drop the oldest (bottom) entry ---
        std::stack<Video> temp;

        // Step 1: Move all elements to temp (reversed order)
        while (!history.empty()) {
            temp.push(history.top());
            history.pop();
        }
        // Step 2: Discard the original bottom (now at top of temp)
        temp.pop();

        // Step 3: Move remaining elements back to history
        while (!temp.empty()) {
            history.push(temp.top());
            temp.pop();
        }
    }
    history.push(video);
}

// Remove and return the most recently watched video.
// PRECONDITION: caller must check isEmpty() before calling.
Video WatchHistory::pop() {
    Video top = history.top();
    history.pop();
    return top;
}

// Return the most recently watched video WITHOUT removing it.
// PRECONDITION: caller must check isEmpty() before calling.
const Video& WatchHistory::peek() const {
    return history.top();
}

// =============================================================
//  UTILITY
// =============================================================

bool WatchHistory::isEmpty() const {
    return history.empty();
}

int WatchHistory::getSize() const {
    return static_cast<int>(history.size());
}

// Remove every entry — called when a user clears their history.
void WatchHistory::clear() {
    while (!history.empty())
        history.pop();
}

// =============================================================
//  DISPLAY
//
//  std::stack doesn't support iteration, so we copy it into a
//  temporary stack, then print top-to-bottom (most recent first)
//  by repeatedly reading and popping the copy.
//  The original 'history' stack is NOT modified.
// =============================================================

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

    // Work on a copy — never mutate the real stack in display()
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
