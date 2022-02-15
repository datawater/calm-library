CC = g++
LINKER = lld
CFLAGS = -Wall -Wextra -std=c++17 -Os -ffunction-sections -fdata-sections -Wl,--gc-sections -s -fuse-ld=$(LINKER)
SRC = src/main.cpp
OUTPUT_ = out/main
UPX_OUT_ = out/main_upx

ifeq ($(OS),Windows_NT)
	OUTPUT = $(OUTPUT_).exe
	UPX_OUT = $(UPX_OUT_).exe
else
	OUTPUT = $(OUTPUT_)
	UPX_OUT = $(UPX_OUT_)
endif

main:
	-mkdir out
	$(CC) $(CFLAGS) $(SRC) -o $(OUTPUT)
	-chmod +x $(OUTPUT)
	-strip $(OUTPUT) -s -R .comment -R .gnu.version --strip-unneeded

run:
	-mkdir out
	$(CC) $(CFLAGS) $(SRC) -o $(OUTPUT)
	-chmod +x $(OUTPUT)	
	-strip $(OUTPUT) -s -R .comment -R .gnu.version --strip-unneeded
	./$(OUTPUT)

upx:
	-mkdir out
	$(CC) $(CFLAGS) $(SRC) -o $(OUTPUT)
	-chmod +x $(OUTPUT)
	-strip $(OUTPUT) -s -R .comment -R .gnu.version --strip-unneeded
	-export upx=""
	upx --best --ultra-brute $(OUTPUT) -o $(UPX_OUT)
	mv $(UPX_OUT) $(OUTPUT)
