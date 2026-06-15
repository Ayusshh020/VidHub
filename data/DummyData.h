#pragma once

#include "../entities/Video.h"
#include "../entities/User.h"
#include "../entities/Creator.h"
#include "../entities/Admin.h"
#include "../modules/WatchHistory.h"
#include "../modules/RecommendationQueue.h"
#include "../modules/LicenseRegistry.h"
#include "../modules/VideoCatalog.h"
#include "../modules/RankingSystem.h"
#include "../modules/VideoSegmentManager.h"
#include "../modules/SharingNetwork.h"
#include "../modules/DeliveryOptimizer.h"

#include <vector>

// =============================================================
//  DummyData — Populates all DSA modules at application start.
//
//  Role: Pure data factory. Creates realistic test objects and
//  feeds them into each DSA module via its public interface.
//  No DSA logic lives here — only construction and loading.
//
//  Dependency rule: DummyData may include everything.
//  Nothing else should include DummyData except main.cpp.
// =============================================================

class DummyData {
public:
    // Pre-built entity collections (public for ConsoleUI access)
    std::vector<Video>   videos;
    std::vector<User>    users;
    std::vector<Creator> creators;
    Admin                admin;

    // DSA module instances (public — ConsoleUI holds references)
    LicenseRegistry      licenseRegistry;
    VideoCatalog         videoCatalog;
    RankingSystem        rankingSystem;
    WatchHistory         watchHistory;
    RecommendationQueue  recommendationQueue;
    SharingNetwork       sharingNetwork;
    DeliveryOptimizer    deliveryOptimizer;

    // Constructor: builds everything
    DummyData();

private:
    void loadLicenses();
    void loadVideos();
    void loadUsers();
    void loadCreators();
    void loadAdmin();
    void loadRankings();
    void loadSharingNetwork();
    // DeliveryOptimizer CDN graph built in constructor directly
};
