# CGS [![Build Status](https://travis-ci.org/coryshrmn/cgs.svg?branch=master)](https://travis-ci.org/coryshrmn/cgs)

Header-only C++17 standard supplement, including:
1. My C++ Standard Wishlist
    * constexpr algorithms, [all the things](https://www.youtube.com/watch?v=HMB9oXFobJc)
2. Differences from GSL
    * assert should _not evaluate_ in completely optimized release builds. I want to assert _expensive_ checks.
    

## Features

* Configurable error handling (ignore / throw / abort)

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
#define CGS_VIOLATE_IGNORE

// default in debug builds (NDEBUG not defined)
// print an error message and abort
#define CGS_VIOLATE_ABORT

// throw a std::logic_error instead of aborting
#define CGS_VIOLATE_THROW

#include "cgs/assert.hpp"

T operator[](size_t i) {
    cgs_assert(i < _size);
    return _data[i];
}
```

### `cgs::unowned_ptr<typename T>`

```cpp
#include "cgs/unowned_ptr.hpp"
using cgs::unowned_ptr;

struct Foo {

    // Unowned pointers can be null.
    unowned_ptr<Resource> resource {};
    unowned_ptr<UpdateFunction> update {};

    void tryUpdate() {
        // Use operator bool to determine if an unowned_ptr is null.
        if(resource && update) {
            // We know resource and update are set before calling doUpdate().
            doUpdate();
        }
    }

private:
    void doUpdate() {
        // We require resource and update are already set.

        // In debug builds, abort for null at runtime.
        // In release builds, optimize null checks out.

        // Can be configured to throw logic_errors, in case you can't risk UB in release. See cgs_assert.

        // Check resource with operator*.
        Resource& res = *resource;

        // Check resource with operator->.
        resource->bind();

        // Check update with function call.
        update(res);
    }
};

static_assert(is_trivially_copyable_v<Foo>);
static_assert(is_trivially_destructible_v<Foo>);
```

## Tests

Tests are built with cmake.

```ini
$ git clone --recursive https://github.com/coryshrmn/cgs.git
$ cd cgs
$ mkdir build
$ cd build
$ cmake -DCMAKE_BUILD_TYPE=Debug ..
$ cmake --build .
$ ./cgs-test
...
[----------] Global test environment tear-down
[==========] Many tests from several test cases ran. (speedy ms total)
[  PASSED  ] Many tests.
```

### Tested compilers

* g++ 7
* ~clang++ 5.0~ Travis errors while downloading clang++ 5.0 recently.


## License

Apache License 2.0, see [LICENSE.txt](LICENSE.txt)
