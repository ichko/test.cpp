#pragma once

#include <list>
#include <string>
#include <ostream>

using std::list;
using std::pair;
using std::string;
using std::ostream;

#define UNKNOWN_EXCEPTION_MESSAGE "Unknown exception occurred"


namespace Test {

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

    };

    class Case {

        string name;
        list<pair<string, void(*)()>> tests;
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

        Case& AddTest(string name, void(*test)()) {
            tests.push_back({ name, test });
            return *this;
        }

        Case& BeforeTest(void(*_before_test)()) {
            before_test = _before_test;
            return *this;
        }

        Case& AfterTest(void(*_after_test)()) {
            after_test = _after_test;
            return *this;
        }

        Case& Setup(void(*_setup)()) {
            setup = _setup;
            return *this;
        }

        Case& Teardown(void(*_tear_down)()) {
            teardown = _tear_down;
            return *this;
        }

        Case& RunAll() {
            RunFunction(setup);

            for (auto& test : tests) {
                auto test_name = test.first;
                auto test_function = test.second;
                auto success = true;
                string message = "";

                try {
                    RunFunction(before_test);
                    RunFunction(test_function);
                    RunFunction(after_test);
                }
                catch (const AssertionException& exception) {
                    success = false;
                    message = exception.message;
                }
                catch (string _message) {
                    success = false;
                    message = _message;
                }
                catch (...) {
                    success = false;
                    message = UNKNOWN_EXCEPTION_MESSAGE;
                }

                test_results.push_back(TestResult(success, test_name, message));
            }
            RunFunction(teardown);

            return *this;
        }

        Case& OutputResults(ostream& os) {
            int counter = 1;
            os << "# " << name << std::endl;
            for (auto& test_result : test_results) {
                os << counter++ << ". [" << (test_result.success ? "+" : "-") << "]" << " "
                    << test_result.test_name << std::endl;
                os << (test_result.success ? "" : "   " + test_result.message + "\n");
            }
            os << std::endl;

            return *this;
        }

        auto GetTestResults() {
            return test_results.begin();
        }

    private:
        void RunFunction(void(*function)()) {
            if (function != nullptr) {
                function();
            }
        }

    };

}