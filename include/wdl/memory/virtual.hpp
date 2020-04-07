// virtual.hpp
//
// Wrappers for virtual API memory allocation functions.

#pragma once

#include <windows.h>

namespace wdl::memory
{
    enum class region_type
    {
        reserve, 
        commit
    };

    class region
    {
        region_type m_type;
        SIZE_T      m_size;
        void*       m_handle;

    public:
        region(
            region_type   type, 
            SIZE_T        size,
            unsigned long protect
            ) : m_type{ type }, m_size{size}
        {
            auto flags = MEM_RESERVE;
            flags = flags | (type == region_type::commit ? MEM_COMMIT : 0);

            m_handle = ::VirtualAlloc(nullptr, m_size, flags, protect);
        }

        ~region()
        {
            release();
        }

        region(region const&)            = delete;
        region& operator=(region const&) = delete;

        region(region&& other)
            : m_type{ other.m_type }
            , m_size{ other.m_size }
            , m_handle{ other.m_handle }
        {
            other.m_handle = nullptr;
        }

        region& operator=(region&& rhs)
        {
            if (this != &rhs)
            {   
                release();
                m_type = rhs.m_type;
                m_size = rhs.m_size;
                m_handle = rhs.m_handle;
                rhs.m_handle = nullptr;
            }

            return *this;
        }

        void* get_base() const noexcept
        {
            return m_handle;
        }

        SIZE_T get_size() const noexcept
        {
            return m_size;
        }

        bool is_valid() const noexcept
        {
            return m_handle != nullptr;
        }

        explicit operator bool() const noexcept
        {
            return m_handle != nullptr;
        }

        static region commit(SIZE_T size, unsigned long protect);
        static region reserve(SIZE_T size, unsigned long protect);

    private:
        void release()
        {
            if (m_handle)
            {
                ::VirtualFree(m_handle, 0, MEM_RELEASE);
            }
        }
    };

    region region::commit(SIZE_T size, unsigned long protect)
    {
        return region{region_type::commit, size, protect};
    }

    region region::reserve(SIZE_T size, unsigned long protect)
    {
        return region{region_type::reserve, size, protect};
    }
}