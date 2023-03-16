#include "TestController.h"
#include "Renderer2D.h"
#include "BatchRenderer2D.h"
#include "config.h"

#include <SDL2/SDL.h>
#include <SDL2/SDL_video.h>
#include <bits/chrono.h>
#include <chrono>
#include <concepts>
#include <fstream>
#include <initializer_list>

namespace test {
    struct TestUnitResult {
        unsigned drawCalls;
        unsigned microseconds;
    };

    unsigned gRepeatTimes;
    Texture gTextures[config::TexturesCount];

    void TestController::InitializeTestSystem(unsigned repeatTimes)
    {
        gRepeatTimes = repeatTimes;

        int width;
        int height;

        SDL_GetWindowSize(config::gWindow, &width, &height);

        Renderer2D::Initialize(width, height, 0, 0);
        BatchRenderer2D::Initialize(width, height, 0, 0);

        Renderer2D::SetClearColor(0.3f, 0.3f, 0.3f, 1.0f);

        std::ostringstream os;

        for (int i = 0; i < 32; ++i) {
            gTextures[i].Generate();
            os << "assets/" << (i + 1) << ".png";
            gTextures[i].Load(os.str().c_str());
            os.str("");
        }
    }

    void TestController::ShutdownTestSystem()
    {
        Renderer2D::Shutdown();
        BatchRenderer2D::Shutdown();
    }

    TestResult TestController::RunAll(const std::vector<TestSpecification>& specs)
    {
        TestResult result = {
            {gRepeatTimes, specs.size()},
            {}
        };


        result.columns.resize(result.metadata.columnsSize);

        bool windowNotClosed = true;
        unsigned specificationListIndex = 0;
        unsigned repeatIndex = 0;
        unsigned totalMicroseconds = 0;

        while (windowNotClosed) {
            for (SDL_Event e; SDL_PollEvent(&e);) {
                switch (e.type) {
                case SDL_QUIT:
                    windowNotClosed = false;
                    break;

                case SDL_WINDOWEVENT:
                    if (e.window.event == SDL_WINDOWEVENT_SIZE_CHANGED) {
                        Renderer2D::SetViewport(0, 0, e.window.data1, e.window.data2);
                        BatchRenderer2D::SetViewport(0, 0, e.window.data1, e.window.data2);
                    }
                    break;

                default:
                    break;
                }
            }

            const auto& spec = specs[specificationListIndex];

            TestUnitResult testUnitResult = TestUnit(spec);

            totalMicroseconds += testUnitResult.microseconds;

            if (++repeatIndex >= gRepeatTimes) {
                result.columns[specificationListIndex] = {
                    spec.rendererType,
                    spec.type,
                    spec.spritesToRender,
                    testUnitResult.drawCalls,
                    totalMicroseconds / gRepeatTimes
                };

                repeatIndex = 0;

                ++specificationListIndex;
                totalMicroseconds = 0;
            }

            if (specificationListIndex == specs.size()) {
                break;
            }
        }

        return result;
    }

    TestUnitResult TestController::TestUnit(const TestSpecification& spec)
    {
        TestUnitResult result = {10, 10};

        auto start = std::chrono::steady_clock::now();

        if (spec.rendererType == TestSpecification::RendererType::Normal) {
            Renderer2D::BeginScene();
            Renderer2D::ClearBuffer();

            if (spec.type == TestSpecification::Type::OneTexture) {
                for (unsigned i = 0; i < spec.spritesToRender; ++i) {
                    Renderer2D::DrawQuad({32, 32}, {32, 32}, gTextures[0]);
                }
            } else {
                for (unsigned i = 0; i < spec.spritesToRender; ++i) {
                    Renderer2D::DrawQuad({32, 32}, {32, 32}, gTextures[i % config::TexturesCount]);
                }
            }


            result.drawCalls = spec.spritesToRender;
        } else {
            BatchRenderer2D::BeginBatch();
            BatchRenderer2D::ClearBuffer();

            if (spec.type == TestSpecification::Type::OneTexture) {
                for (unsigned i = 0; i < spec.spritesToRender; ++i) {
                    BatchRenderer2D::DrawQuad({32, 32}, {32, 32}, gTextures[0]);
                }
            } else {
                for (unsigned i = 0; i < spec.spritesToRender; ++i) {
                    BatchRenderer2D::DrawQuad({32, 32}, {32, 32}, gTextures[i % config::TexturesCount]);
                }
            }

            BatchRenderer2D::EndBatch();
            BatchRenderer2D::Flush();

            result.drawCalls = BatchRenderer2D::GetStats().drawCalls;
            BatchRenderer2D::ResetStats();
        }

        SDL_GL_SwapWindow(config::gWindow);

        auto end = std::chrono::steady_clock::now();

        result.microseconds = std::chrono::duration_cast<std::chrono::microseconds>(end - start).count();

        return result;
    }
} // namespace test
