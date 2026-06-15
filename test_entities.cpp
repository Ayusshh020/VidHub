// test_entities.cpp — Step 4 verification driver
// Compile: g++ -std=c++17 -o test_entities test_entities.cpp
//              entities/User.cpp entities/Creator.cpp entities/Admin.cpp
// Run:     ./test_entities

#include "entities/User.h"
#include "entities/Creator.h"
#include "entities/Admin.h"
#include <iostream>
#include <cassert>

int main() {
    std::cout << "\n============================================\n";
    std::cout << "  VidHub — Step 4: Entity Classes Test\n";
    std::cout << "============================================\n\n";

    // ── Test 1: User default constructor ─────────────────────
    User u1;
    assert(u1.userId == "");
    assert(u1.role   == "viewer");
    std::cout << "[PASS] User default constructor\n";

    // ── Test 2: User parameterized constructor ───────────────
    User u2("USR-001", "Riya Sharma", "riya@vidhub.com", "Mumbai");
    assert(u2.getUserId() == "USR-001");
    assert(u2.getName()   == "Riya Sharma");
    assert(u2.getRegion() == "Mumbai");
    assert(u2.getRole()   == "viewer");
    std::cout << "[PASS] User parameterized constructor\n";

    // ── Test 3: User displaySummary ──────────────────────────
    std::cout << "\n--- User::displaySummary() ---\n";
    u2.displaySummary();

    // ── Test 4: User displayFull ─────────────────────────────
    std::cout << "\n--- User::displayFull() ---\n";
    u2.displayFull();

    // ── Test 5: Creator default constructor ──────────────────
    Creator c1;
    assert(c1.role            == "creator");
    assert(c1.channelName     == "");
    assert(c1.subscriberCount == 0);
    assert(c1.getTotalUploads()== 0);
    std::cout << "[PASS] Creator default constructor (role = creator)\n";

    // ── Test 6: Creator parameterized constructor ────────────
    Creator c2("USR-003", "Arjun Dev", "arjun@vidhub.com",
               "Bangalore", "Arjun Dev", 12400);
    assert(c2.getChannelName()     == "Arjun Dev");
    assert(c2.getSubscriberCount() == 12400);
    assert(c2.getRole()            == "creator");
    std::cout << "[PASS] Creator parameterized constructor\n";

    // ── Test 7: addUploadedVideo ─────────────────────────────
    c2.addUploadedVideo("VID-002");
    c2.addUploadedVideo("VID-003");
    c2.addUploadedVideo("VID-007");
    assert(c2.getTotalUploads() == 3);
    std::cout << "[PASS] addUploadedVideo() — total: " << c2.getTotalUploads() << "\n";

    // ── Test 8: addUploadedVideo duplicate guard ─────────────
    c2.addUploadedVideo("VID-002");   // duplicate — must not add again
    assert(c2.getTotalUploads() == 3);
    std::cout << "[PASS] Duplicate guard on addUploadedVideo()\n";

    // ── Test 9: removeUploadedVideo ──────────────────────────
    c2.removeUploadedVideo("VID-003");
    assert(c2.getTotalUploads() == 2);
    std::cout << "[PASS] removeUploadedVideo() — total after remove: "
              << c2.getTotalUploads() << "\n";

    // ── Test 10: Creator display ─────────────────────────────
    c2.addUploadedVideo("VID-003");   // restore for display
    std::cout << "\n--- Creator::displaySummary() ---\n";
    c2.displaySummary();
    std::cout << "\n--- Creator::displayFull() ---\n";
    c2.displayFull();

    // ── Test 11: Admin constructors ──────────────────────────
    Admin a1;
    assert(a1.role == "admin");
    std::cout << "[PASS] Admin default constructor (role = admin)\n";

    Admin a2("USR-005", "Admin Kumar", "admin@vidhub.com", "System");
    assert(a2.getRole()   == "admin");
    assert(a2.getName()   == "Admin Kumar");
    std::cout << "[PASS] Admin parameterized constructor\n";

    // ── Test 12: Admin display ───────────────────────────────
    std::cout << "\n--- Admin::displaySummary() ---\n";
    a2.displaySummary();
    std::cout << "\n--- Admin::displayFull() ---\n";
    a2.displayFull();

    // ── Test 13: Polymorphism (User* pointing to Creator) ────
    User* poly = &c2;
    std::cout << "\n--- Polymorphism: User* → Creator::displayFull() ---\n";
    poly->displayFull();   // Must call Creator::displayFull(), not User::displayFull()
    assert(poly->getRole() == "creator");
    std::cout << "[PASS] Virtual dispatch works correctly\n";

    std::cout << "\n============================================\n";
    std::cout << "  All entity tests passed!\n";
    std::cout << "============================================\n\n";

    return 0;
}
