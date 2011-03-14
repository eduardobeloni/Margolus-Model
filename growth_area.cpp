
#include "growth_area.h"

#include <cstdlib>
#include <vector>


GrowthArea::GrowthArea(const Margolus::RuleType init_rule, const ConfigType init_config):
    black_color(Gdk::Color("black")),
    white_color(Gdk::Color("white")),
    down_right(0),
    margolus(init_rule),
    src_enable(true)
{
    // initializing randomization
    time_t t;
    time(&t);
    srandom(t);

    this->set_size_request(MAXCELL * 2, MAXCELL * 2);
    this->reset(init_rule, init_config);
}


GrowthArea::~GrowthArea() { }


void GrowthArea::on_realize()
{
    // we need to call the base on_realize()
    Gtk::DrawingArea::on_realize();

    // a colormap to allocate resources
    Glib::RefPtr<Gdk::Colormap> curr_colormap = this->get_default_colormap();
    curr_colormap->alloc_color(this->black_color);
    curr_colormap->alloc_color(this->white_color);

    // getting window to create graphical contexts
    Glib::RefPtr<Gdk::Window> curr_win = this->get_window();

    // black gc
    this->black_gc = Gdk::GC::create(curr_win);
    this->black_gc->set_foreground(this->black_color);

    // white gc
    this->white_gc = Gdk::GC::create(curr_win);
    this->white_gc->set_foreground(this->white_color);
}


bool GrowthArea::on_expose_event(GdkEventExpose *e)
{
    // getting the current window to draw
    Glib::RefPtr<Gdk::Window> curr_win = this->get_window();

    // a black square
    std::vector<Gdk::Point> sqr;
    // storing black square's points
    sqr.push_back(Gdk::Point(2, 2));
    sqr.push_back(Gdk::Point(2, MAXCELL * 2 - 1));  // / colocar um +2 apos MAXCELL*2
    sqr.push_back(Gdk::Point(MAXCELL * 2 - 1, MAXCELL * 2 - 1));
    sqr.push_back(Gdk::Point(MAXCELL * 2 - 1, 2));
    // drawing the black square
    curr_win->draw_polygon(black_gc, true, sqr);

    // drawing the points
    for (int y = 1; y <= MAXCELL; y++)
        for (int x = 1; x <= MAXCELL; x++)
            if (sites[y][x])
            {
                curr_win->draw_point(white_gc, 2 * x, 2 * y);
                curr_win->draw_point(white_gc, 2 * x, 2 * y + 1);
                curr_win->draw_point(white_gc, 2 * x + 1, 2 * y);
                curr_win->draw_point(white_gc, 2 * x + 1, 2 * y + 1);
            }

    return true;    // everything is ok
}



void GrowthArea::source()
{
// sources for the middle north of drawing area
    for (int w = MAXCELL / 2 - 3; w <= MAXCELL / 2 + 2; w++)
        sites[0][w] = rand() % 10 == 1 ? 1 : 0;
}


void GrowthArea::next_state()
{
    short *neigh[2][2];

    for (int w = down_right; w < MAXCELL + down_right; w += 2)
        for (int i = down_right; i < MAXCELL + down_right; i += 2)
        {
            neigh[0][0] = &sites[w][i];
            neigh[0][1] = &sites[w][i + 1];
            neigh[1][0] = &sites[w + 1][i];
            neigh[1][1] = &sites[w + 1][i + 1];

            margolus.update_block(neigh);
        }

    if (src_enable)
        this->source();

    if (down_right)
        down_right = 0;
    else
        down_right = 1;
}


void GrowthArea::reset(const Margolus::RuleType new_rule, const ConfigType new_config)
{
    // setting new_rule
    margolus.set_curr_rule(new_rule);

    for (int w = 0; w <= MAXCELL; w++)
        for (int i = 0; i <= MAXCELL; i++)
            sites[w][i] = 0;

    if (new_config == SOURCE)
        src_enable = true;
    else
    {
        src_enable = false;
        if (new_config == SINGLE)
            sites[MAXCELL / 2][MAXCELL / 2] = 1;
        else
            for (int w = 0; w <= MAXCELL; w++)
                for (int i = 0; i <= MAXCELL; i++)
                    if (rand() % 100 < 10)
                        sites[w][i] = 1;
    }
}
