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

    struct Assert {

        string assertion_description;

        Assert() : assertion_description("Default asserion name") {}

        Assert& Describe(const string&& _assertion_description) {
            assertion_description = _assertion_description;
            return *this;
        }

        template <typename T> Assert& AreEqual(const T&& left, const T&& right) {
            if (left != right) {
                throw AssertionException(assertion_description);
            }

            return *this;
        }

        template <typename T> Assert& AreSame(T& left, T& right) {
            if (&left != &right) {
                throw AssertionException(assertion_description);
            }

            return *this;
        }

        Assert& IsTrue(bool value) {
            if (!value) {
                throw AssertionException(assertion_description);
            }

            return *this;
        }

        Assert& IsFalse(bool value) {
            if (value) {
                throw AssertionException(assertion_description);
            }

            return *this;
        }

        template <typename T> Assert& ArraysEqual(T* left, T* right, size_t size) {
            for (size_t i = 0; i < size; i++) {
                if (left[i] != right[i]) {
                    throw AssertionException(assertion_description);
                }
            }

            return *this;
        }

        template <typename T> Assert& Throws(void(*function)()) {
            try {
                function();
            }
            catch (T) {}
            catch (...) {
                throw AssertionException(assertion_description);
            }

            return *this;
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
