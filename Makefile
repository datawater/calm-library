CC = gcc
CFLAGS = -Wall -Wextra -Werror -static
SRC = src/main.c
DATASQL = src/datadb.c
OUTPUT = out/main

main:
	-mkdir out
	$(CC) $(CFLAGS) $(DATASQL) --shared -o out/datasql.o
	$(CC) $(CFLAGS) out/datasql.o $(SRC) -o $(OUTPUT)
	-chmod +x $(OUTPUT)

run:
	-mkdir out
	$(CC) $(CFLAGS) $(DATASQL) --shared -o out/datasql.o
	$(CC) $(CFLAGS) out/datasql.o $(SRC) -o $(OUTPUT)
	-chmod +x $(OUTPUT)
	@if ./out/main; then \
		echo "";\
	else echo ""; \
		.\out\main.exe;\
	fi
