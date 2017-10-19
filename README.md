# CGS [![Build Status](https://travis-ci.org/coryshrmn/cgs.svg?branch=master)](https://travis-ci.org/coryshrmn/cgs)

Header-only C++17 standard supplement, including:
1. Things I wish were in the C++ standard
2. Customizations to existing standard features

## Features

* Configurable error handling (undefined / throw / abort)

## Guidelines

* C++ Core Guidelines
* constexpr all the things

## Using CGS

CGS is header only.

### Including CGS

Include the [include](include) directory to your project,
and add `#include "cgs.hpp"` in your code.

## Examples

### `cgs_assert`

```cpp
// default in release builds (with NDEBUG defined)
// asserted expressions are not evaluated
#define CGS_ASSERT_IGNORE

// default in debug builds (NDEBUG not defined)
// print an error message and abort
#define CGS_ASSERT_ABORT

// throw a std::logic_error instead of aborting
#define CGS_ASSERT_THROW

#include "cgs.hpp"

T operator[](size_t i) {
    cgs_assert(i < _size);
    return _data[i];
}
```

### Tested compilers

* g++ 7
* ~clang++ 5.0~ Travis errors while downloading clang++ 5.0 recently.


## License

Apache License 2.0, see [LICENSE.txt](LICENSE.txt)
