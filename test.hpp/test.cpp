#include "test.hpp"


namespace Test {

    Case& Case::Test(string name, void(*test)()) {
        tests.push_back(SingleTest(name, test));
        return *this;
    }

    Case& Case::TestThrows(string name, void(*test)()) {
        tests.push_back(SingleTest(name, test, true));
        return *this;
    }

    Case& Case::BeforeTest(void(*_before_test)()) {
        before_test = _before_test;
        return *this;
    }

    Case& Case::AfterTest(void(*_after_test)()) {
        after_test = _after_test;
        return *this;
    }

    Case& Case::Setup(void(*_setup)()) {
        setup = _setup;
        return *this;
    }

    Case& Case::Teardown(void(*_tear_down)()) {
        teardown = _tear_down;
        return *this;
    }

    Case& Case::RunAll() {
        RunFunction(setup);

        for (auto& test : tests) {
            auto success = true;
            string message = "";

            try {
                RunFunction(before_test);
                RunFunction(test.function, test.throws_exception);
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
            catch (const char* _message) {
                success = false;
                message = _message;
            }
            catch (...) {
                success = false;
                message = UNKNOWN_EXCEPTION_MESSAGE;
            }

            test_results.push_back(TestResult(success, test.name, message));
        }
        RunFunction(teardown);

        return *this;
    }

    Case& Case::OutputResults(ostream& os) {
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

    void Case::RunFunction(void(*function)(), bool _catch) {
        if (function != nullptr) {
            if (_catch) {
                try {
                    function();
                }
                catch (...) {}
            }
            else {
                function();
            }
        }

    }

}
