// test_ranking.cpp — Step 9 verification driver
// Compile: g++ -std=c++17 -o test_rs test_ranking.cpp
//              entities/Video.cpp modules/RankingSystem.cpp
// Run:     ./test_rs

#include "modules/RankingSystem.h"
#include "entities/Video.h"
#include <iostream>
#include <cassert>

// Build a video and manually set its engagement score
Video makeRankedVideo(const std::string& id, const std::string& title,
                      long views, long likes, int watchSec, int durSec) {
    Video v("VID-" + id, title, "USR-003", "Education", durSec, "LIC-001", "2024-01-01");
    v.viewCount         = views;
    v.likeCount         = likes;
    v.totalWatchTimeSec = watchSec;
    v.recalculateScore();   // Uses the formula inside Video
    return v;
}

int main() {
    std::cout << "\n================================================\n";
    std::cout << "  VidHub — Step 9: RankingSystem (Heap) Test\n";
    std::cout << "================================================\n\n";

    RankingSystem rs;

    // ── Test 1: Empty system ──────────────────────────────────
    assert(rs.isEmpty()   == true);
    assert(rs.getCount()  == 0);
    std::cout << "[PASS] Empty ranking system on construction\n";

    // ── Test 2: addVideo() ────────────────────────────────────
    // Scores (formula: views*0.5 + likes*2 + (watch/dur)*1000):
    //  VID-003 "Build a REST API":       45200*0.5 + 3100*2 + (45200/1800)*1000 = 22600+6200+25111 ≈ 53911
    //  VID-004 "Morning Yoga":           38100*0.5 + 2800*2 + (38100/1200)*1000 = 19050+5600+31750 = 56400
    //  VID-001 "Python for Beginners":   22400*0.5 + 1800*2 + (22400/2400)*1000 = 11200+3600+9333  ≈ 24133
    //  VID-005 "Top 10 Travel Hacks":    31500*0.5 + 1900*2 + (31500/900)*1000  = 15750+3800+35000 = 54550
    //  VID-002 "Python OOP":             18100*0.5 + 1420*2 + (18100/2700)*1000 = 9050+2840+6703   ≈ 18593
    //  VID-006 "JavaScript Basics":      12000*0.5 + 980*2  + (12000/3000)*1000 = 6000+1960+4000   = 11960

    Video v1 = makeRankedVideo("001", "Python for Beginners",    22400, 1800, 22400, 2400);
    Video v2 = makeRankedVideo("002", "Python OOP Crash Course", 18100, 1420, 18100, 2700);
    Video v3 = makeRankedVideo("003", "Build a REST API",        45200, 3100, 45200, 1800);
    Video v4 = makeRankedVideo("004", "Morning Yoga Routine",    38100, 2800, 38100, 1200);
    Video v5 = makeRankedVideo("005", "Top 10 Travel Hacks",     31500, 1900, 31500,  900);
    Video v6 = makeRankedVideo("006", "JavaScript Basics",       12000,  980, 12000, 3000);

    rs.addVideo(v1); rs.addVideo(v2); rs.addVideo(v3);
    rs.addVideo(v4); rs.addVideo(v5); rs.addVideo(v6);

    assert(rs.getCount() == 6);
    assert(rs.isEmpty()  == false);
    std::cout << "[PASS] addVideo() — 6 videos added\n";

    // ── Test 3: peekTop() — highest score is at root ──────────
    const Video& top = rs.peekTop();
    // Morning Yoga has highest score (~56400) — verify it's on top
    std::cout << "  Top video: " << top.title
              << " (score=" << top.engagementScore << ")\n";
    assert(top.engagementScore >= rs.getTopK(6)[1].engagementScore);
    std::cout << "[PASS] peekTop() returns the highest-scored video\n";

    // ── Test 4: getTopK() returns correct order ───────────────
    std::vector<Video> top3 = rs.getTopK(3);
    assert(top3.size() == 3);
    // Verify descending order
    assert(top3[0].engagementScore >= top3[1].engagementScore);
    assert(top3[1].engagementScore >= top3[2].engagementScore);
    std::cout << "[PASS] getTopK(3) returns 3 videos in descending score order\n";
    std::cout << "  Rank 1: " << top3[0].title << " (" << top3[0].engagementScore << ")\n";
    std::cout << "  Rank 2: " << top3[1].title << " (" << top3[1].engagementScore << ")\n";
    std::cout << "  Rank 3: " << top3[2].title << " (" << top3[2].engagementScore << ")\n";

    // ── Test 5: getTopK() doesn't modify original heap ────────
    std::vector<Video> again = rs.getTopK(3);
    assert(again[0].videoId == top3[0].videoId);  // Same top after re-call
    assert(rs.getCount() == 6);                    // Nothing was removed
    std::cout << "[PASS] getTopK() does not mutate the internal heap\n";

    // ── Test 6: getTopK(k > count) returns all ────────────────
    std::vector<Video> all = rs.getTopK(100);
    assert(static_cast<int>(all.size()) == 6);
    std::cout << "[PASS] getTopK(100) returns all 6 videos when k > count\n";

    // ── Test 7: updateVideo() — score change reflected ────────
    // Simulate Python for Beginners going viral (massive new engagement)
    Video v1Updated = v1;
    v1Updated.viewCount         = 500000;
    v1Updated.likeCount         = 50000;
    v1Updated.totalWatchTimeSec = 1200000;
    v1Updated.recalculateScore();
    std::cout << "\n  Updating VID-001 score to: " << v1Updated.engagementScore << "\n";

    bool updated = rs.updateVideo(v1Updated);
    assert(updated == true);
    assert(rs.getCount() == 6);   // Count unchanged

    // VID-001 should now be at the top
    const Video& newTop = rs.peekTop();
    assert(newTop.videoId == "VID-001");
    std::cout << "[PASS] updateVideo() — VID-001 now at top after score update\n";

    // ── Test 8: updateVideo() — unknown videoId ───────────────
    Video fake("VID-999", "Fake", "USR-0", "X", 60, "X", "2024-01-01");
    bool notFound = rs.updateVideo(fake);
    assert(notFound == false);
    std::cout << "[PASS] updateVideo() returns false for unknown videoId\n";

    // ── Test 9: removeVideo() ─────────────────────────────────
    bool removed = rs.removeVideo("VID-006");
    assert(removed      == true);
    assert(rs.getCount()== 5);
    assert(rs.contains("VID-006") == false);
    std::cout << "[PASS] removeVideo() — VID-006 removed (count=5)\n";

    bool noRemove = rs.removeVideo("VID-999");
    assert(noRemove == false);
    std::cout << "[PASS] removeVideo() returns false for unknown videoId\n";

    // ── Test 10: contains() ───────────────────────────────────
    assert(rs.contains("VID-001") == true);
    assert(rs.contains("VID-999") == false);
    std::cout << "[PASS] contains() works correctly\n";

    // ── Test 11: displayRankings() ────────────────────────────
    std::cout << "\n--- displayRankings(top 5) ---";
    rs.displayRankings(5);

    std::cout << "\n================================================\n";
    std::cout << "  All RankingSystem (Heap) tests passed!\n";
    std::cout << "================================================\n\n";

    return 0;
}
