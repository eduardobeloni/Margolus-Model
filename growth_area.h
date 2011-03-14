
#ifndef GROWT_HAREA_H
#define GROWT_HAREA_H

#include "margolus.h"

#include <gtkmm/drawingarea.h>


class GrowthArea: public Gtk::DrawingArea
{
protected:
    // overridden signal handler
    virtual void on_realize();
    virtual bool on_expose_event(GdkEventExpose *e);

    Glib::RefPtr<Gdk::GC> black_gc, white_gc;
    Gdk::Color black_color, white_color;

private:
    static const short MAXCELL = 180;
    short sites[MAXCELL + 1][MAXCELL + 1];
    short down_right;
    Margolus margolus;
    bool src_enable;

public:
    enum ConfigType
    {
        SINGLE, SOURCE, RAND
    };

    GrowthArea(const Margolus::RuleType init_rule, const ConfigType init_config);
    virtual ~GrowthArea();

    void source();
    void next_state();
    void reset(const Margolus::RuleType new_rule, const ConfigType new_config);
};

#endif // GROWTH_AREA_H
