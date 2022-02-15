#include <fstream>
#include <sstream>
#include <cstring>

#include <unistd.h>

bool fast_print = false;

#ifdef _WIN32
#include <Windows.h>

#define sleep Sleep

//? maybe i should add this to a seperate file

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
			exit(1);
		break;
	}
}


void todo(char* msg) {
	printf("\n");
	console(RED);
	printf("TODO: ");
	printf("%s\n", msg);
	console(RESET);
	//exit(1);
}

bool file_exists(char* fname) {
	if (access(fname, F_OK) == 0) {
		return true;
	}
	return false;
}

bool valid_books_file(std::string fname) {
	FILE *fp = fopen(fname.c_str(), "r");
	if (fp == NULL) {
		return false;
	}
	//read the first line
	char line[256];
	fgets(line, sizeof(line), fp);
	//check if it is a valid header
	if (strcmp(line, "CALM_LIBRARY\n") != 0) {
		fclose(fp);
		return false;
	}
	fclose(fp);
	//printf(line);
	return true;
}

bool valid_settings_file(std::string fname) {
	FILE *fp = fopen(fname.c_str(), "r");
	if (fp == NULL) {
		return false;
	}
	//read the first line
	char line[256];
	fgets(line, sizeof(line), fp);
	if (strcmp(line, "CALM_SETTINGS\n") != 0) {
		fclose(fp);
		return false;
	}
	fclose(fp);
	return true;
}

void ascii_hello() {
	//ascii art by http://patorjk.com/software/taag/#p=display&f=Graffiti&t=Type%20Something%20

	printf(" _     _  \n");
	printf("| |   (_) \n");
	printf("| |__  _  \n");
	printf("| '_ \\| | \n");
	printf("| | | | | \n");
	printf("|_| |_|_| \n");

	for (int i = 0; i < 4; ++i) {
		printf("\n");
	}
}

void ascii_bye() {
	//ascii art by http://patorjk.com/software/taag/#p=display&f=Graffiti&t=Type%20Something%20

	printf(" _\n");
	printf("| |\n");
	printf("| |__  _   _  ___ \n");
	printf("| '_ \\| | | |/ _ \\\n");
	printf("| |_) | |_| |  __/\n");
	printf("|_.__/ \\__, |\\___|\n");
	printf("        __/ |      \n");
	printf("       |___/       \n");

	for (int i = 0; i < 4; ++i) {
		printf("\n");
	}
}

void ascii_search() {
	//ascii art by http://patorjk.com/software/taag/#p=display&f=Graffiti&t=Type%20Something%20

	printf("                         _     \n");
	printf("                     \t| |    \n");
	printf(" ___  ___  __ _ _ __ ___| |__  \n");
	printf("/ __|/ _ \\/ _` | '__/ __| '_ \\ \n");
	printf("\\__ \\  __/ (_| | | | (__| | | |\n");
	printf("|___/\\___|\\__,_|_|  \\___|_| |_|");

	for (int i = 0; i < 4; ++i) {
		printf("\n");
	}

}

void ascii_add() {
	//ascii art by http://patorjk.com/software/taag/#p=display&f=Graffiti&t=Type%20Something%20

	printf("           _     _ \n");
	printf("          | |   | |\n");
	printf("  __ _  __| | __| |\n");
	printf(" / _` |/ _` |/ _` |\n");
	printf("| (_| | (_| | (_| |\n");
	printf(" \\__,_|\\__,_|\\__,_|\n");

	for (int i = 0; i < 4; ++i) {
		printf("\n");
	}
}

void ascii_clear() {
	printf("       _                 \n      | |                \n   ___| | ___  __ _ _ __ \n  / __| |/ _ \\/ _` | '__|\n | (__| |  __/ (_| | |   \n  \\___|_|\\___|\\__,_|_|   \n                         \n                         ");
	for (int i = 0; i < 4; ++i) {
		printf("\n");
	}
}

void ascii_settings(){
	printf("           _   _   _                 \n          | | | | (_)                \n  ___  ___| |_| |_ _ _ __   __ _ ___ \n / __|/ _ \\ __| __| | '_ \\ / _` / __|\n \\__ \\  __/ |_| |_| | | | | (_| \\__ \\\n |___/\\___|\\__|\\__|_|_| |_|\\__, |___/\n                            __/ |    \n                           |___/     ");
	for (int i = 0; i < 4; ++i) {
		printf("\n");
	}
}

std::string uinput() {
	std::string result;

	fflush (stdin);

	char c = getchar();
	while (c != '\n' && c != EOF) {
		result += c;
		c = getchar();
	}
	return result;
}

void add_book() {

	console(CLEAR);
	ascii_add();

	//ask for the database file
	std::string db_file;
	console(GREEN);
	printf("Enter the database file: ");
	console(RESET);

	fflush(stdin);

	db_file = uinput();
	printf("\n");

	if (db_file.find(".calm") == std::string::npos) {
		db_file += ".calm";
	}

	sleep(0.2);

	if (!file_exists(&db_file[0])) {
		std::ofstream fo(db_file, std::ios::binary);
		fo << "CALM_LIBRARY\n";	
		fo.close();
	}

	sleep(0.2);

	if (!valid_books_file(db_file)) {
		console(RED);
		printf("Invalid database file\n");
		console(RESET);
		return;
	}

	std::ofstream _fo(db_file, std::ios::binary | std::ios::app);

	std::string title;
	std::string author;
	std::string genre;
	std::string description;
	std::string bookshelf;
	std::string comment;

	console(GREEN);
	printf("Enter the title: ");
	console(RESET);

	title = uinput();

	if (title.length() < 2) {
		console(RED);
		printf("Invalid title\n");
		console(RESET);
		return;
	}

	console(GREEN);
	printf("Enter the author: ");
	console(RESET);

	author = uinput();

	if (author.length() < 2) {
		console(RED);
		printf("Invalid author\n");
		console(RESET);
		return;
	}

	console(GREEN);
	printf("Enter the genre: (optional) ");
	console(RESET);

	genre = uinput();

	if (genre.length() < 2) {
		genre = (char*) "None";
	}

	console(GREEN);
	printf("Enter the description: (optional) ");
	console(RESET);

	description = uinput();

	if (description.length() < 2) {
		description = (char*) "None";
	}

	console(GREEN);
	printf("Enter the bookshelf number: (optional) ");
	console(RESET);

	bookshelf = uinput();

	if (bookshelf.length() < 2) {
		bookshelf = (char*) "None";
	}

	console(GREEN);	
	printf("Enter A comment: (optional) ");
	console(RESET);

	comment = uinput();

	if (comment.length() < 2) {
		comment = (char*) "None";
	}

	_fo << "_book\n"
	   << "74 " << title << "\n"
	   << "61 " << author << "\n"
	   << "67 " << genre << "\n"
	   << "64 " << description << "\n"
	   << "62 " << bookshelf << "\n"
	   << "63 " << comment << "\n";

	console(GREEN);
	printf("Book added successfully\n");
	console(RESET);
}

void search_book() {

	console(CLEAR);
	ascii_search();

	std::string db_file;
	console(GREEN);
	printf("Enter the database file: ");
	console(RESET);
	fflush(stdin);
	db_file = uinput();

	printf("\n");

	if (db_file.find(".calm") == std::string::npos) {
		db_file += ".calm";
	}

	sleep(0.2);

	if (!file_exists(&db_file[0])) {
		console(RED);
		printf("database file Doesn't exists\n");
		console(RESET);
		exit(1);
	}

	if (!valid_books_file(db_file)) {
		console(RED);
		printf("Invalid database file\n");
		console(RESET);
		exit(1);
	}

	console(GREEN);
	printf("How do you want to search?\n");
	console(RESET);
	console(GREEN);
	printf("[1] ");
	console(RESET);
	console (YELLOW);
	printf("By Title\n");
	console(RESET);
	console(GREEN);
	printf("[2] ");
	console(RESET);
	console (YELLOW);
	printf("quit\n");

	char c = getchar();
	if (c == *"1") {
		std::string title;
		console(GREEN);
		printf("Enter The Title: ");
		console(RESET);
		title = uinput();

		std::ifstream _fi(db_file, std::ios::binary);
		//load the file into memory
		std::string _file;
		std::string line;

		std::stringstream ss;
		bool found = false;
		int to_add = 5;

		while (std::getline(_fi, line)) {
			line.erase(0, 3);
			if (line.find(title) != std::string::npos) {
				ss << line << "\n";
				found = true;
			}
			if (found && to_add > 0) {
				ss << line << "\n";
				to_add--;
			} else if (found && to_add == 0) {
				break;
			}
		}

		if (!found) {
			console(RED);
			printf("Book not found\n");
			console(RESET);
			return;
		} else {
			//print the data
			console(GREEN);
			printf("Found Book! \n");
			console(YELLOW);

			std::stringstream ss2;
			ss2 << ss.str();
			std::string line2;
			while (std::getline(ss2, line2)) {
				printf("%s\n", line2.c_str());
			}

			console(RESET);
		}
	} else if (c == *"2") {
		exit(0);
	} else {
		console(RED);
		printf("Invalid option\n");
		console(RESET);
		return;
	}
}

void clear_db() {

	console(CLEAR);
	ascii_clear();

	console(RED);
	printf("Are you sure that you want to erase the whole database? [y/n]\n");
	console(RESET);

	fflush(stdin);
	std::string choice = uinput();

	//printf(choice.c_str());

	if (choice == "n" || choice == "N") {
		printf("Quiting");
		exit(0);
	}

	std::string db_file;
	console(GREEN);
	printf("Enter the database file: ");
	console(RESET);
	fflush(stdin);
	db_file = uinput();

	printf("\n");

	if (db_file.find(".calm") == std::string::npos) {
		db_file += ".calm";
	}
	sleep(0.2);

	if (!file_exists(&db_file[0])) {
		console(RED);
		printf("database file Doesn't exists\n");
		console(RESET);
		exit(1);
	}
	if (!valid_books_file(db_file)) {
		console(RED);
		printf("Invalid database file\n");
		console(RESET);
		exit(1);
	}

	std::ofstream _ofs;
	_ofs.open(db_file, std::ofstream::out | std::ofstream::trunc);
	_ofs << "CALM_LIBRARY\n";
	_ofs.close();

	console(GREEN);
	printf("Succesfully cleared the Database file");

}

void settings_init() {
	//check if the file exists
	if (!file_exists((char *)".calm_settings")) {
		//create the file
		std::ofstream _ofs;
		_ofs.open(".calm_settings", std::ofstream::out);
		_ofs << "CALM_SETTINGS\n";
		_ofs << "fast_print: false\n";
	} else {
		if (!valid_settings_file(".calm_settings")) {
			console(RED);
			printf("Invalid Or settings file Do you want to Recover it [y/n]\n");
			console(RESET);
			fflush(stdin);
			std::string choice = uinput();
			if (choice == "y" || choice == "Y") {
				std::ofstream _ofs;
				_ofs.open(".calm_settings", std::ofstream::out | std::ofstream::trunc);
				_ofs << "CALM_SETTINGS\n";
				_ofs << "fast_print: false\n";
				_ofs.close();
				console(GREEN);
				printf("Succesfully recovered the settings file\n");
				console(RESET);
			} else {
				exit(1);
			}
		}

		std::ifstream _fi(".calm_settings", std::ios::binary);
		//read the file line by line
		std::string line;
		while (std::getline(_fi, line)) {
			if (line.find("fast_print") != std::string::npos) {
				if (line.find("true") != std::string::npos) {
					fast_print = true;
				} else {
					fast_print = false;
				}
			}
		}
	}
}

void settings_menu() {
	console(CLEAR);
	ascii_settings();

	console(GREEN);
	printf("[1] ");
	console(RESET);
	console (YELLOW);
	printf("Fast Print (Not print some text charachter by charachter) current setting: ");
	if (fast_print) {
		printf("true\n");
	} else {
		printf("false\n");
	}

	fflush(stdin);
	char c = getchar();
	if (c == *"1") {
		//change the setting
		if (fast_print) {
			fast_print = false;
		} else {
			fast_print = true;
		} 
		std::ofstream _ofs;
		_ofs.open(".calm_settings", std::ofstream::out | std::ofstream::trunc);
		_ofs << "CALM_SETTINGS\n";
		_ofs << "fast_print: ";
		if (fast_print) {
			_ofs << "true\n";
		} else {
			_ofs << "false\n";
		}
		_ofs.close();
		console(GREEN);
		printf("Succesfully changed the setting to: ");
		console(RESET);
		console(YELLOW);
		if (fast_print) {
			printf("true\n");
		} else {
			printf("false\n");
		}
		console(RESET);
	} else if (c == *"\n") {} else {
		console(RED);
		printf("Invalid option\n");
		console(RESET);
		exit(1);
	}

	console(RESET);
}

void charbychar(char *msg, int delay) {
	for (size_t i = 0; i < strlen(msg); ++i) {
		printf("%c", msg[i]);
		if (!fast_print) {
			sleep(delay);
		}
	}
}


void welcome() {
	ascii_hello();
	console(YELLOW);
	charbychar((char*) "Hello, Welcome To The \"Calm Library\" Library management system\nWhat would you like to do?\n\n", 25);
	console(RESET);
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
	console(GREEN);
	printf("[3] ");
	console(RESET);
	console (YELLOW);
	printf("Clear the database\n");
	console(RESET);
	console(GREEN);
	printf("[s] ");
	console(RESET);
	console(YELLOW);
	printf("Settings\n");
	console(RESET);
	console(GREEN);
	printf("[q] ");
	console(RESET);
	console (YELLOW);
	printf("Exit\n");
	console(RESET);
}

void bye() {
	console(CLEAR);
	ascii_bye();
	console(YELLOW);
	charbychar((char*) "Thank you for using the \"Calm Library\" Library management system\n\n", 25);
	console(RESET);
	exit(0);
}

void what_do() {
	char c = getchar();

	if (c == *"1") {add_book();} 
	else if (c == *"2") {search_book();} 
	else if (c == *"3") {clear_db();}
	else if (c == *"q") {bye();}
	else if (c == *"s") {settings_menu();}
	else if (c == *"\n") {exit(0);}
	else {
		printf("Invalid input\n");
		exit(1);
	}
}

int main(int argc, char *argv[]) {

	(void) argc;
	(void) argv;

	settings_init();

	console(CLEAR);
	welcome();

	usage();
	what_do();

	return 0;

}
