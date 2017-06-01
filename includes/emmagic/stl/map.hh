#pragma once

#ifndef EMMAGIC_READY
# error "Please include 'emmagic/magic.hh' before this file"
#endif

#include <map>
#include <type_traits>

template <typename KeyType, typename ValueType, int Flags>
struct emmagic_wrap_type_map_base : public emmagic_wrap_type_base<std::map<KeyType, ValueType>, emscripten::val> {

    static std::map<KeyType, ValueType> receive(emscripten::val const & value)
    {
        std::map<KeyType, ValueType> map;

        auto keys = emscripten::val::global("Object").call<emscripten::val>("keys", value);

        for (auto t = 0u, T = keys["length"].as<unsigned>(); t < T; ++t)
            map.insert_or_assign(emmagic_receive<KeyType, Flags | (EMMAGIC_HAS_FLAG(Flags, EMMAGIC_COERCE_KEYS) ? EMMAGIC_COERCE_TYPES : 0)>(keys[t]), emmagic_receive<ValueType, Flags>(value[keys[t]]));

        return map;
    }

};

template <typename KeyType, typename ValueType, int Flags, typename = void>
struct emmagic_wrap_type_map : public emmagic_wrap_type_map_base<KeyType, ValueType, Flags> {

    static emscripten::val transmit(std::map<KeyType, ValueType> const &input)
    {
        auto object = emscripten::val::object();

        for (auto it = input.begin(); it != input.end(); ++it)
            object.set(emmagic_transmit<KeyType, Flags>(it->first), emmagic_transmit<ValueType, Flags>(it->second));

        return object;
    }

};

template <typename KeyType, typename ValueType, int Flags>
struct emmagic_wrap_type_map<KeyType, ValueType, Flags, std::enable_if_t<EMMAGIC_HAS_FLAG(Flags, EMMAGIC_USE_REAL_MAPS)>> : public emmagic_wrap_type_map_base<KeyType, ValueType, Flags> {

    static emscripten::val transmit(std::map<KeyType, ValueType> const &input)
    {
        auto object = emscripten::val::global("Map").new_();

        for (auto it = input.begin(); it != input.end(); ++it)
            object.call<void>("set", emmagic_transmit<KeyType, Flags>(it->first), emmagic_transmit<ValueType, Flags>(it->second));

        return object;
    }

};

template <typename KeyType, typename ValueType, int Flags>
struct emmagic_wrap_type<std::map<KeyType, ValueType>, Flags> : public emmagic_wrap_type_map<KeyType, ValueType, Flags> {};
