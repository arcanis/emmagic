#pragma once

#include <cstdint>

// This file is used to make a quick correspondance between a native type and its corresponding typed array.

template <typename ValueType>
struct emmagic_typed_array_trait;

template <>
struct emmagic_typed_array_trait<std::int8_t> {
  static constexpr char const * constructor = "Int8Array";
};

template <>
struct emmagic_typed_array_trait<std::int16_t> {
  static constexpr char const * constructor = "Int16Array";
};

template <>
struct emmagic_typed_array_trait<std::int32_t> {
  static constexpr char const * constructor = "Int32Array";
};

template <>
struct emmagic_typed_array_trait<std::uint8_t> {
  static constexpr char const * constructor = "Uint8Array";
};

template <>
struct emmagic_typed_array_trait<std::uint16_t> {
  static constexpr char const * constructor = "Uint16Array";
};

template <>
struct emmagic_typed_array_trait<std::uint32_t> {
  static constexpr char const * constructor = "Uint32Array";
};

template <>
struct emmagic_typed_array_trait<float> {
  static constexpr char const * constructor = "Float32Array";
};

template <>
struct emmagic_typed_array_trait<double> {
  static constexpr char const * constructor = "Float64Array";
};
