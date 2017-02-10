# test.cpp
Unit testing library for c++

## Examples

##Usage

```
#include "test.cpp"
using namespace Test; // Easy access to the Assert namespace
```

## Simple test case

### Definition
```cpp
auto SimpleTestCase = Case("Simple test case")

.AddTest("was pythagoras right", []() {
    Assert::AreEqual(5 * 5, 4 * 4 + 3 * 3);
})

.AddTest("assertion true test", []() {
    Assert::IsTrue(true);
})

.AddTest("failing test", []() {
    Assert::IsTrue(false);
});

int main() {
    SimpleTestCase
        .RunAll()
        .OutputResults(std::cout);
}
```

### Output
```
# Simple test case
1. [+] was pythagoras right
2. [+] assertion true test
3. [-] failing test
   Assertion IsTrue failed
```

## Complex test case

### Definition
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

.AddTest("are same test", []() {
    int actual = Context.numeric_value;
    int& expected = actual;

    Assert::AreSame(expected, actual);
})

.AddTest("unknown exception error", []() {
    throw Context.string_value;
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
1. [+] are same test
2. [-] unknown exception error
   unknown exception on the way

> context is destroyed
```
