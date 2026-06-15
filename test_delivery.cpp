// test_delivery.cpp — Step 12 verification driver
// Compile: g++ -std=c++17 -o test_do test_delivery.cpp modules/DeliveryOptimizer.cpp
// Run:     ./test_do
//
// CDN Topology:
//   0: Mumbai-Origin
//   1: Delhi-PoP
//   2: Bangalore-PoP
//   3: Singapore-Edge
//   4: London-Edge
//   5: User-Local
//
// Edges (ms):
//   Mumbai ↔ Delhi      : 10
//   Mumbai ↔ Bangalore  : 8
//   Delhi  ↔ Singapore  : 20
//   Bangalore ↔ Singapore: 15
//   Singapore ↔ User    : 5
//   Delhi  ↔ London     : 25
//   London ↔ User       : 30

#include "modules/DeliveryOptimizer.h"
#include <iostream>
#include <cassert>

int main() {
    std::cout << "\n===============================================\n";
    std::cout << "  VidHub — Step 12: DeliveryOptimizer Test\n";
    std::cout << "===============================================\n\n";

    // Build CDN graph
    DeliveryOptimizer cdn(6);
    cdn.setNodeName(0, "Mumbai-Origin");
    cdn.setNodeName(1, "Delhi-PoP");
    cdn.setNodeName(2, "Bangalore-PoP");
    cdn.setNodeName(3, "Singapore-Edge");
    cdn.setNodeName(4, "London-Edge");
    cdn.setNodeName(5, "User-Local");

    cdn.addEdge(0, 1, 10);   // Mumbai ↔ Delhi
    cdn.addEdge(0, 2, 8);    // Mumbai ↔ Bangalore
    cdn.addEdge(1, 3, 20);   // Delhi  ↔ Singapore
    cdn.addEdge(2, 3, 15);   // Bangalore ↔ Singapore
    cdn.addEdge(3, 5, 5);    // Singapore ↔ User
    cdn.addEdge(1, 4, 25);   // Delhi ↔ London
    cdn.addEdge(4, 5, 30);   // London ↔ User

    // ── Test 1: Node names ────────────────────────────────────
    assert(cdn.getNodeName(0) == "Mumbai-Origin");
    assert(cdn.getNodeName(5) == "User-Local");
    assert(cdn.getNodeCount() == 6);
    std::cout << "[PASS] Node names assigned correctly\n";

    // ── Test 2: Optimal route — Mumbai to User ────────────────
    // Expected:  Mumbai → Bangalore → Singapore → User = 8+15+5 = 28ms
    // Alt path:  Mumbai → Delhi → Singapore → User     = 10+20+5 = 35ms
    // Alt path:  Mumbai → Delhi → London → User        = 10+25+30 = 65ms
    auto res1 = cdn.dijkstra(0, 5);
    int cost1 = res1.first;
    std::vector<int> path1 = res1.second;
    assert(cost1 == 28);
    assert(path1.size() == 4);
    assert(path1[0] == 0);  // Mumbai
    assert(path1[1] == 2);  // Bangalore
    assert(path1[2] == 3);  // Singapore
    assert(path1[3] == 5);  // User
    std::cout << "[PASS] dijkstra(Mumbai→User) = 28ms via Bangalore+Singapore\n";

    // ── Test 3: Route to self ─────────────────────────────────
    auto resSelf = cdn.dijkstra(0, 0);
    int selfCost = resSelf.first;
    std::vector<int> selfPath = resSelf.second;
    assert(selfCost     == 0);
    assert(selfPath.size() == 1);
    assert(selfPath[0]  == 0);
    std::cout << "[PASS] dijkstra(Mumbai→Mumbai) = 0ms (same node)\n";

    // ── Test 4: Dijkstra from Delhi ──────────────────────────
    // Delhi → User: Delhi→Singapore→User = 20+5=25ms
    auto res2 = cdn.dijkstra(1, 5);
    int cost2 = res2.first;
    std::vector<int> path2 = res2.second;
    assert(cost2 == 25);
    assert(path2[0] == 1);   // Delhi
    assert(path2[1] == 3);   // Singapore
    assert(path2[2] == 5);   // User
    std::cout << "[PASS] dijkstra(Delhi→User) = 25ms via Singapore\n";

    // ── Test 5: Shortest not same as fewest hops ──────────────
    // Delhi → User: fewest hops via London (2 hops) = 55ms
    //               fewer cost via Singapore (2 hops) = 25ms
    // Both are 2 hops, but Dijkstra picks Singapore (lower cost)
    assert(path2[1] == 3);  // Singapore NOT London
    std::cout << "[PASS] Dijkstra picks cost-optimal, not just fewest hops\n";

    // ── Test 6: No path (isolated node) ──────────────────────
    DeliveryOptimizer tiny(3);
    tiny.addEdge(0, 1, 10);  // Node 2 is isolated
    auto resNoPath = tiny.dijkstra(0, 2);
    int noPathCost = resNoPath.first;
    std::vector<int> noPath = resNoPath.second;
    assert(noPathCost == -1);
    assert(noPath.empty());
    std::cout << "[PASS] dijkstra returns -1 for unreachable node\n";

    // ── Test 7: updateEdgeCost() — simulate congestion ────────
    // Congestion on Bangalore→Singapore: cost 15 → 50ms
    cdn.updateEdgeCost(2, 3, 50);
    auto res3 = cdn.dijkstra(0, 5);
    int cost3 = res3.first;
    std::vector<int> path3 = res3.second;
    // Now Mumbai→Bangalore→Singapore = 8+50+5=63ms
    // Mumbai→Delhi→Singapore = 10+20+5=35ms  ← new winner
    assert(cost3 == 35);
    assert(path3[0] == 0);   // Mumbai
    assert(path3[1] == 1);   // Delhi (rerouted)
    assert(path3[2] == 3);   // Singapore
    assert(path3[3] == 5);   // User
    std::cout << "[PASS] After congestion on Bangalore, rerouted via Delhi (35ms)\n";

    // ── Test 8: Restore edge and verify re-routing ────────────
    cdn.updateEdgeCost(2, 3, 15);  // Restore
    auto res4 = cdn.dijkstra(0, 5);
    int cost4 = res4.first;
    std::vector<int> path4 = res4.second;
    assert(cost4 == 28);            // Back to Bangalore route
    std::cout << "[PASS] After restoring Bangalore, optimal route returns to 28ms\n";

    // ── Test 9: displayGraph() ────────────────────────────────
    std::cout << "\n--- displayGraph() ---";
    cdn.displayGraph();

    // ── Test 10: displayRoute() ───────────────────────────────
    std::cout << "--- displayRoute(Mumbai → User) ---";
    cdn.displayRoute(0, 5);

    std::cout << "--- displayRoute(after congestion on Bangalore) ---";
    cdn.updateEdgeCost(2, 3, 50);
    cdn.displayRoute(0, 5);
    cdn.updateEdgeCost(2, 3, 15);   // Restore

    // ── Test 11: displayAllRoutes() ───────────────────────────
    std::cout << "--- displayAllRoutes(from Mumbai) ---";
    cdn.displayAllRoutes(0);

    std::cout << "\n===============================================\n";
    std::cout << "  All DeliveryOptimizer tests passed!\n";
    std::cout << "===============================================\n\n";
    return 0;
}
