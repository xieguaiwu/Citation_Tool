#pragma once
#include"includes.h"

enum ShitTypes {authors, titles, years, trans, publish, pushed, types_count};
enum ComanndTypes {add, list, remove_index, clear, output_index, citation, help, command_count};
//"remove_index" and "output_index" = "delete" and "export"

//use "unsigned" when possible
const int books = 10000;//maximum amount of citations
const unsigned char all_types = types_count;//the additional type is for pt[]
const unsigned char types = all_types - 1;
//both types and all_types starts from 1
//Beware when writing loops: write "for(int x = 0 ; x **<** types/all_types; x++)"
const unsigned char command_types = command_count;//command_types also starts from 1

enum Color {red, green, blue, white = 6};//colors

char key;
bool isbadint;//judge whether the input value is okay or not

#ifdef _WIN32
#include <conio.h>
#else
int _getch() {//be prepared for cross-platform development
	struct termios oldt, newt;
	int ch;
	tcgetattr(STDIN_FILENO, &oldt);
	newt = oldt;
	newt.c_lflag &= ~(ICANON | ECHO);
	tcsetattr(STDIN_FILENO, TCSANOW, &newt);
	ch = getchar();
	tcsetattr(STDIN_FILENO, TCSANOW, &oldt);
	return ch;
}
#endif

void clear_screen() {
#ifdef _WIN32
	system("cls");
#else
	system("clear");
#endif
}

//it would be easier to use this within confirmations
bool yes() {
	return (key == 'Y' || key == 'y');
}

bool no() {
	return (key == 'N' || key == 'n');
}

void pause(int screen = 0) {//use this between interface switchings
	cout << " (Press any key to continue...)\n";
	_getch();
	if (screen == 1)clear_screen();//be careful with clear screen commands
}

void confirmation() {//notice when coming to ask for confirm
	cout << " (Press Y to confirm, N to refuse)\n";
}

void hyphen() {//making the menu looks better
	cout << "------------------------------\n";
}

void deal_badint() {//avoid repeating in code
	cin.clear();
	cin.ignore(numeric_limits<streamsize>::max(), '\n');
	isbadint = true;
}

void badint() {//judge wether the input value is okay or not
	if (!cin.fail()) {
		if (cin.peek() != '\n') deal_badint();//unlikely to happen
		else {
			isbadint = false;
		}
	}
	else deal_badint();
}

//this function as well as colorc are only possible under WIN32
void setcopy(char* tocopy) {
#ifdef _WIN32
	if (!OpenClipboard(nullptr)) return;
	EmptyClipboard();//it is necessary to empty clipboard
	size_t len = strlen(tocopy) + 1;
	HGLOBAL hMem = GlobalAlloc(GMEM_MOVEABLE, len);
	if (hMem) {
		char* pMem = static_cast<char*>(GlobalLock(hMem));
		if (pMem) {
			strcpy_s(pMem, len, tocopy);
			GlobalUnlock(hMem);
			SetClipboardData(CF_TEXT, hMem);
		}
	}
	CloseClipboard();
#endif
}

//set colors of texts. Only works when terminal is in default color
//(color 7a in - black back ground and white text)
void colorc(int x) {
#ifdef _WIN32
	if (x == red)SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), FOREGROUND_INTENSITY | FOREGROUND_RED);//pure red
	if (x == green)SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), FOREGROUND_INTENSITY | FOREGROUND_GREEN);//pure green
	if (x == blue)SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), FOREGROUND_INTENSITY | FOREGROUND_BLUE);//pure blue
	if (x == white)SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), FOREGROUND_INTENSITY | FOREGROUND_RED | FOREGROUND_GREEN | FOREGROUND_BLUE);//red+green+blue=white
#else
	//use ANSI on Linux
	switch (x) {
	case red: std::cout << "\033[1;31m"; break;
	case green: std::cout << "\033[1;32m"; break;
	case blue: std::cout << "\033[1;34m"; break;
	case white: std::cout << "\033[0m"; break;
	}
#endif
}