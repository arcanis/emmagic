#pragma once
#define EMMAGIC_READY

// We will always need emscripten::val so we can as well include it

#include <emscripten/val.h>

// Users will most likely want to use some, and there's no drawback exposing them via this header

#include "./flags.hh"

// If users haven't defined the default flags yet, then it's too late - you should NOT undef this constant!
// If you do, any change to the default flags will be ignored.

#ifndef EMMAGIC_DEFAULT_FLAGS
# define EMMAGIC_DEFAULT_FLAGS (0)
#endif

// Include the various core modules of emmagic

#include "./impl/typed_array_trait.hh"
#include "./impl/wrap_type.hh"
#include "./impl/wrap_free_fn.hh"
#include "./impl/wrap_member_fn.hh"
#include "./impl/wrap_property.hh"

// Finally, we expose the public interface that extends embind's interface

namespace emmagic {

    template <auto Fn, int Flags = 0>
    void function(char const * name)
    {
        return emscripten::function(name, emmagic_wrap_free_fn<decltype(Fn), Fn, Flags>::wrap, emscripten::allow_raw_pointers());
    }

}
