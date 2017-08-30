#pragma once

#include <type_traits>

// The emmagic_wrap_property class is a class that implements two static functions:
//
// - Get is called when the JS code wants to access the value of the property
// - Set is called when the JS code tries to change the value of the property

template <typename PropertyType, PropertyType Property, int Flags>
struct emmagic_wrap_property;

// The default implementation, which is probably what you'll want to use, automatically calls:
//
// - Transmit on the property when Get is called
// - Receive on the new value when Set is called

template <typename T, typename PropertyType, PropertyType T::*Property, int Flags>
struct emmagic_wrap_property<PropertyType T::*, Property, Flags> {

    // It's hard to know which version we should expose: const or non-const? Technically, variables retrieved here might be modified on the JS side if they're object. On the other side, they're not modified by the C++ code itself, so is that good enough?
    // Fortunately, a bug(?) in emscripten makes it easier for us to pick a side: the Getter function type used in emscripten::class_::property MUST take a T const& as parameter, or the specialization won't be found (because it hasn't been implemented) and the compilation will crash.
    //
    // static typename emmagic_wrap_type<typename std::decay<PropertyType>::type, Flags>::WireType get(T & t)
    // {
    //     return emmagic_transmit<PropertyType, Flags>(t.*Property);
    // }

    static typename emmagic_wrap_type<typename std::decay<PropertyType>::type, Flags>::WireType get(T const & t)
    {
        return emmagic_transmit<PropertyType, Flags>(t.*Property);
    }

    static void set(T & t, typename emmagic_wrap_type<typename std::decay<PropertyType>::type, Flags>::WireType wire)
    {
        t.*Property = emmagic_receive<PropertyType, Flags>(wire);
    }

};
