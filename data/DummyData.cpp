// Note: Comments are only for understanding—do not modify the code itself.
#include "DummyData.h"

DummyData::DummyData()
    : admin("USR-005", "Admin Kumar", "admin@vidhub.com", "System"),
      deliveryOptimizer(6)
{
    loadLicenses();
    loadVideos();
    loadUsers();
    loadCreators();
    loadAdmin();
    loadRankings();
    loadSharingNetwork();

    deliveryOptimizer.setNodeName(0, "Mumbai-Origin");
    deliveryOptimizer.setNodeName(1, "Delhi-PoP");
    deliveryOptimizer.setNodeName(2, "Bangalore-PoP");
    deliveryOptimizer.setNodeName(3, "Singapore-Edge");
    deliveryOptimizer.setNodeName(4, "London-Edge");
    deliveryOptimizer.setNodeName(5, "User-Local");

    deliveryOptimizer.addEdge(0, 1, 10);
    deliveryOptimizer.addEdge(0, 2,  8);
    deliveryOptimizer.addEdge(1, 3, 20);
    deliveryOptimizer.addEdge(2, 3, 15);
    deliveryOptimizer.addEdge(3, 5,  5);
    deliveryOptimizer.addEdge(1, 4, 25);
    deliveryOptimizer.addEdge(4, 5, 30);
}

void DummyData::loadLicenses() {
    licenseRegistry.registerLicense({"MIT-AJ-002", "Arjun Dev",   "USR-003", "MIT",         "2026-06-01", true});
    licenseRegistry.registerLicense({"MIT-AJ-003", "Arjun Dev",   "USR-003", "MIT",         "2026-06-01", true});
    licenseRegistry.registerLicense({"CC-EDU-001", "CodeBase",    "USR-010", "CC",          "2025-12-31", true});
    licenseRegistry.registerLicense({"CC-HLTH-004","YogaLife",    "USR-004", "CC",          "2025-08-15", true});
    licenseRegistry.registerLicense({"PROP-RM-007","Riya Mehta",  "USR-001", "Proprietary", "NEVER",      true});
    licenseRegistry.revokeLicense("CC-EDU-001");
}

void DummyData::loadVideos() {
    auto make = [](const std::string& id,
                   const std::string& title,
                   const std::string& creator,
                   const std::string& genre,
                   int    dur,
                   const std::string& lic,
                   const std::string& date,
                   long   views, long likes, int watchSec) -> Video {
        Video v(id, title, creator, genre, dur, lic, date);
        v.viewCount         = views;
        v.likeCount         = likes;
        v.totalWatchTimeSec = watchSec;
        v.recalculateScore();
        return v;
    };

    videos = {
        make("VID-001","Python for Beginners",      "USR-003","Education",  2400,"MIT-AJ-002","2024-01-10", 22400, 1800, 22400),
        make("VID-002","Python OOP Crash Course",   "USR-003","Education",  2700,"MIT-AJ-003","2024-02-14", 18100, 1420, 18100),
        make("VID-003","Build a REST API",          "USR-003","Tech",       1800,"MIT-AJ-002","2024-03-01", 45200, 3100, 45200),
        make("VID-004","Morning Yoga Routine",      "USR-004","Health",     1200,"CC-HLTH-004","2024-03-15",38100, 2800, 38100),
        make("VID-005","Top 10 Travel Hacks",       "USR-003","Travel",      900,"MIT-AJ-003","2024-04-20", 31500, 1900, 31500),
        make("VID-006","JavaScript Basics",         "USR-003","Education",  3000,"MIT-AJ-002","2024-05-05", 12000,  980, 12000),
        make("VID-007","Vegan Cooking 101",         "USR-004","Lifestyle",   960,"CC-HLTH-004","2024-05-18",  9400,  760,  9400),
        make("VID-008","Stock Market for Dummies",  "USR-003","Finance",    1500,"MIT-AJ-003","2024-06-01", 27800, 2100, 27800),
        make("VID-009","Guitar Chords for Beginners","USR-004","Music",     1800,"CC-HLTH-004","2024-06-10",14200, 1100, 14200),
        make("VID-010","React.js Full Course",      "USR-003","Tech",       5400,"MIT-AJ-002","2024-06-20", 52300, 4200, 52300),
    };

    for (const auto& v : videos) {
        videoCatalog.insert(v);
    }
}

void DummyData::loadUsers() {
    users = {
        User("USR-001", "Riya Sharma",  "riya@vidhub.com",   "Mumbai"),
        User("USR-002", "Kabir Singh",  "kabir@vidhub.com",  "Delhi"),
        User("USR-006", "Meera Patel",  "meera@vidhub.com",  "Pune"),
        User("USR-007", "Rohan Verma",  "rohan@vidhub.com",  "Hyderabad"),
        User("USR-008", "Ananya Rao",   "ananya@vidhub.com", "Chennai"),
    };
}

void DummyData::loadCreators() {
    Creator c1("USR-003","Arjun Dev",  "arjun@vidhub.com",  "Bangalore","CodeWithArjun", 12400);
    c1.addUploadedVideo("VID-001");
    c1.addUploadedVideo("VID-002");
    c1.addUploadedVideo("VID-003");
    c1.addUploadedVideo("VID-005");
    c1.addUploadedVideo("VID-006");
    c1.addUploadedVideo("VID-008");
    c1.addUploadedVideo("VID-010");

    Creator c2("USR-004","Priya Yoga", "priya@vidhub.com",  "Mumbai",   "PriyaWellness",  8900);
    c2.addUploadedVideo("VID-004");
    c2.addUploadedVideo("VID-007");
    c2.addUploadedVideo("VID-009");

    Creator c3("USR-009","TechWorld",  "tech@vidhub.com",   "Noida",    "TechWorldHub",    340);

    creators = {c1, c2, c3};
}

void DummyData::loadAdmin() {
}

void DummyData::loadRankings() {
    for (const auto& v : videos)
        rankingSystem.addVideo(v);
}

void DummyData::loadSharingNetwork() {
    sharingNetwork.addShare("Arjun Dev",    "Riya Sharma");
    sharingNetwork.addShare("Arjun Dev",    "TechForum-IN");
    sharingNetwork.addShare("Riya Sharma",  "Kabir Singh");
    sharingNetwork.addShare("Riya Sharma",  "CollegeGroup-MU");
    sharingNetwork.addShare("TechForum-IN", "LinkedInPost-Tech");
    sharingNetwork.addShare("Kabir Singh",  "OfficeGroup-DL");
    sharingNetwork.addShare("CollegeGroup-MU","PeerA");
    sharingNetwork.addShare("CollegeGroup-MU","PeerB");
    sharingNetwork.addShare("LinkedInPost-Tech","ExtUser-SG");
    sharingNetwork.addShare("LinkedInPost-Tech","ExtUser-US");
    sharingNetwork.addShare("OfficeGroup-DL",  "Meera Patel");
}
