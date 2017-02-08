#include <iostream>
#include "test.cpp"

using namespace Test;


auto MathTests = Case("Testing math")

.AddTest("was pythagoras right", []() {
    Assert::AreEqual(5 * 5, 4 * 4 + 3 * 3);
})

.AddTest("assertion true test", []() {
    Assert::IsTrue(true);
})

.AddTest("failing test", []() {
    Assert::IsTrue(false);
})

.AddTest("are same test", []() {
    int value = 42;
    int* expected = &value;
    int* actual = &value;

    Assert::AreSame(expected, actual);
})

.AddTest("unknown exception error", []() {
    throw "unknown exception on the way";
});


int main() {
    MathTests
        .RunAll()
        .OutputResults(std::cout);

    return 0;
}
