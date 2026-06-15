// test_watchhistory.cpp — Step 5 verification driver
// Compile: g++ -std=c++17 -o test_wh test_watchhistory.cpp
//              entities/Video.cpp modules/WatchHistory.cpp
// Run:     ./test_wh

#include "modules/WatchHistory.h"
#include "entities/Video.h"
#include <iostream>
#include <cassert>

// Helper: make a quick test video
Video makeVideo(const std::string& id, const std::string& title, int views = 0) {
    Video v("VID-" + id, title, "USR-003", "Education", 1800, "LIC-001", "2024-01-01");
    v.viewCount = views;
    return v;
}

int main() {
    std::cout << "\n==============================================\n";
    std::cout << "  VidHub — Step 5: WatchHistory (Stack) Test\n";
    std::cout << "==============================================\n\n";

    // ── Test 1: isEmpty on fresh stack ────────────────────────
    WatchHistory wh;
    assert(wh.isEmpty()  == true);
    assert(wh.getSize()  == 0);
    std::cout << "[PASS] Fresh stack is empty\n";

    // ── Test 2: push + peek ───────────────────────────────────
    Video v1 = makeVideo("001", "Python for Beginners",      22400);
    Video v2 = makeVideo("002", "Python OOP Crash Course",   18100);
    Video v3 = makeVideo("003", "Build a REST API",          45200);

    wh.push(v1);
    assert(wh.isEmpty()  == false);
    assert(wh.getSize()  == 1);
    assert(wh.peek().videoId == "VID-001");
    std::cout << "[PASS] push(v1) + peek() → VID-001\n";

    wh.push(v2);
    wh.push(v3);
    assert(wh.getSize()  == 3);
    assert(wh.peek().videoId == "VID-003");   // Most recent = top
    std::cout << "[PASS] push(v2, v3) — top is now VID-003 (most recent)\n";

    // ── Test 3: LIFO order verified via display ───────────────
    std::cout << "\n--- display() — expected order: VID-003, VID-002, VID-001 ---";
    wh.display();

    // ── Test 4: pop returns most recent ──────────────────────
    Video popped = wh.pop();
    assert(popped.videoId == "VID-003");
    assert(wh.getSize()   == 2);
    assert(wh.peek().videoId == "VID-002");
    std::cout << "[PASS] pop() returned VID-003, new top is VID-002\n";

    // ── Test 5: clear() empties the stack ────────────────────
    wh.push(v3);
    assert(wh.getSize() == 3);
    wh.clear();
    assert(wh.isEmpty() == true);
    assert(wh.getSize() == 0);
    std::cout << "[PASS] clear() — stack is empty\n";

    // ── Test 6: display on empty stack ───────────────────────
    std::cout << "\n--- display() on empty stack ---";
    wh.display();

    // ── Test 7: maxSize overflow cap ─────────────────────────
    // Create a WatchHistory capped at 3 entries
    WatchHistory smallWH(3);
    Video va = makeVideo("A", "Video A");
    Video vb = makeVideo("B", "Video B");
    Video vc = makeVideo("C", "Video C");
    Video vd = makeVideo("D", "Video D");   // This should evict VID-A

    smallWH.push(va);
    smallWH.push(vb);
    smallWH.push(vc);
    assert(smallWH.getSize() == 3);

    smallWH.push(vd);                       // Triggers overflow handling
    assert(smallWH.getSize() == 3);         // Still capped at 3

    // VID-A (oldest) should be gone; VID-D should be at top
    assert(smallWH.peek().videoId == "VID-D");
    std::cout << "[PASS] maxSize=3 cap — VID-D on top, VID-A evicted\n";

    std::cout << "\n--- Capped history (3 entries) after overflow ---";
    smallWH.display();

    std::cout << "\n==============================================\n";
    std::cout << "  All WatchHistory tests passed!\n";
    std::cout << "==============================================\n\n";

    return 0;
}
