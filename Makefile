.PHONY: all clean

all: $(FNAME)
	$(CXX) $(CXXFLAGS) $(FNAME)

clean:
	-rm -f $(OUTPUT)
