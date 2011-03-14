
#ifndef AUTOMATA_WINDOW_H
#define AUTOMATA_WINDOW_H

#include "growth_area.h"

#include <gtkmm/window.h>
#include <gtkmm/entry.h>
#include <gtkmm/button.h>


class AutomataWindow: public Gtk::Window
{
protected:
    Gtk::Menu rule_menu, config_menu;
    Gtk::Button stop_button, run_button;
    Gtk::Button set_button;
    GrowthArea growth_area;
    sigc::connection timer;

    virtual void on_run_button_clicked();
    virtual void on_stop_button_clicked();
    virtual void on_set_button_clicked();
    virtual void on_rule_menu(Margolus::RuleType new_rule);
    virtual void on_config_menu(GrowthArea::ConfigType new_config);
    virtual bool on_timeout();

private:
    Margolus::RuleType sel_rule;
    GrowthArea::ConfigType sel_config;

public:
    AutomataWindow();
    virtual ~AutomataWindow();
};


#endif  // AUTOMATA_WINDOW_H

