
#include "automata_window.h"

#include <gtkmm/box.h>
#include <gtkmm/buttonbox.h>
#include <gtkmm/menu.h>
#include <gtkmm/optionmenu.h>

#include <iostream>


AutomataWindow::AutomataWindow():
    stop_button("stop"),
    run_button("run"),
    set_button("set"),
    growth_area(Margolus::SAND, GrowthArea::SOURCE),
    sel_rule(Margolus::SAND),
    sel_config(GrowthArea::SOURCE)
{
    // setting properties of AutomataWindow
    this->set_border_width(10);
    this->set_resizable(false);
    this->set_title("Margolus model");
    this->set_icon_from_file("./margolus_neigh.gif");

    // gtk, please, take care of the following blocks of memory
    Gtk::VBox *main_vbox = Gtk::manage(new Gtk::VBox(false, 10));
    Gtk::VBox *vbox2 = Gtk::manage(new Gtk::VBox(false, 10));
    Gtk::HButtonBox *hbbox3 = Gtk::manage(new Gtk::HButtonBox);     // button box
    Gtk::HBox *hbox4 = Gtk::manage(new Gtk::HBox(false, 20));       // menu box

    this->add(*main_vbox);

    main_vbox->pack_start(growth_area);
    main_vbox->pack_start(*vbox2);

    vbox2->pack_start(*hbox4);
    vbox2->pack_start(*hbbox3);

    // inserting items on rule_menu
    {
        using Gtk::Menu_Helpers::MenuElem;

        rule_menu.items().push_back(MenuElem("billard ball",
                                             sigc::bind(sigc::mem_fun(*this, &AutomataWindow::on_rule_menu),
                                                        Margolus::BBM)));
        rule_menu.items().push_back(MenuElem("bounce gas",
                                             sigc::bind(sigc::mem_fun(*this, &AutomataWindow::on_rule_menu),
                                                        Margolus::BOUNCE_GAS)));
        rule_menu.items().push_back(MenuElem("critters",
                                             sigc::bind(sigc::mem_fun(*this, &AutomataWindow::on_rule_menu),
                                                        Margolus::CRITTERS)));
        rule_menu.items().push_back(MenuElem("HPP gas",
                                             sigc::bind(sigc::mem_fun(*this, &AutomataWindow::on_rule_menu),
                                                        Margolus::HPP_GAS)));
        rule_menu.items().push_back(MenuElem("rotations",
                                             sigc::bind(sigc::mem_fun(*this, &AutomataWindow::on_rule_menu),
                                                        Margolus::ROTATIONS)));
        rule_menu.items().push_back(MenuElem("sand pile",
                                             sigc::bind(sigc::mem_fun(*this, &AutomataWindow::on_rule_menu),
                                                        Margolus::SAND)));
        rule_menu.items().push_back(MenuElem("string thing",
                                             sigc::bind(sigc::mem_fun(*this, &AutomataWindow::on_rule_menu),
                                                        Margolus::STRING_THING)));
        rule_menu.items().push_back(MenuElem("swap on diag",
                                             sigc::bind(sigc::mem_fun(*this, &AutomataWindow::on_rule_menu),
                                                        Margolus::SWAP_ON_DIAG)));
        rule_menu.items().push_back(MenuElem("tron",
                                             sigc::bind(sigc::mem_fun(*this, &AutomataWindow::on_rule_menu),
                                                        Margolus::TRON)));
    }
    rule_menu.set_active(5);    // actives sand pile
    Gtk::OptionMenu *optmenu = Gtk::manage(new Gtk::OptionMenu);
    optmenu->set_menu(rule_menu);
    hbox4->pack_start(*optmenu);

    // inserting items on config_menu
    {
        using Gtk::Menu_Helpers::MenuElem;

        config_menu.items().push_back(MenuElem("single cell",
                                               sigc::bind(sigc::mem_fun(*this, &AutomataWindow::on_config_menu),
                                                          GrowthArea::SINGLE)));
        config_menu.items().push_back(MenuElem("source",
                                               sigc::bind(sigc::mem_fun(*this, &AutomataWindow::on_config_menu),
                                                          GrowthArea::SOURCE)));
        config_menu.items().push_back(MenuElem("random",
                                               sigc::bind(sigc::mem_fun(*this, &AutomataWindow::on_config_menu),
                                                          GrowthArea::RAND)));
    }
    config_menu.set_active(1);      // actives source
    Gtk::OptionMenu *config_optmenu = Gtk::manage(new Gtk::OptionMenu);
    config_optmenu->set_menu(config_menu);
    hbox4->pack_start(*config_optmenu);

    hbbox3->pack_start(run_button);
    hbbox3->pack_start(set_button);
    hbbox3->pack_start(stop_button);
    hbbox3->set_border_width(5);

    run_button.signal_clicked().connect(sigc::mem_fun(*this, &AutomataWindow::on_run_button_clicked));
    run_button.grab_focus();
    run_button.set_sensitive(true);

    stop_button.signal_clicked().connect(sigc::mem_fun(*this, &AutomataWindow::on_stop_button_clicked));
    stop_button.set_sensitive(false);

    set_button.signal_clicked().connect(sigc::mem_fun(*this, &AutomataWindow::on_set_button_clicked));
    set_button.set_sensitive(true);

    this->show_all_children();
}


AutomataWindow::~AutomataWindow() { }


void AutomataWindow::on_run_button_clicked()
{
    // run button can't be pressed until timeout finishes
    run_button.set_sensitive(false);

    stop_button.set_sensitive(true);
    stop_button.grab_focus();
    set_button.set_sensitive(false);

    // slot I wish to have called when timeout occurs
    sigc::slot<bool> my_slot = sigc::mem_fun(*this, &AutomataWindow::on_timeout);

    // connecting my_slot to Glib::signal_timeout(): slot, delay(ms)
    this->timer = Glib::signal_timeout().connect(my_slot, 50);
}


void AutomataWindow::on_stop_button_clicked()
{
    run_button.set_sensitive(true);
    stop_button.set_sensitive(false);
    set_button.set_sensitive(true);
    run_button.grab_focus();

    this->timer.disconnect();   // stops timeout
}


void AutomataWindow::on_rule_menu(Margolus::RuleType new_rule)
{
    sel_rule = new_rule;
}


void AutomataWindow::on_config_menu(GrowthArea::ConfigType new_config)
{
    sel_config = new_config;
}


void AutomataWindow::on_set_button_clicked()
{
    growth_area.reset(sel_rule, sel_config);
    growth_area.queue_draw();
}


bool AutomataWindow::on_timeout()
{
    growth_area.next_state();
    growth_area.queue_draw();

    return true;    // keeps on timeout until stop button
}
