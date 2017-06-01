#pragma once

#include <sstream>
#include <type_traits>

// The emmagic_wrap_type_base class is a class that is simply used to contain the local type and the wire type. It cannot do anything on its own.
//
// - The local type is the type that is used inside the C++ application
// - The wire type is the type that we send to embind after having been converted
//
// It contains two primitives that its children must implement:
//
// - Receive is called when the JS code calls one of your function, once for each argument. It converts from the wire type to the local type.
// - Transmit is the opposite; when your C++ functions return, their return value goes through the transmit function, which convert it from the local type to the wire type.

template <typename LocalTypeTpl, typename WireTypeTpl>
struct emmagic_wrap_type_base {

    using LocalType = LocalTypeTpl;
    using WireType = WireTypeTpl;

};

// The emmagic_wrap_type_simple class is a very simple implementation that doesn't make any kind of conversion.
// It will be mainly used for primitive types, those that embind already supports.

template <typename LocalType, typename WireType>
struct emmagic_wrap_type_simple : public emmagic_wrap_type_base<LocalType, WireType> {

    static LocalType receive(WireType data)
    {
        return data;
    }

    static WireType transmit(LocalType data)
    {
        return data;
    }

};

// The emmagic_wrap_type is the default conversion routine (because of its template parameter, any type that doesn't match a specialization will use this one).
// Because it directly inherits from emmagic_wrap_type_simple, any type without specialization will just skip the conversion process and be forwarded to embind as such.

template <typename LocalType, int Flags, typename = void>
struct emmagic_wrap_type : public emmagic_wrap_type_simple<LocalType, LocalType> {};

// The emmagic_receive function is used when you want the right converter to convert a wire type into a local type.
// Note that you obviously need to know the type you're expecting to receive in response!

template <typename LocalType, int Flags>
LocalType emmagic_receive(typename emmagic_wrap_type<std::decay_t<LocalType>, Flags>::WireType data)
{
    return emmagic_wrap_type<std::decay_t<LocalType>, Flags>::receive(data);
}

// We also add an overload for emscripten::val because we need to go through this indirection when accessing properties.
// So, even if a type has a WireType that is not emscripten::val, we still want to forward it (later, we'll try to convert this val into the wire type).
// We only enable it if the type

template <typename LocalType, int Flags>
std::enable_if_t<!std::is_same_v<typename emmagic_wrap_type<std::decay_t<LocalType>, Flags>::WireType, emscripten::val>, LocalType> emmagic_receive(emscripten::val val)
{
    return emmagic_wrap_type<std::decay_t<LocalType>, Flags>::receive(val);
}

// The emmagic_transmit function detects the right converter for a given local type, and call its "transmit" method on the specified data.
// You don't need to know the exact type of the wire type (just use `auto`).

template <typename LocalType, int Flags>
typename emmagic_wrap_type<std::decay_t<LocalType>, Flags>::WireType emmagic_transmit(LocalType data)
{
    return emmagic_wrap_type<std::decay_t<LocalType>, Flags>::transmit(data);
}

//

template <typename LocalType, bool CoerceTypes>
struct emmagic_wrap_type_arithmetic;

template <typename LocalType>
struct emmagic_wrap_type_arithmetic<LocalType, false> : public emmagic_wrap_type_simple<LocalType, LocalType> {

    static LocalType receive(emscripten::val val)
    {
        return val.as<LocalType>();
    }

};

template <typename LocalType>
struct emmagic_wrap_type_arithmetic<LocalType, true> : public emmagic_wrap_type_simple<LocalType, LocalType> {

    static LocalType receive(std::string const& string)
    {
        return emscripten::val::global("Number")(string).as<LocalType>();
    }

    static LocalType receive(emscripten::val val)
    {
        auto type = val.typeof().as<std::string>();

        if (type == "string") {
            return receive(val.as<std::string>());
        } else {
            return val.as<LocalType>();
        }
    }

};

//

template <typename LocalTypeTpl, typename BridgeTypeTpl, int Flags>
struct emmagic_wrap_type_custom {

    using LocalType = LocalTypeTpl;
    using BridgeType = BridgeTypeTpl;
    using WireType = typename emmagic_wrap_type<BridgeTypeTpl, Flags>::WireType;

    static LocalType receive(WireType data)
    {
        return LocalType::fromJS(emmagic_receive<BridgeType, Flags>(data));
    }

    static WireType transmit(LocalType data)
    {
        return emmagic_transmit<BridgeType, Flags>(data.toJS());
    }

};

template <typename Type, typename = std::void_t<>>
struct emmagic_has_tojs : public std::false_type {};

template <typename Type>
struct emmagic_has_tojs<Type, std::void_t<decltype(std::declval<Type>().toJS())>> : public std::true_type {};

//

template <int Flags> // void is a special case - we need to handle it explicitely
struct emmagic_wrap_type<void, Flags> : public emmagic_wrap_type_base<void, void> {};

template <typename LocalType, int Flags>
struct emmagic_wrap_type<LocalType, Flags, std::enable_if_t<std::is_arithmetic<LocalType>::value>> : public emmagic_wrap_type_arithmetic<LocalType, EMMAGIC_HAS_FLAG(Flags, EMMAGIC_COERCE_TYPES)> {};

template <typename LocalType, int Flags>
struct emmagic_wrap_type<LocalType, Flags, std::enable_if_t<emmagic_has_tojs<LocalType>::value>> : public emmagic_wrap_type_custom<LocalType, typename std::decay_t<typename std::result_of<decltype(&LocalType::toJS)(LocalType)>::type>, Flags> {};
