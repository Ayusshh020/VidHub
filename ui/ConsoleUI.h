// ui/ConsoleUI.h
// Purpose: Header file defining the ConsoleUI class, implementing the main console application menu loops.

#pragma once

#include "../data/DummyData.h"
#include "../entities/Video.h"
#include "../entities/User.h"
#include "../entities/Creator.h"
#include "../entities/Admin.h"

#include <string>

/**
 * @class ConsoleUI
 * @brief Coordinates the interactive text-based console menus.
 *
 * Implements roles for viewers, creators, and administrators. It routes user actions
 * to the underlying modules (Catalog, Segmenting, Rankings, Graphs, Dijkstra paths).
 */
class ConsoleUI {
private:
    DummyData& data; // Reference to Mock/Dummy database populated at startup

    // Active session details
    std::string  activeUserId;
    std::string  activeRole;
    WatchHistory activeWatchHistory;
    RecommendationQueue activeRecommendations;

    // Helper functions for terminal display and inputs
    void        clearScreen()              const;
    void        printBanner()             const;
    void        printSectionHeader(const std::string& title) const;
    int         getIntInput(const std::string& prompt,
                            int min, int max)    const;
    std::string getStringInput(const std::string& prompt) const;

    // Session login helpers
    void selectSession();
    User*    findUser(const std::string& userId);
    Creator* findCreator(const std::string& userId);

    // Video play/details helpers
    Video* findVideo(const std::string& videoId);
    void   watchVideo(const std::string& videoId);

    // Menu loop coordinators
    void viewerMenu();
    void creatorMenu();
    void adminMenu();

    // Viewer workflows
    void showHomeFeed();
    void browseAllVideos();
    void searchVideos();
    void watchFromCatalog();
    void showWatchHistory();
    void showRecommendations();
    void playNextRecommendation();
    void shareVideo();

    // Creator workflows
    void showCreatorDashboard();
    void uploadVideo();
    void deleteVideo();

    // Administrator workflows
    void showAdminDashboard();
    void inspectLicenseRegistry();
    void showPlatformRankings();
    void showSharingNetwork();
    void showDeliveryRoutes();
    void revokeLicense();

public:
    /**
     * @brief Constructs ConsoleUI with a reference to the global database.
     */
    explicit ConsoleUI(DummyData& data);

    /**
     * @brief Launches the application login select session screen and enters the command loop.
     */
    void run();
};
