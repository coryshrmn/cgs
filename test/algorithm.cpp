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
#include "cgs/algorithm.hpp"

#include <algorithm>
#include <array>

constexpr auto fillArray()
{
    std::array<int, 3> result {};
    cgs::fill(result.begin(), result.end(), 20);
    return result;
}

constexpr auto filled = fillArray();

TEST(Algorithm, Fill)
{
    EXPECT_EQ(filled[0], 20);
    EXPECT_EQ(filled[1], 20);
    EXPECT_EQ(filled[2], 20);
}

constexpr int transformReduceArray()
{
    return cgs::transform_reduce(filled.begin(), filled.end(),
        // transform: - 1
        [](int val) { return val - 1; },
        // reduce: add
        std::plus<int>{}
    );
}

TEST(Algorithm, TransformReduce)
{
    EXPECT_EQ(transformReduceArray(), 19 * 3);
}

struct Person
{
    int age;
};

constexpr std::array<Person, 3> people { Person{45}, Person{15}, Person{21} };

constexpr int getAge(const Person& person)
{
    return person.age;
}

constexpr int minAge = cgs::transform_reduce(people.begin(), people.end(),
    getAge,
    cgs::min2<int>,
    std::numeric_limits<int>::max()
);

constexpr int maxAge = cgs::transform_reduce(people.begin(), people.end(),
    getAge,
    cgs::max2<int>
);

TEST(Algorithm, TransformReduceStruct)
{
    EXPECT_EQ(minAge, 15);
    EXPECT_EQ(maxAge, 45);
}
