# 🎬 VidHub — Data Structures & Algorithms Video Platform

Welcome to **VidHub**, a premium console-based video platform simulation that demonstrates the practical application of core Data Structures and Algorithms (DSA) in a modern system architecture. 

VidHub simulates user flows for viewers, content creators, and administrators—orchestrating everything from recommendation algorithms and license verification to network distribution paths and sharing networks.

---

## 🚀 Key Features & DSA Architecture

Each module in VidHub is built using a custom or optimized Data Structure to solve a real-world system design challenge:

| Feature / Module | Data Structure Used | Purpose |
| :--- | :--- | :--- |
| **Watch History** | **Stack (LIFO)** | Keeps track of recently watched videos, allowing viewers to see their history with the most recent first. |
| **Recommendation Queue** | **Queue (FIFO)** | Manages the "Up Next" play queue, populating recommendations based on the active video's genre. |
| **License Registry** | **Hash Map** | Provides $O(1)$ fast lookup and verification of video license keys (e.g., MIT, Creative Commons, Proprietary). |
| **Video Catalog** | **Binary Search Tree (BST)** | Stores and searches all platform videos sorted alphabetically (A-Z). Supports rapid lookup and insertion. |
| **Ranking System** | **Max Heap (Priority Queue)** | Ranks all platform videos dynamically based on popularity score (combination of views, likes, and watch time). |
| **Video Segment Manager** | **Deque (Double-Ended Queue)** | Simulates segment buffering and playback. Allows pre-appending for repairs, appending for streaming, and seek operations. |
| **Sharing Network** | **Directed Graph** | Model connections between users and platforms. Supports BFS (for hop levels and reach analysis) and DFS. |
| **CDN Delivery Optimizer** | **Weighted Graph + Dijkstra** | Computes the optimal (lowest latency) network path from media origins through PoPs and Edge servers to the user. |

---

## 🛠️ Getting Started

### Prerequisites
* A C++ compiler supporting **C++17** (e.g., `g++` or `clang++`).
* `make` build utility.

### Compilation
Build the main application and all unit test binaries:
```bash
make
```

### Running the Application
Launch the interactive console UI:
```bash
./vidhub
```

### Running Unit Tests
VidHub includes individual unit tests for every module to ensure correctness:
```bash
make test_all
```
This compiles and runs the individual tests:
* `./test_video`
* `./test_entities`
* `./test_wh` (Watch History)
* `./test_rq` (Recommendation Queue)
* `./test_lr` (License Registry)
* `./test_cat` (Video Catalog BST)
* `./test_rs` (Ranking System Max Heap)
* `./test_seg` (Segment Manager Deque)
* `./test_sn` (Sharing Network Graph)
* `./test_do` (Delivery Optimizer Dijkstra)

---

## 📂 Project Structure

```
VidHub/
├── data/
│   ├── DummyData.h / .cpp       # Generates sample users, creators, videos, and network nodes
│   └── ...
├── entities/
│   ├── Video.h / .cpp           # Core metadata structure (views, likes, score, duration)
│   ├── User.h / .cpp            # User properties (viewer credentials)
│   ├── Creator.h / .cpp         # Creator properties & uploaded catalog references
│   └── Admin.h / .cpp           # Administrative actions
├── modules/
│   ├── WatchHistory.h / .cpp    # Stack implementation
│   ├── RecommendationQueue.h    # Queue implementation
│   ├── LicenseRegistry.h        # Hash Map implementation
│   ├── VideoCatalog.h / .cpp    # BST implementation
│   ├── RankingSystem.h / .cpp   # Max Heap implementation
│   ├── VideoSegmentManager.h    # Deque implementation
│   ├── SharingNetwork.h / .cpp  # Directed Graph BFS/DFS
│   └── DeliveryOptimizer.h      # Weighted Graph & Dijkstra's Algorithm
├── samples/
│   ├── <module>_input.txt       # Example inputs used in automated testing drivers
│   ├── <module>_output.txt      # Direct captured console outputs of unit tests
│   └── README.md                # Summary index of module test cases
├── ui/
│   ├── ConsoleUI.h / .cpp       # Command Line Interface (CLI) loop and menus
│   └── ...
├── utils/
│   └── Helpers.h                # String transformation helpers
├── Makefile                     # Compiler directives and target configurations
└── README.md                    # Project documentation
```

---

## 👥 Interactive Sessions

When you run `./vidhub`, you can log in under three different roles:
1. **Viewer**: Browse Top Feed, search by keyword/genre (BST search), play/like videos (updating Max Heap dynamically), see watch history (Stack), and share videos with other users (Graph).
2. **Creator**: View upload dashboard, upload new videos (requires validation against Hash Map license registry), delete videos, and share videos.
3. **Admin**: View system analytics, inspect/revoke licenses, print/traverse the sharing network graph, and run the CDN latency path simulator.
