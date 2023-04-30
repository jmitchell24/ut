#pragma once

// https://codereview.stackexchange.com/a/176013
#if __cplusplus >= 201701L
#include <optional>
namespace ut {
template <typename T>
using optional = std::optional<T>;
using nullopt = std::nullopt;
} // namespace util
#else
#if __cplusplus >= 201300L
#include <experimental/optional>
namespace ut {
template <typename T>
using optional = std::experimental::optional<T>;
using nullopt_t = std::experimental::nullopt_t;
constexpr auto nullopt = std::experimental::nullopt;
} // namespace util
#else /* pre-C++14 - must be C++11 or earlier */

#error "C++11 and earlier not supported - you'll need to get your own implementation of std::optional (e.g. Boost's or https://github.com/akrzemi1/Optional)"
#endif /* __cplusplus >= 201402L */
#endif /* __cplusplus >= 201701L */
