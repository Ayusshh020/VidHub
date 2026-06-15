// ============================================================
//  VidHub — main.cpp
//  Entry point for the simplified YouTube-like DSA platform.
//
//  Startup sequence:
//    1. DummyData()  — loads all entities + DSA modules
//    2. ConsoleUI()  — creates the UI with a reference
//    3. ui.run()     — shows session picker → role menu loop
// ============================================================

#include "data/DummyData.h"
#include "ui/ConsoleUI.h"

int main() {
    // Build and seed all DSA modules with realistic data
    DummyData data;

    // Hand the data to the UI and start the application
    ConsoleUI ui(data);
    ui.run();

    return 0;
}
