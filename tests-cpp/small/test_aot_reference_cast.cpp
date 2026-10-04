#include <doctest/doctest.h>
#include <type_traits>
#include "daScript/daScript.h"
#include "daScript/simulate/aot.h"

TEST_CASE("AOT pointer reinterpret references alias their pointer storage") {
    int number = 5;
    int * same = &number;
    auto & sameView = das::das_cast<int *&>::cast(same);
    static_assert(std::is_same<decltype(sameView), int *&>::value, "reference cast returns a reference");
    static_assert(std::is_same<decltype(das::das_cast<int *>::cast(same)), int *>::value, "value cast stays a value");
    CHECK(&sameView == &same);
    sameView = nullptr;
    CHECK(same == nullptr);
    char bytes[] = {7, 9};
    char * pointer = bytes;
    auto & view = das::das_cast<const uint8_t *&>::cast(pointer);
    CHECK(static_cast<const void *>(&view) == static_cast<const void *>(&pointer));
    CHECK(view[0] == 7);
    view++;
    CHECK(pointer == bytes + 1);
    const int value = 31;
    const int * readonly = &value;
    auto & mutableView = das::das_cast<int *&>::cast(readonly);
    CHECK(static_cast<const void *>(&mutableView) == static_cast<const void *>(&readonly));
    CHECK(*mutableView == 31);
    mutableView = nullptr;
    CHECK(readonly == nullptr);
}
