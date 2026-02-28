CXX = g++
CXXFLAGS = -std=c++11 -Wall -Wextra -pedantic

TARGET = lab18_variant26
SOURCES = main.cpp database.cpp
HEADERS = database.h record.h

all: $(TARGET)

$(TARGET): $(SOURCES) $(HEADERS)
	$(CXX) $(CXXFLAGS) $(SOURCES) -o $(TARGET)

clean:
	rm -f $(TARGET) *.dat
