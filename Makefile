CC = gcc
CFLAGS = -Wall -Wextra -Werror -static
SRC = src/main.c
DATADB = src/datadb.c
OUTPUT = out/main

main:
	-mkdir out
	$(CC) $(CFLAGS) $(DATADB) --shared -o out/datadb.o
	$(CC) $(CFLAGS) out/datadb.o $(SRC) -o $(OUTPUT)
	-chmod +x $(OUTPUT)

run:
	-mkdir out
	$(CC) $(CFLAGS) $(DATADB) --shared -o out/datadb.o
	$(CC) $(CFLAGS) out/datadb.o $(SRC) -o $(OUTPUT)
	-chmod +x $(OUTPUT)
	@if ./out/main; then \
		echo "";\
	else echo ""; \
		.\out\main.exe;\
	fi
