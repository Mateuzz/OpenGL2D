#pragma once

#include "TestSpecification.h"

#include <stddef.h>
#include <vector>

namespace test {
    struct TestResult {
        struct Metadata {
            unsigned repeatTimes;
            size_t columnsSize;
        };

        struct ColumnData {
            TestSpecification::RendererType rendererType;
            TestSpecification::Type type;
            unsigned spritesToRender;
            unsigned drawCalls;
            unsigned microseconds;
        };

        Metadata metadata;
        std::vector<ColumnData> columns;
    };
}
