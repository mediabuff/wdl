// shared_memory.cpp
//
// Class Implementation: wdl::memory::shared_memory

#pragma once

#include <windows.h>
#include <wdl/handle/generic.hpp>

namespace wdl::memory
{
	// wdl::memory::shared_memory
	//
	// RAII wrapper around memory mapped file
    // back by system page file.

    enum class shared_memory_type
    {
        readonly,
        readwrite
    };

	class shared_memory
	{
		using null_handle = wdl::handle::null_handle;

        null_handle    m_mapping;

		char*          m_begin{};
		char*          m_end{};
		ULARGE_INTEGER m_size{};

	public:
		explicit shared_memory(
            wchar_t const*     name,
            PULARGE_INTEGER    size,
            shared_memory_type type = shared_memory_type::readwrite) noexcept
		{
            auto const readonly = type == shared_memory_type::readonly;
            
            auto const mapping_flags = readonly ? PAGE_READONLY : PAGE_READWRITE;
            auto const view_flags = readonly ? FILE_MAP_READ : (FILE_MAP_READ | FILE_MAP_WRITE);

			m_mapping = null_handle
			{
				::CreateFileMappingW(
					INVALID_HANDLE_VALUE,
					nullptr,
					mapping_flags | SEC_COMMIT,
					size->HighPart, 
                    size->LowPart,
					name)
			};

			if (!m_mapping) return;

			m_begin = static_cast<char*>(
				::MapViewOfFile(
					m_mapping.get(),
					view_flags,
					0, 0, 
                    size->QuadPart));

			if (!m_begin) return;

			m_end = m_begin + size->QuadPart;
			m_size.QuadPart = size->QuadPart;
		}

		~shared_memory()
		{
			unmap();
		}

		shared_memory(shared_memory const& other)          = delete;
		shared_memory& operator=(shared_memory const& rhs) = delete;

		shared_memory(shared_memory&& other) noexcept
			: m_begin{ other.m_begin }, m_end{ other.m_end }
		{
			other.m_begin = nullptr;
			other.m_end   = nullptr;
		}

		shared_memory& operator=(shared_memory&& rhs) noexcept
		{
			if (this != &rhs)
			{
				unmap();

				m_begin = rhs.m_begin;
				m_end   = rhs.m_end;

				rhs.m_begin = nullptr;
				rhs.m_end = nullptr;
			}

			return *this;
		}

		explicit operator bool() const noexcept
		{
			return m_begin != nullptr;
		}

		char* begin() noexcept
		{
			return m_begin;
		}

        char const* cbegin() const noexcept
        {
            return m_begin;
        }

		char* end() noexcept
		{
			return m_end;
		}

        char const* cend() const noexcept
        {
            return m_end;
        }

		ULARGE_INTEGER size() const noexcept
		{
			return m_size;
		}

	private:
		void unmap() noexcept
		{
			if (m_begin)
			{
				::UnmapViewOfFile(m_begin);
			}
		}
	};

	// wdl::memory::begin()

	char* begin(shared_memory& memory) noexcept
	{
		return memory.begin();
	}

    // wdl::memory::cbegin()

    char const* cbegin(shared_memory const& memory) noexcept
	{
		return memory.cbegin();
	}

	// wdl::memory::end()

	char* end(shared_memory& memory) noexcept
	{
		return memory.end();
	}

    // wdl::memory::cend()

    char const* cend(shared_memory const& memory) noexcept
	{
		return memory.cend();
	}
}