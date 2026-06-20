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

class DummyData {
public:
    std::vector<Video>   videos;
    std::vector<User>    users;
    std::vector<Creator> creators;
    Admin                admin;

    LicenseRegistry      licenseRegistry;
    VideoCatalog         videoCatalog;
    RankingSystem        rankingSystem;
    WatchHistory         watchHistory;
    RecommendationQueue  recommendationQueue;
    SharingNetwork       sharingNetwork;
    DeliveryOptimizer    deliveryOptimizer;

    DummyData();

private:
    void loadLicenses();
    void loadVideos();
    void loadUsers();
    void loadCreators();
    void loadAdmin();
    void loadRankings();
    void loadSharingNetwork();
};
