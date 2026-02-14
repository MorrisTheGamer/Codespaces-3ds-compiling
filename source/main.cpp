#include <3ds.h>
#include <citro2d.h>
#include <string>
#include "graphics.h"
#include "network.h"
#include "settings.h"

int main() {
    // Initialisierung der Hardware und Bibliotheken
    gfxInitDefault();
    C3D_Init(C3D_DEFAULT_CMDBUF_SIZE);
    C2D_Init(C2D_DEFAULT_MAX_OBJECTS);
    C2D_Prepare();
    
    // Netzwerk-Modul starten
    Network::init();

    // Text-Buffer und statische Texte vorbereiten
    C2D_TextBuf g_staticBuf = C2D_TextBufNew(4096);
    C2D_Text miiCordText;
    C2D_TextParse(&miiCordText, g_staticBuf, "MiiCord");
    C2D_TextOptimize(&miiCordText);

    // Dateisystem und Einstellungen laden
    Settings::initFileSystem();
    std::string myName = Settings::getUsername();
    
    // Ziel für den oberen Bildschirm erstellen (Fix: Syntax korrigiert)
    C3D_RenderTarget* topScreen = C2D_CreateScreenTarget(GFX_TOP, GFX_LEFT);

    // Hauptschleife
    while (aptMainLoop()) {
        // Eingaben scannen
        hidScanInput();
        u32 kDown = hidKeysDown();

        // Programm beenden mit der START-Taste
        if (kDown & KEY_START) break;

        // Tastatur öffnen bei Druck auf A
        if (kDown & KEY_A) {
            char* msg = Graphics::showKeyboard("Messages:");
            if (msg) {
                Network::sendMessage(myName, msg);
            }
        }

        // --- Rendering Start ---
        C3D_FrameBegin(C3D_FRAME_SYNCDRAW);
        C2D_TargetClear(topScreen, Graphics::CLR_BLUEBERRY);
        C2D_SceneBegin(topScreen);

        // UI zeichnen (Benutzername anzeigen)
        Graphics::drawUI(0, myName);

        // Das MiiCord Logo oben rechts zeichnen
        // Die Online-Anzeige (Punkt und Status-Text) wurde hier entfernt
        C2D_DrawText(&miiCordText, C2D_AtBaseline, 330.0f, 20.0f, 0.5f, 0.6f, 0.6f, C2D_Color32(255, 255, 255, 255));

        C3D_FrameEnd(0);
        // --- Rendering Ende ---
    }

    // Sauberes Beenden und Speicher freigeben
    Network::exit();
    C2D_TextBufDelete(g_staticBuf);
    C2D_Fini();
    C3D_Fini();
    gfxExit();
    
    return 0;
}
