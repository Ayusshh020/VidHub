// test_segments.cpp — Step 10 verification driver
// Compile: g++ -std=c++17 -o test_seg test_segments.cpp
//              modules/VideoSegmentManager.cpp
// Run:     ./test_seg

#include "modules/VideoSegmentManager.h"
#include <iostream>
#include <cassert>

int main() {
    std::cout << "\n===================================================\n";
    std::cout << "  VidHub — Step 10: VideoSegmentManager (Deque)\n";
    std::cout << "===================================================\n\n";

    // ── Test 1: Default construction ─────────────────────────
    VideoSegmentManager vsm("VID-002", 2700);  // 45-min video
    assert(vsm.isEmpty()          == true);
    assert(vsm.getTotalSegments() == 0);
    std::cout << "[PASS] Default construction — empty deque\n";

    // ── Test 2: segmentVideo() ────────────────────────────────
    vsm.segmentVideo(10, "720p");   // 10-second chunks
    // 2700s / 10s = 270 segments exactly
    assert(vsm.getTotalSegments()    == 270);
    assert(vsm.getTotalBufferedSec() == 2700);
    assert(vsm.isEmpty()             == false);
    std::cout << "[PASS] segmentVideo(10s) — 270 segments, 2700s total\n";

    // ── Test 3: First and last segment correctness ────────────
    const Segment& first = vsm.getSegment(0);
    assert(first.index    == 1);
    assert(first.startSec == 0);
    assert(first.endSec   == 10);
    assert(first.quality  == "720p");
    std::cout << "[PASS] First segment: SEG-001 | 00:00 → 00:10 | 720p\n";

    const Segment& last = vsm.getSegment(269);
    assert(last.index    == 270);
    assert(last.startSec == 2690);
    assert(last.endSec   == 2700);
    std::cout << "[PASS] Last segment:  SEG-270 | 44:50 → 45:00 | 720p\n";

    // ── Test 4: Uneven division (remainder segment) ───────────
    VideoSegmentManager vsm2("VID-004", 1210);  // 1210s not divisible by 10
    vsm2.segmentVideo(10, "1080p");
    // 1210 / 10 = 121 full segments (1210 / 10 = 121 exactly → 121 segments)
    // Actually 1210 / 10 = 121 with no remainder — let's use 1215s
    VideoSegmentManager vsm3("VID-005", 1215);
    vsm3.segmentVideo(10, "480p");
    // 1215 / 10 = 121 full + 1 partial (5s) = 122 segments
    assert(vsm3.getTotalSegments()    == 122);
    assert(vsm3.getTotalBufferedSec() == 1215);
    const Segment& partialLast = vsm3.getSegment(121);
    assert(partialLast.getDuration() == 5);   // Last segment is 5s
    std::cout << "[PASS] Uneven division — last segment is 5s (1215s ÷ 10s)\n";

    // ── Test 5: O(1) indexed access (seek simulation) ─────────
    // Seek to minute 20 = second 1200 = segment index 119 (0-based)
    const Segment& seekSeg = vsm.getSegment(119);
    assert(seekSeg.startSec == 1190);
    assert(seekSeg.endSec   == 1200);
    std::cout << "[PASS] getSegment(119) — seek to 19:50→20:00 ✓\n";

    // ── Test 6: peekFront() — non-destructive ─────────────────
    const Segment& front1 = vsm.peekFront();
    assert(front1.index == 1);
    assert(vsm.getTotalSegments() == 270);   // NOT consumed
    std::cout << "[PASS] peekFront() — SEG-001 visible, deque unchanged\n";

    // ── Test 7: consumeFront() — O(1) playback simulation ─────
    Segment played1 = vsm.consumeFront();
    assert(played1.index    == 1);
    assert(played1.startSec == 0);
    assert(vsm.getTotalSegments() == 269);   // One removed

    Segment played2 = vsm.consumeFront();
    assert(played2.index == 2);
    assert(vsm.getTotalSegments() == 268);
    std::cout << "[PASS] consumeFront() — played SEG-001, SEG-002, 268 remain\n";

    // ── Test 8: appendSegment() — O(1) back append ────────────
    Segment liveSeg(271, 2700, 2710, "720p", 3052);
    vsm.appendSegment(liveSeg);
    assert(vsm.getTotalSegments() == 269);   // 268 + 1 appended
    const Segment& back = vsm.getSegment(vsm.getTotalSegments() - 1);
    assert(back.index == 271);
    std::cout << "[PASS] appendSegment() — live SEG-271 added to back\n";

    // ── Test 9: prependSegment() — O(1) front prepend ─────────
    Segment repairSeg(0, 0, 5, "480p", 610);   // Priority repair chunk
    vsm.prependSegment(repairSeg);
    assert(vsm.getTotalSegments() == 270);
    assert(vsm.peekFront().index  == 0);        // Repair seg at front
    std::cout << "[PASS] prependSegment() — repair SEG-0 added to front\n";

    // ── Test 10: displaySegments() (large video — truncated) ──
    std::cout << "\n--- displaySegments() [270 segs — shows first5 + last3] ---";
    vsm.displaySegments();

    // ── Test 11: displaySegmentRange() ───────────────────────
    std::cout << "--- displaySegmentRange(1, 5) ---";
    vsm.displaySegmentRange(1, 5);

    // ── Test 12: displayPlaybackStatus() ─────────────────────
    std::cout << "--- Playback status at segment 90/270 ---";
    vsm.displayPlaybackStatus(90);
    std::cout << "--- Playback status at segment 270/270 ---";
    vsm.displayPlaybackStatus(270);

    // ── Test 13: segmentVideo() clears previous segments ──────
    vsm.segmentVideo(30, "1080p");   // Re-segment with 30s chunks
    assert(vsm.getTotalSegments() == 90);     // 2700 / 30 = 90
    assert(vsm.getSegment(0).quality == "1080p");
    std::cout << "[PASS] Re-segmentation clears old segments (90 @ 1080p)\n";

    // ── Test 14: clear() ──────────────────────────────────────
    vsm.clear();
    assert(vsm.isEmpty()          == true);
    assert(vsm.getTotalSegments() == 0);
    std::cout << "[PASS] clear() empties the deque\n";

    std::cout << "\n===================================================\n";
    std::cout << "  All VideoSegmentManager tests passed!\n";
    std::cout << "===================================================\n\n";

    return 0;
}
