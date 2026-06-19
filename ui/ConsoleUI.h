#pragma once
// Note: Comments are only for understanding—do not modify the code itself.

#include "../data/DummyData.h"
#include "../entities/Video.h"
#include "../entities/User.h"
#include "../entities/Creator.h"
#include "../entities/Admin.h"

#include <string>

class ConsoleUI {
private:
    DummyData& data;

    std::string  activeUserId;
    std::string  activeRole;
    WatchHistory activeWatchHistory;
    RecommendationQueue activeRecommendations;

    void        clearScreen()              const;
    void        printBanner()             const;
    void        printSectionHeader(const std::string& title) const;
    int         getIntInput(const std::string& prompt,
                            int min, int max)    const;
    std::string getStringInput(const std::string& prompt) const;

    void selectSession();
    User*    findUser(const std::string& userId);
    Creator* findCreator(const std::string& userId);

    Video* findVideo(const std::string& videoId);
    void   watchVideo(const std::string& videoId);

    void viewerMenu();
    void creatorMenu();
    void adminMenu();

    void showHomeFeed();
    void browseAllVideos();
    void searchVideos();
    void watchFromCatalog();
    void showWatchHistory();
    void showRecommendations();
    void playNextRecommendation();

    void showCreatorDashboard();
    void uploadVideo();
    void deleteVideo();

    void showAdminDashboard();
    void inspectLicenseRegistry();
    void showPlatformRankings();
    void showSharingNetwork();
    void showDeliveryRoutes();
    void revokeLicense();

public:
    explicit ConsoleUI(DummyData& data);
    void run();
};
