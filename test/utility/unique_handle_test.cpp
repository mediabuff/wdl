// UniqueHandles.cpp
// Unit Test: 
//	wdl::utility::unique_handle
//	wdl::utility::mapped_file_deleter
//
// Demonstration of WDL API and functionality.
//
// Build
//	cl /EHsc /nologo /std:c++17 /W4 /I C:\Dev\WDL unique_handle_test.cpp

#include <windows.h>
#include <cstdio>
#include <memory>

#include "wdl/utility/deleter.hpp"
#include "wdl/utility/unique_handle.hpp"

constexpr auto STATUS_SUCCESS_I = 0x0;
constexpr auto STATUS_FAILURE_I = 0x1;

inline void print_with_info(const char* msg);
inline void print_with_err(const char* msg);

int main(int argc, char* argv[])
{
	if (argc != 2)
	{
		print_with_err("Invalid arguments");
		return STATUS_FAILURE_I;
	}

	auto file = wdl::utility::invalid_handle
	{
		::CreateFileA(
			argv[1],
			GENERIC_READ,
			FILE_SHARE_READ,
			nullptr,
			OPEN_EXISTING,
			FILE_ATTRIBUTE_NORMAL,
			nullptr
		)
	};

	if (!file)
	{
		print_with_err("CreateFile() failed");
		return STATUS_FAILURE_I;
	}

	auto size = LARGE_INTEGER{};
	if (!::GetFileSizeEx(file.get(), &size))
	{
		print_with_err("GetFileSizeEx() failed");
		return STATUS_FAILURE_I;
	}

	if (0 == size.QuadPart)
	{
		print_with_err("Invalid file size");
		return STATUS_FAILURE_I;
	}

	auto mapping = wdl::utility::null_handle
	{
		::CreateFileMappingW(
			file.get(),
			nullptr,
			PAGE_READONLY,
			0, 0,
			nullptr
		)
	};

	if (!mapping)
	{
		print_with_err("CreateFileMapping() failed");
		return STATUS_FAILURE_I;
	}

	auto view = std::unique_ptr<char, wdl::utility::map_view_deleter>
	{
		static_cast<char*>(
			::MapViewOfFile(
				mapping.get(),
				FILE_MAP_READ,
				0, 0,
				0
			))
	};

	if (!view)
	{
		print_with_err("MapViewOfFile() failed");
		return STATUS_FAILURE_I;
	}

	print_with_info("Successfully mapped view of file");

	printf("%.*s\n", static_cast<unsigned>(size.QuadPart), view.get());

	return STATUS_SUCCESS_I;
}

inline void print_with_info(const char* msg)
{
	printf("[+] %s\n", msg);
}

inline void print_with_err(const char* msg)
{
	printf("[!] %s (GLE: 0x%08X)\n", msg, ::GetLastError());
}