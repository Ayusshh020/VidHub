#pragma once

#include "User.h"
#include <string>
#include <vector>

// =============================================================
//  Creator — A User who can upload and manage videos.
//
//  Inherits all User fields (userId, name, email, region, role).
//  Adds a channel identity and a record of uploaded video IDs.
//
//  Note: Creator does NOT implement upload logic.
//  The upload workflow (license check, BST insert, heap insert,
//  segmentation) is coordinated by ConsoleUI using the
//  appropriate DSA modules. Creator is just the data holder.
// =============================================================

class Creator : public User {
public:
    // ── Channel Identity ─────────────────────────────────────
    std::string channelName;              // e.g. "Arjun Dev"
    long        subscriberCount;          // Total subscribers

    // ── Upload Record ────────────────────────────────────────
    // Stores IDs of videos this creator has uploaded.
    // Actual Video objects live in VideoCatalog (BST).
    std::vector<std::string> uploadedVideoIds;

    // ── Constructors ─────────────────────────────────────────
    Creator();

    Creator(std::string userId,
            std::string name,
            std::string email,
            std::string region,
            std::string channelName,
            long subscriberCount = 0);

    // ── Upload Record Management ─────────────────────────────
    void addUploadedVideo(const std::string& videoId);  // Record a new upload
    void removeUploadedVideo(const std::string& videoId); // Remove from record

    // ── Getters ──────────────────────────────────────────────
    std::string              getChannelName()     const;
    long                     getSubscriberCount() const;
    std::vector<std::string> getUploadedVideoIds()const;
    int                      getTotalUploads()    const; // uploadedVideoIds.size()

    // ── Display (overrides User) ──────────────────────────────
    void displaySummary() const;          // [ID] ChannelName | Uploads | Subscribers
    void displayFull()    const override; // All fields including upload list
};
