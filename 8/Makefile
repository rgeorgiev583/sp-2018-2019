BINARIES := $(patsubst %.c, %, $(wildcard *.c))

.PHONY: all clean

all: $(BINARIES)

clean:
	$(RM) $(BINARIES)

%: %.c
	$(CC) -g $< -o $@
