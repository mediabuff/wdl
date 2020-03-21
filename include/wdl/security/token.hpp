// token.hpp
//
// Free Function Definitions:
//	- wdl::security::open_current_token()
//
// Utility functions for working with Windows access tokens.

#pragma once

#include <windows.h>
#include <iostream>

#include <wdl/utility/unique_handle.hpp>

namespace wdl::security
{
	enum class open_thread_token_as
	{
		current,   // perform security check using impersonation token of calling thread
		self       // perform security check using process token of containing process
	};

	// wdl::security::open_current_token()
	//
	// Acquire a handle to access token representing the security context
	// of the currently executing thread or its containing process.
	//
	// Arguments:
	//	access  - desired access mask for token object
	//	open_as - enumeration value specifying whether access control checks 
	//			  on the token object should be performed in the context of 
	//			  the currently thread's impersonation token or the token of
	//			  its containing process
	//
	// Returns: a unique_handle wrapper around the token object

	wdl::utility::null_handle 
	open_current_token(
		unsigned long        access  = TOKEN_QUERY,
		open_thread_token_as open_as = open_thread_token_as::current
	)
	{
		HANDLE h_token{};
		bool open_as_self = open_as == open_thread_token_as::self;

		if (!::OpenThreadToken(
			::GetCurrentThread(),
			access,
			open_as_self,
			&h_token
		))
		{
			// call failed, determine the reason
			if (ERROR_NO_TOKEN != ::GetLastError())
			{
				// other error unrelated to impersonation level
				return wdl::utility::null_handle{};
			}
			
			// the calling thread is not impersonating; 
			// proceed to try process token

			if (!::OpenProcessToken(
				GetCurrentProcess(),
				access,
				&h_token
			))
			{
				return wdl::utility::null_handle{};
			}
		}

		// h_token now contains a valid HANDLE to the desired token object
		return wdl::utility::null_handle{ h_token };
	}
}