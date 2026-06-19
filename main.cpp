// Note: Comments are only for understanding—do not modify the code itself.
#include "data/DummyData.h"
#include "ui/ConsoleUI.h"

int main() {
    DummyData data;
    ConsoleUI ui(data);
    ui.run();
    return 0;
}
