.PHONY: all clean

all: $(FNAME)
	g++ $(CXXFLAGS) $(FNAME)

clean:
	-rm -f $(OUTPUT)
