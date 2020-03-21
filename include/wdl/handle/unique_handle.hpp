// unique_handle.hpp
//
// Class Implementation: wdl::handle::unique_handle
//
// RAII wrapper class for raw Windows handles that 
// ensure automatic, safe resource management.

#pragma once

#include <windows.h>
#include <wdl/debug.hpp>

namespace wdl::handle
{
    // wdl::handle::unique_handle
    //
    // Resource management wrapper for raw Windows
    // handle values; semantics similar to C++ std::unique_ptr

    template <typename Traits>
    class unique_handle
    {
        using pointer = typename Traits::pointer;

    public:
        explicit unique_handle(pointer value = Traits::invalid()) noexcept
            : m_value(value)
        {
        }

        ~unique_handle() noexcept
        {
            close();
        }
        
        // non-copyable
        unique_handle(unique_handle const&)            = delete;
        unique_handle& operator=(unique_handle const&) = delete;

        unique_handle(unique_handle&& other) noexcept
            : m_value{ other.release() }
        {}

        unique_handle& operator=(unique_handle&& rhs) noexcept
        {
            reset(rhs.release());
            return *this;
        }

        explicit operator bool() const noexcept
        {
            return m_value != Traits::invalid();
        }

        bool is_valid() const noexcept
        {
            return m_value != Traits::invalid();
        }

        pointer* operator& () noexcept
        {
            close();
            m_value = Traits::invalid();
            return &m_value;
        }

        pointer get() const noexcept
        {
            return m_value;
        }

        pointer* put() noexcept
        {
            close();
            m_value = Traits::invalid();
            return &m_value;
        }

        pointer release() noexcept
        {
            auto value = m_value;
            m_value = Traits::invalid();
            return value;
        }

        bool reset(pointer value = Traits::invalid()) noexcept
        {
            if (m_value != value)
            {
                close();
                m_value = value;
            }

            return static_cast<bool>(*this);
        }

        pointer* addressof() const noexcept
        {
            return &m_value;
        }

        pointer* get_address_of() noexcept
        {
            return &m_value;
        }

        void swap(unique_handle<Traits>& other) noexcept
        {
            std::swap(m_value, other.m_value);
        }

    private:
        pointer m_value;

        auto close() noexcept -> void
        {
            if (*this)
            {
                Traits::close(m_value);
            }
        }
    };
}
