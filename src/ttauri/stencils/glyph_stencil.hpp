// Copyright Take Vos 2020.
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE_1_0.txt or copy at https://www.boost.org/LICENSE_1_0.txt)

#pragma once

#include "image_stencil.hpp"
#include "../text/font_glyph_ids.hpp"
#include "../aarect.hpp"
#include "../mat.hpp"

namespace tt {

class glyph_stencil : public image_stencil {
public:
    using super = image_stencil;

    glyph_stencil(alignment alignment, font_glyph_ids glyph) noexcept;

    void draw(draw_context context, bool use_context_color=false) noexcept override;

private:
    font_glyph_ids _glyph;
    aarect _glyph_bounding_box;
    mat _glyph_transform;
};

}
