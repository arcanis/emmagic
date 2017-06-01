# [![](/logo.png?raw=true)](https://github.com/manaflair/emmagic)

> Easily transport your data structures between C++ and Javascript

[![](https://img.shields.io/npm/v/@manaflair/emmagic.svg)]() [![](https://img.shields.io/npm/l/@manaflair/emmagic.svg)]()

[Check out our other OSS projects!](https://manaflair.github.io)

## Installation

**Requirements:** Because it uses a lot of metaprogramming tricks, Emmagic requires a C++17 compiler. You'll need to compile your code with `-std=c++17`, or possibly `-std=c++1z`.

```
$> npm install --save-dev @manaflair/emmagic
```

Then add the following into your `node.gyp`:

```json
{
    "include_dirs": [
        "./node_modules/emmagic/includes"
    ]
}
```

## Features

  - Automatic bindings for most of the STL data structures
  - Automatic (and optional) coercion strings ⟷ numbers
  - In/out support for ES2015 data structures (Maps, Sets)
  - In/out support for browsers typed arrays
  - Can support any user-defined type

## Usage

```c++
#include <set>
#include <vector>

std::set<int> getUniqueNumbers(std::vector<int> const & input)
{
    return std::set<int>(input.begin(), input.end());
}

#include <emscripten/bind.h>
#include <emmagic/emmagic.hh>
#include <emmagic/stl.hh>

EMSCRIPTEN_BINDINGS(myModule)
{
    using namespace emmagic;

    function<getUniqueNumbers>("getUniqueNumbers");
}
```

## Advanced Usage

### Custom classes

You can add an automatic conversion from Javascript to C++ to any of your classes by implementing the two following methods:

```c++
struct MyType
{
    std::vector<int> toJS(void) const;
    static MyType fromJS(std::vector<int> const & data);
};
```

You can use any type supported by emmagic instead of `std::vector<int>` - even other custom class instances! They will automatically get unwrapped and converted through the regular conversion pipeline.

### Flags

The library supports adding flags to the conversion pipeline. The next sections will describe those flags in detail, but the general concept is that each function that you use inside `EMSCRIPTEN_BINDINGS` usually accepts an extra optional template parameter:

```c++
EMSCRIPTEN_BINDINGS(myModule)
{
    using namespace emmagic;

    function<..., EMMAGIC_SOME_FLAG_A | EMMAGIC_SOME_FLAG_B>("...");
}
```

### Typed arrays

When used, the `EMMAGIC_USE_TYPED_ARRAYS` flag will instruct the `std::array` and `sd::vector` converters to generate typed arrays on the JS side rather than regular arrays. This will oviously only work for arithmetic types, and will be ignored otherwise.

**Note:** It is advised you use the fixed-width integral data types (`std::int8_t`, `std::int32_t`, ...) rather than `char`, `int`, ... Those types are available through the header `<cstdint>`. However, note that this recommandation doesn't apply to `float` and `double`, which do not have any fixed-width definition at the time of this writing.

```c++
std::vector<std::uint8_t> makeBuffer(void)
{
    return std::vector<std::uint8_t>({
        0x00, 0x01, 0x02, 0x03,
        0x04, 0x05, 0x06, 0x07,
        0x08, 0x09, 0x0A, 0x0B,
        0x0C, 0x0D, 0x0E, 0x0F,
    });
}

EMSCRIPTEN_BINDINGS(myModule)
{
    using namespace emmagic;

    function<makeBuffer, EMMAGIC_USE_TYPED_ARRAYS>("makeBuffer");
}
```

### Using native Sets & Maps

When used, the `EMMAGIC_USE_REAL_MAPS` and `EMMAGIC_USE_REAL_SETS` flags will instruct the `std::map` and `std::set` converters to use the `Map` and `Set` constructor when generating data on the JS side (note that they will accept them as input regardless of how this setting is configured). Without them, maps will be returned as regular JS objects, and sets will be returned as regular arrays.

```c++
std::set<int> getUniqueNumbers(std::vector<int> const & input)
{
    return std::set<int>(input.begin(), input.end());
}

EMSCRIPTEN_BINDINGS(myModule)
{
    using namespace emmagic;

    function<getUniqueNumbers, EMMAGIC_USE_REAL_SETS>("getUniqueNumbers");
}
```

### Automatic coercion

When used, the `EMMAGIC_COERCE_TYPES` flag will instruct all arithmetic converters to coerce strings into numbers. Because enabling this flag might have an heavy impact on the performance, you can use `EMMAGIC_COERCE_KEYS` instead, which will apply the same rule but only on object keys.

**Note:** This flag only affects the data you *receive*. There is no way for an object in Javascript to have anything else than a string (or a symbol) as a key.

**Note:** Unless you use the `EMMAGIC_USE_REAL_MAPS` flag, it is advised to use `EMMAGIC_COERCE_KEYS` on functions that accept maps as arguments. Otherwise, the maps you will return from your C++ code will not be usable as argument.

**Note:** Because the automatic coercion requires running `typeof` on each value *before even knowing if it has to be coerced*, you probably should only use it only when required (for example when you need to work with an object map whose keys are expected to be integers). That being said, if you only work on small data sets, then maybe the added usability for your users might be worth it.

```c++
std::map<int, int> getScores(void) {
    return std::map<int, int>({
        { 42, 10 },
        { 69, 11 },
        { 66, 12 },
    });
}

EMSCRIPTEN_BINDINGS(myModule)
{
    using namespace emmagic;

    function<getScores, EMMAGIC_COERCE_KEYS>("getScores");
}
```

### Default flags

You can define flags to be used as default.

```c++
#include <emmagic/flags.hh>
#define EMMAGIC_DEFAULT_FLAGS EMMAGIC_COERCE_TYPES
#include <emmagic/magic.hh>

EMSCRIPTEN_BINDINGS(myModule)
{
    using namespace emmagic;

    function<myFunction>("myFunction");
}
```

## License (MIT)

> **Copyright © 2016 Manaflair**
>
> Permission is hereby granted, free of charge, to any person obtaining a copy of this software and associated documentation files (the "Software"), to deal in the Software without restriction, including without limitation the rights to use, copy, modify, merge, publish, distribute, sublicense, and/or sell copies of the Software, and to permit persons to whom the Software is furnished to do so, subject to the following conditions:
>
> The above copyright notice and this permission notice shall be included in all copies or substantial portions of the Software.
>
> THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.
