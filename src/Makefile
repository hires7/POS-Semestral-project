CC = gcc
CFLAGS = -Wall -Wextra -std=c11
LDFLAGS = -lpthread

# Zoznam zdrojových súborov
SRC = main.c client.c server.c ipc.c
OBJ = $(SRC:.c=.o)

# Výstupný súbor
TARGET = hadik

# Predvolený cieľ
all: $(TARGET)

# Zostavenie cieľa
$(TARGET): $(OBJ)
	$(CC) $(CFLAGS) $(OBJ) -o $(TARGET) $(LDFLAGS)

# Kompilácia jednotlivých objektových súborov
%.o: %.c
	$(CC) $(CFLAGS) -c $< -o $@

# Vyčistenie
clean:
	rm -f $(TARGET) $(OBJ)

