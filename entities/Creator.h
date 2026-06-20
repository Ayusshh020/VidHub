// entities/Creator.h
// Purpose: Header file defining the Creator class, inheriting from User, representing content uploaders.

#pragma once

#include "User.h"
#include <string>
#include <vector>

/**
 * @class Creator
 * @brief Represents a content creator on the platform.
 *
 * Inherits from User, adding features specific to creators, such as channel name,
 * subscriber count tracking, and a list of uploaded video ID references.
 */
class Creator : public User {
public:
    std::string channelName;                  // Brand name of the creator's channel
    long subscriberCount;                     // Number of subscribed viewer accounts

    std::vector<std::string> uploadedVideoIds; // Log of video IDs uploaded by this creator

    /**
     * @brief Constructs a default Creator object.
     */
    Creator();

    /**
     * @brief Constructs a Creator object initialized with profile and channel attributes.
     */
    Creator(std::string userId,
            std::string name,
            std::string email,
            std::string region,
            std::string channelName,
            long subscriberCount = 0);

    /**
     * @brief Registers a new video ID in the creator's uploaded library list.
     * @param videoId Unique ID of the uploaded video.
     */
    void addUploadedVideo(const std::string& videoId);

    /**
     * @brief Deletes a video ID reference from the creator's library.
     * @param videoId Unique ID of the video to remove.
     */
    void removeUploadedVideo(const std::string& videoId);

    // Getters for encapsulation of private channel metrics
    std::string getChannelName() const;
    long getSubscriberCount() const;
    std::vector<std::string> getUploadedVideoIds() const;
    int getTotalUploads() const;

    /**
     * @brief Displays creator channel statistics in list views.
     */
    void displaySummary() const;

    /**
     * @brief Overrides the base displayFull method to show detailed creator stats and video logs.
     */
    void displayFull() const override;
};
