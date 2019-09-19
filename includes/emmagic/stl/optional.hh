#pragma once

#ifndef EMMAGIC_READY
# error "Please include 'emmagic/magic.hh' before this file"
#endif

#include <optional>

template <typename ValueType, int Flags>
struct emmagic_wrap_type<std::optional<ValueType>, Flags> : public emmagic_wrap_type_base<std::optional<ValueType>, emscripten::val> {

    static std::optional<ValueType> receive(emscripten::val const & value) {
        if (value.isNull()) {
            return std::nullopt;
        } else {
            return emmagic_receive<ValueType, Flags>(value);
        }
    }

    static emscripten::val transmit(std::optional<ValueType> const & opt) {
        if (opt) {
            return emmagic_transmit<ValueType, Flags>(*opt);
        } else {
            return emscripten::val::null();
        }
    }

};
