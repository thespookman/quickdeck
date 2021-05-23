#ifndef CANVAS_H
#define CANVAS_H

#include "SPKlog.h"

#include <Magick++.h>
#include <string>

using namespace Magick;

class Canvas {
  private:
    Logger* l;
    Image   canvas;
    bool    init = false;
    bool    check_init (const char* action);
    Color   transparent;

  public:
    Canvas (Logger* _l);
    void create_canvas (int width, int height);
    void save_card (const char* filename);
    void rectangle (bool fill, int x, int y, int width, int height, int thickness,
                    std::string colour);
    void text (char* text, char* font, int x, int y, std::string colour);
    void image (char* filename, int x, int y, int width, int height);
    void line (int x1, int y1, int x2, int y2, int thickness, std::string colour);
    ~Canvas ();
};

#endif
