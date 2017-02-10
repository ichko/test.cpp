#include <iostream>
#include "test.cpp"

using namespace Test;

struct Context {

    bool success;
    string string_value;
    int numeric_value;
    int destroyed;

    Context() : destroyed(false) {}

    void Destroy() { destroyed = true; }

} Context;


auto MathTests = Case("Testing math")

.Setup([]() {
    Context.success = true;
    Context.string_value = "unknown exception on the way";
    Context.numeric_value = 42;
})

.Teardown([]() {
    Context.Destroy();
})


.AddTest("was pythagoras right", []() {
    Assert::AreEqual(5 * 5, 4 * 4 + 3 * 3);
})

.AddTest("assertion true test", []() {
    Assert::IsTrue(Context.success);
})

.AddTest("failing test", []() {
    Assert::IsTrue(!Context.success);
})

.AddTest("are same test", []() {
    int value = Context.numeric_value;
    int* expected = &value;
    int* actual = &value;

    Assert::AreSame(expected, actual);
})

.AddTest("unknown exception error", []() {
    throw Context.string_value;
});


int main() {
    MathTests
        .RunAll()
        .OutputResults(std::cout);

    std::cout << "> context is "
        << (Context.destroyed ? "" : "not ")
        << "destroyed" << std::endl;

    return 0;
}
