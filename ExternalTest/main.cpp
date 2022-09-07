#include "undocumented.h"
#include <vector>


int main()
{
	SetConsoleTitle(L"");



	// Call Windows32 function to get handle to process
	HWND hWnd = FindWindowA("ConsoleWindowClass", "FuckingDummyProgramStupidBitch"); //FindWindowA("Valve001", "Counter-Strike: Global Offensive - Direct3D 9");
	DWORD PID = 0;
	GetWindowThreadProcessId(hWnd, &PID);
	HANDLE hProc = OpenProcess(PROCESS_ALL_ACCESS, false, PID);
	// Call native system service rountine to get informaton about process specifically PBI information
	PROCESS_BASIC_INFORMATION pbi;
	tNtQueryInformationProcess NtQueryInformationProcess = 
		(tNtQueryInformationProcess)GetProcAddress(GetModuleHandle(L"ntdll.dll"), "NtQueryInformationProcess");
	NtQueryInformationProcess(hProc, ProcessBasicInformation, &pbi, sizeof(pbi), 0);


	const char* ModuleName = "PebTest.dll";
	int Status = 2;
	// 1 = Print all modules and base address of that module
	// 2 = Hide your module from PEB //InLoadOrderModuleList, //InMemoryOrderModuleList, //InInitializationOrderModuleList
	//0x77685d80



	uintptr_t AddressOfPeb = 0;
	ReadProcessMemory(hProc, (LPCVOID)((uintptr_t)pbi.PebBaseAddress + 0xC), &AddressOfPeb, 0x4, nullptr);
	uintptr_t InLoadOrderModuleList = ((uintptr_t)AddressOfPeb + 0xC);
	uintptr_t InMemoryOrderModuleList = ((uintptr_t)AddressOfPeb + 0x14);
	uintptr_t InInitializationOrderModuleList = ((uintptr_t)AddressOfPeb + 0x1C);


	int Element = 0;

	/*
	Didn't feel like compacting it into a function so you get this LOL
	
	*/

	while (true) {

		uintptr_t StartFlink = InMemoryOrderModuleList;
		uintptr_t CurrentFlink;
		short LengthOfName;
		short MaxLengthOfName;

		uintptr_t ModuleBefore; // For Hiding
		uintptr_t ModuleAfter; // For Hiding
		uintptr_t CurrentModuleName;
		uintptr_t CurrentModuleBase;
		uintptr_t NextFlink;


		if (Element == 0) {
			ReadProcessMemory(hProc, (LPCVOID)StartFlink, &CurrentFlink, 0x4, 0);
			Element++;
		}
		else {
			ReadProcessMemory(hProc, (LPCVOID)CurrentFlink, &CurrentFlink, 0x4, 0);
			if (CurrentFlink == StartFlink) {
				Element = 0; // Reset for next one
				break;
			}
		}

		ReadProcessMemory(hProc, (LPCVOID)CurrentFlink, &ModuleAfter, 0x4, 0);
		ReadProcessMemory(hProc, (LPCVOID)(CurrentFlink + 0x4), &ModuleBefore, 0x4, 0);
		ReadProcessMemory(hProc, (LPCVOID)(CurrentFlink + 0x10), &CurrentModuleBase, 0x4, 0);
		ReadProcessMemory(hProc, (LPCVOID)(CurrentFlink + 0x24), &LengthOfName, 0x2, 0);
		ReadProcessMemory(hProc, (LPCVOID)(CurrentFlink + 0x26), &MaxLengthOfName, 0x2, 0);
		ReadProcessMemory(hProc, (LPCVOID)(CurrentFlink + 0x28), &CurrentModuleName, 0x4, 0);


		char* NameArray{ new char[LengthOfName] };
		wchar_t CurrentCharacterInName;
		int SeperateIndexCounter = 0; //

		for (int Index = 0; Index < LengthOfName; Index++) {

			ReadProcessMemory(hProc, (LPCVOID)(CurrentModuleName + Index), &CurrentCharacterInName, 0x2, 0);
			if (CurrentCharacterInName < 0x7F) {
				NameArray[SeperateIndexCounter] = CurrentCharacterInName;
				SeperateIndexCounter++;
			}
		}

		if (Status == 1) {
			std::cout << NameArray << std::endl;
			std::cout << std::hex << "Base Address: [" << CurrentModuleBase << "]" << std::endl;
		}
		else if (Status == 2) {

			if (_stricmp(ModuleName, NameArray) == 0) {
				std::cout << "Found Module: " << NameArray << std::endl;
				std::cout << std::hex << "Base Address: [" << CurrentModuleBase << "]" << std::endl;
				std::cout << "Hiding Module" << std::endl;
				WriteProcessMemory(hProc, (LPVOID)ModuleBefore, &ModuleAfter, 0x4, 0);
				std::cout << "Module Hidden From: InLoadOrderModuleList" << std::endl;

			}
		}



	};
	while (true) {

		uintptr_t StartFlink = InLoadOrderModuleList;
		uintptr_t CurrentFlink;
		short LengthOfName;
		short MaxLengthOfName;

		uintptr_t ModuleBefore; // For Hiding
		uintptr_t ModuleAfter; // For Hiding
		uintptr_t CurrentModuleName;
		uintptr_t CurrentModuleBase;
		uintptr_t NextFlink;


		if (Element == 0) {
			ReadProcessMemory(hProc, (LPCVOID)StartFlink, &CurrentFlink, 0x4, 0);
			Element++;
		}
		else {
			ReadProcessMemory(hProc, (LPCVOID)CurrentFlink, &CurrentFlink, 0x4, 0);
			if (CurrentFlink == StartFlink) {
				Element = 0; // Reset for next one
				break;
			}
		}

		ReadProcessMemory(hProc, (LPCVOID)CurrentFlink, &ModuleAfter, 0x4, 0);
		ReadProcessMemory(hProc, (LPCVOID)(CurrentFlink + 0x4), &ModuleBefore, 0x4, 0);
		ReadProcessMemory(hProc, (LPCVOID)(CurrentFlink + 0x18), &CurrentModuleBase, 0x4, 0);
		ReadProcessMemory(hProc, (LPCVOID)(CurrentFlink + 0x2C), &LengthOfName, 0x2, 0);
		ReadProcessMemory(hProc, (LPCVOID)(CurrentFlink + 0x2E), &MaxLengthOfName, 0x2, 0);
		ReadProcessMemory(hProc, (LPCVOID)(CurrentFlink + 0x30), &CurrentModuleName, 0x4, 0);


		char* NameArray{ new char[LengthOfName] };
		wchar_t CurrentCharacterInName;
		int SeperateIndexCounter = 0; //

		for (int Index = 0; Index < LengthOfName; Index++) {

			ReadProcessMemory(hProc, (LPCVOID)(CurrentModuleName + Index), &CurrentCharacterInName, 0x2, 0);
			if (CurrentCharacterInName < 0x7F) {
				NameArray[SeperateIndexCounter] = CurrentCharacterInName;
				SeperateIndexCounter++;
			}
		}

		/*
		if (Status == 1) {
			std::cout << NameArray << std::endl;
			std::cout << std::hex << "Base Address: [" << CurrentModuleBase << "]" << std::endl;
		}
		*/
		if (Status == 2) {

			if (_stricmp(ModuleName, NameArray) == 0) {
				std::cout << "Found Module: " << NameArray << std::endl;
				std::cout << std::hex << "Base Address: [" << CurrentModuleBase << "]" << std::endl;
				std::cout << "Hiding Module" << std::endl;
				WriteProcessMemory(hProc, (LPVOID)ModuleBefore, &ModuleAfter, 0x4, 0);
				std::cout << "Module Hidden From: InMemoryOrderModuleList" << std::endl;

			}
		}



	};
	while (true) {

		uintptr_t StartFlink = InInitializationOrderModuleList;
		uintptr_t CurrentFlink;
		short LengthOfName;
		short MaxLengthOfName;

		uintptr_t ModuleBefore; // For Hiding
		uintptr_t ModuleAfter; // For Hiding
		uintptr_t CurrentModuleName;
		uintptr_t CurrentModuleBase;
		uintptr_t NextFlink;


		if (Element == 0) {
			ReadProcessMemory(hProc, (LPCVOID)StartFlink, &CurrentFlink, 0x4, 0);
			Element++;
		}
		else {
			ReadProcessMemory(hProc, (LPCVOID)CurrentFlink, &CurrentFlink, 0x4, 0);
			if (CurrentFlink == StartFlink) {
				Element = 0; // Reset for next one
				break;
			}
		}

		ReadProcessMemory(hProc, (LPCVOID)CurrentFlink, &ModuleAfter, 0x4, 0);
		ReadProcessMemory(hProc, (LPCVOID)(CurrentFlink + 0x4), &ModuleBefore, 0x4, 0);
		ReadProcessMemory(hProc, (LPCVOID)(CurrentFlink + 0x8), &CurrentModuleBase, 0x4, 0);
		ReadProcessMemory(hProc, (LPCVOID)(CurrentFlink + 0x1C), &LengthOfName, 0x2, 0);
		ReadProcessMemory(hProc, (LPCVOID)(CurrentFlink + 0x1E), &MaxLengthOfName, 0x2, 0);
		ReadProcessMemory(hProc, (LPCVOID)(CurrentFlink + 0x20), &CurrentModuleName, 0x4, 0);


		char* NameArray{ new char[LengthOfName] };
		wchar_t CurrentCharacterInName;
		int SeperateIndexCounter = 0; //

		for (int Index = 0; Index < LengthOfName; Index++) {

			ReadProcessMemory(hProc, (LPCVOID)(CurrentModuleName + Index), &CurrentCharacterInName, 0x2, 0);
			if (CurrentCharacterInName < 0x7F) {
				NameArray[SeperateIndexCounter] = CurrentCharacterInName;
				SeperateIndexCounter++;
			}
		}


		/*
		if (Status == 1) {
			std::cout << NameArray << std::endl;
			std::cout << std::hex << "Base Address: [" << CurrentModuleBase << "]" << std::endl;
		}
		*/
		if (Status == 2) {

			if (_stricmp(ModuleName, NameArray) == 0) {
				std::cout << "Found Module: " << NameArray << std::endl;
				std::cout << std::hex << "Base Address: [" << CurrentModuleBase << "]" << std::endl;
				std::cout << "Hiding Module" << std::endl;
				WriteProcessMemory(hProc, (LPVOID)ModuleBefore, &ModuleAfter, 0x4, 0);
				std::cout << "Module Hidden From: InInitializationOrderModuleList" << std::endl;

			}
		}



	};

	while (true) {
	}

	return 0;
}
