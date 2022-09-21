#pragma once

#include "ut/string/view.hpp"

namespace ut
{
    template <typename Char, Char C>
    inline static constexpr bool delimUnary(Char c)
    { return C == c; }

    template <bool Trim, typename Char, typename Traits>
    struct basic_strview_split
    {
        using char_type         = Char;
        using traits_type       = Traits;
        using stringview_type   = basic_strview<Char, Traits>;
        using size_type         = typename stringview_type::size_type;
        using pointer_type      = typename stringview_type::pointer_type;
        using container_type    = std::vector<basic_strview<char_type>>;

        template <typename Unary>
        inline constexpr static bool is_unary_delim_v =
                std::is_invocable_r_v<bool, Unary, char_type>;

        template <typename Callable>
        inline constexpr static bool is_callback_v =
                std::is_invocable_r_v<bool, Callable, stringview_type>;

        template <typename T>
        inline constexpr static bool is_tieable_v =
                std::is_same_v<T, stringview_type> ||
                std::is_same_v<T, decltype(std::ignore)>;

        template <typename Callable, typename Unary>
        inline static void split(stringview_type const& sv, Callable&& callback, Unary&& unary_delim)
        {
            static_assert (is_unary_delim_v<Unary>, "Unary must accept 1 parameter of char_type, return value must cast to bool");
            static_assert (is_callback_v<Callable>, "Callable must accept 1 parameter of view_type, return value must cast to bool");

            if (sv.empty())
                return;

            pointer_type token_begin = sv.begin();
            for (pointer_type i = token_begin; i != sv.end(); ++i)
            {
                if (static_cast<bool>(unary_delim(*i)))
                {
                    if constexpr(Trim)
                    {
                        stringview_type tok = sv.with(token_begin, i);
                        if (!tok.empty())
                        {
                            if (!callback(tok))
                                return;
                        }
                    }
                    else
                    {
                        if (!callback(sv.with(token_begin, i)))
                            return;
                    }

                    token_begin = i+1;
                }
            }

            if constexpr(Trim)
            {
                stringview_type tok = sv.withBegin(token_begin);
                if (!tok.empty())
                    callback(tok);
            }
            else
            {
                callback(sv.withBegin(token_begin));
            }
        }


        template <typename Callable>
        inline static void splitWS(stringview_type const& sv, Callable&& callback)
        { return split(sv, callback, &std::iswspace); }

        template <typename Callable>
        inline static void splitLn(stringview_type const& sv, Callable&& callback)
        { return split(sv, callback, delimUnary<Char, '\n'>); }

        template <typename Callable>
        inline static void splitComma(stringview_type const& sv, Callable&& callback)
        { return split(sv, callback, delimUnary<Char, ','>); }

        template <typename Unary>
        inline static container_type container(stringview_type const& sv, Unary&& unary_delim)
        {
            static_assert (is_unary_delim_v<Unary>, "Unary must accept 1 parameter of char_type, return value must cast to bool");

            container_type tokens;
            split(
                sv,
                [&tokens](auto&& s){ tokens.emplace_back(s); return true; },
                unary_delim
            );
            return tokens;
        }

        inline static container_type containerWS(stringview_type const& sv)
        { return container(sv, &std::iswspace); }

        inline static container_type containerLn(stringview_type const& sv)
        { return container(sv, delimUnary<Char, '\n'>); }

        inline static container_type containerComma(stringview_type const& sv)
        { return container(sv, delimUnary<Char, ','>); }

        template <typename Unary, typename... Params>
        inline static size_type tie(stringview_type const& sv, Unary&& unary_delim, Params&... params)
        {
            static_assert (is_unary_delim_v<Unary>, "Unary must accept 1 parameter of char_type, return value must cast to bool");
            static_assert (sizeof...(Params) > 0, "Non-zero number of out parameters are required");

            if (sv.empty())
                return 0;

            return tie_impl<1, Unary, Params...>(sv, sv.begin(), unary_delim, params...);
        }

        template <typename... Params>
        inline static size_type tieWS(stringview_type const& sv, Params&... params)
        { return tie(sv, &std::iswspace, params...); }

        template <typename... Params>
        inline static size_type tieLn(stringview_type const& sv, Params&... params)
        { return tie(sv, delimUnary<Char, '\n'>, params...); }

        template <typename... Params>
        inline static size_type tieComma(stringview_type const& sv, Params&... params)
        { return tie(sv, delimUnary<Char, ','>, params...); }

    private:

        template <size_type I, typename U, typename T>
        inline static size_type tie_impl(stringview_type const& sv, pointer_type token_begin, U unary_delim, T& out)
        {
            static_assert (is_tieable_v<T>, "out parameter must be strview or std::ignore");

            for (pointer_type i = token_begin; i != sv.end(); ++i)
            {
                if (static_cast<bool>(unary_delim(*i)))
                {
                    if constexpr(Trim)
                    {
                        stringview_type tok = sv.with(token_begin, i);
                        if (!tok.empty())
                        {
                            out = tok;
                            return I;
                        }

                        token_begin = i+1;
                    }
                    else
                    {
                        out = sv.with(token_begin, i);
                        return I;
                    }
                }
            }

            if constexpr(Trim)
            {
                stringview_type tok = sv.withBegin(token_begin);
                if (!tok.empty())
                {
                    out = tok;
                    return I;
                }
                return 0;
            }
            else
            {
                out = sv.withBegin(token_begin);
                return I;
            }
        }

        template <size_type I, typename U, typename T, typename... TT>
        inline static size_type tie_impl(stringview_type const& sv, pointer_type token_begin, U unary_delim, T& out, TT&... tt)
        {
            static_assert (is_tieable_v<T>, "out parameter must be strview or std::ignore");

            for (pointer_type i = token_begin; i != sv.end(); ++i)
            {
                if (static_cast<bool>(unary_delim(*i)))
                {
                    if constexpr(Trim)
                    {
                        stringview_type tok = sv.with(token_begin, i);
                        if (!tok.empty())
                        {
                            out = tok;
                            return tie_impl<I+1,U>(sv, i+1, unary_delim, tt...);
                        }

                        token_begin = i+1;
                    }
                    else
                    {
                        out = sv.with(token_begin, i);
                        return tie_impl<I+1,U>(sv, i+1, unary_delim, tt...);
                    }
                }
            }

            if constexpr(Trim)
            {
                stringview_type tok = sv.withBegin(token_begin);
                if (!tok.empty())
                {
                    out = tok;
                    return I;
                }
                return 0;
            }
            else
            {
                out = sv.withBegin(token_begin);
                return I;
            }
        }
    };

    using split     = basic_strview_split<false, char, std::char_traits<char>>;
    using trimsplit = basic_strview_split<true, char, std::char_traits<char>>;

}