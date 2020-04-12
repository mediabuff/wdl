// buffer.hpp
//
// Type Definitions
//  - wdl::crypto::mutable_buffer
//  - wdl::crypto::const_buffer
//
// Free Function Implementations
//  - wdl::crypto::buffer() 

#include <wdl/crypto/base.hpp>

namespace wdl::crypto
{
    // wdl::crypto::mutable_buffer

    class mutable_buffer
    {
        void*  m_data;
        size_t m_size;
    
    public:
        mutable_buffer() : m_data{nullptr}, m_size{0}
        {}

        mutable_buffer(void* data, size_t size)
            : m_data{data}, m_size{size}
        {}

        void* data() const noexcept
        {
            return m_data;
        }

        size_t size() const noexcept
        {
            return m_size;
        }
    };

    // wdl::crypto::const_buffer

    class const_buffer
    {
        void const*  m_data;
        size_t       m_size;

    public:
        const_buffer() : m_data{nullptr}, m_size{0}
        {}

        const_buffer(void const* data, size_t size)
            : m_data{data}, m_size{size}
        {}

        void const* data() const noexcept
        {
            return m_data;
        }

        size_t size() const noexcept
        {
            return m_size;
        }   
    };

    // wdl::crypto::buffer()
    //
    // initialize mutable buffer from pointer and size

    mutable_buffer buffer(void* data, size_t size_in_bytes) noexcept
    {
        return mutable_buffer(data, size_in_bytes);
    }

    // wdl::crypto::buffer()
    //
    // initialize const buffer from pointer and size

    const_buffer buffer(void const* data, size_t size_in_bytes) noexcept
    {
        return const_buffer(data, size_in_bytes);
    }

    // wdl::crypto::buffer()
    //
    // initialize mutable buffer from std::string

    template <typename Elem, typename Traits, typename Allocator>
    mutable_buffer buffer(
        std::basic_string<Elem, Traits, Allocator>& data) noexcept
    {
        return mutable_buffer(data.data(), data.size() * sizeof(Elem));
    }

    // wdl::crypto::buffer()
    //
    // initialize const buffer from std::string

    template <typename Elem, typename Traits, typename Allocator>
    const_buffer buffer(
        std::basic_string<Elem, Traits, Allocator> const& data) noexcept
    {
        return const_buffer(data.data(), data.size() * sizeof(Elem));
    }

    // wdl::crypto::buffer()
    //
    // initialize const buffer from std::vector

    template <typename PodType, typename Allocator>
    mutable_buffer buffer(
        std::vector<PodType, Allocator>& data) noexcept
    {
        return mutable_buffer(
            data.size() ? &data[0] : 0, 
            data.size() * sizeof(PodType));
    }

    // wdl::crypto::buffer()
    //
    // initialize const buffer from std::vector

    template <typename PodType, typename Allocator>
    const_buffer buffer(
        std::vector<PodType, Allocator> const& data) noexcept
    {
        return const_buffer(
            data.size() ? &data[0] : 0, 
            data.size() * sizeof(PodType));
    }
}