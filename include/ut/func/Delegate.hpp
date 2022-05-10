// Copyright 2013-2021, James Mitchell, All rights reserved.

#pragma once

#include <cstddef>
#include <cstring>
#include <utility>
#include <memory>

#define M_DECL_PURE             [[nodiscard]] inline
#define M_DECL                  inline

namespace ut
{
    template <typename Return, typename... Params> class func;

namespace internal
{
    class AnyClass;
    using AnyMemFunc = void(AnyClass::*)();

    std::size_t constexpr ANY_MEMBER_FUNCTION_SIZE = sizeof(AnyMemFunc);

    template<typename A, typename B>
    inline A evil_cast(B b) noexcept
    {
        static_assert(sizeof(B) == sizeof(A), "Cannot use evil_cast");
        union { B b; A a; } conv{b};
        return conv.a;
    }

    template<typename XMemFunc, typename AnyMemFunc>
    inline static void generalize(XMemFunc x_memfunc, AnyMemFunc& any_memfunc) noexcept
    {
        static_assert(sizeof(XMemFunc) == ANY_MEMBER_FUNCTION_SIZE, "Unsupported member function pointer on this compiler");
        any_memfunc = evil_cast<AnyMemFunc>(x_memfunc);
    }

    template<typename X, typename XMemFunc, typename AnyMemFunc>
    inline static void generalize(X const volatile* x_object, XMemFunc x_memfunc, AnyClass*& any_object, AnyMemFunc& any_memfunc) noexcept
    {
        static_assert(sizeof(XMemFunc) == ANY_MEMBER_FUNCTION_SIZE, "Unsupported member function pointer on this compiler");
        any_object  = reinterpret_cast<AnyClass*>(const_cast<X*>(x_object));
        any_memfunc = evil_cast<AnyMemFunc>(x_memfunc);
    }

    struct closure_token
    {
        AnyMemFunc function;
        AnyClass*  object;

        inline static bool same(AnyClass* a_o, AnyMemFunc a_f, AnyClass* b_o, AnyMemFunc b_f)
        {
            return a_o == b_o && a_f == b_f;
        }

        inline static bool less(AnyClass* a_o, AnyMemFunc a_f, AnyClass* b_o, AnyMemFunc b_f)
        {
            return a_o != b_o ? a_o < b_o : std::memcmp(&a_f, &b_f, sizeof(AnyMemFunc)) < 0;
        }

        inline static bool empty(AnyClass* a_o, AnyMemFunc a_f)
        {
            return a_o == nullptr && a_f == nullptr;
        }

        void clear()
        {
            function = nullptr;
            object    = nullptr;
        }

        inline bool same (closure_token const& m) const noexcept { return same (object, function, m.object, m.function); }
        inline bool less (closure_token const& m) const noexcept { return less (object, function, m.object, m.function); }

        inline bool empty() const { return empty(object, function); }

        inline bool operator== (closure_token const& t) const { return same(t); }
        inline bool operator!= (closure_token const& t) const { return !(*this == t); }
        inline bool operator<  (closure_token const& t) const { return less (t); }
        inline bool operator>  (closure_token const& t) const { return t < *this; }
        inline bool operator<= (closure_token const& t) const { return !(*this > t); }
        inline bool operator>= (closure_token const& t) const { return !(*this < t); }
    };

    template<class TAnyMemFunc, class TStaticFunc> struct closure
    {
        union
        {
            AnyMemFunc  function;
            TAnyMemFunc member_function;
        };

        AnyClass* object;

        inline closure() noexcept
            : function(nullptr), object(nullptr)
        {}

        inline closure(closure const&) noexcept =default;
        inline closure& operator= (closure const&) noexcept =default;

        inline void clear() noexcept { object = nullptr; function = nullptr; }

        //inline bool equal(TStaticFunc const s) const noexcept { return s == nullptr ? empty() : s == reinterpret_cast<TStaticFunc>(this); }
        inline bool equal(closure       const& m) const noexcept { return closure_token::same (object, function, m.object, m.function); } //{ return object == m.object && function == m.function; }
        inline bool less (closure       const& m) const noexcept { return closure_token::less (object, function, m.object, m.function); } //{ return object != m.object ? object < m.object : std::memcmp(&function, &m.function, sizeof(function)) < 0; }
        inline bool equal(closure_token const& m) const noexcept { return closure_token::same (object, function, m.object, m.function); }
        inline bool less (closure_token const& m) const noexcept { return closure_token::less (object, function, m.object, m.function); }
        inline bool empty()                       const noexcept { return closure_token::empty(object, function); }

        template<class InvokerMemFunc>
        inline bool hasInvoker(InvokerMemFunc invoker) const
        {
            AnyMemFunc tmp;
            generalize(invoker, tmp);
            return tmp == function;
        }

        inline closure_token token() const { return { function, object }; }

        template<class X, class XMemFunc> inline void bindMemberFunction(X* x, XMemFunc x_memfunc) noexcept
        {
            generalize(x, x_memfunc, object, function);
        }

        template<class InvokerMemFunc> inline void bindStaticFunction(InvokerMemFunc invoker, TStaticFunc function) noexcept
        {
            if(function == nullptr)
                this->function = nullptr;
            else
                generalize(invoker, this->function);
            object = evil_cast<AnyClass*>(function);
        }
    };
}

    using func_token = internal::closure_token;


    template <typename Return, typename... Params> class func<Return(Params...)>
    {
    public:
        template <typename T>
        using member_function_type = Return(T::*)(Params...);
        using static_function_type = Return(*)(Params...);
        using return_type          = Return;

        M_DECL_PURE bool operator== (std::nullptr_t const) const { return  empty(); }
        M_DECL_PURE bool operator!= (std::nullptr_t const) const { return !empty(); }

        M_DECL_PURE bool operator== (func const& d) const { return m_closure.equal(d.m_closure); }
        M_DECL_PURE bool operator!= (func const& d) const { return !(*this == d); }
        M_DECL_PURE bool operator<  (func const& d) const { return m_closure.less (d.m_closure); }
        M_DECL_PURE bool operator>  (func const& d) const { return d < *this; }
        M_DECL_PURE bool operator<= (func const& d) const { return !(*this > d); }
        M_DECL_PURE bool operator>= (func const& d) const { return !(*this < d); }

        M_DECL_PURE bool operator== (func_token const& t) const { return m_closure.equal(t); }
        M_DECL_PURE bool operator!= (func_token const& t) const { return !(*this == t); }
        M_DECL_PURE bool operator<  (func_token const& t) const { return m_closure.less (t); }
        M_DECL_PURE bool operator>  (func_token const& t) const { return t < *this; }
        M_DECL_PURE bool operator<= (func_token const& t) const { return !(*this > t); }
        M_DECL_PURE bool operator>= (func_token const& t) const { return !(*this < t); }

        M_DECL_PURE operator func_token() const { return token(); }
        M_DECL_PURE operator bool() const { return !empty();     }

        M_DECL_PURE Return operator() (Params... p) const { return (m_closure.object->*(m_closure.member_function))(p...); }

        M_DECL_PURE bool empty() const { return m_closure.empty(); }
        M_DECL_PURE bool isClosure() const { return !m_closure.hasInvoker(&func::invokeStaticFunction); }

        M_DECL_PURE func_token token() const { return m_closure.token(); }

        M_DECL void clear() { m_closure.clear(); m_storage.reset(); }

        M_DECL void bind(                       ) { clear(); }
        M_DECL void bind(std::nullptr_t const   ) { clear(); }
        M_DECL void bind(func       const& d) { m_storage = d.m_storage; m_closure = d.m_closure; }
        M_DECL void bind(func            & d) { bind((func const&)d); }

        M_DECL void bind(Return(*function)(Params... p)) { clear(); m_closure.bindStaticFunction(&func::invokeStaticFunction, function); }

        template <typename X, typename Y> M_DECL void bind(Y *               y, Return(X::*member)(Params...)               ) { bind_p   (y, member); }
        template <typename X, typename Y> M_DECL void bind(Y const*          y, Return(X::*member)(Params...) const         ) { bind_p_c (y, member); }
        template <typename X, typename Y> M_DECL void bind(Y volatile*       y, Return(X::*member)(Params...) volatile      ) { bind_p_v (y, member); }
        template <typename X, typename Y> M_DECL void bind(Y const volatile* y, Return(X::*member)(Params...) const volatile) { bind_p_cv(y, member); }

        template <typename Y> M_DECL void bind(Y *               y) { bind_p   (y, &Y::operator()); }
        template <typename Y> M_DECL void bind(Y const*          y) { bind_p_c (y, &Y::operator()); }
        template <typename Y> M_DECL void bind(Y volatile*       y) { bind_p_v (y, &Y::operator()); }
        template <typename Y> M_DECL void bind(Y const volatile* y) { bind_p_cv(y, &Y::operator()); }

#define ENABLE_IF_NOT_POINTER typename std::enable_if<!std::is_pointer<Y>::value,void>::type
        template <typename X, typename Y> ENABLE_IF_NOT_POINTER M_DECL bind(Y &               y, Return(X::*member)(Params...)               ) { bind_p   (y, member); }
        template <typename X, typename Y> ENABLE_IF_NOT_POINTER M_DECL bind(Y const&          y, Return(X::*member)(Params...) const         ) { bind_p_c (y, member); }
        template <typename X, typename Y> ENABLE_IF_NOT_POINTER M_DECL bind(Y volatile&       y, Return(X::*member)(Params...) volatile      ) { bind_p_v (y, member); }
        template <typename X, typename Y> ENABLE_IF_NOT_POINTER M_DECL bind(Y const volatile& y, Return(X::*member)(Params...) const volatile) { bind_p_cv(y, member); }

        template <typename Y> ENABLE_IF_NOT_POINTER M_DECL bind(Y &               y) { bind_p   (y, &Y::operator()); }
        template <typename Y> ENABLE_IF_NOT_POINTER M_DECL bind(Y const&          y) { bind_p_c (y, &Y::operator()); }
        template <typename Y> ENABLE_IF_NOT_POINTER M_DECL bind(Y volatile&       y) { bind_p_v (y, &Y::operator()); }
        template <typename Y> ENABLE_IF_NOT_POINTER M_DECL bind(Y const volatile& y) { bind_p_cv(y, &Y::operator()); }
#undef ENABLE_IF_NOT_POINTER

    private:
        using closure_type = internal::closure<Return(internal::AnyClass::*)(Params...), Return(*)(Params...)>;
        using storage_type = std::shared_ptr<void>;

        closure_type m_closure;
        storage_type m_storage;

        template<typename X> static void destroyStorage(void* p) { delete reinterpret_cast<X*>(p); }

        template <typename X> X* setStorage(X& x)
        {
            clear();
            m_storage.reset(operator new(sizeof(X)), destroyStorage<X>);
            return new (m_storage.get()) X(x);
        }

        template <typename X> X const*          setStorage(X const& x)          { return setStorage((X&)x); }
        template <typename X> X volatile*       setStorage(X volatile& x)       { return setStorage((X&)x); }
        template <typename X> X const volatile* setStorage(X const volatile& x) { return setStorage((X&)x); }

        template <typename X, typename Y> inline void bind_p   (Y *               y, Return(X::*member)(Params...)               ) { clear(); m_closure.bindMemberFunction(static_cast<X *>              (y), member); }
        template <typename X, typename Y> inline void bind_p_c (Y const*          y, Return(X::*member)(Params...) const         ) { clear(); m_closure.bindMemberFunction(static_cast<X const*>         (y), member); }
        template <typename X, typename Y> inline void bind_p_v (Y volatile*       y, Return(X::*member)(Params...) volatile      ) { clear(); m_closure.bindMemberFunction(static_cast<X volatile*>      (y), member); }
        template <typename X, typename Y> inline void bind_p_cv(Y const volatile* y, Return(X::*member)(Params...) const volatile) { clear(); m_closure.bindMemberFunction(static_cast<X const volatile*>(y), member); }

        template <typename X, typename Y> inline void bind_p   (Y &               y, Return(X::*member)(Params...)               ) { clear(); m_closure.bindMemberFunction(static_cast<X *>              (setStorage(y)), member); }
        template <typename X, typename Y> inline void bind_p_c (Y const&          y, Return(X::*member)(Params...) const         ) { clear(); m_closure.bindMemberFunction(static_cast<X const*>         (setStorage(y)), member); }
        template <typename X, typename Y> inline void bind_p_v (Y volatile&       y, Return(X::*member)(Params...) volatile      ) { clear(); m_closure.bindMemberFunction(static_cast<X volatile*>      (setStorage(y)), member); }
        template <typename X, typename Y> inline void bind_p_cv(Y const volatile& y, Return(X::*member)(Params...) const volatile) { clear(); m_closure.bindMemberFunction(static_cast<X const volatile*>(setStorage(y)), member); }

        /* Explaination:
         * This is a very strange (and brilliant) hack concocted by Don Clugston, from which this Delegate is based.
         * The delegate needs an object and a member function pointer to call
         * To make a static function call within this paradigm, you give the delegate
         * the delegate's own member function (invokeStaticFunction) and provide the actual static function pointer
         * as the 'this' pointer
         * so invokeStaticFunction is never called with a real this pointer, but a static function pointer
         * which gets cast and then called from within
         *
         * This only works when data pointers are the same size as function pointers
         */
        Return invokeStaticFunction(Params... p) const { return (*internal::evil_cast<static_function_type>(this))(p...); }
    };

#define DELEGATE                func<R(P...)>
#define MEMBER_FUNC             R (Y::* y)(P...)
#define RETURN(...)             DELEGATE d; d.bind(__VA_ARGS__); return d

namespace internal
{
    template <typename X, typename Y, typename R, typename... P> inline DELEGATE fn_p   (X *               x, MEMBER_FUNC               ) { RETURN(x, y); }
    template <typename X, typename Y, typename R, typename... P> inline DELEGATE fn_p_c (X const*          x, MEMBER_FUNC const         ) { RETURN(x, y); }
    template <typename X, typename Y, typename R, typename... P> inline DELEGATE fn_p_v (X volatile*       x, MEMBER_FUNC volatile      ) { RETURN(x, y); }
    template <typename X, typename Y, typename R, typename... P> inline DELEGATE fn_p_cv(X const volatile* x, MEMBER_FUNC const volatile) { RETURN(x, y); }

    template <typename X, typename Y, typename R, typename... P> inline DELEGATE fn_r   (X &               x, MEMBER_FUNC               ) { RETURN(x, y); }
    template <typename X, typename Y, typename R, typename... P> inline DELEGATE fn_r_c (X const&          x, MEMBER_FUNC const         ) { RETURN(x, y); }
    template <typename X, typename Y, typename R, typename... P> inline DELEGATE fn_r_v (X volatile&       x, MEMBER_FUNC volatile      ) { RETURN(x, y); }
    template <typename X, typename Y, typename R, typename... P> inline DELEGATE fn_r_cv(X const volatile& x, MEMBER_FUNC const volatile) { RETURN(x, y); }

}
    template <typename R, typename... P> inline DELEGATE &               fn(DELEGATE&                d) { return d; }
    template <typename R, typename... P> inline DELEGATE const&          fn(DELEGATE const&          d) { return d; }
    template <typename R, typename... P> inline DELEGATE volatile&       fn(DELEGATE volatile&       d) { return d; }
    template <typename R, typename... P> inline DELEGATE const volatile& fn(DELEGATE const volatile& d) { return d; }

    template <typename R, typename... P> inline DELEGATE *               fn(DELEGATE*                d) { return d; }
    template <typename R, typename... P> inline DELEGATE const*          fn(DELEGATE const*          d) { return d; }
    template <typename R, typename... P> inline DELEGATE volatile*       fn(DELEGATE volatile*       d) { return d; }
    template <typename R, typename... P> inline DELEGATE const volatile* fn(DELEGATE const volatile* d) { return d; }

    template <typename R, typename... P> inline DELEGATE fn(R (*function)(P...) ) { RETURN(function); }

    template <typename X, typename Y, typename R, typename... P> inline DELEGATE fn(X *               x, MEMBER_FUNC               ) { return internal::fn_p   (x, y); }
    template <typename X, typename Y, typename R, typename... P> inline DELEGATE fn(X const*          x, MEMBER_FUNC const         ) { return internal::fn_p_c (x, y); }
    template <typename X, typename Y, typename R, typename... P> inline DELEGATE fn(X volatile*       x, MEMBER_FUNC volatile      ) { return internal::fn_p_v (x, y); }
    template <typename X, typename Y, typename R, typename... P> inline DELEGATE fn(X const volatile* x, MEMBER_FUNC const volatile) { return internal::fn_p_cv(x, y); }

    template <typename X, typename Y, typename R, typename... P> inline DELEGATE fn(X &               x, MEMBER_FUNC               ) { return internal::fn_r   (x, y); }
    template <typename X, typename Y, typename R, typename... P> inline DELEGATE fn(X const&          x, MEMBER_FUNC const         ) { return internal::fn_r_c (x, y); }
    template <typename X, typename Y, typename R, typename... P> inline DELEGATE fn(X volatile&       x, MEMBER_FUNC volatile      ) { return internal::fn_r_v (x, y); }
    template <typename X, typename Y, typename R, typename... P> inline DELEGATE fn(X const volatile& x, MEMBER_FUNC const volatile) { return internal::fn_r_cv(x, y); }

#undef RETURN_TYPE
#undef MEMBER_FUNC
#undef RETURN

    template <typename X> inline auto fn(X *               x) { return internal::fn_p   (x, &X::operator()); }
    template <typename X> inline auto fn(X const*          x) { return internal::fn_p_c (x, &X::operator()); }
    template <typename X> inline auto fn(X volatile*       x) { return internal::fn_p_v (x, &X::operator()); }
    template <typename X> inline auto fn(X const volatile* x) { return internal::fn_p_cv(x, &X::operator()); }

    template <typename X> inline auto fn(X &               x) { return internal::fn_r   (x, &X::operator()); }
    template <typename X> inline auto fn(X const&          x) { return internal::fn_r_c (x, &X::operator()); }
    template <typename X> inline auto fn(X volatile&       x) { return internal::fn_r_v (x, &X::operator()); }
    template <typename X> inline auto fn(X const volatile& x) { return internal::fn_r_cv(x, &X::operator()); }
}

#undef M_DECL_PURE
#undef M_DECL
