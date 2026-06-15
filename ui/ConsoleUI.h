#pragma once

#include "../data/DummyData.h"
#include "../entities/Video.h"
#include "../entities/User.h"
#include "../entities/Creator.h"
#include "../entities/Admin.h"

#include <string>

// =============================================================
//  ConsoleUI — The single presentation layer of VidHub.
//
//  Architecture rules:
//    - ConsoleUI is the ONLY class that reads from stdin /
//      prints menus. No module or entity does I/O except display().
//    - ConsoleUI calls module methods — never manipulates raw
//      DSA structures directly.
//    - All DSA modules are owned by DummyData, ConsoleUI
//      receives them by reference.
//
//  Session model:
//    At startup the user picks a "session" (which account they
//    are logging in as). The active role determines which menu
//    is shown:  viewer → viewerMenu()
//               creator → creatorMenu()
//               admin   → adminMenu()
// =============================================================

class ConsoleUI {
private:
    DummyData& data;      // All modules and entities live here

    // ── Session State ─────────────────────────────────────────
    std::string  activeUserId;
    std::string  activeRole;    // "viewer" | "creator" | "admin"
    WatchHistory activeWatchHistory;
    RecommendationQueue activeRecommendations;

    // ── Internal Helpers ──────────────────────────────────────
    void        clearScreen()              const;
    void        printBanner()             const;
    void        printSectionHeader(const std::string& title) const;
    int         getIntInput(const std::string& prompt,
                            int min, int max)    const;
    std::string getStringInput(const std::string& prompt) const;

    // Session picker
    void selectSession();
    User*    findUser(const std::string& userId);
    Creator* findCreator(const std::string& userId);

    // ── Video helper ──────────────────────────────────────────
    Video* findVideo(const std::string& videoId);
    void   watchVideo(const std::string& videoId);

    // ── Role Menus ────────────────────────────────────────────
    void viewerMenu();
    void creatorMenu();
    void adminMenu();

    // ── Viewer Features ───────────────────────────────────────
    void showHomeFeed();
    void browseAllVideos();
    void searchVideos();
    void watchFromCatalog();
    void showWatchHistory();
    void showRecommendations();
    void playNextRecommendation();

    // ── Creator Features ──────────────────────────────────────
    void showCreatorDashboard();
    void uploadVideo();
    void deleteVideo();

    // ── Admin Features ────────────────────────────────────────
    void showAdminDashboard();
    void inspectLicenseRegistry();
    void showPlatformRankings();
    void showSharingNetwork();
    void showDeliveryRoutes();
    void revokeLicense();

public:
    explicit ConsoleUI(DummyData& data);
    void run();   // Entry point — called by main()
};
