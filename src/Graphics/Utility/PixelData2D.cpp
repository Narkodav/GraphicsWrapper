#include <stb_image.h>
#include "Graphics/Utility/PixelData2D.h"
#include "Graphics/Common.h"

namespace Graphics::Utility {

    PixelData2D::PixelData2D(std::string_view filepath)
    {
        load(filepath);
    }

    void PixelData2D::load(std::string_view filepath)
    {
        int texWidth, texHeight, texChannels;
        m_pixels = stbi_load(filepath.data(), &texWidth, &texHeight, &texChannels, STBI_rgb_alpha);
        m_width = static_cast<size_t>(texWidth);
        m_height = static_cast<size_t>(texHeight);
        m_bpp = static_cast<size_t>(texChannels);
        m_capacity = m_width * m_height * STBI_rgb_alpha;
        GRAPHICS_VERIFY(m_pixels, "failed to load texture image");
    }

    void PixelData2D::destroy() {
        if (!m_pixels)
            return;

        stbi_image_free(m_pixels);
        m_width = 0;
        m_height = 0;
        m_bpp = 0;
        m_capacity = 0;
        m_pixels = nullptr;
    };
}