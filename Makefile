CXX = g++
CXXFLAGS = -std=c++17 -O2 -Wall -Wextra -Isrc
SRCDIR = src
BUILDDIR = build
TARGET = $(BUILDDIR)/raytracer
SRCS = $(SRCDIR)/main.cpp
OBJS = $(SRCS:$(SRCDIR)/%.cpp=$(BUILDDIR)/%.o)

.PHONY: all clean run

all: $(TARGET)

$(TARGET): $(OBJS) | $(BUILDDIR)
	$(CXX) $(CXXFLAGS) -o $@ $^

$(BUILDDIR)/%.o: $(SRCDIR)/%.cpp | $(BUILDDIR)
	$(CXX) $(CXXFLAGS) -c -o $@ $<

$(BUILDDIR):
	mkdir -p $(BUILDDIR)

run: $(TARGET)
	$(TARGET)

clean:
	rm -rf $(BUILDDIR)
