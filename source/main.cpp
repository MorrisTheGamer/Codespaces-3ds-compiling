#include <3ds.h>
#include <citro2d.h>
#include "graphics.h"
#include "network.h"
#include "settings.h"

int main() {
    gfxInitDefault();
    C3D_Init(C3D_DEFAULT_CMDBUF_SIZE);
    C2D_Init(C2D_DEFAULT_MAX_OBJECTS);
    C2D_Prepare();
    Network::init();

    C2D_TextBuf g_staticBuf = C2D_TextBufNew(4096);
    C2D_Text miiCordText;
    C2D_TextParse(&miiCordText, g_staticBuf, "MiiCord");
    C2D_TextOptimize(&miiCordText);

    Settings::initFileSystem();
    std::string myName = Settings::getUsername();
    
    // Korrigiert: Fehlende Klammer/Semikolon ergänzt
    C3D_RenderTarget* topScreen = C2D_CreateScreenTarget(GFX_TOP, GFX_LEFT);

    while (aptMainLoop()) {
        hidScanInput();
        if (hidKeysDown() & KEY_START) break;

        // Timer und Online-Check wurden entfernt

        if (hidKeysDown() & KEY_A) {
            char* msg = Graphics::showKeyboard("Messages:");
            if (msg) Network::sendMessage(myName, msg);
        }

        C3D_FrameBegin(C3D_FRAME_SYNCDRAW);
        C2D_TargetClear(topScreen, Graphics::CLR_BLUEBERRY);
        C2D_SceneBegin(topScreen);

        // Zeichnet das UI
        Graphics::drawUI(0, myName);

        // Zeichnet nur noch das MiiCord Logo oben rechts
        C2D_DrawText(&miiCordText, C2D_AtBaseline, 330.0f, 20.0f, 0.5f, 0.6f, 0.6f, C2D_Color32(255, 255, 255, 255));

        C3D_FrameEnd(0);
    }

    Network::exit();
    C2D_TextBufDelete(g_staticBuf); // Kleiner Bonus: Buffer aufräumen
    C2D_Fini();
    C3D_Fini();
    gfxExit();
    return 0;
}
