// main.cpp
// Purpose: Application entry point initializing mock databases and launching the interactive console menu.

#include "data/DummyData.h"
#include "ui/ConsoleUI.h"

int main() {
    DummyData data;     // Populates sample system entities (Users, Videos, CDN, Share nodes)
    ConsoleUI ui(data); // Instantiates console user interface driver
    ui.run();           // Starts select session interface login loop
    return 0;
}
