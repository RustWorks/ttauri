// Copyright 2019 Pokitec
// All rights reserved.

#include "StaticResources.hpp"

namespace TTauri {

#include "GUI/PipelineImage.vert.spv.inl"
#include "GUI/PipelineImage.frag.spv.inl"
#include "GUI/PipelineFlat.vert.spv.inl"
#include "GUI/PipelineFlat.frag.spv.inl"

StaticResources::StaticResources() noexcept
{
    intrinsic.try_emplace(PipelineImage_vert_spv_filename, PipelineImage_vert_spv_bytes);
    intrinsic.try_emplace(PipelineImage_frag_spv_filename, PipelineImage_frag_spv_bytes);
    intrinsic.try_emplace(PipelineFlat_vert_spv_filename, PipelineFlat_vert_spv_bytes);
    intrinsic.try_emplace(PipelineFlat_frag_spv_filename, PipelineFlat_frag_spv_bytes);
}

gsl::span<std::byte const> const StaticResources::get(std::string const &key) const
{
    let i = intrinsic.find(key);
    if (i == intrinsic.end()) {
        TTAURI_THROW(key_error("Could not find static resource")
            << error_info("key", key)
        );
    }
    return i->second;
}

}