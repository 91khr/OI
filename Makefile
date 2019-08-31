.PHONY: all

CXXFLAGS+= -g -Wall -Wextra -std=c++17
ifdef DEBUG
	CXXFLAGS+= -DDEBUG -g
endif

all: $(FNAME)
	$(CXX) $(CXXFLAGS) $(FNAME)

