CC=cc
CXX=c++
DEFINES=
CFLAGS=-c -O2 -std=c++11 -MMD
LDFLAGS=-s -std=c++11 -lnetsnmp
SOURCES=main.cxx
OBJECTS=$(SOURCES:.cxx=.o)
DEPENDENCIES=$(SOURCES:.cxx=.d)
EXECUTABLE=snmpwalktranslate

.PHONY: all clean

all: $(EXECUTABLE)

-include $(DEPENDENCIES)

$(EXECUTABLE): $(OBJECTS)
	$(CXX) $(LDFLAGS) $(OBJECTS) -o $@

clean :
	rm $(EXECUTABLE) $(OBJECTS) $(DEPENDENCIES)

%.o: %.cxx
	$(CXX) $(CFLAGS) $(DEFINES) $< -o $@
