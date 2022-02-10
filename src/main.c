#include <stdio.h>
#include <assert.h>
#include <stdbool.h>

#include "include/datadb.h"

#ifdef _WIN32
#include <Windows.h>
#define sleep Sleep

void clear_win() {
    COORD topLeft  = { 0, 0 };
    HANDLE console = GetStdHandle(STD_OUTPUT_HANDLE);
    CONSOLE_SCREEN_BUFFER_INFO screen;
    DWORD written;

    GetConsoleScreenBufferInfo(console, &screen);
    FillConsoleOutputCharacterA(
        console, ' ', screen.dwSize.X * screen.dwSize.Y, topLeft, &written
    );
    FillConsoleOutputAttribute(
        console, FOREGROUND_GREEN | FOREGROUND_RED | FOREGROUND_BLUE,
        screen.dwSize.X * screen.dwSize.Y, topLeft, &written
    );
    SetConsoleCursorPosition(console, topLeft);
}

static WORD win_default_attributes(HANDLE hConsole) {
	static WORD defaultAttributes = 0;
	CONSOLE_SCREEN_BUFFER_INFO info;

    if (!defaultAttributes && GetConsoleScreenBufferInfo(hConsole, &info)) {
		defaultAttributes = info.wAttributes;
	}

	return defaultAttributes;
}

static void win_set_attributes(int fg, int bg) {
    WORD defaultAttributes;
    CONSOLE_SCREEN_BUFFER_INFO info;
    HANDLE hConsole = GetStdHandle(STD_OUTPUT_HANDLE);

    defaultAttributes = win_default_attributes(hConsole);

    if (fg == -1 && bg == -1) {
        SetConsoleTextAttribute(hConsole, defaultAttributes);
        return;
    }

    if (!GetConsoleScreenBufferInfo(hConsole, &info)) {
        return;
    }

    if (fg != -1) {
        info.wAttributes &= ~(info.wAttributes & 0x0F);
        info.wAttributes |= (WORD)fg;
    }

    if (bg != -1) {
        info.wAttributes &= ~(info.wAttributes & 0xF0);
        info.wAttributes |= (WORD)bg;
    }

    SetConsoleTextAttribute(hConsole, info.wAttributes);
}

void reset_win() {win_set_attributes(-1, -1);}
void red_win() {win_set_attributes(FOREGROUND_RED, -1);}
void green_win() {win_set_attributes(FOREGROUND_GREEN, -1);}
void yellow_win() {win_set_attributes(FOREGROUND_RED | FOREGROUND_GREEN, -1);}
void blue_win() {win_set_attributes(FOREGROUND_BLUE, -1);}
void magenta_win() {win_set_attributes(FOREGROUND_RED | FOREGROUND_BLUE, -1);}

#else

#include <unistd.h>
#define sleep usleep

#define ANSII_COLOR_RESET "\033[0m"
#define ANSII_COLOR_RED "\033[31m"
#define ANSII_COLOR_GREEN "\033[32m"
#define ANSII_COLOR_YELLOW "\033[33m"
#define ANSII_COLOR_BLUE "\033[34m"
#define ANSII_COLOR_MAGENTA "\033[35m"
#define ANSII_CLEAR "\033[2J"

void _reset() {printf("%s", ANSII_COLOR_RESET);}
void _red() {printf("%s", ANSII_COLOR_RED);}
void _green() {printf("%s", ANSII_COLOR_GREEN);}
void _yellow() {printf("%s", ANSII_COLOR_YELLOW);}
void _blue() {printf("%s", ANSII_COLOR_BLUE);}
void _magenta() {printf("%s", ANSII_COLOR_MAGENTA);}
void _clear() {printf("%s", ANSII_CLEAR);}

#endif

#define RED 1
#define GREEN 2
#define YELLOW 3
#define BLUE 4
#define MAGENTA 5
#define RESET 6
#define CLEAR 7

void console(int c) {
	switch (c) {
		case RED:
			#ifdef _WIN32
				red_win();
			#else
				_red();
			#endif
		break;
		case GREEN:
			#ifdef _WIN32
				green_win();
			#else
				_green();
			#endif
		break;
		case YELLOW:
			#ifdef _WIN32
				yellow_win();
			#else
				_yellow();
			#endif
		break;
		case BLUE:
			#ifdef _WIN32
				blue_win();
			#else
				_blue();
			#endif
		break;
		case MAGENTA:
			#ifdef _WIN32
				magenta_win();
			#else
				_magenta();
			#endif
		break;
		case RESET:
			#ifdef _WIN32
				reset_win();
			#else
				_reset();
			#endif
		break;
		case CLEAR:
			#ifdef _WIN32
				clear_win();
			#else
				_clear();
			#endif
		break;
		default:
			printf("NON EXISTANT COLOR");
		break;
	}
}

void charbychar(char *msg, int delay) {
	for (size_t i = 0; i < strlen(msg); ++i) {
		printf("%c", msg[i]);
		sleep(delay);
	}
}

void ascii_hello() {
	printf("_$$_____$$_$$$$$$__$$______$$________$$$$\n");
	printf("_$$_____$$_$$______$$______$$______$$____$$\n");
	printf("_$$$$$$$$$_$$$$$$$_$$______$$______$$____$$\n");
	printf("_$$_____$$_$$______$$______$$______$$____$$\n");
	printf("_$$_____$$_$$______$$______$$______$$____$$\n");
	printf("_$$_____$$_$$$$$$$_$$$$$$$_$$$$$$$___$$$$\n\n");
	
	for (int i; i < 5; ++i) {
		printf("\n");
	}
}

void todo(char* msg) {
	printf("\n");
	console(RED);
	printf("TODO: ");
	printf("%s\n", msg);
	console(RESET);
	assert(1);
}

void usage() {
	console(GREEN);
	printf("[1] ");
	console(RESET);
	console (YELLOW);
	printf("Add a new book\n");
	console(RESET);
	console(GREEN);
	printf("[2] ");
	console(RESET);
	console (YELLOW);
	printf("Search For A book\n");
	console (RESET);
}

void add_book() {
	todo("Add a new book");
	console(RED);
	printf("File: %s Line: %d", __FILE__, __LINE__);
	console(RESET);
	assert(1);
	//TODO(#2): Implement add_book
}

void search_book() {
	todo("Search book");
	console(RED);
	printf("File: %s Line: %d", __FILE__, __LINE__);
	console(RESET);
	assert(1);
	//TODO: Implement search_book
}

//TODO: Implement more functions

void what_do() {
	char c = getchar();

	if (c == *"1") {
		add_book();
	} else if (c == *"2") {
		search_book();
	} else if (c == *"\n") {assert(1);} 
	else {
		printf("Invalid input\n");
		assert(1);
	}

}

int main() {
	
	datadb_t _db;
	_db.fname = "test.ddb";
	_db.db_file = NULL;
	_db.INIT_DONE = false;
	_db.STATUS = DATADB_OK;

	datadb_t db = datadb_init(_db);
	printf(db.INIT_DONE ? "INIT DONE\n" : "INIT NOT DONE\n");

	//return 0;

	console(CLEAR);
	ascii_hello();
	console(YELLOW);
	charbychar("Hello, Welcome To The \"Calm Library\" Library management system\nWhat would you like to do?\n\n", 25);
	console(RESET);

	usage();
	what_do();

	return 0;

}
