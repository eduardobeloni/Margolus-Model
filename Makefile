
CXXFLAGS = `pkg-config gtkmm-2.4 --cflags`
LDLIBS = `pkg-config gtkmm-2.4 --libs`
ADDFLAGS = -Wall -pipe -O2

all: margolus_model

margolus_model: main.o automata_window.o growth_area.o margolus.o
	g++ main.o automata_window.o growth_area.o margolus.o -o margolus_model $(CXXFLAGS) $(LDLIBS) $(ADDFLAGS)

main.o: main.cpp
	g++ -c $(CXXFLAGS) main.cpp $(ADDFLAGS)

automata_window.o: automata_window.cpp
	g++ -c $(CXXFLAGS) automata_window.cpp $(ADDFLAGS)

growth_area.o: growth_area.cpp
	g++ -c $(CXXFLAGS) growth_area.cpp $(ADDFLAGS)

margolus.o: margolus.cpp
	g++ -c margolus.cpp $(ADDFLAGS)

clean:
	rm -rf *.o margolus_model
