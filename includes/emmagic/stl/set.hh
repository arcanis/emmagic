#pragma once

#ifndef EMMAGIC_READY
# error "Please include 'emmagic/magic.hh' before this file"
#endif

#include <set>
#include <type_traits>

template <typename ValueType, int Flags>
struct emmagic_wrap_type_set_base : public emmagic_wrap_type_base<std::set<ValueType>, emscripten::val> {

    static std::set<ValueType> receive(emscripten::val const & value)
    {
        std::set<ValueType> set;

        for (auto t = 0u, T = value["length"].as<unsigned>(); t < T; ++t)
            set.insert(emmagic_receive<ValueType, Flags>(value[t]));

        return set;
    }

};

template <typename ValueType, int Flags, typename = void>
struct emmagic_wrap_type_set : public emmagic_wrap_type_set_base<ValueType, Flags> {

    static emscripten::val transmit(std::set<ValueType> const &input)
    {
        auto array = emscripten::val::array();
        array.set("length", input.size());

        auto it = input.begin(); for (auto t = 0u, T = input.size(); t < T; ++t)
            array.set(t, emmagic_transmit<ValueType, Flags>(*it++));

        return array;
    }

};

template <typename ValueType, int Flags>
struct emmagic_wrap_type_set<ValueType, Flags, std::enable_if_t<EMMAGIC_HAS_FLAG(Flags, EMMAGIC_USE_REAL_SETS)>> : public emmagic_wrap_type_set_base<ValueType, Flags> {

    static emscripten::val transmit(std::set<ValueType> const &input)
    {
        auto set = emscripten::val::global("Set").new_();

        for (auto const & item : input)
            set.call<void>("add", emmagic_transmit<ValueType, Flags>(item));

        return set;
    }

};

template <typename ValueType, int Flags>
struct emmagic_wrap_type<std::set<ValueType>, Flags> : public emmagic_wrap_type_set<ValueType, Flags> {};
