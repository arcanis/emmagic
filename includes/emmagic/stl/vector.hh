#pragma once

#ifndef EMMAGIC_READY
# error "Please include 'emmagic/magic.hh' before this file"
#endif

#include <type_traits>
#include <vector>

template <typename ValueType, int Flags>
struct emmagic_wrap_type_vector_base : public emmagic_wrap_type_base<std::vector<ValueType>, emscripten::val> {

    static std::vector<ValueType> receive(emscripten::val const &value)
    {
        std::vector<ValueType> vector(value["length"].as<unsigned>());

        for (auto t = 0ul, T = vector.size(); t < T; ++t)
            vector[t] = emmagic_receive<ValueType, Flags>(value[t]);

        return vector;
    }

};

template <typename ValueType, int Flags, typename = void>
struct emmagic_wrap_type_vector : public emmagic_wrap_type_vector_base<ValueType, Flags> {

    static emscripten::val transmit(std::vector<ValueType> const &input)
    {
        auto array = emscripten::val::array();
        array.set("length", input.size());

        for (auto t = 0ul, T = input.size(); t < T; ++t)
            array.set(t, emmagic_transmit<ValueType, Flags>(input.at(t)));

        return array;
    }

};

template <typename ValueType, int Flags>
struct emmagic_wrap_type_vector<ValueType, Flags, std::enable_if_t<EMMAGIC_HAS_FLAG(Flags, EMMAGIC_USE_TYPED_ARRAYS) && emmagic_typed_array_trait<ValueType>::constructor>> : public emmagic_wrap_type_vector_base<ValueType, Flags> {

    static emscripten::val transmit(std::vector<ValueType> const &input)
    {
        auto typedArray = emscripten::val::global(emmagic_typed_array_trait<ValueType>::constructor).new_(input.size());

        for (auto t = 0ul, T = input.size(); t < T; ++t)
            typedArray.set(t, emmagic_transmit<ValueType, Flags>(input.at(t)));

        return typedArray;
    }

};

template <typename ValueType, int Flags>
struct emmagic_wrap_type<std::vector<ValueType>, Flags> : public emmagic_wrap_type_vector<ValueType, Flags> {};
