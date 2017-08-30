#pragma once

#include <type_traits>

// In this file, we add support for MANY type of member function calls. For each of them, we create a wrapper that:
//
// - Call the "receive" method from emmagic_wrap_type on each argument
// - Call the "transmit" method from emmagic_wrap_type on the return value

template <typename T, T Fn, int Flags>
struct emmagic_wrap_member_fn;

// Member functions that don't return anything

template <typename T, typename ... Args, void (T::*Fn)(Args ...), int Flags>
struct emmagic_wrap_member_fn<void (T::*)(Args ...), Fn, Flags> {

    static void wrap(T & t, typename emmagic_wrap_type<typename std::decay<Args>::type, Flags>::WireType ... args)
    {
        return (t.*Fn)(emmagic_wrap_type<typename std::decay<Args>::type, Flags>::receive(args) ...);
    }

};

// Const member functions that don't return anything

template <typename T, typename ... Args, void (T::*Fn)(Args ...) const, int Flags>
struct emmagic_wrap_member_fn<void (T::*)(Args ...) const, Fn, Flags> {

    static void wrap(T const & t, typename emmagic_wrap_type<typename std::decay<Args>::type, Flags>::WireType ... args)
    {
        return (t.*Fn)(emmagic_wrap_type<typename std::decay<Args>::type, Flags>::receive(args) ...);
    }

};

// Member functions that return a single value

template <typename T, typename Ret, typename ... Args, Ret (T::*Fn)(Args ...), int Flags>
struct emmagic_wrap_member_fn<Ret (T::*)(Args ...), Fn, Flags> {

    static typename emmagic_wrap_type<typename std::decay<Ret>::type, Flags>::WireType wrap(T & t, typename emmagic_wrap_type<typename std::decay<Args>::type, Flags>::WireType ... args)
    {
        return emmagic_wrap_type<typename std::decay<Ret>::type, Flags>::transmit((t.*Fn)(emmagic_wrap_type<typename std::decay<Args>::type, Flags>::receive(args) ...));
    }

};

// Const member functions that return a single value

template <typename T, typename Ret, typename ... Args, Ret (T::*Fn)(Args ...) const, int Flags>
struct emmagic_wrap_member_fn<Ret (T::*)(Args ...) const, Fn, Flags> {

    static typename emmagic_wrap_type<typename std::decay<Ret>::type, Flags>::WireType wrap(T const & t, typename emmagic_wrap_type<typename std::decay<Args>::type, Flags>::WireType ... args)
    {
        return emmagic_wrap_type<typename std::decay<Ret>::type, Flags>::transmit((t.*Fn)(emmagic_wrap_type<typename std::decay<Args>::type, Flags>::receive(args) ...));
    }

};

// C-style member functions that don't return anything
// ex: list_add(list, ...)

template <typename T, typename ... Args, void (*Fn)(T &, Args ...), int Flags>
struct emmagic_wrap_member_fn<void (*)(T &, Args ...), Fn, Flags> {

    static void wrap(T & t, typename emmagic_wrap_type<typename std::decay<Args>::type, Flags>::WireType ... args)
    {
        return Fn(t, emmagic_wrap_type<typename std::decay<Args>::type, Flags>::receive(args) ...);
    }

};

// C-style const member functions that don't return anything
// ex: list_foreach(list, ...)

template <typename T, typename ... Args, void (*Fn)(T const &, Args ...), int Flags>
struct emmagic_wrap_member_fn<void (*)(T const &, Args ...), Fn, Flags> {

    static void wrap(T const & t, typename emmagic_wrap_type<typename std::decay<Args>::type, Flags>::WireType ... args)
    {
        return Fn(t, emmagic_wrap_type<typename std::decay<Args>::type, Flags>::receive(args) ...);
    }

};

// C-style member functions that return a single value
// ex: list_pop(list)

template <typename T, typename Ret, typename ... Args, Ret (*Fn)(T &, Args ...), int Flags>
struct emmagic_wrap_member_fn<Ret (*)(T &, Args ...), Fn, Flags> {

    static typename emmagic_wrap_type<typename std::decay<Ret>::type, Flags>::WireType wrap(T & t, typename emmagic_wrap_type<typename std::decay<Args>::type, Flags>::WireType ... args)
    {
        return emmagic_wrap_type<typename std::decay<Ret>::type, Flags>::transmit(Fn(t, emmagic_wrap_type<typename std::decay<Args>::type, Flags>::receive(args) ...));
    }

};

// C-style const member functions that return a single value
// ex: list_head(list)

template <typename T, typename Ret, typename ... Args, Ret (*Fn)(T const &, Args ...), int Flags>
struct emmagic_wrap_member_fn<Ret (*)(T const &, Args ...), Fn, Flags> {

    static typename emmagic_wrap_type<typename std::decay<Ret>::type, Flags>::WireType wrap(T const & t, typename emmagic_wrap_type<typename std::decay<Args>::type, Flags>::WireType ... args)
    {
        return emmagic_wrap_type<typename std::decay<Ret>::type, Flags>::transmit(Fn(t, emmagic_wrap_type<typename std::decay<Args>::type, Flags>::receive(args) ...));
    }

};
