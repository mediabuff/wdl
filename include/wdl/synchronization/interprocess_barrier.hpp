// interprocess_barrier.hpp
//
// Class Implementation: wdl::synchroninzation::interprocess_barrier

#pragma once

#include <windows.h>
#include <iostream>

#include <wdl/handle/generic.hpp>
#include <wdl/error/win32_exception.hpp>

namespace wdl::synchronization
{
    // wdl::synchronization::interprocess_barrier
    //
    // An extension of the Windows SYNCHRONIZATION_BARRIER
    // that supports among distinct process contexts.

    class interprocess_barrier
    {
        using null_handle = wdl::handle::null_handle;
        using win32_exception = wdl::error::win32_exception;

        null_handle m_mapping;
        null_handle m_semaphore;
        char*       m_shared;

    public:
        interprocess_barrier(
            wchar_t const* name,
            long           count
            )
        {
            auto const size 
                = ULARGE_INTEGER{sizeof(long long), 0};

			m_mapping = null_handle
			{
				::CreateFileMappingW(
					INVALID_HANDLE_VALUE,
					nullptr,
					PAGE_READWRITE | SEC_COMMIT,
					size.HighPart, 
                    size.LowPart,
					name)
			};

			if (!m_mapping) throw win32_exception{};

            // the barrier should only be initialized once,
            // the first time that the mapping is created
            auto const should_initialize = 
                !(::GetLastError() == ERROR_ALREADY_EXISTS);

			m_shared = static_cast<char*>(
				::MapViewOfFile(
					m_mapping.get(),
					FILE_MAP_READ | FILE_MAP_WRITE,
					0, 0, 
                    size.QuadPart));

            if (!m_shared) throw win32_exception{};

            if (should_initialize)
            {
                ::InterlockedExchange64(
                    reinterpret_cast<long long*>(m_shared),
                    0);
            }

            m_semaphore = null_handle
            {
                ::CreateSemaphoreW(nullptr, 0, count, name)
            };

            if (!m_semaphore) throw win32_exception{};
        }

        ~interprocess_barrier()
        {
            unmap();
        }

        interprocess_barrier(interprocess_barrier const&)            = delete;
        interprocess_barrier& operator=(interprocess_barrier const&) = delete;

        void enter()
        {   
            auto* count = reinterpret_cast<long long*>(m_shared);
            auto const waiting = ::InterlockedIncrement64(count);
        }

    private:
		void unmap() noexcept
		{
            auto* begin = reinterpret_cast<char*>(m_shared);

			if (begin)
			{
				::UnmapViewOfFile(begin);
			}
		}
    };
}