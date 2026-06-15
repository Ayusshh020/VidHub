CXX      := g++
CXXFLAGS := -std=c++17 -Wall -Wextra

# ── Source files ──────────────────────────────────────────────
SRCS := main.cpp \
        data/DummyData.cpp \
        entities/Video.cpp \
        entities/User.cpp \
        entities/Creator.cpp \
        entities/Admin.cpp \
        modules/WatchHistory.cpp \
        modules/RecommendationQueue.cpp \
        modules/LicenseRegistry.cpp \
        modules/VideoCatalog.cpp \
        modules/RankingSystem.cpp \
        modules/VideoSegmentManager.cpp \
        modules/SharingNetwork.cpp \
        modules/DeliveryOptimizer.cpp \
        ui/ConsoleUI.cpp

TARGET := vidhub

# ── Build targets ─────────────────────────────────────────────
all: $(TARGET)

$(TARGET): $(SRCS)
	$(CXX) $(CXXFLAGS) -o $(TARGET) $(SRCS)
	@echo "\n  Build successful → ./$(TARGET)\n"

run: $(TARGET)
	./$(TARGET)

# ── Individual module tests ───────────────────────────────────
test_video:
	$(CXX) $(CXXFLAGS) -o test_video test_video.cpp entities/Video.cpp

test_entities:
	$(CXX) $(CXXFLAGS) -o test_entities test_entities.cpp \
	       entities/Video.cpp entities/User.cpp \
	       entities/Creator.cpp entities/Admin.cpp

test_wh:
	$(CXX) $(CXXFLAGS) -o test_wh test_watchhistory.cpp \
	       entities/Video.cpp modules/WatchHistory.cpp

test_rq:
	$(CXX) $(CXXFLAGS) -o test_rq test_recqueue.cpp \
	       entities/Video.cpp modules/RecommendationQueue.cpp

test_lr:
	$(CXX) $(CXXFLAGS) -o test_lr test_license.cpp \
	       modules/LicenseRegistry.cpp

test_cat:
	$(CXX) $(CXXFLAGS) -o test_cat test_catalog.cpp \
	       entities/Video.cpp modules/VideoCatalog.cpp

test_rs:
	$(CXX) $(CXXFLAGS) -o test_rs test_ranking.cpp \
	       entities/Video.cpp modules/RankingSystem.cpp

test_seg:
	$(CXX) $(CXXFLAGS) -o test_seg test_segments.cpp \
	       modules/VideoSegmentManager.cpp

test_sn:
	$(CXX) $(CXXFLAGS) -o test_sn test_sharing.cpp \
	       modules/SharingNetwork.cpp

test_do:
	$(CXX) $(CXXFLAGS) -o test_do test_delivery.cpp \
	       modules/DeliveryOptimizer.cpp

# Run all individual tests in sequence
test_all: test_video test_entities test_wh test_rq test_lr \
          test_cat test_rs test_seg test_sn test_do
	@echo "\n  Running all module tests...\n"
	./test_video
	./test_entities
	./test_wh
	./test_rq
	./test_lr
	./test_cat
	./test_rs
	./test_seg
	./test_sn
	./test_do
	@echo "\n  All tests complete.\n"

# ── Cleanup ───────────────────────────────────────────────────
clean:
	rm -f $(TARGET) test_video test_entities test_wh test_rq \
	      test_lr test_cat test_rs test_seg test_sn test_do

.PHONY: all run test_all clean
