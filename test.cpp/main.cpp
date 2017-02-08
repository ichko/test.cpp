#include <iostream>
#include "test.cpp"

int main() {

    typedef Test::Assert Assert;

    Test::Case("Testing math")

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


        .RunAll()
        .OutputResults(std::cout);

}