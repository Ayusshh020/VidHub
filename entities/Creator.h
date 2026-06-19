#pragma once
// Note: Comments are only for understanding—do not modify the code itself.

#include "User.h"
#include <string>
#include <vector>

class Creator : public User {
public:
    std::string channelName;
    long subscriberCount;

    std::vector<std::string> uploadedVideoIds;

    Creator();

    Creator(std::string userId,
            std::string name,
            std::string email,
            std::string region,
            std::string channelName,
            long subscriberCount = 0);

    void addUploadedVideo(const std::string& videoId);
    void removeUploadedVideo(const std::string& videoId);

    std::string getChannelName() const;
    long getSubscriberCount() const;
    std::vector<std::string> getUploadedVideoIds() const;
    int getTotalUploads() const;

    void displaySummary() const;
    void displayFull() const override;
};
