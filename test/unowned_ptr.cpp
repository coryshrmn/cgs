/*
   Copyright 2017 Cory Sherman

   Licensed under the Apache License, Version 2.0 (the "License");
   you may not use this file except in compliance with the License.
   You may obtain a copy of the License at

       http://www.apache.org/licenses/LICENSE-2.0

   Unless required by applicable law or agreed to in writing, software
   distributed under the License is distributed on an "AS IS" BASIS,
   WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
   See the License for the specific language governing permissions and
   limitations under the License.
*/
#include "gtest/gtest.h"

#define CGS_VIOLATE_THROW
#include "cgs/unowned_ptr.hpp"

using cgs::unowned_ptr;

TEST(Unowned, Create)
{
    int i = 5;

    unowned_ptr<int> a {};
    unowned_ptr<int> b { nullptr };
    unowned_ptr<int> c { &i };

    EXPECT_FALSE(a);
    EXPECT_FALSE(b);
    EXPECT_TRUE(c);

    EXPECT_EQ(a, b);
    EXPECT_NE(a, c);
}

TEST(Unowned, Assign)
{
    int i = 5;
    unowned_ptr<int> a { &i };
    EXPECT_EQ(a, &i);
    a = nullptr;
    EXPECT_EQ(a, nullptr);
    a = {};
    EXPECT_EQ(a, nullptr);
    unowned_ptr<int> b { &i };
    a = b;
    EXPECT_EQ(a, &i);
    EXPECT_EQ(a, b);

}

TEST(Unowned, Compare)
{
    int i = 5;
    unowned_ptr<int> pI { &i };
    unowned_ptr<int> pNone {};
    unowned_ptr<int> pNone2 {nullptr};
    EXPECT_EQ(pNone, pNone2);
    EXPECT_NE(pI, pNone);
    EXPECT_EQ(pNone, {});
    EXPECT_EQ(pNone, nullptr);
    EXPECT_EQ(pI, &i);

    int i2 = 5;
    unowned_ptr<int> pI2 { &i2 };
    EXPECT_NE(pI, pI2);
    EXPECT_EQ(*pI, *pI2);
    unowned_ptr<int> pI2Twin { &i2 };
    EXPECT_EQ(pI2, pI2Twin);
    unowned_ptr<int> pI2Copy { pI2 };
    EXPECT_EQ(pI2, pI2Copy);
}

TEST(Unowned, Throw)
{
    unowned_ptr<int> pNone {};
    EXPECT_ANY_THROW(++*pNone);

    struct Foo
    {
        int bar;
    };

    Foo foo;
    foo.bar = 0;
    unowned_ptr<Foo> pFoo { &foo };
    ++pFoo->bar;
    EXPECT_EQ(foo.bar, 1);
    pFoo = {};
    EXPECT_ANY_THROW(++pFoo->bar);
}

static int saved = 0;
static void save(int value) {
    saved = value;
}

TEST(Unowned, Function)
{
    unowned_ptr<decltype(save)> pNull {};
    EXPECT_ANY_THROW(pNull(1));

    unowned_ptr<decltype(save)> pSave {&save};
    saved = 0;
    pSave(1);
    EXPECT_EQ(saved, 1);
    pSave(2);
    EXPECT_EQ(saved, 2);
    pSave = {};
    EXPECT_ANY_THROW(pSave(2));
}


#include <type_traits>

template <typename T>
constexpr auto is_trivially_copyable_v = std::is_trivially_copyable<T>::value;

template <typename T>
constexpr auto is_trivially_destructible_v = std::is_trivially_destructible<T>::value;


struct Resource
{
    void bind()
    { }
};

using UpdateFunction = void(Resource&);

struct Foo {

    // Unowned pointers can be null.
    unowned_ptr<Resource> resource {};
    unowned_ptr<UpdateFunction> update {};

    void tryUpdate() {
        // use operator bool to determine if an unowned_ptr is null
        if(resource && update) {
            // We know resource and update are set before calling doUpdate()
            doUpdate();
        }
    }

private:
    void doUpdate() {
        // We require resource and update are already set.

        // In debug builds, abort for null at runtime.
        // In release builds, optimize null checks out.

        // Can be configured to throw logic_errors, in case you can't risk UB in release. See cgs_assert.

        // check resource with operator*
        Resource& res = *resource;

        // check resource with operator->
        resource->bind();

        // check update with function call
        update(res);
    }
};

static_assert(is_trivially_copyable_v<Foo>);
static_assert(is_trivially_destructible_v<Foo>);
