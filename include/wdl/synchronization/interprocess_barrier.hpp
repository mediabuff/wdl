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
    // that supports synchronization among distinct process contexts.

    class interprocess_barrier
    {
        using null_handle     = wdl::handle::null_handle;
        using win32_exception = wdl::error::win32_exception;
        
        null_handle m_event;
        null_handle m_mutex;

        null_handle         m_mapping;

        unsigned long long* m_count;
        unsigned long long  m_threshold;

        static constexpr const auto BUFFER_SIZE = 64;

    public:
        interprocess_barrier(
            wchar_t const*           name,
            unsigned long long const threshold)
            : m_threshold{ threshold }
        {
            wchar_t mutex_name_buffer[BUFFER_SIZE];
            wchar_t event_name_buffer[BUFFER_SIZE];
            wchar_t mapping_name_buffer[BUFFER_SIZE];

            wcscpy_s(mutex_name_buffer, name);
            wcscpy_s(event_name_buffer, name);
            wcscpy_s(mapping_name_buffer, name);

            wcscat_s(mutex_name_buffer, L"mutex");
            wcscat_s(event_name_buffer, L"event");
            wcscat_s(mapping_name_buffer, L"mapping");

            m_mutex = null_handle
            {
                ::CreateMutexW(nullptr, FALSE, mutex_name_buffer)
            };

            // barrier should only be initialized once
            // creation of mutex implies that this thread also owns mutex
            auto const creator = ::GetLastError() != ERROR_ALREADY_EXISTS;

            m_event = null_handle 
            {
                ::CreateEventW(nullptr, TRUE, FALSE, event_name_buffer)
            };

            auto const size 
                = ULARGE_INTEGER{sizeof(unsigned long long), 0};

			m_mapping = null_handle
			{
				::CreateFileMappingW(
					INVALID_HANDLE_VALUE,
					nullptr,
					PAGE_READWRITE | SEC_COMMIT,
					size.HighPart, 
                    size.LowPart,
					mapping_name_buffer)
			};

            if (!m_mapping) throw win32_exception{};

			m_count = static_cast<unsigned long long*>(
				::MapViewOfFile(
					m_mapping.get(),
					FILE_MAP_READ | FILE_MAP_WRITE,
					0, 0, 
                    size.QuadPart));

            if (!m_count) throw win32_exception{};

            if (creator)
            {
                *m_count = 0;
            }
        }

        ~interprocess_barrier()
        {
            unmap();
        }

        interprocess_barrier(interprocess_barrier const&)            = delete;
        interprocess_barrier& operator=(interprocess_barrier const&) = delete;

        void enter()
        {               
            ::WaitForSingleObject(m_mutex.get(), INFINITE);
            
            ++(*m_count);
            while (*m_count < m_threshold)
            {
                ::SignalObjectAndWait(m_mutex.get(), m_event.get(), INFINITE, FALSE);
                ::WaitForSingleObject(m_mutex.get(), INFINITE);
            }
            ::PulseEvent(m_event.get());
            
            ::ReleaseMutex(m_mutex.get());
        }

    private:
		void unmap() noexcept
		{
            auto* begin = reinterpret_cast<char*>(m_count);

			if (begin)
			{
				::UnmapViewOfFile(begin);
			}
		}
    };
}