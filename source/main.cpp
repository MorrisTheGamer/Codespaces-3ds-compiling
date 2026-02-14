#include <3ds.h>
#include <citro2d.h>
#include <string>
#include <fstream>

const std::string savePath = "sdmc:/clicker_save.txt";

int main() {
    gfxInitDefault();
    C3D_Init(C3D_DEFAULT_CMDBUF_SIZE);
    C2D_Init(C2D_DEFAULT_MAX_OBJECTS);
    C2D_Prepare();
    romfsInit(); // Wichtig: Aktiviert den Zugriff auf mitgepackte Dateien

    C3D_RenderTarget* top = C2D_CreateScreenTarget(GFX_TOP, GFX_LEFT);
    C3D_RenderTarget* bottom = C2D_CreateScreenTarget(GFX_BOTTOM, GFX_LEFT);

    // Bild laden (muss als button.t3x im Ordner romfs/ liegen)
    C2D_SpriteSheet spriteSheet = C2D_SpriteSheetLoad("romfs:/button.t3x");
    
    long long clickCount = 0;
    std::ifstream saveFileIn(savePath);
    if (saveFileIn.is_open()) {
        saveFileIn >> clickCount;
        saveFileIn.close();
    }

    C2D_TextBuf dynamicBuf = C2D_TextBufNew(4096);
    
    while (aptMainLoop()) {
        hidScanInput();
        u32 kDown = hidKeysDown();
        if (kDown & KEY_START) break;

        if (kDown & KEY_TOUCH) clickCount++;

        if (kDown & KEY_X) {
            std::ofstream saveFileOut(savePath);
            if (saveFileOut.is_open()) {
                saveFileOut << clickCount;
                saveFileOut.close();
            }
        }

        C2D_TextBufClear(dynamicBuf);
        C2D_Text titleText, countText;
        C2D_TextParse(&titleText, dynamicBuf, "Virtueller Clicker");
        C2D_TextParse(&countText, dynamicBuf, std::to_string(clickCount).c_str());
        C2D_TextOptimize(&titleText);
        C2D_TextOptimize(&countText);

        C3D_FrameBegin(C3D_FRAME_SYNCDRAW);
        
        // Oberer Bildschirm
        C2D_TargetClear(top, C2D_Color32(30, 30, 30, 255));
        C2D_SceneBegin(top);
        C2D_DrawText(&titleText, 0, 100.0f, 40.0f, 0.5f, 1.0f, 1.0f, C2D_Color32(255, 255, 255, 255));
        C2D_DrawText(&countText, 0, 150.0f, 100.0f, 0.5f, 1.5f, 1.5f, C2D_Color32(0, 255, 0, 255));

        // Unterer Bildschirm
        C2D_TargetClear(bottom, C2D_Color32(50, 50, 50, 255));
        C2D_SceneBegin(bottom);
        // Zeichnet das erste Bild aus dem SpriteSheet bei Position 40, 40
        if (spriteSheet) {
            C2D_DrawImageAt(C2D_SpriteSheetGetImage(spriteSheet, 0), 40.0f, 40.0f, 0.5f, NULL, 1.0f, 1.0f);
        }
        
        C3D_FrameEnd(0);
    }

    C2D_SpriteSheetFree(spriteSheet);
    C2D_TextBufDelete(dynamicBuf);
    romfsExit();
    C2D_Fini();
    C3D_Fini();
    gfxExit();
    return 0;
}
