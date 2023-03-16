#define SDL_MAIN_HANDLED

#include <GL/glew.h>
#include <SDL2/SDL.h>
#include <iostream>
#include <fstream>

#include "TestController.h"
#include "TestResult.h"
#include "config.h"

void PrintToFile(const char *filepath, const test::TestResult& result)
{
    std::ofstream ost(filepath);

    ost << "Repeat times: " << result.metadata.repeatTimes << '\n';
    ost << "Number of tests: " << result.metadata.columnsSize << "\n\n\n";

    for (const auto& co : result.columns) {
        ost << "Renderer: " << (co.rendererType == test::TestSpecification::RendererType::Normal ? "Normal" : "Batch Renderer") << '\n';
        ost << "Type: " << (co.type == test::TestSpecification::Type::OneTexture ? "1 texture" : "Max Textures alternated") << '\n';
        ost << "Sprites: " << co.spritesToRender << "     Draw calls: " << co.drawCalls << "     Microseconds: " << co.microseconds << "\n\n";
    }
}

int main(int argc, char** argv)
{
    SDL_Init(SDL_INIT_VIDEO);

    config::gWindow = SDL_CreateWindow("Batch Rendering Performance",
                                   SDL_WINDOWPOS_UNDEFINED,
                                   SDL_WINDOWPOS_UNDEFINED,
                                   1200,
                                   800,
                                   SDL_WINDOW_RESIZABLE | SDL_WINDOW_OPENGL);

    if (!config::gWindow) {
        return 1;
    }

    auto openglContext = SDL_GL_CreateContext(config::gWindow);
    SDL_GL_SetSwapInterval(false);

    if (glewInit() != GLEW_OK) {
        return 1;
    }

    {
        test::TestController::InitializeTestSystem(config::RepeatTestNumber);

        auto normalRenderSpecsList = std::initializer_list<test::TestSpecification>{
            {test::TestSpecification::Normal, test::TestSpecification::OneTexture, 10},
            {test::TestSpecification::Normal, test::TestSpecification::OneTexture, 50},
            {test::TestSpecification::Normal, test::TestSpecification::OneTexture, 200},
            {test::TestSpecification::Normal, test::TestSpecification::OneTexture, 1000},
            {test::TestSpecification::Normal, test::TestSpecification::OneTexture, 5000},

            {test::TestSpecification::Normal, test::TestSpecification::MultipleAlternatingTextures, 10},
            {test::TestSpecification::Normal, test::TestSpecification::MultipleAlternatingTextures, 50},
            {test::TestSpecification::Normal, test::TestSpecification::MultipleAlternatingTextures, 200},
            {test::TestSpecification::Normal, test::TestSpecification::MultipleAlternatingTextures, 1000},
            {test::TestSpecification::Normal, test::TestSpecification::MultipleAlternatingTextures, 5000},
        };

        auto batchRenderSpecsList = std::initializer_list<test::TestSpecification>{
            {test::TestSpecification::BatchRenderer, test::TestSpecification::OneTexture, 10},
            {test::TestSpecification::BatchRenderer, test::TestSpecification::OneTexture, 50},
            {test::TestSpecification::BatchRenderer, test::TestSpecification::OneTexture, 200},
            {test::TestSpecification::BatchRenderer, test::TestSpecification::OneTexture, 1000},
            {test::TestSpecification::BatchRenderer, test::TestSpecification::OneTexture, 5000},

            {test::TestSpecification::BatchRenderer, test::TestSpecification::MultipleAlternatingTextures, 10},
            {test::TestSpecification::BatchRenderer, test::TestSpecification::MultipleAlternatingTextures, 50},
            {test::TestSpecification::BatchRenderer, test::TestSpecification::MultipleAlternatingTextures, 200},
            {test::TestSpecification::BatchRenderer, test::TestSpecification::MultipleAlternatingTextures, 1000},
            {test::TestSpecification::BatchRenderer, test::TestSpecification::MultipleAlternatingTextures, 5000},
        };

        auto resultBatchRendering = test::TestController::RunAll(batchRenderSpecsList);
        auto resultNormalRendering = test::TestController::RunAll(normalRenderSpecsList);

        PrintToFile("Normal.txt", resultNormalRendering);
        PrintToFile("Batch Rendering.txt", resultBatchRendering);

        test::TestController::ShutdownTestSystem();
    }

    SDL_GL_DeleteContext(openglContext);
    SDL_DestroyWindow(config::gWindow);
    SDL_Quit();
}
