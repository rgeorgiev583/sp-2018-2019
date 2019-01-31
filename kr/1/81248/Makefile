BINARIES := $(patsubst %.c, %.out, $(wildcard *.c))

.PHONY: all clean

all: $(BINARIES)

clean:
	$(RM) $(BINARIES)

%.out: %.c
	$(CC) -g $< -o $@
