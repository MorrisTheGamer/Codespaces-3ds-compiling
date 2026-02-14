#include "graphics.h"
#include <cstdio>

const u32 Graphics::CLR_BLUEBERRY = C2D_Color32(20, 35, 65, 255);
const u32 Graphics::CLR_HEADER    = C2D_Color32(10, 20, 40, 255);

void Graphics::drawUI(int online, std::string myName) {
    // Hintergrund
    C2D_DrawRectSolid(0, 0, 0, 400, 240, CLR_BLUEBERRY);

    // Header
    C2D_DrawRectSolid(0, 0, 0, 400, 30, CLR_HEADER);

    // Status Text vorbereiten
    char statusBuf[64];
    sprintf(statusBuf, "User: %s | Online: %d", myName.c_str(), online);

    // (Text Rendering kommt später mit C2D_TextBuf)
}

char* Graphics::showKeyboard(const char* hint) {
    static SwkbdState swkbd;
    static char buf[256];

    swkbdInit(&swkbd, SWKBD_TYPE_NORMAL, 2, -1);
    swkbdSetHintText(&swkbd, hint);
    swkbdInputText(&swkbd, buf, sizeof(buf));

    return buf;
}
