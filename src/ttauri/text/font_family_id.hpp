// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE_1_0.txt or copy at https://www.boost.org/LICENSE_1_0.txt)

#pragma once

#include "../tagged_id.hpp"

namespace tt {

struct font_family_id_tag {};

using font_family_id = tagged_id<uint16_t, font_family_id_tag>;

}