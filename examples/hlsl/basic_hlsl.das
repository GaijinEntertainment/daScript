require hlsl
require math
require strings

[cbuffer(register=b0)]
struct cbPerObject
	[[packoffset=c0]] g_vObjectColor : float4

[cbuffer(register=b1)]
struct cbPerFrame
	[[packoffset=c0]]       g_vLightDir : float3
	[[packoffset="c0.w"]]   g_fAmbient : float

var [[register=t0]] g_txDiffuse : Texture2D
var [[register=s0]] g_samLinear : SamplerState

struct PS_INPUT
    [[NORMAL]]      vNormal : float3
    [[TEXCOORD]]    vTexcoord : float2

[pixel_shader(name="g_psMain",output=SV_TARGET)]
def PSMain ( Input:PS_INPUT ) : float4
    let vDiffuse = g_txDiffuse->Sample(g_samLinear, Input.vTexcoord)
    var fLighting = saturate(dot(g_vLightDir, Input.vNormal))
    fLighting = max(fLighting, g_fAmbient)
    return vDiffuse * fLighting

[export]
def main
    print("{g_psMain}\n")
