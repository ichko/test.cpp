#pragma once

#include <list>
#include <string>
#include <ostream>
#include <typeinfo>

using std::list;
using std::pair;
using std::string;
using std::ostream;

#define UNKNOWN_EXCEPTION_MESSAGE "Unknown exception occurred"


namespace Test {

    struct SingleTest {

        string name;
        void(*function)();
        bool throws_exception;

        SingleTest(string _name, void(*_function)(), bool _throws_exception = false) :
            name(_name), function(_function), throws_exception(_throws_exception) {
        }

    };

    struct AssertionException {

        string message;

        AssertionException(string _message = "") : message(_message) {}

    };

    struct TestResult {

        bool success;
        string test_name;
        string message;

        TestResult(bool _success = true, string _test_name = "", string _message = "") :
            success(_success), test_name(_test_name), message(_message) {
        }

    };

    namespace Assert {

        template <typename T> static void AreEqual(T left, T right) {
            if (left != right) {
                throw AssertionException("Assertion are equal failed");
            }
        }

        template <typename T> static void AreSame(T& left, T& right) {
            if (&left != &right) {
                throw AssertionException("Assertion are equal failed");
            }
        }

        static void IsTrue(bool value) {
            if (!value) {
                throw AssertionException("Assertion IsTrue failed");
            }
        }

        static void IsFalse(bool value) {
            if (value) {
                throw AssertionException("Assertion IsFalse failed");
            }
        }

        template <typename T> static void ArraysEqual(T* left, T* right, size_t size) {
            for (size_t i = 0; i < size; i++) {
                if (left[i] != right[i]) {
                    throw AssertionException("Assertion AssertArraysEqual failed");
                }
            }
        }

        template <typename T> static void Throws(void(*function)()) {
            try {
                function();
            }
            catch (T) {}
            catch (...) {
                throw AssertionException("Assertion AssertThrows failed");
            }
        }

    };

    class Case {

        string name;
        list<SingleTest> tests;
        list<TestResult> test_results;

        void(*before_test)();
        void(*after_test)();
        void(*setup)();
        void(*teardown)();

    public:
        Case(string _name) :
            name(_name),
            before_test(nullptr), after_test(nullptr),
            setup(nullptr), teardown(nullptr) {
        }

        Case& Test(string name, void(*test)());

        Case& TestThrows(string name, void(*test)());

        Case& BeforeTest(void(*_before_test)());

        Case& AfterTest(void(*_after_test)());

        Case& Setup(void(*_setup)());

        Case& Teardown(void(*_tear_down)());

        Case& RunAll();

        Case& OutputResults(ostream& os);

        auto GetTestResults() {
            return test_results.begin();
        }

    private:
        void RunFunction(void(*function)(), bool _catch = false);

    };

}
