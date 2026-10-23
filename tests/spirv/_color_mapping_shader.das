options gen2
options indenting = 4

module _color_mapping_shader shared public

require spirv/spirv_shader
require daslib/color_mapping

var @in @location = 0 color_mapping_input : float3
var @out @location = 0 color_mapping_reinhard : float4
var @out @location = 1 color_mapping_aces : float4
var @out @location = 2 color_mapping_neutral : float4
var @out @location = 3 color_mapping_channels : float4

[fragment_shader(name="color_mapping_spv"), marker(no_coverage)]
def color_mapping_fragment {
    color_mapping_reinhard = float4(color_srgb_encode(tone_reinhard(color_mapping_input)), 1.0)
    color_mapping_aces = float4(tone_aces_fit(color_srgb_decode(color_mapping_input)), 1.0)
    color_mapping_neutral = float4(tone_pbr_neutral(color_mapping_input), 1.0)
    color_mapping_channels = float4(color_srgb_encode_channel(color_mapping_input.x),
        color_srgb_decode_channel(color_mapping_input.y), 0.0, 1.0)
}

def public color_mapping_words : array<uint> => clone_to_move(color_mapping_spv)
