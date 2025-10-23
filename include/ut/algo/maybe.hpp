//
// Created by james on 23/10/25.
//

#pragma once

#include <optional>
#include <iterator>

namespace ut
{
    // Iterator wrapper for optional
    template<typename T> class optional_iterator
    {
        T* ptr;

    public:
        using iterator_category = std::random_access_iterator_tag;
        using value_type        = T;
        using difference_type   = std::ptrdiff_t;
        using pointer           = T*;
        using reference         = T&;

        constexpr optional_iterator(T* p = nullptr) : ptr(p) {}

        constexpr reference operator*() const { return *ptr; }
        constexpr pointer operator->() const { return ptr; }

        constexpr optional_iterator& operator++() { ptr = nullptr; return *this; }
        constexpr optional_iterator operator++(int) { auto tmp = *this; ++(*this); return tmp; }

        constexpr bool operator==(optional_iterator const& other) const { return ptr == other.ptr; }
        constexpr bool operator!=(optional_iterator const& other) const { return ptr != other.ptr; }
    };

    // Const iterator wrapper
    template<typename T> class optional_const_iterator
    {
        T const* ptr;

    public:
        using iterator_category = std::random_access_iterator_tag;
        using value_type        = T;
        using difference_type   = std::ptrdiff_t;
        using pointer           = T const*;
        using reference         = T const&;

        constexpr optional_const_iterator(T const* p = nullptr) : ptr(p) {}

        constexpr reference operator*() const { return *ptr; }
        constexpr pointer operator->() const { return ptr; }

        constexpr optional_const_iterator& operator++() { ptr = nullptr; return *this; }
        constexpr optional_const_iterator operator++(int) { auto tmp = *this; ++(*this); return tmp; }

        constexpr bool operator==(const optional_const_iterator& other) const { return ptr == other.ptr; }
        constexpr bool operator!=(const optional_const_iterator& other) const { return ptr != other.ptr; }
    };

    template <typename T>
    struct maybe_ptr
    {
        T* ptr=nullptr;

        auto begin(){ return optional_iterator<T>(ptr); }
        auto end() { return optional_iterator<T>(nullptr); }

        auto begin() const{ return optional_iterator<T>(ptr); }
        auto end() const { return optional_iterator<T>(nullptr); }
    };
}


// Only define these if C++26 optional range support isn't available
#if __cplusplus < 202600L && !defined(__cpp_lib_optional_range_support)

namespace std
{
    // Non-member begin/end functions for std::optional
    template<typename T>
    constexpr auto begin(std::optional<T>& opt)
    { return ut::optional_iterator<T>(opt ? &*opt : nullptr); }

    template<typename T>
    constexpr auto end(std::optional<T>& opt)
    { return ut::optional_iterator<T>(nullptr); }

    template<typename T>
    constexpr auto begin(const std::optional<T>& opt)
    { return ut::optional_const_iterator<T>(opt ? &*opt : nullptr); }

    template<typename T>
    constexpr auto end(const std::optional<T>& opt)
    { return ut::optional_const_iterator<T>(nullptr); }
}

#endif // __cplusplus < 202600L && !defined(__cpp_lib_optional_range_support)