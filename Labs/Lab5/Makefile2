CC = clang
CFLAGS = -std=gnu11 -O2 -Wall -Wextra -Werror -pedantic -Wno-unused-result -Wconversion -Wvla
L5_SRC = *.c
L5 = l5
L5MC = l5_memory_check
L5MC_FLAGS = -fsanitize=address -fno-omit-frame-pointer -fsanitize=undefined -fsanitize=integer
.PHONY: clean

all: $(L5) $(L5MC)
    @echo l5 successfully constructed

$(L5): $(L5_SRC)
    $(CC) $(CFLAGS) -o $(L5) $(L5_SRC)

$(L5MC) : $(L5_SRC)
    $(CC) $(CFLAGS) $(L5MC_FLAGS) -o $(L5MC) $(L5_SRC)

.c.o:
    $(CC) $(CFLAGS) -c $< -o $@

clean:
    $(RM) *.o *.a *~ $(L5) $(L5MC)