#include "Video.h"

#include <iostream>
#include <iomanip>   // std::setw, std::setprecision, std::left
#include <string>

// ─────────────────────────────────────────────────────────────
//  Helper: Format seconds into "Xh Ym" or "Ym Zs" for display
// ─────────────────────────────────────────────────────────────
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

// ─────────────────────────────────────────────────────────────
//  Divider line for displayFull()
// ─────────────────────────────────────────────────────────────
static void printDivider() {
    std::cout << "  " << std::string(54, '-') << "\n";
}

// =============================================================
//  CONSTRUCTORS
// =============================================================

// Default constructor — all numeric fields zeroed, strings empty.
// Needed by STL containers and DummyData pre-allocation.
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

// Parameterized constructor — used by DummyData to build test videos.
// engagementScore starts at 0.0 and is computed on first update.
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

// =============================================================
//  ENGAGEMENT MUTATORS
//  Each mutator updates one metric then immediately recomputes
//  the engagementScore so the RankingSystem (Heap) always has
//  a fresh value to sort on.
// =============================================================

void Video::incrementViews() {
    viewCount++;
    recalculateScore();
}

void Video::addLike() {
    likeCount++;
    recalculateScore();
}

// Called after a simulated watch session ends.
// 'seconds' = how many seconds the viewer watched this time.
void Video::addWatchTime(int seconds) {
    if (seconds > 0)
        totalWatchTimeSec += seconds;
    recalculateScore();
}

// =============================================================
//  SCORE COMPUTATION
//
//  Formula:
//    engagementScore = (viewCount  * 0.5)
//                    + (likeCount  * 2.0)
//                    + (watchRatio * 1000.0)
//
//  watchRatio = totalWatchTimeSec / durationSec
//    → 0.0  means nobody finished the video
//    → 1.0  means every viewer watched it fully
//    → >1.0 means the video was re-watched multiple times
//
//  Guard: if durationSec == 0 (bad data), watchRatio = 0.
// =============================================================

void Video::recalculateScore() {
    double watchRatio = 0.0;
    if (durationSec > 0)
        watchRatio = static_cast<double>(totalWatchTimeSec)
                   / static_cast<double>(durationSec);

    engagementScore = (viewCount  * 0.5)
                    + (likeCount  * 2.0)
                    + (watchRatio * 1000.0);
}

double Video::getEngagementScore() const {
    return engagementScore;
}

// =============================================================
//  DISPLAY FUNCTIONS
// =============================================================

// One-line summary — used in Home Feed lists, search results,
// history, and recommendation queues.
//
// Output format:
//   [VID-001] Python OOP Crash Course | Genre: Education | Views: 18,101 | Score: 2940.5
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

// Full-detail block — used when the user selects a specific video.
//
// Output format:
//   ──────────────────────────────────────────────────────
//   🎬  Python OOP Crash Course
//   ──────────────────────────────────────────────────────
//     Video ID    : VID-002
//     Creator     : USR-003
//     Genre       : Education
//     Duration    : 45m 0s
//     Uploaded    : 2024-01-15
//     License     : MIT-AJ-002
//   ──────────────────────────────────────────────────────
//     Views       : 18,101
//     Likes       : 1,420
//     Watch Time  : 270,000s
//     Score       : 2940.5
//   ──────────────────────────────────────────────────────
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

// =============================================================
//  EQUALITY OPERATOR
//  Two Video objects are the same if their videoId matches.
//  Used for existence checks (e.g., "is this video already in
//  the catalog?") regardless of ordering or score.
// =============================================================

bool Video::operator==(const Video& other) const {
    return this->videoId == other.videoId;
}
