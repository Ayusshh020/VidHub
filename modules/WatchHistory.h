// modules/WatchHistory.h
// Purpose: Header file defining the WatchHistory class, representing a watch history log using a Stack.

#pragma once

#include "../entities/Video.h"
#include <stack>
#include <string>

/**
 * @class WatchHistory
 * @brief Manages recently watched videos using a LIFO Stack structure.
 *
 * Keeps track of video playback history. It maintains a maximum size capacity,
 * evicting the oldest watched video (at the bottom of the stack) when the cap is exceeded.
 */
class WatchHistory {
private:
    std::stack<Video> history; // Internal stack storing watched Video objects
    int maxSize;               // Maximum number of entries allowed in history

public:
    /**
     * @brief Constructs a WatchHistory stack with a specified capacity limit.
     */
    explicit WatchHistory(int maxSize = 50);

    /**
     * @brief Pushes a newly watched video onto the top of the stack.
     * Evicts the oldest entry if size exceeds maxSize.
     * @param video The Video object to log.
     */
    void push(const Video& video);

    /**
     * @brief Pops and returns the most recently watched video from the stack.
     * @return The Video popped from the top of the stack.
     */
    Video pop();

    /**
     * @brief Returns the most recently watched video without removing it from the stack.
     */
    const Video& peek() const;

    /**
     * @brief Returns true if the watch history is empty.
     */
    bool isEmpty() const;

    /**
     * @brief Returns the current number of logged videos in history.
     */
    int getSize() const;

    /**
     * @brief Clears all entries from the watch history stack.
     */
    void clear();

    /**
     * @brief Prints the watch history in LIFO order (most recent first).
     */
    void display() const;
};
