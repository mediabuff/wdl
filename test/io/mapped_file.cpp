// mapped_file.cpp
// Unit Test: wdl::io::mapped_file
//
// Demonstration of WDL utility for simplified memory-mapped file API.
//
// Build
//	cl /EHsc /nologo /std:c++17 /W4 /I C:\Dev\WDL mapped_file_test.cpp

#include <windows.h>
#include <cstdio>

#include "wdl/io/mapped_file.hpp"

constexpr auto STATUS_SUCCESS_I = 0x0;
constexpr auto STATUS_FAILURE_I = 0x1;

int wmain(int argc, wchar_t *argv[])
{
	if (argc != 2)
	{
		printf("[-] Invalid arguments\n");
		printf("[-] Usage: %ws [FILE NAME]\n", argv[0]);
		return STATUS_FAILURE_I;
	}

	auto view = wdl::io::mapped_file{ argv[1] };
	if (!view)
	{
		printf("[-] Mapped file construction failed\n");
		printf("[-] GLE: %u\n", ::GetLastError());
		return STATUS_FAILURE_I;
	}

	printf("[+] Mapped file construction succeeded\n");
	printf("[+] Size: %llu\n", view.size().QuadPart);

	return STATUS_SUCCESS_I;
}