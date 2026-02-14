#ifndef GRAPHICS_H
#define GRAPHICS_H

#include <3ds.h>
#include <citro2d.h>
#include <string>

class Graphics {
public:
    static const u32 CLR_BLUEBERRY;
    static const u32 CLR_HEADER;

    static void drawUI(int online, std::string myName);
    static char* showKeyboard(const char* hint);
};

#endif
