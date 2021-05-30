#ifndef CANVAS_H
#define CANVAS_H

#include "SPKlog.h"

#include <Magick++.h>
#include <string>

using namespace Magick;

class Canvas {
  private:
    Logger*     l;
    Image       canvas;
    bool        init = false;
    bool        check_init (std::string action);
    Color       transparent;
    std::string depends;

  public:
    Canvas (Logger* _l);
    void create_canvas (int width, int height);
    void save_card (std::string filename);
    void rectangle (bool fill, int x, int y, int width, int height, int thickness,
                    std::string colour);
    void text (std::string text, std::string font, int x, int y, std::string colour);
    void image (std::string filename, int x, int y, int width, int height);
    void line (int x1, int y1, int x2, int y2, int thickness, std::string colour);
    ~Canvas ();
};

#endif
