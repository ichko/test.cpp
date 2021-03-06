# test.cpp
Unit testing library for c++

## Examples

## Usage

```cpp
#include "test.hpp"
using namespace Test; // Easy access to the Assert namespace
```

## Simple test case

### Source
```cpp
auto SimpleTestCase = Case("Simple test case")

.Test("was Pythagoras right", []() {
    Assert()
        .Describe("simple Pythagoras use case")
        .AreEqual(5 * 5, 4 * 4 + 3 * 3)
        .AreEqual(10 * 10, 8 * 8 + 6 * 6)

        .Describe("complex Pythagoras use case")
        .AreEqual(25 * 25, 20 * 20 + 15 * 15);
})

.Test("assertion true test", []() {
    Assert().IsTrue(true);
})

.Test("are same test", []() {
    int actual = Context.numeric_value;
    int& expected = actual;

    Assert().AreSame(expected, actual);
})

.Test("failing test", []() {
    Assert()
        .Describe("this will pass")
        .IsTrue(true)
        .Describe("this will fail")
        .IsTrue(false);
});
```

### Output
```
# Simple test case
1. [+] was Pythagoras right
2. [+] assertion true test
3. [+] are same test
4. [-] failing test
   this will fail
```

## Complex test case

### Source
```cpp
// Tests context struct
struct Context {

    bool success;
    string string_value;
    int numeric_value;
    int destroyed;

    Context() : destroyed(false) {}

    void Destroy() { destroyed = true; }

} Context;


auto ComplexTestCase = Case("Compex test case")

.Setup([]() {
    Context.success = true;
    Context.string_value = "unknown exception on the way";
    Context.numeric_value = 42;
})

.Teardown([]() {
    Context.Destroy();
})

.Test("unknown exception error", []() {
    throw Context.string_value;
})

.Test("test assert throws", []() {
    Assert().Throws<char*>([]() {
        throw "throwing string";
    });
})

.Test("assert arrays equal", []() {
    char* left_array = "hello";
    char* right_array = "hello";
    Assert().ArraysEqual(left_array, right_array, 5);
});


int main() {
    ComplexTestCase
        .RunAll()
        .OutputResults(std::cout);

    std::cout << "> context is "
        << (Context.destroyed ? "" : "not ")
        << "destroyed" << std::endl;
}
```

### Output
```
# Compex test case
1. [-] unknown exception error
   unknown exception on the way
2. [+] test assert throws
3. [+] assert arrays equal

> context is destroyed
```
