options indenting = 4

module hlsl shared public

require hlsl_internal public

enum AddressMode
    Wrap
    Clamp

struct SamplerState
    AddressU : AddressMode
    AddressV : AddressMode
    AddressW : AddressMode
/*
BorderColor
Filter
MaxAnisotropy
MaxLOD
MinLOD
MipLODBias

// sampler-comparison state
ComparisonFunc
*/

// texture objects

class Texture1D {}

class Texture1DArray {}

class Texture2D
    [sideeffects] def Sample  ( state:SamplerState; texCoord:float2 ) : float4
        return float4(0.)

    // g_txDiffuse.Sample( g_samLinear, Input.vTexcoord )

class Texture2DArray {}

class Texture3D {}

class TextureCube {}
