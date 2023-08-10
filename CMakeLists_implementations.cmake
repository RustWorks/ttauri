
target_sources(hikogui PRIVATE
    ${HIKOGUI_SOURCE_DIR}/audio/audio_device_impl.cpp
    $<$<PLATFORM_ID:Windows>:${HIKOGUI_SOURCE_DIR}/audio/audio_device_win32_impl.cpp>
    ${HIKOGUI_SOURCE_DIR}/audio/audio_sample_format_impl.cpp
    ${HIKOGUI_SOURCE_DIR}/audio/audio_sample_packer_impl.cpp
    ${HIKOGUI_SOURCE_DIR}/audio/audio_sample_unpacker_impl.cpp
    $<$<PLATFORM_ID:Windows>:${HIKOGUI_SOURCE_DIR}/audio/audio_stream_format_win32_impl.cpp>
    ${HIKOGUI_SOURCE_DIR}/audio/audio_system_impl.cpp
    $<$<PLATFORM_ID:Windows>:${HIKOGUI_SOURCE_DIR}/audio/audio_system_win32_impl.cpp>
    ${HIKOGUI_SOURCE_DIR}/audio/speaker_mapping_impl.cpp
    $<$<PLATFORM_ID:Windows>:${HIKOGUI_SOURCE_DIR}/audio/speaker_mapping_win32_impl.cpp>
    ${HIKOGUI_SOURCE_DIR}/audio/win32_device_interface_impl.cpp
    ${HIKOGUI_SOURCE_DIR}/audio/win32_wave_device_impl.cpp
    ${HIKOGUI_SOURCE_DIR}/codec/BON8_impl.cpp
    ${HIKOGUI_SOURCE_DIR}/codec/gzip_impl.cpp
    ${HIKOGUI_SOURCE_DIR}/codec/inflate_impl.cpp
    ${HIKOGUI_SOURCE_DIR}/codec/png_impl.cpp
    ${HIKOGUI_SOURCE_DIR}/codec/zlib_impl.cpp
    $<$<PLATFORM_ID:Windows>:${HIKOGUI_SOURCE_DIR}/console/console_win32_impl.cpp>
    $<$<PLATFORM_ID:Windows>:${HIKOGUI_SOURCE_DIR}/console/dialog_win32_impl.cpp>
    $<$<PLATFORM_ID:Windows>:${HIKOGUI_SOURCE_DIR}/crt/crt_utils_win32_impl.cpp>
    ${HIKOGUI_SOURCE_DIR}/crt/terminate_impl.cpp
    $<$<PLATFORM_ID:Linux>:${HIKOGUI_SOURCE_DIR}/file/file_posix_impl.cpp>
    $<$<PLATFORM_ID:Linux>:${HIKOGUI_SOURCE_DIR}/file/file_view_posix_impl.cpp>
    $<$<PLATFORM_ID:Windows>:${HIKOGUI_SOURCE_DIR}/file/file_view_win32_impl.cpp>
    $<$<PLATFORM_ID:Windows>:${HIKOGUI_SOURCE_DIR}/file/file_win32_impl.cpp>
    ${HIKOGUI_SOURCE_DIR}/font/font_book_impl.cpp
    ${HIKOGUI_SOURCE_DIR}/font/font_impl.cpp
    ${HIKOGUI_SOURCE_DIR}/font/glyph_ids_impl.cpp
    ${HIKOGUI_SOURCE_DIR}/font/true_type_font_coverage_impl.cpp
    #${HIKOGUI_SOURCE_DIR}/font/true_type_font_GSUB_impl.cpp
    ${HIKOGUI_SOURCE_DIR}/font/true_type_font_impl.cpp
    ${HIKOGUI_SOURCE_DIR}/formula/formula_post_process_context_impl.cpp
    ${HIKOGUI_SOURCE_DIR}/GFX/draw_context_impl.cpp
    ${HIKOGUI_SOURCE_DIR}/GFX/gfx_device_impl.cpp
    ${HIKOGUI_SOURCE_DIR}/GFX/gfx_device_vulkan_impl.cpp
    ${HIKOGUI_SOURCE_DIR}/GFX/gfx_surface_impl.cpp
    ${HIKOGUI_SOURCE_DIR}/GFX/gfx_surface_vulkan_impl.cpp
    ${HIKOGUI_SOURCE_DIR}/GFX/gfx_system_impl.cpp
    ${HIKOGUI_SOURCE_DIR}/GFX/gfx_system_vulkan_impl.cpp
    $<$<PLATFORM_ID:Windows>:${HIKOGUI_SOURCE_DIR}/GFX/gfx_system_vulkan_win32_impl.cpp>
    ${HIKOGUI_SOURCE_DIR}/GFX/paged_image_impl.cpp
    ${HIKOGUI_SOURCE_DIR}/GFX/pipeline_alpha_device_shared_impl.cpp
    ${HIKOGUI_SOURCE_DIR}/GFX/pipeline_alpha_impl.cpp
    ${HIKOGUI_SOURCE_DIR}/GFX/pipeline_box_device_shared_impl.cpp
    ${HIKOGUI_SOURCE_DIR}/GFX/pipeline_box_impl.cpp
    ${HIKOGUI_SOURCE_DIR}/GFX/pipeline_image_device_shared_impl.cpp
    ${HIKOGUI_SOURCE_DIR}/GFX/pipeline_image_impl.cpp
    ${HIKOGUI_SOURCE_DIR}/GFX/pipeline_image_texture_map_impl.cpp
    ${HIKOGUI_SOURCE_DIR}/GFX/pipeline_SDF_device_shared_impl.cpp
    ${HIKOGUI_SOURCE_DIR}/GFX/pipeline_SDF_impl.cpp
    ${HIKOGUI_SOURCE_DIR}/GFX/pipeline_SDF_texture_map_impl.cpp
    ${HIKOGUI_SOURCE_DIR}/GFX/pipeline_tone_mapper_device_shared_impl.cpp
    ${HIKOGUI_SOURCE_DIR}/GFX/pipeline_tone_mapper_impl.cpp
    ${HIKOGUI_SOURCE_DIR}/GFX/pipeline_vulkan_impl.cpp
    ${HIKOGUI_SOURCE_DIR}/GFX/RenderDoc_impl.cpp
    ${HIKOGUI_SOURCE_DIR}/GFX/VulkanMemoryAllocator_impl.cpp
    ${HIKOGUI_SOURCE_DIR}/GUI/gui_system_impl.cpp
    $<$<PLATFORM_ID:Windows>:${HIKOGUI_SOURCE_DIR}/GUI/gui_system_win32_impl.cpp>
    ${HIKOGUI_SOURCE_DIR}/GUI/gui_window_impl.cpp
    $<$<PLATFORM_ID:Windows>:${HIKOGUI_SOURCE_DIR}/GUI/gui_window_win32_impl.cpp>
    ${HIKOGUI_SOURCE_DIR}/GUI/keyboard_bindings_impl.cpp
    $<$<PLATFORM_ID:Windows>:${HIKOGUI_SOURCE_DIR}/GUI/keyboard_virtual_key_win32_impl.cpp>
    ${HIKOGUI_SOURCE_DIR}/GUI/theme_book_impl.cpp
    ${HIKOGUI_SOURCE_DIR}/GUI/theme_impl.cpp
    ${HIKOGUI_SOURCE_DIR}/graphic_path/graphic_path_impl.cpp
    ${HIKOGUI_SOURCE_DIR}/skeleton/skeleton_impl.cpp
    ${HIKOGUI_SOURCE_DIR}/skeleton/skeleton_parse_context_impl.cpp
    ${HIKOGUI_SOURCE_DIR}/text/text_shaper_char_impl.cpp
    ${HIKOGUI_SOURCE_DIR}/text/text_shaper_impl.cpp
    ${HIKOGUI_SOURCE_DIR}/text/text_shaper_line_impl.cpp
    ${HIKOGUI_SOURCE_DIR}/text/text_style_impl.cpp
) 
