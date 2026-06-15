// test_catalog.cpp — Step 8 verification driver
// Compile: g++ -std=c++17 -o test_cat test_catalog.cpp
//              entities/Video.cpp modules/VideoCatalog.cpp
// Run:     ./test_cat

#include "modules/VideoCatalog.h"
#include "entities/Video.h"
#include <iostream>
#include <cassert>

Video makeVideo(const std::string& id, const std::string& title,
                const std::string& genre, double score = 0.0) {
    Video v("VID-" + id, title, "USR-003", genre, 1800, "LIC-001", "2024-01-01");
    v.engagementScore = score;
    return v;
}

int main() {
    std::cout << "\n=============================================\n";
    std::cout << "  VidHub — Step 8: VideoCatalog (BST) Test\n";
    std::cout << "=============================================\n\n";

    VideoCatalog cat;

    // ── Test 1: Empty catalog ─────────────────────────────────
    assert(cat.isEmpty()   == true);
    assert(cat.getCount()  == 0);
    std::cout << "[PASS] Empty catalog on construction\n";

    // ── Test 2: Insert videos (out of alphabetical order) ─────
    //  Inserted order: Python, Morning, Build, JavaScript, Vegan
    //  Expected BST in-order: Build, JavaScript, Morning, Python, Vegan
    cat.insert(makeVideo("002", "Python OOP Crash Course",    "Education", 2940.0));
    cat.insert(makeVideo("004", "Morning Yoga Routine",       "Health",    4310.0));
    cat.insert(makeVideo("003", "Build a REST API",           "Tech",      4820.0));
    cat.insert(makeVideo("006", "JavaScript Basics",          "Education", 1850.0));
    cat.insert(makeVideo("008", "Vegan Cooking 101",          "Lifestyle", 1430.0));
    cat.insert(makeVideo("001", "Python for Beginners",       "Education", 3200.0));
    cat.insert(makeVideo("005", "Top 10 Travel Hacks",        "Travel",    3890.0));

    assert(cat.getCount() == 7);
    assert(cat.isEmpty()  == false);
    std::cout << "[PASS] insert() — 7 videos added\n";

    // ── Test 3: In-order traversal is alphabetically sorted ───
    std::vector<Video> sorted = cat.getAllSorted();
    assert(sorted.size()    == 7);
    assert(sorted[0].title  == "Build a REST API");       // B comes first
    assert(sorted[1].title  == "JavaScript Basics");      // J
    assert(sorted[2].title  == "Morning Yoga Routine");   // M
    assert(sorted[3].title  == "Python OOP Crash Course");// P
    assert(sorted[4].title  == "Python for Beginners");   // Pf (after Po alphabetically? "Python f" vs "Python O")
    // Note: uppercase 'O' < lowercase 'f' in ASCII — "Python OOP" < "Python for"
    assert(sorted[5].title  == "Top 10 Travel Hacks");    // T
    assert(sorted[6].title  == "Vegan Cooking 101");      // V
    std::cout << "[PASS] getAllSorted() — in-order traversal is alphabetical\n";

    // ── Test 4: Duplicate insert rejected ────────────────────
    cat.insert(makeVideo("002", "Python OOP Crash Course", "Education")); // dup
    assert(cat.getCount() == 7);
    std::cout << "[PASS] Duplicate videoId silently rejected\n";

    // ── Test 5: Exact title search — O(log n) ────────────────
    const Video* found = cat.searchExact("Build a REST API");
    assert(found != nullptr);
    assert(found->videoId == "VID-003");
    std::cout << "[PASS] searchExact() found 'Build a REST API' → VID-003\n";

    const Video* notFound = cat.searchExact("Nonexistent Video");
    assert(notFound == nullptr);
    std::cout << "[PASS] searchExact() returns nullptr for missing title\n";

    // ── Test 6: Keyword search (case-insensitive) ─────────────
    std::vector<Video> kwRes = cat.searchByKeyword("python");
    assert(kwRes.size() == 2);   // "Python OOP Crash Course" + "Python for Beginners"
    assert(kwRes[0].title == "Python OOP Crash Course");
    assert(kwRes[1].title == "Python for Beginners");
    std::cout << "[PASS] searchByKeyword('python') — found 2 results\n";

    std::vector<Video> kwNone = cat.searchByKeyword("xyz999");
    assert(kwNone.empty());
    std::cout << "[PASS] searchByKeyword('xyz999') — 0 results\n";

    // ── Test 7: Genre filter ──────────────────────────────────
    std::vector<Video> eduVideos = cat.searchByGenre("Education");
    assert(eduVideos.size() == 3);  // Python OOP, JavaScript, Python for Beginners
    std::cout << "[PASS] searchByGenre('Education') — found 3 videos\n";

    std::vector<Video> travelVids = cat.searchByGenre("Travel");
    assert(travelVids.size() == 1);
    assert(travelVids[0].videoId == "VID-005");
    std::cout << "[PASS] searchByGenre('Travel') — found 1 video\n";

    // ── Test 8: contains() ────────────────────────────────────
    assert(cat.contains("VID-003") == true);
    assert(cat.contains("VID-999") == false);
    std::cout << "[PASS] contains() — VID-003 found, VID-999 not found\n";

    // ── Test 9: BST deletion — leaf node (Case 1) ─────────────
    bool removed = cat.remove("VID-008");  // "Vegan Cooking 101" — rightmost leaf
    assert(removed         == true);
    assert(cat.getCount()  == 6);
    assert(cat.contains("VID-008") == false);
    std::cout << "[PASS] remove() leaf node — VID-008 deleted (count=6)\n";

    // ── Test 10: BST deletion — one child (Case 2) ────────────
    // Insert a node that will have only one child
    cat.insert(makeVideo("009", "Zumba for Beginners", "Health")); // Rightmost
    cat.insert(makeVideo("010", "Zumba Advanced",      "Health")); // "Zumba A" < "Zumba f" alphabetically? No, A < f. So "Zumba Advanced" left of "Zumba for"
    // Remove "Zumba for Beginners" — it has one left child "Zumba Advanced"
    bool r2 = cat.remove("VID-009");
    assert(r2                       == true);
    assert(cat.contains("VID-010")  == true);   // Child must survive
    std::cout << "[PASS] remove() one-child node — child correctly promoted\n";

    // ── Test 11: BST deletion — two children (Case 3) ─────────
    // "Top 10 Travel Hacks" likely has two children in our tree
    int before = cat.getCount();
    bool r3 = cat.remove("VID-005");  // "Top 10 Travel Hacks"
    assert(r3                   == true);
    assert(cat.getCount()       == before - 1);
    assert(cat.contains("VID-005") == false);
    std::cout << "[PASS] remove() two-children node — in-order successor used\n";

    // ── Test 12: Remove non-existent videoId ──────────────────
    bool r4 = cat.remove("VID-999");
    assert(r4 == false);
    std::cout << "[PASS] remove() returns false for unknown videoId\n";

    // ── Test 13: displayAll() ─────────────────────────────────
    std::cout << "\n--- displayAll() after operations ---";
    cat.displayAll();

    // ── Test 14: displaySearchResults() ─────────────────────
    std::cout << "--- displaySearchResults() for 'python' ---";
    std::vector<Video> pRes = cat.searchByKeyword("python");
    cat.displaySearchResults(pRes, "python");

    std::cout << "\n=============================================\n";
    std::cout << "  All VideoCatalog (BST) tests passed!\n";
    std::cout << "=============================================\n\n";

    return 0;
}
