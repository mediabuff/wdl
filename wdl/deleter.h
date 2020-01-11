// deleter.h
// Custom deleter definitions for use with C++ standard library smart pointers.

#pragma once

#include <windows.h>

namespace wdl
{
	// map_view_deleter
	// 
	// Custom deleter for mapped file views
	// i.e. CreateFileMapping(), MapViewOfFile()

	struct map_view_deleter
	{
		using pointer = char* const;

		void operator()(pointer value) const noexcept
		{
			::UnmapViewOfFile(value);
		}
	};
}