// test_sharing.cpp — Step 11 verification driver
// Compile: g++ -std=c++17 -o test_sn test_sharing.cpp modules/SharingNetwork.cpp
// Run:     ./test_sn

#include "modules/SharingNetwork.h"
#include <iostream>
#include <cassert>

int main() {
    std::cout << "\n==============================================\n";
    std::cout << "  VidHub — Step 11: SharingNetwork Test\n";
    std::cout << "==============================================\n\n";

    SharingNetwork sn;

    // ── Test 1: Empty graph ───────────────────────────────────
    assert(sn.getNodeCount() == 0);
    assert(sn.getEdgeCount() == 0);
    std::cout << "[PASS] Empty graph on construction\n";

    // ── Test 2: addShare() auto-registers nodes ───────────────
    // Riya shares "Build a REST API" to Dev and TechForum
    sn.addShare("Riya",       "Dev");
    sn.addShare("Riya",       "TechForum");
    sn.addShare("Dev",        "CollegeGroup");
    sn.addShare("TechForum",  "LinkedInPost");
    sn.addShare("CollegeGroup","PeerA");
    sn.addShare("CollegeGroup","PeerB");
    sn.addShare("LinkedInPost","ExternalUser1");
    sn.addShare("LinkedInPost","ExternalUser2");

    assert(sn.getNodeCount() == 9);   // 9 unique nodes
    assert(sn.getEdgeCount() == 8);   // 8 directed edges
    std::cout << "[PASS] addShare() — 9 nodes, 8 edges\n";

    // ── Test 3: Duplicate edge rejected ──────────────────────
    sn.addShare("Riya", "Dev");       // Already exists
    assert(sn.getEdgeCount() == 8);   // No change
    std::cout << "[PASS] Duplicate edge silently rejected\n";

    // ── Test 4: hasNode() / hasEdge() ────────────────────────
    assert(sn.hasNode("Riya")        == true);
    assert(sn.hasNode("Nobody")      == false);
    assert(sn.hasEdge("Riya", "Dev") == true);
    assert(sn.hasEdge("Dev",  "Riya")== false);  // Directed: no reverse
    std::cout << "[PASS] hasNode() and hasEdge() correct\n";

    // ── Test 5: BFS from Riya ─────────────────────────────────
    std::vector<std::string> bfsResult = sn.BFS("Riya");
    // BFS order: Riya → Dev, TechForum → CollegeGroup, LinkedInPost
    //          → PeerA, PeerB, ExternalUser1, ExternalUser2
    assert(bfsResult[0] == "Riya");          // Source is first
    assert(bfsResult.size() == 9);            // All 9 nodes reachable
    std::cout << "[PASS] BFS from Riya visits all 9 nodes\n";

    // ── Test 6: getReachCount() ───────────────────────────────
    assert(sn.getReachCount("Riya")  == 9);  // Reaches everyone
    assert(sn.getReachCount("PeerA") == 1);  // PeerA has no shares (isolated)
    std::cout << "[PASS] getReachCount() — Riya:9, PeerA:1\n";

    // ── Test 7: DFS from Riya ─────────────────────────────────
    std::vector<std::string> dfsResult = sn.DFS("Riya");
    assert(dfsResult[0]    == "Riya");        // Source first
    assert(dfsResult.size()== 9);             // All reachable
    std::cout << "[PASS] DFS from Riya visits all 9 nodes\n";

    // ── Test 8: BFS levels show hop distances ─────────────────
    auto levels = sn.BFSLevels("Riya");
    assert(levels[0].size() == 1);            // Level 0: Riya
    assert(levels[1].size() == 2);            // Level 1: Dev, TechForum
    assert(levels[2].size() == 2);            // Level 2: CollegeGroup, LinkedInPost
    assert(levels[3].size() == 4);            // Level 3: PeerA, PeerB, EU1, EU2
    std::cout << "[PASS] BFSLevels() — 4 hops from Riya\n";

    // ── Test 9: BFS on isolated node ─────────────────────────
    sn.addNode("Lurker");
    auto lBFS = sn.BFS("Lurker");
    assert(lBFS.size() == 1);    // Only itself — no edges
    assert(sn.getReachCount("Lurker") == 1);
    std::cout << "[PASS] BFS on isolated node returns only itself\n";

    // ── Test 10: BFS on unknown node ─────────────────────────
    auto unknown = sn.BFS("Nobody");
    assert(unknown.empty());
    std::cout << "[PASS] BFS on unknown node returns empty\n";

    // ── Test 11: displayNetwork() ─────────────────────────────
    std::cout << "\n--- displayNetwork() ---";
    sn.displayNetwork();

    // ── Test 12: displayBFSTraversal() ────────────────────────
    std::cout << "--- displayBFSTraversal('Riya') ---";
    sn.displayBFSTraversal("Riya");

    // ── Test 13: displayDFSTraversal() ────────────────────────
    std::cout << "--- displayDFSTraversal('Riya') ---";
    sn.displayDFSTraversal("Riya");

    std::cout << "\n==============================================\n";
    std::cout << "  All SharingNetwork tests passed!\n";
    std::cout << "==============================================\n\n";
    return 0;
}
