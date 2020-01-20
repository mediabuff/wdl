// io.h
// Input / output utilties.

#pragma once

#include <windows.h>
#include "unique_handle.h"

namespace wdl
{
	// mapped_file
	// RAII wrapper around memory mapped file.
	// Mapped for readonly access.

	class mapped_file
	{
	public:
		explicit mapped_file(const wchar_t* filename) noexcept
		{
			auto file = invalid_handle
			{
				::CreateFileW(
					filename,
					GENERIC_READ,
					FILE_SHARE_READ,
					nullptr,
					OPEN_EXISTING,
					FILE_ATTRIBUTE_NORMAL,
					nullptr
					)
			};

			if (!file) return;

			auto mapping = null_handle
			{
				::CreateFileMappingW(
					file.get(),
					nullptr,
					PAGE_READONLY,
					0, 0,
					nullptr
					)
			};

			if (!mapping) return;

			auto size = LARGE_INTEGER{};
			if (!::GetFileSizeEx(file.get(), &size)) return;

			m_begin = static_cast<const char*>(
				::MapViewOfFile(
					mapping.get(),
					FILE_MAP_READ,
					0, 0,
					0
				));

			if (!m_begin) return;

			m_end  = m_begin + size.QuadPart;
			m_size = size;
		}

		~mapped_file()
		{
			unmap();
		}

		mapped_file(mapped_file&& other) noexcept
			: m_begin{ other.m_begin }, m_end{ other.m_end }
		{
			other.m_begin = nullptr;
			other.m_end = nullptr;
		}

		mapped_file& operator=(mapped_file&& rhs) noexcept
		{
			if (this != &rhs)
			{
				unmap();

				m_begin = rhs.m_begin;
				m_end = rhs.m_end;

				rhs.m_begin = nullptr;
				rhs.m_end = nullptr;
			}

			return *this;
		}

		explicit operator bool() const noexcept
		{
			return m_begin != nullptr;
		}

		const char* begin() const noexcept
		{
			return m_begin;
		}

		const char* end() const noexcept
		{
			return m_end;
		}

		LARGE_INTEGER size() const noexcept
		{
			return m_size;
		}

		mapped_file(const mapped_file& other) = delete;
		mapped_file& operator=(const mapped_file& rhs) = delete;

	private:
		void unmap() noexcept
		{
			if (m_begin)
			{
				::UnmapViewOfFile(m_begin);
			}
		}

		const char*   m_begin{};
		const char*   m_end{};
		LARGE_INTEGER m_size{};
	};

	// begin
	// Free iterator function.

	const char* begin(const mapped_file& file) noexcept
	{
		return file.begin();
	}

	// end
	// Free iterator function.

	const char* end(const mapped_file& file) noexcept
	{
		return file.end();
	}
}

