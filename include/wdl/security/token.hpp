// token.hpp
//
// Free Function Definitions:
//	- wdl::security::open_current_token()
//	- wdl::security::get_token_information()
//
// Utility functions for working with Windows access tokens.

#pragma once

#include <windows.h>

#include <memory>
#include <iostream>

#include <wdl/handle/generic.hpp>

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
	// Returns: a null_handle wrapper around the token object

	wdl::handle::null_handle 
	open_current_token(
		unsigned long        access  = TOKEN_QUERY,
		open_thread_token_as open_as = open_thread_token_as::current
	)
	{
		auto token = wdl::handle::null_handle{};
		bool const open_as_self = open_as == open_thread_token_as::self;

		if (!::OpenThreadToken(
			::GetCurrentThread(),
			access,
			open_as_self,
			token.put()
		))
		{
			// call failed, determine the reason
			if (ERROR_NO_TOKEN != ::GetLastError())
			{
				// other error unrelated to impersonation level
				return wdl::handle::null_handle{};
			}
			
			// the calling thread is not impersonating; 
			// proceed to try process token

			if (!::OpenProcessToken(
				::GetCurrentProcess(),
				access,
				token.put()
			))
			{
				return wdl::handle::null_handle{};
			}
		}

		return token;
	}

	std::unique_ptr<char[]> get_token_information(
		HANDLE                  token,
		TOKEN_INFORMATION_CLASS info_class
		)
	{
		auto required_size = unsigned long{};
		if (!::GetTokenInformation(token, info_class, nullptr, 0, &required_size)
			&& ::GetLastError() != ERROR_INSUFFICIENT_BUFFER)
		{
			return std::unique_ptr<char[]>{};
		}

		auto buffer = std::make_unique<char[]>(required_size);
		if (!::GetTokenInformation(
			token, 
			info_class,
			buffer.get(),
			required_size,
			&required_size))
		{
			return std::unique_ptr<char[]>{};
		}

		return buffer;
	}
}