#include "Creator.h"

#include <iostream>
#include <iomanip>
#include <algorithm>
#include <string>

static void printCreatorDivider() {
    std::cout << "  " << std::string(50, '-') << "\n";
}

Creator::Creator()
    : User(),
      channelName(""),
      subscriberCount(0)
{
    role = "creator";
}

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

void Creator::addUploadedVideo(const std::string& videoId) {
    for (const auto& id : uploadedVideoIds) {
        if (id == videoId) return;
    }
    uploadedVideoIds.push_back(videoId);
}

void Creator::removeUploadedVideo(const std::string& videoId) {
    uploadedVideoIds.erase(
        std::remove(uploadedVideoIds.begin(), uploadedVideoIds.end(), videoId),
        uploadedVideoIds.end()
    );
}

std::string              Creator::getChannelName()     const { return channelName;      }
long                     Creator::getSubscriberCount() const { return subscriberCount;  }
std::vector<std::string> Creator::getUploadedVideoIds()const { return uploadedVideoIds; }
int                      Creator::getTotalUploads()    const {
    return static_cast<int>(uploadedVideoIds.size());
}

void Creator::displaySummary() const {
    std::cout << "  ["
              << std::left << std::setw(7)  << userId      << "] "
              << std::left << std::setw(22) << channelName
              << " | Uploads: "      << std::setw(3)  << getTotalUploads()
              << " | Subscribers: " << subscriberCount
              << "\n";
}

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
