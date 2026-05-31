#pragma once

#if defined(_DEBUG)
#include"../DirectX12/ColorBuffer.h"

//---------------------------------------------------------------------------------
class ColorEditor final {
    ColorEditor() = default;
    ~ColorEditor() = default;
public:
    //---------------------------------------------------------------------------------
    static void draw(colorDate& data) noexcept;
};
#endif