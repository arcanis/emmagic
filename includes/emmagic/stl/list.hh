#pragma once

#ifndef EMMAGIC_READY
# error "Please include 'emmagic/magic.hh' before this file"
#endif

#include <list>

template <typename ValueType, int Flags>
struct emmagic_wrap_type<std::list<ValueType>, Flags> : public emmagic_wrap_type_base<std::list<ValueType>, emscripten::val> {

    static std::list<ValueType> receive(emscripten::val const &value)
    {
        std::list<ValueType> list;

        for (auto t = 0u, T = value["length"].as<unsigned>(); t < T; ++t)
            list.push_back(emmagic_receive<ValueType, Flags>(value[t]));

        return list;
    }

    static emscripten::val transmit(std::list<ValueType> const &input)
    {
        auto array = emscripten::val::array();
        array.set("length", input.size());

        auto it = input.begin(); for (auto t = 0u, T = input.size(); t < T; ++t)
            array.set(t, emmagic_transmit<ValueType, Flags>(*it++));

        return array;
    }

};
