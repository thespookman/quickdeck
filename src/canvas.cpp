#include "canvas.h"

#include <cstdlib>
#include <fstream>
#include <stdexcept>
#include <vector>

Canvas::Canvas (Logger* _l) : l {_l} {
    depends = "";
    init    = false;
    try {
        transparent = Color ("#00000000");
    } catch (Warning& w) {
        l->inf () << "Warning: " << w.what () << std::endl;
    } catch (std::exception& e) {
        l->err () << "Could not initialise transparent colour: " << e.what () << std::endl;
    }
}

bool Canvas::check_init (std::string action) {
    if (init) return true;
    l->err () << "Canvas has not been created, skipping action: " << action << std::endl;
    return false;
}

void Canvas::create_canvas (int width, int height) {
    l->inf () << "Creating canvas: " << width << "x" << height << std::endl;
    try {
        canvas = Image (Geometry (width, height), Color ("#FFFFFFFF"));
        init   = true;
    } catch (Warning& w) {
        l->inf () << "Warning: " << w.what () << std::endl;
    } catch (std::exception& e) {
        l->err () << "Could not create canvas: " << e.what () << std::endl;
    }
}

void Canvas::save_card (std::string filename) {
    if (!check_init ("SAVE")) return;
    l->inf () << "Saving to " << filename << std::endl;
    depends = std::string (filename) + ": " + depends;
    std::string depends_file (filename);
    depends_file += ".d";
    try {
        canvas.write (filename);
        std::ofstream ofs;
        ofs.open (depends_file);
        ofs << depends;
        ofs.close ();
    } catch (Warning& w) {
        l->inf () << "Warning: " << w.what () << std::endl;
    } catch (std::exception& e) { l->err () << "Could not save file: " << e.what () << std::endl; }
}

void Canvas::rectangle (bool fill, int x, int y, int width, int height, int thickness,
                        std::string colour) {
    if (!check_init ("RECT")) return;
    l->inf () << "Drawing box: " << width << "x" << height << " at (" << x << "," << y << ")"
              << std::endl;
    try {
        Color                 c (colour);
        std::vector<Drawable> draw_list;
        draw_list.push_back (DrawableRectangle (x, y, x + width, y + height));
        draw_list.push_back (DrawableStrokeColor (c));
        draw_list.push_back (DrawableStrokeWidth (thickness));
        if (!fill) {
            l->dbg () << "Setting no fill." << std::endl;
            draw_list.push_back (DrawableFillOpacity (0));
        } else
            draw_list.push_back (DrawableFillColor (c));
        canvas.draw (draw_list);
    } catch (Warning& w) {
        l->inf () << "Warning: " << w.what () << std::endl;
    } catch (std::exception& e) {
        l->err () << "Could not draw rectangle: " << e.what () << std::endl;
    }
}

void Canvas::text (std::string text, std::string font, double size, int x, int y,
                   std::string colour) {
    if (!check_init ("TEXT")) return;
    l->inf () << "Drawing text: " << text << std::endl;
    try {
        Color                 c (colour);
        std::vector<Drawable> draw_list;
        draw_list.push_back (DrawableFont (font));
        draw_list.push_back (DrawablePointSize (size));
        draw_list.push_back (DrawableText (x, y, text));
        draw_list.push_back (DrawableStrokeColor (c));
        canvas.draw (draw_list);
    } catch (Warning& w) {
        l->inf () << "Warning: " << w.what () << std::endl;
    } catch (std::exception& e) { l->err () << "Could not draw text: " << e.what () << std::endl; }
}

void Canvas::image (std::string filename, int x, int y, int width, int height) {
    if (!check_init ("IMAGE")) return;
    l->inf () << "Drawing image: " << filename << std::endl;
    depends += filename;
    depends += " ";
    try {
        canvas.draw (DrawableCompositeImage (x, y, width, height, filename));
    } catch (Warning& w) {
        l->inf () << "Warning: " << w.what () << std::endl;
    } catch (std::exception& e) { l->err () << "Could not draw image: " << e.what () << std::endl; }
}

void Canvas::line (int x1, int y1, int x2, int y2, int thickness, std::string colour) {
    if (!check_init ("LINE")) return;
    l->inf () << "Drawing line: (" << x1 << "," << y1 << ") -> (" << x2 << "," << y2 << std::endl;
    try {
        Color                 c (colour);
        std::vector<Drawable> draw_list;
        draw_list.push_back (DrawableLine (x1, y1, x2, y2));
        draw_list.push_back (DrawableStrokeWidth (thickness));
        draw_list.push_back (DrawableStrokeColor (c));
        canvas.draw (draw_list);
    } catch (Warning& w) {
        l->inf () << "Warning: " << w.what () << std::endl;
    } catch (std::exception& e) { l->err () << "Could not draw line: " << e.what () << std::endl; }
}

Canvas::~Canvas () {}
