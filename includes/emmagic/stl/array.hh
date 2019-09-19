#pragma once

#ifndef EMMAGIC_READY
# error "Please include 'emmagic/magic.hh' before this file"
#endif

#include <array>
#include <type_traits>

template <typename ValueType, int Size, int Flags>
struct emmagic_wrap_type_array_base : public emmagic_wrap_type_base<std::array<ValueType, Size>, emscripten::val> {

    static std::array<ValueType, Size> receive(emscripten::val const &value)
    {
        std::array<ValueType, Size> array;

        for (auto t = 0ul; t < Size; ++t)
            array[t] = emmagic_receive<ValueType, Flags>(value[t]);

        return array;
    }

};

template <typename ValueType, int Size, int Flags, typename = void>
struct emmagic_wrap_type_array : public emmagic_wrap_type_array_base<ValueType, Size, Flags> {

    static emscripten::val transmit(std::array<ValueType, Size> const &input)
    {
        auto array = emscripten::val::array();
        array.set("length", Size);

        for (auto t = 0ul, T = input.size(); t < T; ++t)
            array.set(t, emmagic_transmit<ValueType, Flags>(input.at(t)));

        return array;
    }

};

template <typename ValueType, int Size, int Flags>
struct emmagic_wrap_type_array<ValueType, Size, Flags, std::enable_if_t<EMMAGIC_HAS_FLAG(Flags, EMMAGIC_USE_TYPED_ARRAYS) && emmagic_typed_array_trait<ValueType>::constructor>> : public emmagic_wrap_type_array_base<ValueType, Size, Flags> {

    static emscripten::val transmit(std::array<ValueType, Size> const &input)
    {
        auto typedArray = emscripten::val::global(emmagic_typed_array_trait<ValueType>::constructor).new_(Size);

        for (auto t = 0ul, T = input.size(); t < T; ++t)
            typedArray.set(t, emmagic_transmit<ValueType, Flags>(input.at(t)));

        return typedArray;
    }

};

template <typename ValueType, int Size, int Flags>
struct emmagic_wrap_type<std::array<ValueType, Size>, Flags> : public emmagic_wrap_type_array<ValueType, Size, Flags> {};
