#include <iostream>
#include "test.cpp"

using namespace Test;


// Tests context struct

struct Context {

    bool success;
    string string_value;
    int numeric_value;
    int destroyed;

    Context() : destroyed(false) {}

    void Destroy() { destroyed = true; }

} Context;


// Defining tests

auto SimpleTestCase = Case("Simple test case")

.Test("was pythagoras right", []() {
    Assert::AreEqual(5 * 5, 4 * 4 + 3 * 3);
})

.Test("assertion true test", []() {
    Assert::IsTrue(true);
})

.Test("are same test", []() {
    int actual = Context.numeric_value;
    int& expected = actual;

    Assert::AreSame(expected, actual);
})

.Test("failing test", []() {
    Assert::IsTrue(false);
});


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
    Assert::Throws<char*>([]() {
        throw "throwing string";
    });
})

.Test("assert arrays equal", []() {
    char* left_array = "hello";
    char* right_array = "hello";
    Assert::ArraysEqual(left_array, right_array, 5);
});


int main() {
    SimpleTestCase
        .RunAll()
        .OutputResults(std::cout);

    ComplexTestCase
        .RunAll()
        .OutputResults(std::cout);

    std::cout << "> context is "
        << (Context.destroyed ? "" : "not ")
        << "destroyed" << std::endl;
}
