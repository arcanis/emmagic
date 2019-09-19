#pragma once

#ifndef EMMAGIC_READY
# error "Please include 'emmagic/magic.hh' before this file"
#endif

#include <string>

template <int Flags>
struct emmagic_wrap_type<std::string, Flags> : public emmagic_wrap_type_base<std::string, emscripten::val> {

    static std::string receive(emscripten::val const & value) {
        return value.as<std::string>();
    }

    static emscripten::val transmit(std::string const & str) {
        return emscripten::val(str);
    }

};
