// test_license.cpp — Step 7 verification driver
// Compile: g++ -std=c++17 -o test_lr test_license.cpp
//              modules/LicenseRegistry.cpp
// Run:     ./test_lr

#include "modules/LicenseRegistry.h"
#include <iostream>
#include <cassert>

int main() {
    std::cout << "\n================================================\n";
    std::cout << "  VidHub — Step 7: LicenseRegistry Test\n";
    std::cout << "================================================\n\n";

    LicenseRegistry lr;

    // ── Test 1: Empty registry ────────────────────────────────
    assert(lr.getCount()       == 0);
    assert(lr.getActiveCount() == 0);
    std::cout << "[PASS] Empty registry on construction\n";

    // ── Test 2: registerLicense ───────────────────────────────
    LicenseInfo l1("MIT-AJ-002", "Arjun Dev",   "USR-003", "MIT", "2026-06-01");
    LicenseInfo l2("MIT-AJ-003", "Arjun Dev",   "USR-003", "MIT", "2026-06-01");
    LicenseInfo l3("CC-EDU-001", "CodeBase",     "USR-010", "CC",  "2025-12-31");
    LicenseInfo l4("CC-HLTH-004","YogaLife",     "USR-011", "CC",  "2025-08-15");
    LicenseInfo l5("PROP-WG-006","WebGuru",      "USR-012", "Proprietary", "NEVER");

    bool r1 = lr.registerLicense(l1);
    bool r2 = lr.registerLicense(l2);
    bool r3 = lr.registerLicense(l3);
    bool r4 = lr.registerLicense(l4);
    bool r5 = lr.registerLicense(l5);

    assert(r1 && r2 && r3 && r4 && r5);
    assert(lr.getCount()       == 5);
    assert(lr.getActiveCount() == 5);
    std::cout << "[PASS] registerLicense() — 5 licenses added\n";

    // ── Test 3: Duplicate rejection ───────────────────────────
    bool dup = lr.registerLicense(l1);   // MIT-AJ-002 already exists
    assert(dup == false);
    assert(lr.getCount() == 5);          // Count unchanged
    std::cout << "[PASS] Duplicate key rejected (count still 5)\n";

    // ── Test 4: verify() — active license ────────────────────
    assert(lr.verify("MIT-AJ-002")  == true);
    assert(lr.verify("CC-EDU-001")  == true);
    assert(lr.verify("PROP-WG-006") == true);
    std::cout << "[PASS] verify() returns true for 3 active licenses\n";

    // ── Test 5: verify() — key not in registry ────────────────
    assert(lr.verify("FAKE-KEY-999") == false);
    assert(lr.verify("")             == false);
    std::cout << "[PASS] verify() returns false for unknown keys\n";

    // ── Test 6: revokeLicense() ───────────────────────────────
    bool rv = lr.revokeLicense("CC-EDU-001");
    assert(rv == true);
    assert(lr.getCount()       == 5);    // Record still in map (audit trail)
    assert(lr.getActiveCount() == 4);    // One fewer active
    std::cout << "[PASS] revokeLicense() — CC-EDU-001 revoked (entry preserved)\n";

    // ── Test 7: verify() on revoked license ───────────────────
    assert(lr.verify("CC-EDU-001") == false);
    std::cout << "[PASS] verify() returns false for revoked license\n";

    // ── Test 8: exists() vs verify() distinction ─────────────
    assert(lr.exists("CC-EDU-001") == true);   // Still in map
    assert(lr.verify("CC-EDU-001") == false);  // But not active
    std::cout << "[PASS] exists()=true but verify()=false for revoked key\n";

    // ── Test 9: getInfo() retrieval ───────────────────────────
    const LicenseInfo& info = lr.getInfo("MIT-AJ-002");
    assert(info.ownerName == "Arjun Dev");
    assert(info.type      == "MIT");
    assert(info.isActive  == true);
    std::cout << "[PASS] getInfo() retrieved correct LicenseInfo\n";

    // ── Test 10: removeLicense() hard delete ──────────────────
    bool rm = lr.removeLicense("PROP-WG-006");
    assert(rm == true);
    assert(lr.getCount()  == 4);
    assert(lr.exists("PROP-WG-006") == false);
    std::cout << "[PASS] removeLicense() — PROP-WG-006 hard-deleted\n";

    // ── Test 11: revoke non-existent key ─────────────────────
    bool badRv = lr.revokeLicense("NOT-A-KEY");
    assert(badRv == false);
    std::cout << "[PASS] revokeLicense() returns false for unknown key\n";

    // ── Test 12: displayAll() ─────────────────────────────────
    std::cout << "\n--- displayAll() ---";
    lr.displayAll();

    // ── Test 13: displayActive() ──────────────────────────────
    std::cout << "--- displayActive() ---";
    lr.displayActive();

    // ── Test 14: displayRevoked() ────────────────────────────
    std::cout << "--- displayRevoked() ---";
    lr.displayRevoked();

    std::cout << "\n================================================\n";
    std::cout << "  All LicenseRegistry tests passed!\n";
    std::cout << "================================================\n\n";

    return 0;
}
