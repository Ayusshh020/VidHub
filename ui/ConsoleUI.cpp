#include "ConsoleUI.h"
#include "../utils/Helpers.h"

#include <iostream>
#include <limits>
#include <algorithm>
#include <string>

static const std::string VIDHUB_VERSION = "1.0.0";

ConsoleUI::ConsoleUI(DummyData& data)
    : data(data),
      activeUserId(""),
      activeRole(""),
      activeWatchHistory(20),
      activeRecommendations(8)
{}

void ConsoleUI::clearScreen() const {
#ifdef _WIN32
    system("cls");
#else
    system("clear");
#endif
}

void ConsoleUI::printBanner() const {
    std::cout << "\n";
    std::string border;
    for (int i = 0; i < 60; ++i) border += "\xE2\x95\x90";

    std::cout << "  \xE2\x95\x94" << border << "\xE2\x95\x97\n";
    std::cout << "  \xE2\x95\x91";
    std::cout << "         \xF0\x9F\x8E\xAC  VidHub  v" << VIDHUB_VERSION;
    std::cout << "  \xE2\x80\x94  DSA Video Platform         ";
    std::cout << "\xE2\x95\x91\n";
    std::cout << "  \xE2\x95\x9A" << border << "\xE2\x95\x9D\n\n";
}

void ConsoleUI::printSectionHeader(const std::string& title) const {
    std::cout << "\n  " << std::string(58, '=') << "\n";
    std::cout << "  " << title << "\n";
    std::cout << "  " << std::string(58, '=') << "\n";
}

int ConsoleUI::getIntInput(const std::string& prompt, int min, int max) const {
    int val;
    while (true) {
        std::cout << prompt;
        if (std::cin >> val && val >= min && val <= max) {
            std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
            return val;
        }
        std::cin.clear();
        std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
        std::cout << "  \xE2\x9D\x8C  Please enter a number between " << min << " and " << max << "\n";
    }
}

std::string ConsoleUI::getStringInput(const std::string& prompt) const {
    std::cout << prompt;
    std::string s;
    std::getline(std::cin, s);
    return s;
}

User* ConsoleUI::findUser(const std::string& userId) {
    for (auto& u : data.users)
        if (toLower(u.getUserId()) == toLower(userId)) return &u;
    return nullptr;
}

Creator* ConsoleUI::findCreator(const std::string& userId) {
    for (auto& c : data.creators)
        if (toLower(c.getUserId()) == toLower(userId)) return &c;
    return nullptr;
}

Video* ConsoleUI::findVideo(const std::string& videoId) {
    for (auto& v : data.videos)
        if (toLower(v.videoId) == toLower(videoId)) return &v;
    return nullptr;
}

void ConsoleUI::selectSession() {
    printBanner();
    printSectionHeader("  \xF0\x9F\x94\x91  Select a Session (Login As)");

    std::cout << "\n  --- Viewers ---\n";
    int idx = 1;
    for (const auto& u : data.users) {
        std::cout << "  [" << idx++ << "] ";
        u.displaySummary();
    }

    std::cout << "\n  --- Creators ---\n";
    for (const auto& c : data.creators) {
        std::cout << "  [" << idx++ << "] ";
        c.displaySummary();
    }

    std::cout << "\n  --- Admin ---\n";
    std::cout << "  [" << idx++ << "] ";
    data.admin.displaySummary();

    std::cout << "\n";
    int choice = getIntInput("  Select session [1-" + std::to_string(idx - 1) + "]: ", 1, idx - 1);

    int total_users    = static_cast<int>(data.users.size());
    int total_creators = static_cast<int>(data.creators.size());

    if (choice <= total_users) {
        User& u = data.users[choice - 1];
        activeUserId = u.getUserId();
        activeRole   = "viewer";
        std::cout << "\n  \xE2\x9C\x85  Logged in as Viewer: " << u.getName() << "\n";
    } else if (choice <= total_users + total_creators) {
        Creator& c = data.creators[choice - total_users - 1];
        activeUserId = c.getUserId();
        activeRole   = "creator";
        std::cout << "\n  \xE2\x9C\x85  Logged in as Creator: " << c.getChannelName() << "\n";
    } else {
        activeUserId = data.admin.getUserId();
        activeRole   = "admin";
        std::cout << "\n  \xE2\x9C\x85  Logged in as Admin: " << data.admin.getName() << "\n";
    }

    activeWatchHistory     = WatchHistory(20);
    activeRecommendations  = RecommendationQueue(8);
}

void ConsoleUI::watchVideo(const std::string& videoId) {
    Video* vp = findVideo(videoId);
    if (!vp) {
        std::cout << "  \xE2\x9D\x8C  Video not found.\n";
        return;
    }

    vp->incrementViews();
    int watchSec = vp->durationSec;
    vp->addWatchTime(watchSec);
    vp->addLike();
    vp->recalculateScore();

    data.rankingSystem.updateVideo(*vp);
    activeWatchHistory.push(*vp);

    VideoSegmentManager vsm(vp->videoId, vp->durationSec);
    vsm.segmentVideo(30, "720p");

    std::cout << "\n  \xF0\x9F\x8E\xAC  Now Playing:\n";
    vp->displayFull();
    vsm.displayPlaybackStatus(vsm.getTotalSegments());

    activeRecommendations.populate(vp->genre, vp->videoId, data.videos);
    std::cout << "  \xF0\x9F\x8E\xAF  " << activeRecommendations.getSize()
              << " recommendations queued.\n";
}

void ConsoleUI::viewerMenu() {
    while (true) {
        printSectionHeader("  \xF0\x9F\x91\xA4  Viewer Menu");
        std::cout << "  [1] Home Feed (Top Videos by Score)\n";
        std::cout << "  [2] Browse All Videos (A-Z)\n";
        std::cout << "  [3] Search Videos\n";
        std::cout << "  [4] Watch a Video\n";
        std::cout << "  [5] Watch History\n";
        std::cout << "  [6] Recommended — Up Next\n";
        std::cout << "  [7] Play Next Recommendation\n";
        std::cout << "  [8] Switch Session\n";
        std::cout << "  [0] Exit VidHub\n\n";

        int choice = getIntInput("  > ", 0, 8);
        switch (choice) {
            case 1: showHomeFeed();             break;
            case 2: browseAllVideos();          break;
            case 3: searchVideos();             break;
            case 4: watchFromCatalog();         break;
            case 5: showWatchHistory();         break;
            case 6: showRecommendations();      break;
            case 7: playNextRecommendation();   break;
            case 8: selectSession(); return;
            case 0: std::cout << "\n  Goodbye!\n\n"; exit(0);
        }
    }
}

void ConsoleUI::creatorMenu() {
    while (true) {
        printSectionHeader("  \xF0\x9F\x8E\xAC  Creator Menu");
        std::cout << "  [1] My Dashboard\n";
        std::cout << "  [2] Upload a Video\n";
        std::cout << "  [3] Delete a Video\n";
        std::cout << "  [4] Home Feed (Top Videos)\n";
        std::cout << "  [5] Browse Catalog (A-Z)\n";
        std::cout << "  [6] Watch a Video\n";
        std::cout << "  [7] Watch History\n";
        std::cout << "  [8] Switch Session\n";
        std::cout << "  [0] Exit VidHub\n\n";

        int choice = getIntInput("  > ", 0, 8);
        switch (choice) {
            case 1: showCreatorDashboard(); break;
            case 2: uploadVideo();          break;
            case 3: deleteVideo();          break;
            case 4: showHomeFeed();         break;
            case 5: browseAllVideos();      break;
            case 6: watchFromCatalog();     break;
            case 7: showWatchHistory();     break;
            case 8: selectSession(); return;
            case 0: std::cout << "\n  Goodbye!\n\n"; exit(0);
        }
    }
}

void ConsoleUI::adminMenu() {
    while (true) {
        printSectionHeader("  \xE2\x9A\x99\xEF\xB8\x8F  Admin Panel");
        std::cout << "  [1] Admin Dashboard\n";
        std::cout << "  [2] Platform Rankings\n";
        std::cout << "  [3] License Registry\n";
        std::cout << "  [4] Revoke a License\n";
        std::cout << "  [5] Sharing Network (BFS/DFS)\n";
        std::cout << "  [6] CDN Delivery Optimizer\n";
        std::cout << "  [7] Browse Catalog (A-Z)\n";
        std::cout << "  [8] Switch Session\n";
        std::cout << "  [0] Exit VidHub\n\n";

        int choice = getIntInput("  > ", 0, 8);
        switch (choice) {
            case 1: showAdminDashboard();        break;
            case 2: showPlatformRankings();      break;
            case 3: inspectLicenseRegistry();    break;
            case 4: revokeLicense();             break;
            case 5: showSharingNetwork();        break;
            case 6: showDeliveryRoutes();        break;
            case 7: browseAllVideos();           break;
            case 8: selectSession(); return;
            case 0: std::cout << "\n  Goodbye!\n\n"; exit(0);
        }
    }
}

void ConsoleUI::showHomeFeed() {
    printSectionHeader("  \xF0\x9F\x8F\xA0  Home Feed — Top 10 Videos");
    data.rankingSystem.displayRankings(10);
}

void ConsoleUI::browseAllVideos() {
    printSectionHeader("  \xF0\x9F\x93\xBC  All Videos (A-Z)");
    data.videoCatalog.displayAll();
}

void ConsoleUI::searchVideos() {
    printSectionHeader("  \xF0\x9F\x94\x8D  Search Videos");
    std::cout << "  [1] Search by keyword\n";
    std::cout << "  [2] Search by genre\n";
    int mode = getIntInput("  > ", 1, 2);

    if (mode == 1) {
        std::string kw = getStringInput("  Enter keyword: ");
        auto results = data.videoCatalog.searchByKeyword(kw);
        data.videoCatalog.displaySearchResults(results, kw);
    } else {
        std::cout << "  Genres: Education | Tech | Health | Lifestyle | Travel | Finance | Music\n";
        std::string genre = getStringInput("  Enter genre: ");
        auto results = data.videoCatalog.searchByGenre(genre);
        data.videoCatalog.displaySearchResults(results, genre);
    }
}

void ConsoleUI::watchFromCatalog() {
    printSectionHeader("  \xF0\x9F\x8E\xAC  Watch a Video");
    data.videoCatalog.displayAll();
    std::string vid = getStringInput("  Enter Video ID (e.g. VID-001): ");
    watchVideo(vid);
}

void ConsoleUI::showWatchHistory() {
    printSectionHeader("  \xF0\x9F\x93\x8B  Watch History");
    activeWatchHistory.display();
}

void ConsoleUI::showRecommendations() {
    printSectionHeader("  \xF0\x9F\x8E\xAF  Up Next — Recommendations");
    activeRecommendations.display();
}

void ConsoleUI::playNextRecommendation() {
    if (activeRecommendations.isEmpty()) {
        std::cout << "\n  No recommendations in queue. Watch a video first.\n";
        return;
    }
    Video next = activeRecommendations.dequeue();
    watchVideo(next.videoId);
}

void ConsoleUI::showCreatorDashboard() {
    Creator* cp = findCreator(activeUserId);
    if (!cp) { std::cout << "  Creator not found.\n"; return; }
    printSectionHeader("  \xF0\x9F\x8E\xAC  Creator Dashboard");
    cp->displayFull();
}

void ConsoleUI::uploadVideo() {
    Creator* cp = findCreator(activeUserId);
    if (!cp) { std::cout << "  Creator not found.\n"; return; }

    printSectionHeader("  \xF0\x9F\x93\xA4  Upload New Video");

    std::string title   = getStringInput("  Title      : ");
    std::string genre   = getStringInput("  Genre      : ");
    std::string licKey  = getStringInput("  License Key: ");
    int         dur     = getIntInput("  Duration (sec) [30-7200]: ", 30, 7200);

    if (!data.licenseRegistry.verify(licKey)) {
        std::cout << "\n  \xE2\x9D\x8C  License \"" << licKey << "\" is invalid or revoked. Upload rejected.\n";
        return;
    }

    std::string newId = "VID-0" + std::to_string(data.videos.size() + 1);
    std::string date  = "2024-06-15";

    Video newVid(newId, title, activeUserId, genre, dur, licKey, date);
    newVid.recalculateScore();

    data.videos.push_back(newVid);
    data.videoCatalog.insert(newVid);
    data.rankingSystem.addVideo(newVid);
    cp->addUploadedVideo(newId);

    std::cout << "\n  \xE2\x9C\x85  Video uploaded successfully!\n";
    newVid.displaySummary();
}

void ConsoleUI::deleteVideo() {
    Creator* cp = findCreator(activeUserId);
    if (!cp || cp->getTotalUploads() == 0) {
        std::cout << "  No uploaded videos to delete.\n";
        return;
    }

    printSectionHeader("  \xF0\x9F\x97\x91  Delete a Video");
    cp->displayFull();

    std::string vid = getStringInput("  Enter Video ID to delete: ");

    auto ids = cp->getUploadedVideoIds();
    bool owns = false;
    for (const auto& id : ids) if (toLower(id) == toLower(vid)) { owns = true; break; }

    if (!owns) {
        std::cout << "  \xE2\x9D\x8C  You don't own video " << vid << ".\n";
        return;
    }

    data.videoCatalog.remove(vid);
    data.rankingSystem.removeVideo(vid);
    cp->removeUploadedVideo(vid);
    data.videos.erase(std::remove_if(data.videos.begin(), data.videos.end(),
        [&](const Video& v){ return toLower(v.videoId) == toLower(vid); }), data.videos.end());

    std::cout << "  \xE2\x9C\x85  Video " << vid << " deleted.\n";
}

void ConsoleUI::showAdminDashboard() {
    printSectionHeader("  \xE2\x9A\x99\xEF\xB8\x8F  Admin Dashboard");
    data.admin.displayFull();

    std::cout << "  Platform Summary:\n";
    std::cout << "    Total Videos  : " << data.videos.size()               << "\n";
    std::cout << "    Total Users   : " << data.users.size()                << "\n";
    std::cout << "    Total Creators: " << data.creators.size()             << "\n";
    std::cout << "    Licenses (total/active): "
              << data.licenseRegistry.getCount() << " / "
              << data.licenseRegistry.getActiveCount() << "\n";
    std::cout << "    Catalog size  : " << data.videoCatalog.getCount()     << "\n\n";
}

void ConsoleUI::inspectLicenseRegistry() {
    printSectionHeader("  \xF0\x9F\x94\x92  License Registry");
    std::cout << "  [1] View All\n  [2] View Active\n  [3] View Revoked\n";
    int c = getIntInput("  > ", 1, 3);
    if      (c == 1) data.licenseRegistry.displayAll();
    else if (c == 2) data.licenseRegistry.displayActive();
    else             data.licenseRegistry.displayRevoked();
}

void ConsoleUI::showPlatformRankings() {
    printSectionHeader("  \xF0\x9F\x8F\x86  Platform Rankings");
    data.rankingSystem.displayRankings(10);
}

void ConsoleUI::showSharingNetwork() {
    printSectionHeader("  \xF0\x9F\x8C\x90  Video Sharing Network");
    std::cout << "  [1] View Adjacency List\n";
    std::cout << "  [2] BFS Traversal from a node\n";
    std::cout << "  [3] DFS Traversal from a node\n";
    int c = getIntInput("  > ", 1, 3);
    if (c == 1) {
        data.sharingNetwork.displayNetwork();
    } else {
        std::string node = getStringInput("  Enter starting node (e.g. Arjun Dev): ");
        if (c == 2) data.sharingNetwork.displayBFSTraversal(node);
        else        data.sharingNetwork.displayDFSTraversal(node);
    }
}

void ConsoleUI::showDeliveryRoutes() {
    printSectionHeader("  \xF0\x9F\x9A\x80  CDN Delivery Optimizer");
    data.deliveryOptimizer.displayGraph();
    std::cout << "  [1] Find optimal route between two nodes\n";
    std::cout << "  [2] Show all routes from Mumbai-Origin (node 0)\n";
    int c = getIntInput("  > ", 1, 2);
    if (c == 1) {
        std::cout << "  Nodes: 0=Mumbai, 1=Delhi, 2=Bangalore, 3=Singapore, 4=London, 5=User\n";
        int src  = getIntInput("  Source node [0-5]: ",      0, 5);
        int dest = getIntInput("  Destination node [0-5]: ", 0, 5);
        data.deliveryOptimizer.displayRoute(src, dest);
    } else {
        data.deliveryOptimizer.displayAllRoutes(0);
    }
}

void ConsoleUI::revokeLicense() {
    printSectionHeader("  \xE2\x9C\x98  Revoke a License");
    data.licenseRegistry.displayActive();
    std::string key = getStringInput("  Enter license key to revoke: ");
    if (data.licenseRegistry.revokeLicense(key))
        std::cout << "  \xE2\x9C\x85  License \"" << key << "\" has been revoked.\n";
    else
        std::cout << "  \xE2\x9D\x8C  License key not found.\n";
}

void ConsoleUI::run() {
    clearScreen();
    selectSession();

    while (true) {
        if      (activeRole == "viewer")  viewerMenu();
        else if (activeRole == "creator") creatorMenu();
        else if (activeRole == "admin")   adminMenu();
        else break;
    }
}
