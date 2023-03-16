#pragma once

#include "TestResult.h"
#include "TestSpecification.h"

#include <SDL2/SDL_video.h>
#include <vector>

namespace test {
    struct TestUnitResult;

    class TestController {
    public:
        static void InitializeTestSystem(unsigned repeatTimes = 100);
        static void ShutdownTestSystem();

        static TestResult RunAll(const std::vector<TestSpecification>& specs);

    private:
        static TestUnitResult TestUnit(const TestSpecification& spec);
    };
} // namespace test
