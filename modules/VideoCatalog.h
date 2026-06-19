#pragma once
// Note: Comments are only for understanding—do not modify the code itself.

#include "../entities/Video.h"
#include <string>
#include <vector>

class VideoCatalog {
private:
    struct BSTNode {
        Video    data;
        BSTNode* left;
        BSTNode* right;

        explicit BSTNode(const Video& v)
            : data(v), left(nullptr), right(nullptr) {}
    };

    BSTNode* root;
    int      nodeCount;

    BSTNode* insertHelper  (BSTNode* node, const Video& video);
    BSTNode* removeHelper  (BSTNode* node, const std::string& videoId);
    BSTNode* getMinNode    (BSTNode* node) const;
    BSTNode* searchByTitle (BSTNode* node,
                            const std::string& title) const;

    void inOrderHelper    (BSTNode* node,
                           std::vector<Video>& result) const;
    void keywordHelper    (BSTNode* node,
                           const std::string& keyword,
                           std::vector<Video>& result) const;
    void genreHelper      (BSTNode* node,
                           const std::string& genre,
                           std::vector<Video>& result) const;
    void clearHelper      (BSTNode* node);

public:
    VideoCatalog();
    ~VideoCatalog();

    void insert(const Video& video);
    bool remove(const std::string& videoId);

    const Video* searchExact(const std::string& title) const;
    std::vector<Video> searchByKeyword(const std::string& keyword) const;
    std::vector<Video> searchByGenre(const std::string& genre) const;

    std::vector<Video> getAllSorted() const;

    int  getCount()  const;
    bool isEmpty()   const;
    bool contains(const std::string& videoId) const;

    void displayAll()          const;
    void displaySearchResults  (const std::vector<Video>& results,
                                const std::string& query)  const;
};
