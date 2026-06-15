#include "Creator.h"

#include <iostream>
#include <iomanip>
#include <algorithm>   // std::find, std::remove
#include <string>

// ─────────────────────────────────────────────────────────────
//  File-local helpers
// ─────────────────────────────────────────────────────────────

static void printCreatorDivider() {
    std::cout << "  " << std::string(50, '-') << "\n";
}

// =============================================================
//  CONSTRUCTORS
// =============================================================

// Default constructor — zeroed channel data.
// Role is hardcoded to "creator" so menus route correctly.
Creator::Creator()
    : User(),
      channelName(""),
      subscriberCount(0)
{
    role = "creator";
}

// Parameterized constructor — used by DummyData.
// Role is hardcoded to "creator"; no caller should override it.
Creator::Creator(std::string userId,
                 std::string name,
                 std::string email,
                 std::string region,
                 std::string channelName,
                 long        subscriberCount)
    : User(userId, name, email, region, "creator"),
      channelName(channelName),
      subscriberCount(subscriberCount)
{}

// =============================================================
//  UPLOAD RECORD MANAGEMENT
// =============================================================

// Record that this creator uploaded a video.
// Called by ConsoleUI after the full upload workflow succeeds
// (license verified → BST inserted → Heap inserted → segmented).
// Does NOT insert video anywhere — Creator is a data holder only.
void Creator::addUploadedVideo(const std::string& videoId) {
    // Guard: avoid duplicate entries
    for (const auto& id : uploadedVideoIds) {
        if (id == videoId) return;
    }
    uploadedVideoIds.push_back(videoId);
}

// Remove a video ID from this creator's record.
// Called when ConsoleUI handles a "delete video" action.
void Creator::removeUploadedVideo(const std::string& videoId) {
    uploadedVideoIds.erase(
        std::remove(uploadedVideoIds.begin(), uploadedVideoIds.end(), videoId),
        uploadedVideoIds.end()
    );
}

// =============================================================
//  GETTERS
// =============================================================

std::string              Creator::getChannelName()     const { return channelName;      }
long                     Creator::getSubscriberCount() const { return subscriberCount;  }
std::vector<std::string> Creator::getUploadedVideoIds()const { return uploadedVideoIds; }
int                      Creator::getTotalUploads()    const {
    return static_cast<int>(uploadedVideoIds.size());
}

// =============================================================
//  DISPLAY
// =============================================================

// One-line summary — used in creator selection menus and
// admin overview lists.
//
// Output:
//   [USR-003] Arjun Dev              | Uploads: 3 | Subscribers: 12,400
void Creator::displaySummary() const {
    std::cout << "  ["
              << std::left << std::setw(7)  << userId      << "] "
              << std::left << std::setw(22) << channelName
              << " | Uploads: "      << std::setw(3)  << getTotalUploads()
              << " | Subscribers: " << subscriberCount
              << "\n";
}

// Full profile block — used on "Creator Dashboard" screen.
//
// Output:
//   --------------------------------------------------
//   🎬  Arjun Dev  [CREATOR]
//   --------------------------------------------------
//     User ID      : USR-003
//     Real Name    : Arjun Dev
//     Email        : arjun@vidhub.com
//     Region       : Bangalore
//     Channel      : Arjun Dev
//     Subscribers  : 12,400
//     Total Uploads: 3
//   --------------------------------------------------
//   Uploaded Video IDs:
//     [1] VID-002
//     [2] VID-003
//     [3] VID-007
//   --------------------------------------------------
void Creator::displayFull() const {
    printCreatorDivider();
    std::cout << "  \xF0\x9F\x8E\xAC  " << channelName << "  [CREATOR]\n";
    printCreatorDivider();
    std::cout << std::left;
    std::cout << "  " << std::setw(16) << "User ID"       << ": " << userId          << "\n";
    std::cout << "  " << std::setw(16) << "Real Name"     << ": " << name            << "\n";
    std::cout << "  " << std::setw(16) << "Email"         << ": " << email           << "\n";
    std::cout << "  " << std::setw(16) << "Region"        << ": " << region          << "\n";
    std::cout << "  " << std::setw(16) << "Channel"       << ": " << channelName     << "\n";
    std::cout << "  " << std::setw(16) << "Subscribers"   << ": " << subscriberCount << "\n";
    std::cout << "  " << std::setw(16) << "Total Uploads" << ": " << getTotalUploads()<< "\n";
    printCreatorDivider();

    if (uploadedVideoIds.empty()) {
        std::cout << "  No videos uploaded yet.\n";
    } else {
        std::cout << "  Uploaded Video IDs:\n";
        for (int i = 0; i < static_cast<int>(uploadedVideoIds.size()); ++i) {
            std::cout << "    [" << (i + 1) << "] " << uploadedVideoIds[i] << "\n";
        }
    }
    printCreatorDivider();
}
