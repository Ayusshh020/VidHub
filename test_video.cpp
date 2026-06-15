// test_video.cpp — Step 3 verification driver
// Compile: g++ -std=c++17 -o test_video test_video.cpp entities/Video.cpp
// Run:     ./test_video

#include "entities/Video.h"
#include <iostream>
#include <cassert>

int main() {
    std::cout << "\n========================================\n";
    std::cout << "  VidHub — Step 3: Video Class Test\n";
    std::cout << "========================================\n\n";

    // ── Test 1: Default Constructor ───────────────────────────
    Video v1;
    assert(v1.videoId      == "");
    assert(v1.viewCount    == 0);
    assert(v1.engagementScore == 0.0);
    std::cout << "[PASS] Default constructor\n";

    // ── Test 2: Parameterized Constructor ────────────────────
    Video v2("VID-002",
             "Python OOP Crash Course",
             "USR-003",
             "Education",
             2700,            // 45 minutes
             "MIT-AJ-002",
             "2024-01-15");

    assert(v2.videoId    == "VID-002");
    assert(v2.title      == "Python OOP Crash Course");
    assert(v2.durationSec == 2700);
    assert(v2.viewCount   == 0);
    std::cout << "[PASS] Parameterized constructor\n";

    // ── Test 3: incrementViews() ──────────────────────────────
    v2.incrementViews();
    v2.incrementViews();
    assert(v2.viewCount == 2);
    std::cout << "[PASS] incrementViews() — viewCount = " << v2.viewCount << "\n";

    // ── Test 4: addLike() ─────────────────────────────────────
    v2.addLike();
    assert(v2.likeCount == 1);
    std::cout << "[PASS] addLike()        — likeCount = " << v2.likeCount << "\n";

    // ── Test 5: addWatchTime() ────────────────────────────────
    v2.addWatchTime(2700);   // Simulates one full watch
    assert(v2.totalWatchTimeSec == 2700);
    std::cout << "[PASS] addWatchTime()   — watchTime = " << v2.totalWatchTimeSec << "s\n";

    // ── Test 6: recalculateScore() ────────────────────────────
    // Expected: (2 * 0.5) + (1 * 2.0) + (2700/2700 * 1000.0)
    //         = 1.0 + 2.0 + 1000.0 = 1003.0
    double expectedScore = (2 * 0.5) + (1 * 2.0) + (1.0 * 1000.0);
    assert(v2.getEngagementScore() == expectedScore);
    std::cout << "[PASS] recalculateScore() — score = "
              << v2.getEngagementScore() << " (expected " << expectedScore << ")\n";

    // ── Test 7: operator== ────────────────────────────────────
    Video v3("VID-002", "Different Title", "USR-999", "Tech", 100, "X", "2024-01-01");
    Video v4("VID-999", "Python OOP Crash Course", "USR-003", "Education", 2700, "MIT-AJ-002", "2024-01-15");

    assert(  v2 == v3);   // Same videoId → equal
    assert(!(v2 == v4));  // Different videoId → not equal
    std::cout << "[PASS] operator==       — matches on videoId only\n";

    // ── Test 8: displaySummary() ──────────────────────────────
    std::cout << "\n--- displaySummary() output ---\n";
    v2.displaySummary();

    // ── Test 9: displayFull() ─────────────────────────────────
    std::cout << "\n--- displayFull() output ---\n";
    v2.displayFull();

    // ── Test 10: Zero-duration guard ─────────────────────────
    Video v5("VID-BAD", "Bad Video", "USR-000", "Test", 0, "NONE", "2024-01-01");
    v5.addWatchTime(100);   // durationSec == 0, must not crash
    std::cout << "[PASS] Zero-duration guard — score = " << v5.getEngagementScore() << "\n";

    std::cout << "\n========================================\n";
    std::cout << "  All tests passed!\n";
    std::cout << "========================================\n\n";

    return 0;
}
