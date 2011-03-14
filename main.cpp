
#include "automata_window.h"

#include <gtkmm/main.h>

int main(int argc, char **argv)
{
    Gtk::Main automata(argc, argv);
    AutomataWindow margolus_model;

    automata.run(margolus_model);

    return 0;
}
