// test_recqueue.cpp — Step 6 verification driver
// Compile: g++ -std=c++17 -o test_rq test_recqueue.cpp
//              entities/Video.cpp modules/RecommendationQueue.cpp
// Run:     ./test_rq

#include "modules/RecommendationQueue.h"
#include "entities/Video.h"
#include <iostream>
#include <cassert>
#include <vector>

// Helper: build a quick test video
Video makeVideo(const std::string& id,
                const std::string& title,
                const std::string& genre) {
    return Video("VID-" + id, title, "USR-003", genre, 1800, "LIC-001", "2024-01-01");
}

int main() {
    std::cout << "\n================================================\n";
    std::cout << "  VidHub — Step 6: RecommendationQueue Test\n";
    std::cout << "================================================\n\n";

    // ── Test 1: Empty queue on construction ──────────────────
    RecommendationQueue rq;
    assert(rq.isEmpty()  == true);
    assert(rq.getSize()  == 0);
    std::cout << "[PASS] Fresh queue is empty\n";

    // ── Test 2: enqueue + front (FIFO order) ─────────────────
    Video v1 = makeVideo("001", "Python for Beginners",    "Education");
    Video v2 = makeVideo("002", "Python OOP Crash Course", "Education");
    Video v3 = makeVideo("003", "Build a REST API",        "Tech");

    rq.enqueue(v1);
    assert(rq.isEmpty()          == false);
    assert(rq.getSize()          == 1);
    assert(rq.front().videoId    == "VID-001");
    std::cout << "[PASS] enqueue(v1) — front is VID-001\n";

    rq.enqueue(v2);
    rq.enqueue(v3);
    assert(rq.getSize()          == 3);
    assert(rq.front().videoId    == "VID-001");  // Front stays VID-001 (FIFO)
    std::cout << "[PASS] enqueue(v2, v3) — front still VID-001 (FIFO preserved)\n";

    // ── Test 3: FIFO dequeue order ────────────────────────────
    Video d1 = rq.dequeue();
    assert(d1.videoId            == "VID-001");  // First in, first out
    assert(rq.front().videoId    == "VID-002");  // VID-002 is now at front
    assert(rq.getSize()          == 2);
    std::cout << "[PASS] dequeue() returned VID-001 (FIFO order confirmed)\n";

    Video d2 = rq.dequeue();
    assert(d2.videoId            == "VID-002");
    std::cout << "[PASS] dequeue() returned VID-002\n";

    // ── Test 4: display() shows queue order ──────────────────
    rq.enqueue(v1);
    rq.enqueue(v2);
    std::cout << "\n--- display() — expected order: VID-003, VID-001, VID-002 ---";
    rq.display();
    assert(rq.getSize() == 3);   // display() must NOT dequeue anything
    std::cout << "[PASS] display() did not modify the queue\n";

    // ── Test 5: clear() empties the queue ────────────────────
    rq.clear();
    assert(rq.isEmpty()  == true);
    assert(rq.getSize()  == 0);
    std::cout << "[PASS] clear() — queue is empty\n";

    // ── Test 6: display on empty queue ───────────────────────
    std::cout << "\n--- display() on empty queue ---";
    rq.display();

    // ── Test 7: maxSize cap (enqueue ignores beyond limit) ────
    RecommendationQueue smallRQ(3);
    smallRQ.enqueue(v1);
    smallRQ.enqueue(v2);
    smallRQ.enqueue(v3);
    assert(smallRQ.getSize() == 3);

    Video v4 = makeVideo("004", "Extra Video", "Lifestyle");
    smallRQ.enqueue(v4);           // Should be silently ignored
    assert(smallRQ.getSize() == 3);
    assert(smallRQ.front().videoId == "VID-001");  // Front unchanged
    std::cout << "[PASS] maxSize=3 cap — 4th enqueue ignored\n";

    // ── Test 8: populate() with genre-based filtering ─────────
    std::vector<Video> pool = {
        makeVideo("101", "Python for Beginners",      "Education"),
        makeVideo("102", "Python OOP Crash Course",   "Education"),
        makeVideo("103", "Morning Yoga Routine",      "Health"),
        makeVideo("104", "Python Django Full Course", "Education"),
        makeVideo("105", "Top 10 Travel Hacks",       "Travel"),
        makeVideo("106", "JavaScript Basics",         "Education"),
    };

    RecommendationQueue popRQ(5);
    // User just watched VID-102 (Education genre) → populate with other Education videos
    popRQ.populate("Education", "VID-102", pool);

    // Expected: VID-101, VID-104, VID-106 (Education, not VID-102)
    assert(popRQ.getSize() == 3);
    assert(popRQ.front().videoId == "VID-101");
    std::cout << "[PASS] populate() found 3 Education videos (excludes VID-102)\n";

    std::cout << "\n--- Populated queue (genre=Education, exclude=VID-102) ---";
    popRQ.display();

    // ── Test 9: populate() clears stale entries ───────────────
    popRQ.populate("Health", "VID-999", pool);
    assert(popRQ.getSize() == 1);   // Only VID-103 is Health
    assert(popRQ.front().videoId == "VID-103");
    std::cout << "[PASS] populate() cleared old entries and refilled with Health videos\n";

    std::cout << "\n================================================\n";
    std::cout << "  All RecommendationQueue tests passed!\n";
    std::cout << "================================================\n\n";

    return 0;
}
