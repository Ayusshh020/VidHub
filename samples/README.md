# VidHub Module Test Samples

This directory contains input definitions and captured execution outputs for each core Data Structure and Algorithm (DSA) module in VidHub. These files document the test drivers used to verify module correctness.

## 📂 Samples Index

### 1. Watch History (Stack)
* [watch_history_input.txt](file:///Users/ayush/DSA-exam/VidHub/samples/watch_history_input.txt) — Lists the video objects and step-by-step push/pop operations, including overflow cap testing.
* [watch_history_output.txt](file:///Users/ayush/DSA-exam/VidHub/samples/watch_history_output.txt) — Captured console output of the WatchHistory driver verifying LIFO ordering and eviction rules.

### 2. Recommendation Queue (Queue)
* [recommendation_queue_input.txt](file:///Users/ayush/DSA-exam/VidHub/samples/recommendation_queue_input.txt) — Lists the initial videos, pool of videos, and operations showing FIFO ordering and genre-based population filtering.
* [recommendation_queue_output.txt](file:///Users/ayush/DSA-exam/VidHub/samples/recommendation_queue_output.txt) — Captured console output of the RecommendationQueue driver verifying FIFO enqueues/dequeues and updates.

### 3. License Registry (Hash Map)
* [license_registry_input.txt](file:///Users/ayush/DSA-exam/VidHub/samples/license_registry_input.txt) — Defines the 5 sample licenses, duplicate test values, and registry operations.
* [license_registry_output.txt](file:///Users/ayush/DSA-exam/VidHub/samples/license_registry_output.txt) — Captured console output showing instant $O(1)$ lookups, revocations, and hard deletions.

### 4. Video Catalog (Binary Search Tree)
* [video_catalog_input.txt](file:///Users/ayush/DSA-exam/VidHub/samples/video_catalog_input.txt) — Outlines the initial unsorted list of videos inserted to form the BST, and title search queries.
* [video_catalog_output.txt](file:///Users/ayush/DSA-exam/VidHub/samples/video_catalog_output.txt) — Captured console output demonstrating in-order alphabetical traversal and tree re-balancing during node deletions.

### 5. Ranking System (Max Heap)
* [ranking_system_input.txt](file:///Users/ayush/DSA-exam/VidHub/samples/ranking_system_input.txt) — Describes the engagement statistics used to compute scores, and the "going viral" update simulation.
* [ranking_system_output.txt](file:///Users/ayush/DSA-exam/VidHub/samples/ranking_system_output.txt) — Captured console output showing top-ranked videos retrieval, priority queue maintenance, and score recalculations.

### 6. Video Segment Manager (Deque)
* [segment_manager_input.txt](file:///Users/ayush/DSA-exam/VidHub/samples/segment_manager_input.txt) — Details video durations, segments sizes, quality bitrates, and chunk buffers.
* [segment_manager_output.txt](file:///Users/ayush/DSA-exam/VidHub/samples/segment_manager_output.txt) — Captured console output showing chunk management, buffering progress bars, prepending, and appending.

### 7. Sharing Network (Directed Graph)
* [sharing_network_input.txt](file:///Users/ayush/DSA-exam/VidHub/samples/sharing_network_input.txt) — Specifies the users and direct sharing connections representing edges in the directed graph.
* [sharing_network_output.txt](file:///Users/ayush/DSA-exam/VidHub/samples/sharing_network_output.txt) — Captured console output illustrating BFS (hop levels) and DFS traversal routes.

### 8. CDN Delivery Optimizer (Weighted Graph + Dijkstra)
* [delivery_optimizer_input.txt](file:///Users/ayush/DSA-exam/VidHub/samples/delivery_optimizer_input.txt) — Defines the CDN topology (origins, PoPs, client node) and bidirectional link costs.
* [delivery_optimizer_output.txt](file:///Users/ayush/DSA-exam/VidHub/samples/delivery_optimizer_output.txt) — Captured console output confirming shortest-latency routing and dynamic rerouting after edge congestion.
