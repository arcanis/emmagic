#pragma once

#include <type_traits>

// In this file, we add support for every type of free function calls. For each of them, we create a wrapper that:
//
// - Call the "receive" method from emmagic_wrap_type on each argument
// - Call the "transmit" method from emmagic_wrap_type on the return value
//
// Note that we don't implement C-style member function calls here, because they are implemented in wrap_member_fn instead.

template <typename T, T fn, int Flags>
struct emmagic_wrap_free_fn;

// Free functions that don't return anything

template <typename ... Args, void (*fn)(Args ...), int Flags>
struct emmagic_wrap_free_fn<void (*)(Args ...), fn, Flags> {

    static void wrap(typename emmagic_wrap_type<typename std::decay<Args>::type, Flags>::WireType ... args)
    {
        return fn(emmagic_wrap_type<typename std::decay<Args>::type, Flags>::receive(args) ...);
    }

};

// Free functions that return a single value

template <typename Ret, typename ... Args, Ret (*fn)(Args ...), int Flags>
struct emmagic_wrap_free_fn<Ret (*)(Args ...), fn, Flags> {

    static typename emmagic_wrap_type<typename std::decay<Ret>::type, Flags>::WireType wrap(typename emmagic_wrap_type<typename std::decay<Args>::type, Flags>::WireType ... args)
    {
        return emmagic_wrap_type<typename std::decay<Ret>::type, Flags>::transmit(fn(emmagic_wrap_type<typename std::decay<Args>::type, Flags>::receive(args) ...));
    }

};
