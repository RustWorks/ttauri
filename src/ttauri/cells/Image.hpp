// Copyright 2020 Pokitec
// All rights reserved.

#pragma once

#include "ImageCell.hpp"
#include "../URL.hpp"
#include "../PixelMap.hpp"
#include "../R16G16B16A16SFloat.hpp"
#include "../text/FontGlyphIDs.hpp"
#include "../text/ElusiveIcons.hpp"
#include "../text/TTauriIcons.hpp"
#include <variant>

namespace tt {
class DrawContext;

/** An image, in different formats.
 */
class Image {
public:
    using image_type = std::variant<std::monostate,FontGlyphIDs,PixelMap<R16G16B16A16SFloat>>;

    image_type image;
public:
    Image(URL const &url);
    Image(PixelMap<R16G16B16A16SFloat> &&image) noexcept;
    Image(FontGlyphIDs const &glyph) noexcept;
    Image(ElusiveIcon const &icon) noexcept;
    Image(TTauriIcon const &icon) noexcept;

    Image() noexcept;
    Image(Image const &) noexcept;
    Image(Image &&) noexcept;
    Image &operator=(Image const &) noexcept;
    Image &operator=(Image &&) noexcept;

    [[nodiscard]] std::unique_ptr<ImageCell> makeCell() const noexcept;
};


}
