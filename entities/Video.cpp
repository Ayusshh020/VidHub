// entities/Video.cpp
// Purpose: Implementation of the Video class methods, handling playback stats and popularity score computations.

#include "Video.h"

#include <iostream>
#include <iomanip>
#include <string>

// Helper function to format duration in seconds into a human-readable HHh MMm SSs string
static std::string formatDuration(int totalSeconds) {
    int hours   = totalSeconds / 3600;
    int minutes = (totalSeconds % 3600) / 60;
    int seconds = totalSeconds % 60;

    std::string result = "";
    if (hours > 0)
        result += std::to_string(hours) + "h ";
    if (minutes > 0 || hours > 0)
        result += std::to_string(minutes) + "m ";
    result += std::to_string(seconds) + "s";
    return result;
}

// Utility function to print a uniform visual divider for console displays
static void printDivider() {
    std::cout << "  " << std::string(54, '-') << "\n";
}

// Default constructor initializing all fields to empty/zero values
Video::Video()
    : videoId(""),
      title(""),
      creatorId(""),
      genre(""),
      uploadDate(""),
      licenseKey(""),
      durationSec(0),
      viewCount(0),
      likeCount(0),
      totalWatchTimeSec(0),
      engagementScore(0.0)
{}

// Parameterized constructor mapping user-supplied parameters to class members
Video::Video(std::string id,
             std::string title,
             std::string creatorId,
             std::string genre,
             int         durationSec,
             std::string licenseKey,
             std::string uploadDate)
    : videoId(id),
      title(title),
      creatorId(creatorId),
      genre(genre),
      uploadDate(uploadDate),
      licenseKey(licenseKey),
      durationSec(durationSec),
      viewCount(0),
      likeCount(0),
      totalWatchTimeSec(0),
      engagementScore(0.0)
{}

// Increments the video's views and recalculates popularity score
void Video::incrementViews() {
    viewCount++;
    recalculateScore();
}

// Increments the video's likes and recalculates popularity score
void Video::addLike() {
    likeCount++;
    recalculateScore();
}

// Adds watch time duration in seconds and updates popularity score
void Video::addWatchTime(int seconds) {
    if (seconds > 0)
        totalWatchTimeSec += seconds;
    recalculateScore();
}

// Recalculates popularity rating using weights: views (0.5), likes (2.0), and retention ratio (1000.0)
void Video::recalculateScore() {
    double watchRatio = 0.0;
    if (durationSec > 0)
        watchRatio = static_cast<double>(totalWatchTimeSec)
                   / static_cast<double>(durationSec);

    engagementScore = (viewCount  * 0.5)
                    + (likeCount  * 2.0)
                    + (watchRatio * 1000.0);
}

// Simple getter returning the latest engagement score
double Video::getEngagementScore() const {
    return engagementScore;
}

// Prints a single-line summary of video stats, formatted for tables
void Video::displaySummary() const {
    std::cout << "  ["
              << std::left << std::setw(7) << videoId << "] "
              << std::left << std::setw(35) << title
              << " | " << std::left << std::setw(12) << genre
              << " | Views: " << std::right << std::setw(7) << viewCount
              << " | Score: " << std::fixed << std::setprecision(1)
              << engagementScore
              << "\n";
}

// Prints a detailed multi-line block showing all metadata and engagement metrics
void Video::displayFull() const {
    printDivider();
    std::cout << "  \xF0\x9F\x8E\xAC  " << title << "\n";
    printDivider();

    std::cout << std::left;
    std::cout << "  " << std::setw(14) << "Video ID"   << ": " << videoId    << "\n";
    std::cout << "  " << std::setw(14) << "Creator"    << ": " << creatorId  << "\n";
    std::cout << "  " << std::setw(14) << "Genre"      << ": " << genre      << "\n";
    std::cout << "  " << std::setw(14) << "Duration"   << ": "
              << formatDuration(durationSec)                                  << "\n";
    std::cout << "  " << std::setw(14) << "Uploaded"   << ": " << uploadDate << "\n";
    std::cout << "  " << std::setw(14) << "License"    << ": " << licenseKey << "\n";

    printDivider();

    std::cout << "  " << std::setw(14) << "Views"      << ": " << viewCount         << "\n";
    std::cout << "  " << std::setw(14) << "Likes"      << ": " << likeCount         << "\n";
    std::cout << "  " << std::setw(14) << "Watch Time" << ": "
              << formatDuration(static_cast<int>(totalWatchTimeSec))                 << "\n";
    std::cout << "  " << std::setw(14) << "Score"      << ": "
              << std::fixed << std::setprecision(1) << engagementScore               << "\n";

    printDivider();
}

// Overloaded equality comparison operator, checking uniqueness by video ID
bool Video::operator==(const Video& other) const {
    return this->videoId == other.videoId;
}
