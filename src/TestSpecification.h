#pragma once

namespace test {
    struct TestSpecification {
        enum RendererType { Normal, BatchRenderer };
        enum Type { OneTexture, MultipleAlternatingTextures };

        RendererType rendererType;
        Type type;
        unsigned spritesToRender;
    };
} // namespace test
