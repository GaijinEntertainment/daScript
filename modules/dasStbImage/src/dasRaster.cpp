#include "daScript/misc/platform.h"

#include "daScript/simulate/simulate_nodes.h"
#include "daScript/ast/ast_interop.h"
#include "daScript/ast/ast_policy_types.h"
#include "daScript/ast/ast_handle.h"

#include "aot_builtin_raster.h"

namespace das
{
    void rast_hspan_u8 ( TArray<uint8_t> & Span, int32_t spanOffset, const TArray<uint8_t> & Tspan, int32_t tspanOffset, float uvY, float dUVY, int32_t _count, LineInfoArg * at, Context * context ) {
        if ( uint32_t(spanOffset+_count) > Span.size ) {
            context->throw_error_at(at,"rast_hspan: span out of range %i+%i >= %i", spanOffset, _count, Span.size);
        }
        if ( tspanOffset<0 ) {
            context->throw_error_at(at,"rast_hspan: tspan offset is negative");
        }
        uint8_t * pspan = (uint8_t *) Span.data + spanOffset;
        uint8_t * tspan = (uint8_t *) Tspan.data + tspanOffset;
        int32_t count = _count;
        int32_t count4 = count >> 2;
        count &= 3;
        uint8_t * PSP = pspan;
        vec4f uv4 = v_add(v_splats(uvY),v_mul(v_make_vec4f(0.,1.,2.,3.),v_splats(dUVY)));
        vec4f duv4 = v_mul(v_splats(dUVY),v_splats(4.));
        // uint32_t maxi = uint32_t(Tspan.size - tspanOffset);
        vec4i maxi = v_splatsi(uint32_t(Tspan.size - tspanOffset));

        for ( int32_t P=0; P!=count4; ++P ) {
            vec4i iuv4 = v_cvt_vec4i(uv4);
            auto mask = v_signmask(v_cast_vec4f(v_cmp_lti(maxi,iuv4)));
            if ( mask!=0 ) context->throw_error_at(at,"rast_hspan: tspan out of range");
            auto i0 = uint32_t(v_extract_xi(iuv4));
            auto i1 = uint32_t(v_extract_yi(iuv4));
            auto i2 = uint32_t(v_extract_zi(iuv4));
            auto i3 = uint32_t(v_extract_wi(iuv4));
            auto b0 = tspan[i0];
            auto b1 = tspan[i1];
            auto b2 = tspan[i2];
            auto b3 = tspan[i3];
            *((uint32_t *)PSP) = uint32_t(b0) + (uint32_t(b1) << 8u) + (uint32_t(b2) << 16u) + (uint32_t(b3) << 24u);
            PSP += 4;
            uv4 = v_add(uv4,duv4);
        }
        if ( count ) {
            vec4i iuv4 = v_cvt_vec4i(uv4);
            auto mask = v_signmask(v_cast_vec4f(v_cmp_lti(maxi,iuv4)));
            mask &= ((1<<count)-1);
            if ( mask!=0 ) context->throw_error_at(at,"rast_hspan: tspan out of range");
            auto i0 = uint32_t(v_extract_xi(iuv4));
            auto b0 = tspan[i0];
            *PSP++ = b0;
            if ( count > 1 ) {
                auto i1 = uint32_t(v_extract_yi(iuv4));
                auto b1 = tspan[i1];
                *PSP++ = b1;
                if ( count > 2 ) {
                    auto i2 = uint32_t(v_extract_zi(iuv4));
                    auto b2 = tspan[i2];
                    *PSP++ = b2;
                }
            }
        }
    }

    void rast_hspan_masked_u8 ( TArray<uint8_t> & Span, int32_t spanOffset, const TArray<uint8_t> & Tspan, int32_t tspanOffset, float uvY, float dUVY, int32_t _count, LineInfoArg * at, Context * context ) {
        if ( uint32_t(spanOffset+_count) > Span.size ) {
            context->throw_error_at(at,"rast_hspan: span out of range %i+%i >= %i", spanOffset, _count, Span.size);
        }
        if ( tspanOffset<0 ) {
            context->throw_error_at(at,"rast_hspan: tspan offset is negative");
        }
        uint8_t * pspan = (uint8_t *) Span.data + spanOffset;
        uint8_t * tspan = (uint8_t *) Tspan.data + tspanOffset;
        int32_t count = _count;
        int32_t count4 = count >> 2;
        count &= 3;
        uint8_t * PSP = pspan;
        vec4f uv4 = v_add(v_splats(uvY),v_mul(v_make_vec4f(0.,1.,2.,3.),v_splats(dUVY)));
        vec4f duv4 = v_mul(v_splats(dUVY),v_splats(4.));
        // uint32_t maxi = uint32_t(Tspan.size - tspanOffset);
        vec4i maxi = v_splatsi(uint32_t(Tspan.size - tspanOffset));

        for ( int32_t P=0; P!=count4; ++P ) {
            vec4i iuv4 = v_cvt_vec4i(uv4);
            auto mask = v_signmask(v_cast_vec4f(v_cmp_lti(maxi,iuv4)));
            if ( mask!=0 ) context->throw_error_at(at,"rast_hspan: tspan out of range");
            auto i0 = uint32_t(v_extract_xi(iuv4));
            auto i1 = uint32_t(v_extract_yi(iuv4));
            auto i2 = uint32_t(v_extract_zi(iuv4));
            auto i3 = uint32_t(v_extract_wi(iuv4));
            if ( auto b0 = tspan[i0] ) PSP[0] = b0;
            if ( auto b1 = tspan[i1] ) PSP[1] = b1;
            if ( auto b2 = tspan[i2] ) PSP[2] = b2;
            if ( auto b3 = tspan[i3] ) PSP[3] = b3;
            PSP += 4;
            uv4 = v_add(uv4,duv4);
        }
        if ( count ) {
            vec4i iuv4 = v_cvt_vec4i(uv4);
            auto mask = v_signmask(v_cast_vec4f(v_cmp_lti(maxi,iuv4)));
            mask &= ((1<<count)-1);
            if ( mask!=0 ) context->throw_error_at(at,"rast_hspan: tspan out of range");
            auto i0 = uint32_t(v_extract_xi(iuv4));
            if ( auto b0 = tspan[i0] ) PSP[0] = b0;
            if ( count > 1 ) {
                auto i1 = uint32_t(v_extract_yi(iuv4));
                if ( auto b1 = tspan[i1] ) PSP[1] = b1;
                if ( count > 2 ) {
                    auto i2 = uint32_t(v_extract_zi(iuv4));
                    if ( auto b2 = tspan[i2] ) PSP[2] = b2;
                }
            }
        }
    }

    void rast_hspan_masked_solid_u8 ( uint8_t solid, TArray<uint8_t> & Span, int32_t spanOffset, const TArray<uint8_t> & Tspan, int32_t tspanOffset, float uvY, float dUVY, int32_t _count, LineInfoArg * at, Context * context ) {
        if ( uint32_t(spanOffset+_count) > Span.size ) {
            context->throw_error_at(at,"rast_hspan: span out of range %i+%i >= %i", spanOffset, _count, Span.size);
        }
        if ( tspanOffset<0 ) {
            context->throw_error_at(at,"rast_hspan: tspan offset is negative");
        }
        uint8_t * pspan = (uint8_t *) Span.data + spanOffset;
        uint8_t * tspan = (uint8_t *) Tspan.data + tspanOffset;
        int32_t count = _count;
        int32_t count4 = count >> 2;
        count &= 3;
        uint8_t * PSP = pspan;
        vec4f uv4 = v_add(v_splats(uvY),v_mul(v_make_vec4f(0.,1.,2.,3.),v_splats(dUVY)));
        vec4f duv4 = v_mul(v_splats(dUVY),v_splats(4.));
        // uint32_t maxi = uint32_t(Tspan.size - tspanOffset);
        vec4i maxi = v_splatsi(uint32_t(Tspan.size - tspanOffset));

        for ( int32_t P=0; P!=count4; ++P ) {
            vec4i iuv4 = v_cvt_vec4i(uv4);
            auto mask = v_signmask(v_cast_vec4f(v_cmp_lti(maxi,iuv4)));
            if ( mask!=0 ) context->throw_error_at(at,"rast_hspan: tspan out of range");
            auto i0 = uint32_t(v_extract_xi(iuv4));
            auto i1 = uint32_t(v_extract_yi(iuv4));
            auto i2 = uint32_t(v_extract_zi(iuv4));
            auto i3 = uint32_t(v_extract_wi(iuv4));
            if ( auto b0 = tspan[i0] ) PSP[0] = solid;
            if ( auto b1 = tspan[i1] ) PSP[1] = solid;
            if ( auto b2 = tspan[i2] ) PSP[2] = solid;
            if ( auto b3 = tspan[i3] ) PSP[3] = solid;
            PSP += 4;
            uv4 = v_add(uv4,duv4);
        }
        if ( count ) {
            vec4i iuv4 = v_cvt_vec4i(uv4);
            auto mask = v_signmask(v_cast_vec4f(v_cmp_lti(maxi,iuv4)));
            mask &= ((1<<count)-1);
            if ( mask!=0 ) context->throw_error_at(at,"rast_hspan: tspan out of range");
            auto i0 = uint32_t(v_extract_xi(iuv4));
            if ( auto b0 = tspan[i0] ) PSP[0] = solid;
            if ( count > 1 ) {
                auto i1 = uint32_t(v_extract_yi(iuv4));
                if ( auto b1 = tspan[i1] ) PSP[1] = solid;
                if ( count > 2 ) {
                    auto i2 = uint32_t(v_extract_zi(iuv4));
                    if ( auto b2 = tspan[i2] ) PSP[2] = solid;
                }
            }
        }
    }

    // ---- copy_reversed implementations (8 pixel sizes) ----

    template <typename T>
    static void rast_copy_reversed_impl ( T * dst, const T * src, int32_t count ) {
        for ( int32_t i = 0; i < count; ++i )
            dst[i] = src[count - 1 - i];
    }

    void rast_copy_reversed_1  ( uint8_t *d, const uint8_t *s, int32_t c )  { rast_copy_reversed_impl(d, s, c); }
    void rast_copy_reversed_2  ( uint16_t *d, const uint16_t *s, int32_t c ) { rast_copy_reversed_impl(d, s, c); }
    void rast_copy_reversed_4  ( uint32_t *d, const uint32_t *s, int32_t c ) { rast_copy_reversed_impl(d, s, c); }
    void rast_copy_reversed_8  ( uint64_t *d, const uint64_t *s, int32_t c ) { rast_copy_reversed_impl(d, s, c); }

    void rast_copy_reversed_3  ( void *d, void *s, int32_t c ) {
        rast_copy_reversed_impl((rast_rgb8 *)d, (const rast_rgb8 *)s, c);
    }
    void rast_copy_reversed_6  ( void *d, void *s, int32_t c ) {
        rast_copy_reversed_impl((rast_rgb16 *)d, (const rast_rgb16 *)s, c);
    }
    void rast_copy_reversed_12 ( void *d, void *s, int32_t c ) {
        rast_copy_reversed_impl((rast_float3 *)d, (const rast_float3 *)s, c);
    }
    void rast_copy_reversed_16 ( void *d, void *s, int32_t c ) {
        rast_copy_reversed_impl((rast_float4 *)d, (const rast_float4 *)s, c);
    }

    // ---- copy_rect implementations (8 pixel sizes) ----

    template <typename T>
    static void rast_copy_rect_impl ( T * dst, const T * src, int32_t dw, int32_t sw,
            int32_t sx0, int32_t sy0, int32_t dx0, int32_t dy0, int32_t w, int32_t h ) {
        for ( int32_t row = 0; row < h; ++row ) {
            T * dr = dst + (dy0 + row) * dw + dx0;
            const T * sr = src + (sy0 + row) * sw + sx0;
            for ( int32_t col = 0; col < w; ++col )
                dr[col] = sr[col];
        }
    }

    void rast_copy_rect_1  ( uint8_t *d, const uint8_t *s, int32_t dw, int32_t sw, int32_t sx0, int32_t sy0, int32_t dx0, int32_t dy0, int32_t w, int32_t h ) {
        rast_copy_rect_impl(d, s, dw, sw, sx0, sy0, dx0, dy0, w, h);
    }
    void rast_copy_rect_2  ( uint16_t *d, const uint16_t *s, int32_t dw, int32_t sw, int32_t sx0, int32_t sy0, int32_t dx0, int32_t dy0, int32_t w, int32_t h ) {
        rast_copy_rect_impl(d, s, dw, sw, sx0, sy0, dx0, dy0, w, h);
    }
    void rast_copy_rect_4  ( uint32_t *d, const uint32_t *s, int32_t dw, int32_t sw, int32_t sx0, int32_t sy0, int32_t dx0, int32_t dy0, int32_t w, int32_t h ) {
        rast_copy_rect_impl(d, s, dw, sw, sx0, sy0, dx0, dy0, w, h);
    }
    void rast_copy_rect_8  ( uint64_t *d, const uint64_t *s, int32_t dw, int32_t sw, int32_t sx0, int32_t sy0, int32_t dx0, int32_t dy0, int32_t w, int32_t h ) {
        rast_copy_rect_impl(d, s, dw, sw, sx0, sy0, dx0, dy0, w, h);
    }
    void rast_copy_rect_3  ( void *d, void *s, int32_t dw, int32_t sw, int32_t sx0, int32_t sy0, int32_t dx0, int32_t dy0, int32_t w, int32_t h ) {
        rast_copy_rect_impl((rast_rgb8 *)d, (const rast_rgb8 *)s, dw, sw, sx0, sy0, dx0, dy0, w, h);
    }
    void rast_copy_rect_6  ( void *d, void *s, int32_t dw, int32_t sw, int32_t sx0, int32_t sy0, int32_t dx0, int32_t dy0, int32_t w, int32_t h ) {
        rast_copy_rect_impl((rast_rgb16 *)d, (const rast_rgb16 *)s, dw, sw, sx0, sy0, dx0, dy0, w, h);
    }
    void rast_copy_rect_12 ( void *d, void *s, int32_t dw, int32_t sw, int32_t sx0, int32_t sy0, int32_t dx0, int32_t dy0, int32_t w, int32_t h ) {
        rast_copy_rect_impl((rast_float3 *)d, (const rast_float3 *)s, dw, sw, sx0, sy0, dx0, dy0, w, h);
    }
    void rast_copy_rect_16 ( void *d, void *s, int32_t dw, int32_t sw, int32_t sx0, int32_t sy0, int32_t dx0, int32_t dy0, int32_t w, int32_t h ) {
        rast_copy_rect_impl((rast_float4 *)d, (const rast_float4 *)s, dw, sw, sx0, sy0, dx0, dy0, w, h);
    }

    // ---- channel conversion (loops inside each branch) ----

    void rast_convert_channels_u8 ( uint8_t * dst, const uint8_t * src, int32_t num_pixels, int32_t src_ch, int32_t dst_ch ) {
        if ( src_ch == 1 && dst_ch == 2 ) {
            for ( int32_t i = 0; i < num_pixels; ++i ) { dst[i*2] = src[i]; dst[i*2+1] = 255; }
        } else if ( src_ch == 1 && dst_ch == 3 ) {
            for ( int32_t i = 0; i < num_pixels; ++i ) { dst[i*3] = src[i]; dst[i*3+1] = src[i]; dst[i*3+2] = src[i]; }
        } else if ( src_ch == 1 && dst_ch == 4 ) {
            for ( int32_t i = 0; i < num_pixels; ++i ) { dst[i*4] = src[i]; dst[i*4+1] = src[i]; dst[i*4+2] = src[i]; dst[i*4+3] = 255; }
        } else if ( src_ch == 2 && dst_ch == 1 ) {
            for ( int32_t i = 0; i < num_pixels; ++i ) { dst[i] = src[i*2]; }
        } else if ( src_ch == 2 && dst_ch == 3 ) {
            for ( int32_t i = 0; i < num_pixels; ++i ) { dst[i*3] = src[i*2]; dst[i*3+1] = src[i*2]; dst[i*3+2] = src[i*2]; }
        } else if ( src_ch == 2 && dst_ch == 4 ) {
            for ( int32_t i = 0; i < num_pixels; ++i ) { dst[i*4] = src[i*2]; dst[i*4+1] = src[i*2]; dst[i*4+2] = src[i*2]; dst[i*4+3] = src[i*2+1]; }
        } else if ( src_ch == 3 && dst_ch == 1 ) {
            for ( int32_t i = 0; i < num_pixels; ++i ) { dst[i] = uint8_t((uint32_t(src[i*3])*77u + uint32_t(src[i*3+1])*150u + uint32_t(src[i*3+2])*29u + 128u) >> 8u); }
        } else if ( src_ch == 3 && dst_ch == 2 ) {
            for ( int32_t i = 0; i < num_pixels; ++i ) { dst[i*2] = uint8_t((uint32_t(src[i*3])*77u + uint32_t(src[i*3+1])*150u + uint32_t(src[i*3+2])*29u + 128u) >> 8u); dst[i*2+1] = 255; }
        } else if ( src_ch == 3 && dst_ch == 4 ) {
            for ( int32_t i = 0; i < num_pixels; ++i ) { dst[i*4] = src[i*3]; dst[i*4+1] = src[i*3+1]; dst[i*4+2] = src[i*3+2]; dst[i*4+3] = 255; }
        } else if ( src_ch == 4 && dst_ch == 1 ) {
            for ( int32_t i = 0; i < num_pixels; ++i ) { dst[i] = uint8_t((uint32_t(src[i*4])*77u + uint32_t(src[i*4+1])*150u + uint32_t(src[i*4+2])*29u + 128u) >> 8u); }
        } else if ( src_ch == 4 && dst_ch == 2 ) {
            for ( int32_t i = 0; i < num_pixels; ++i ) { dst[i*2] = uint8_t((uint32_t(src[i*4])*77u + uint32_t(src[i*4+1])*150u + uint32_t(src[i*4+2])*29u + 128u) >> 8u); dst[i*2+1] = src[i*4+3]; }
        } else if ( src_ch == 4 && dst_ch == 3 ) {
            for ( int32_t i = 0; i < num_pixels; ++i ) { dst[i*3] = src[i*4]; dst[i*3+1] = src[i*4+1]; dst[i*3+2] = src[i*4+2]; }
        } else if ( src_ch == dst_ch ) {
            memcpy(dst, src, num_pixels * src_ch);
        }
    }

    void rast_convert_channels_u16 ( uint16_t * dst, const uint16_t * src, int32_t num_pixels, int32_t src_ch, int32_t dst_ch ) {
        if ( src_ch == 1 && dst_ch == 2 ) {
            for ( int32_t i = 0; i < num_pixels; ++i ) { dst[i*2] = src[i]; dst[i*2+1] = 65535; }
        } else if ( src_ch == 1 && dst_ch == 3 ) {
            for ( int32_t i = 0; i < num_pixels; ++i ) { dst[i*3] = src[i]; dst[i*3+1] = src[i]; dst[i*3+2] = src[i]; }
        } else if ( src_ch == 1 && dst_ch == 4 ) {
            for ( int32_t i = 0; i < num_pixels; ++i ) { dst[i*4] = src[i]; dst[i*4+1] = src[i]; dst[i*4+2] = src[i]; dst[i*4+3] = 65535; }
        } else if ( src_ch == 2 && dst_ch == 1 ) {
            for ( int32_t i = 0; i < num_pixels; ++i ) { dst[i] = src[i*2]; }
        } else if ( src_ch == 2 && dst_ch == 3 ) {
            for ( int32_t i = 0; i < num_pixels; ++i ) { dst[i*3] = src[i*2]; dst[i*3+1] = src[i*2]; dst[i*3+2] = src[i*2]; }
        } else if ( src_ch == 2 && dst_ch == 4 ) {
            for ( int32_t i = 0; i < num_pixels; ++i ) { dst[i*4] = src[i*2]; dst[i*4+1] = src[i*2]; dst[i*4+2] = src[i*2]; dst[i*4+3] = src[i*2+1]; }
        } else if ( src_ch == 3 && dst_ch == 1 ) {
            for ( int32_t i = 0; i < num_pixels; ++i ) { dst[i] = uint16_t((uint32_t(src[i*3])*77u + uint32_t(src[i*3+1])*150u + uint32_t(src[i*3+2])*29u + 128u) >> 8u); }
        } else if ( src_ch == 3 && dst_ch == 2 ) {
            for ( int32_t i = 0; i < num_pixels; ++i ) { dst[i*2] = uint16_t((uint32_t(src[i*3])*77u + uint32_t(src[i*3+1])*150u + uint32_t(src[i*3+2])*29u + 128u) >> 8u); dst[i*2+1] = 65535; }
        } else if ( src_ch == 3 && dst_ch == 4 ) {
            for ( int32_t i = 0; i < num_pixels; ++i ) { dst[i*4] = src[i*3]; dst[i*4+1] = src[i*3+1]; dst[i*4+2] = src[i*3+2]; dst[i*4+3] = 65535; }
        } else if ( src_ch == 4 && dst_ch == 1 ) {
            for ( int32_t i = 0; i < num_pixels; ++i ) { dst[i] = uint16_t((uint32_t(src[i*4])*77u + uint32_t(src[i*4+1])*150u + uint32_t(src[i*4+2])*29u + 128u) >> 8u); }
        } else if ( src_ch == 4 && dst_ch == 2 ) {
            for ( int32_t i = 0; i < num_pixels; ++i ) { dst[i*2] = uint16_t((uint32_t(src[i*4])*77u + uint32_t(src[i*4+1])*150u + uint32_t(src[i*4+2])*29u + 128u) >> 8u); dst[i*2+1] = src[i*4+3]; }
        } else if ( src_ch == 4 && dst_ch == 3 ) {
            for ( int32_t i = 0; i < num_pixels; ++i ) { dst[i*3] = src[i*4]; dst[i*3+1] = src[i*4+1]; dst[i*3+2] = src[i*4+2]; }
        } else if ( src_ch == dst_ch ) {
            memcpy(dst, src, num_pixels * src_ch * sizeof(uint16_t));
        }
    }

    void rast_convert_channels_f32 ( float * dst, const float * src, int32_t num_pixels, int32_t src_ch, int32_t dst_ch ) {
        if ( src_ch == 1 && dst_ch == 2 ) {
            for ( int32_t i = 0; i < num_pixels; ++i ) { dst[i*2] = src[i]; dst[i*2+1] = 1.0f; }
        } else if ( src_ch == 1 && dst_ch == 3 ) {
            for ( int32_t i = 0; i < num_pixels; ++i ) { dst[i*3] = src[i]; dst[i*3+1] = src[i]; dst[i*3+2] = src[i]; }
        } else if ( src_ch == 1 && dst_ch == 4 ) {
            for ( int32_t i = 0; i < num_pixels; ++i ) { dst[i*4] = src[i]; dst[i*4+1] = src[i]; dst[i*4+2] = src[i]; dst[i*4+3] = 1.0f; }
        } else if ( src_ch == 2 && dst_ch == 1 ) {
            for ( int32_t i = 0; i < num_pixels; ++i ) { dst[i] = src[i*2]; }
        } else if ( src_ch == 2 && dst_ch == 3 ) {
            for ( int32_t i = 0; i < num_pixels; ++i ) { dst[i*3] = src[i*2]; dst[i*3+1] = src[i*2]; dst[i*3+2] = src[i*2]; }
        } else if ( src_ch == 2 && dst_ch == 4 ) {
            for ( int32_t i = 0; i < num_pixels; ++i ) { dst[i*4] = src[i*2]; dst[i*4+1] = src[i*2]; dst[i*4+2] = src[i*2]; dst[i*4+3] = src[i*2+1]; }
        } else if ( src_ch == 3 && dst_ch == 1 ) {
            for ( int32_t i = 0; i < num_pixels; ++i ) { dst[i] = src[i*3]*0.299f + src[i*3+1]*0.587f + src[i*3+2]*0.114f; }
        } else if ( src_ch == 3 && dst_ch == 2 ) {
            for ( int32_t i = 0; i < num_pixels; ++i ) { dst[i*2] = src[i*3]*0.299f + src[i*3+1]*0.587f + src[i*3+2]*0.114f; dst[i*2+1] = 1.0f; }
        } else if ( src_ch == 3 && dst_ch == 4 ) {
            for ( int32_t i = 0; i < num_pixels; ++i ) { dst[i*4] = src[i*3]; dst[i*4+1] = src[i*3+1]; dst[i*4+2] = src[i*3+2]; dst[i*4+3] = 1.0f; }
        } else if ( src_ch == 4 && dst_ch == 1 ) {
            for ( int32_t i = 0; i < num_pixels; ++i ) { dst[i] = src[i*4]*0.299f + src[i*4+1]*0.587f + src[i*4+2]*0.114f; }
        } else if ( src_ch == 4 && dst_ch == 2 ) {
            for ( int32_t i = 0; i < num_pixels; ++i ) { dst[i*2] = src[i*4]*0.299f + src[i*4+1]*0.587f + src[i*4+2]*0.114f; dst[i*2+1] = src[i*4+3]; }
        } else if ( src_ch == 4 && dst_ch == 3 ) {
            for ( int32_t i = 0; i < num_pixels; ++i ) { dst[i*3] = src[i*4]; dst[i*3+1] = src[i*4+1]; dst[i*3+2] = src[i*4+2]; }
        } else if ( src_ch == dst_ch ) {
            memcpy(dst, src, num_pixels * src_ch * sizeof(float));
        }
    }

    // ---- fill rect ----

    void rast_fill_rect_1 ( uint8_t * dst, int32_t dw, int32_t dx, int32_t dy, int32_t w, int32_t h, uint8_t val ) {
        for ( int32_t row = 0; row < h; ++row ) {
            uint8_t * dr = dst + (dy + row) * dw + dx;
            memset(dr, val, w);
        }
    }

    void rast_fill_rect_4 ( uint32_t * dst, int32_t dw, int32_t dx, int32_t dy, int32_t w, int32_t h, uint32_t val ) {
        for ( int32_t row = 0; row < h; ++row ) {
            uint32_t * dr = dst + (dy + row) * dw + dx;
            for ( int32_t col = 0; col < w; ++col )
                dr[col] = val;
        }
    }

    // ---- alpha blend core (SIMD, 1 pixel at a time, 4 channels in parallel) ----
    //
    // out = (alpha * src + (255 - alpha) * dst + 128) >> 8
    //
    // This processes all 4 RGBA channels simultaneously using vec4i.
    // The same core works for both 1ch→4ch (constant color) and future 4ch→4ch blits.

    __forceinline void rast_blend_pixel ( uint32_t * dst_px, vec4i src_rgba, vec4i alpha4 ) {
        vec4i dst_rgba = v_cvt_byte_vec4i(*dst_px);
        vec4i v255 = v_splatsi(255);
        vec4i inv_alpha = v_subi(v255, alpha4);
        // exact integer divide by 255: (x + 1 + (x >> 8)) >> 8
        vec4i x = v_addi(v_muli(alpha4, src_rgba), v_muli(inv_alpha, dst_rgba));
        vec4i result = v_srli(v_addi(v_addi(x, v_splatsi(1)), v_srli(x, 8)), 8);
        // alpha channel: saturate(dst_a + src_a)
        vec4i out_a = v_mini(v_addi(dst_rgba, alpha4), v255);
        // replace .w with saturated alpha
        vec4i mask = v_make_vec4i(-1, -1, -1, 0);
        result = v_ori(v_andi(result, mask), v_andi(out_a, v_xori(mask, v_splatsi(-1))));
        // pack int32 -> int16 -> uint8
        vec4i packed = v_packus16(v_packs(result), v_packs(result));
        *dst_px = uint32_t(v_extract_xi(packed));
    }

    // ---- alpha blit (1ch alpha source onto 4ch RGBA dest with color) ----

    void rast_blit_alpha ( uint8_t * dst, const uint8_t * src, int32_t dw, int32_t sw,
            int32_t sx0, int32_t sy0, int32_t dx0, int32_t dy0, int32_t w, int32_t h,
            uint8_t r, uint8_t g, uint8_t b ) {
        vec4i src_rgba = v_make_vec4i(r, g, b, 255);
        for ( int32_t row = 0; row < h; ++row ) {
            const uint8_t * sr = src + (sy0 + row) * sw + sx0;
            uint32_t * dr = (uint32_t *)(dst + ((dy0 + row) * dw + dx0) * 4);
            for ( int32_t col = 0; col < w; ++col ) {
                uint32_t a = sr[col];
                if ( a == 0 ) continue;
                vec4i alpha4 = v_splatsi(a);
                rast_blend_pixel(dr + col, src_rgba, alpha4);
            }
        }
    }

    // ---- bpc conversion ----

    void rast_convert_u8_to_f32 ( float * dst, const uint8_t * src, int32_t count ) {
        for ( int32_t i = 0; i < count; ++i ) dst[i] = float(src[i]) / 255.0f;
    }
    void rast_convert_f32_to_u8 ( uint8_t * dst, const float * src, int32_t count ) {
        for ( int32_t i = 0; i < count; ++i ) {
            float v = src[i] < 0.0f ? 0.0f : (src[i] > 1.0f ? 1.0f : src[i]);
            dst[i] = uint8_t(v * 255.0f + 0.5f);
        }
    }
    void rast_convert_u8_to_u16 ( uint16_t * dst, const uint8_t * src, int32_t count ) {
        for ( int32_t i = 0; i < count; ++i ) dst[i] = uint16_t(src[i]) * 257u;
    }
    void rast_convert_u16_to_u8 ( uint8_t * dst, const uint16_t * src, int32_t count ) {
        for ( int32_t i = 0; i < count; ++i ) dst[i] = uint8_t((uint32_t(src[i]) + 128u) / 257u);
    }
    void rast_convert_u16_to_f32 ( float * dst, const uint16_t * src, int32_t count ) {
        for ( int32_t i = 0; i < count; ++i ) dst[i] = float(src[i]) / 65535.0f;
    }
    void rast_convert_f32_to_u16 ( uint16_t * dst, const float * src, int32_t count ) {
        for ( int32_t i = 0; i < count; ++i ) {
            float v = src[i] < 0.0f ? 0.0f : (src[i] > 1.0f ? 1.0f : src[i]);
            dst[i] = uint16_t(v * 65535.0f + 0.5f);
        }
    }

    class Module_Raster : public Module {
    public:
        Module_Raster() : Module("raster") {
            ModuleLibrary lib(this);
            lib.addBuiltInModule();
            // gather
            addExternEx<float4(const float *,uint4),DAS_BIND_FUN(v_gather)>(*this, lib, "gather",
                SideEffects::none,"v_gather")->args({"from","index4"})->unsafeOperation = true;
            addExternEx<int4(const int32_t *,uint4),DAS_BIND_FUN(v_gather)>(*this, lib, "gather",
                SideEffects::none,"v_gather")->args({"from","index4"})->unsafeOperation = true;
            addExternEx<uint4(const uint32_t *,uint4),DAS_BIND_FUN(v_gather)>(*this, lib, "gather",
                SideEffects::none,"v_gather")->args({"from","index4"})->unsafeOperation = true;

            addExternEx<float4(const float *,int4),DAS_BIND_FUN(v_gather)>(*this, lib, "gather",
                SideEffects::none,"v_gather")->args({"from","index4"})->unsafeOperation = true;
            addExternEx<int4(const int32_t *,int4),DAS_BIND_FUN(v_gather)>(*this, lib, "gather",
                SideEffects::none,"v_gather")->args({"from","index4"})->unsafeOperation = true;
            addExternEx<uint4(const uint32_t *,int4),DAS_BIND_FUN(v_gather)>(*this, lib, "gather",
                SideEffects::none,"v_gather")->args({"from","index4"})->unsafeOperation = true;
            // scatter
            addExternEx<void(float *,uint4,float4),DAS_BIND_FUN(v_scatter)>(*this, lib, "scatter",
                SideEffects::modifyArgument,"v_scatter")->args({"to","index4","from"})->unsafeOperation = true;
            addExternEx<void(int32_t *,uint4,int4),DAS_BIND_FUN(v_scatter)>(*this, lib, "scatter",
                SideEffects::modifyArgument,"v_scatter")->args({"to","index4","from"})->unsafeOperation = true;
            addExternEx<void(uint32_t *,uint4,uint4),DAS_BIND_FUN(v_scatter)>(*this, lib, "scatter",
                SideEffects::modifyArgument,"v_scatter")->args({"to","index4","from"})->unsafeOperation = true;

            addExternEx<void(float *,int4,float4),DAS_BIND_FUN(v_scatter)>(*this, lib, "scatter",
                SideEffects::modifyArgument,"v_scatter")->args({"to","index4","from"})->unsafeOperation = true;
            addExternEx<void(int32_t *,int4,int4),DAS_BIND_FUN(v_scatter)>(*this, lib, "scatter",
                SideEffects::modifyArgument,"v_scatter")->args({"to","index4","from"})->unsafeOperation = true;
            addExternEx<void(uint32_t *,int4,uint4),DAS_BIND_FUN(v_scatter)>(*this, lib, "scatter",
                SideEffects::modifyArgument,"v_scatter")->args({"to","index4","from"})->unsafeOperation = true;
            // scatter mask
            addExternEx<void(float *,uint4,float4,float4),DAS_BIND_FUN(v_scatter_mask)>(*this, lib, "scatter_neq_mask",
                SideEffects::modifyArgument,"v_scatter_mask")->args({"to","index4","from","mask"})->unsafeOperation = true;
            addExternEx<void(int32_t *,uint4,int4,int4),DAS_BIND_FUN(v_scatter_mask)>(*this, lib, "scatter_neq_mask",
                SideEffects::modifyArgument,"v_scatter_mask")->args({"to","index4","from","mask"})->unsafeOperation = true;
            addExternEx<void(uint32_t *,uint4,uint4,uint4),DAS_BIND_FUN(v_scatter_mask)>(*this, lib, "scatter_neq_mask",
                SideEffects::modifyArgument,"v_scatter_mask")->args({"to","index4","from","mask"})->unsafeOperation = true;

            addExternEx<void(float *,int4,float4,float4),DAS_BIND_FUN(v_scatter_mask)>(*this, lib, "scatter_neq_mask",
                SideEffects::modifyArgument,"v_scatter_mask")->args({"to","index4","from","mask"})->unsafeOperation = true;
            addExternEx<void(int32_t *,int4,int4,int4),DAS_BIND_FUN(v_scatter_mask)>(*this, lib, "scatter_neq_mask",
                SideEffects::modifyArgument,"v_scatter_mask")->args({"to","index4","from","mask"})->unsafeOperation = true;
            addExternEx<void(uint32_t *,int4,uint4,uint4),DAS_BIND_FUN(v_scatter_mask)>(*this, lib, "scatter_neq_mask",
                SideEffects::modifyArgument,"v_scatter_mask")->args({"to","index4","from","mask"})->unsafeOperation = true;
            // store mask
            addExternEx<void(float4 *,float4,float4),DAS_BIND_FUN(v_store_mask)>(*this, lib, "store_neq_mask",
                SideEffects::modifyArgument,"v_store_mask")->args({"to","from","mask"});
            addExternEx<void(int4 *,int4,int4),DAS_BIND_FUN(v_store_mask)>(*this, lib, "store_neq_mask",
                SideEffects::modifyArgument,"v_store_mask")->args({"to","from","mask"});
            addExternEx<void(uint4 *,uint4,uint4),DAS_BIND_FUN(v_store_mask)>(*this, lib, "store_neq_mask",
                SideEffects::modifyArgument,"v_store_mask")->args({"to","from","mask"});
            // gather-scatter
            addExternEx<void(float *,uint4,const float *,uint4),DAS_BIND_FUN(v_gather_scatter)>(*this, lib, "gather_scatter",
                SideEffects::modifyArgument,"v_gather_scatter")->args({"to","to_index4","from","from_index4"})->unsafeOperation = true;
            addExternEx<void(int32_t *,uint4,const int32_t *,uint4),DAS_BIND_FUN(v_gather_scatter)>(*this, lib, "gather_scatter",
                SideEffects::modifyArgument,"v_gather_scatter")->args({"to","to_index4","from","from_index4"})->unsafeOperation = true;
            addExternEx<void(uint32_t *,uint4,const uint32_t *,uint4),DAS_BIND_FUN(v_gather_scatter)>(*this, lib, "gather_scatter",
                SideEffects::modifyArgument,"v_gather_scatter")->args({"to","to_index4","from","from_index4"})->unsafeOperation = true;

            addExternEx<void(float *,int4,const float *,int4),DAS_BIND_FUN(v_gather_scatter)>(*this, lib, "gather_scatter",
                SideEffects::modifyArgument,"v_gather_scatter")->args({"to","to_index4","from","from_index4"})->unsafeOperation = true;
            addExternEx<void(int32_t *,int4,const int32_t *,int4),DAS_BIND_FUN(v_gather_scatter)>(*this, lib, "gather_scatter",
                SideEffects::modifyArgument,"v_gather_scatter")->args({"to","to_index4","from","from_index4"})->unsafeOperation = true;
            addExternEx<void(uint32_t *,int4,const uint32_t *,int4),DAS_BIND_FUN(v_gather_scatter)>(*this, lib, "gather_scatter",
                SideEffects::modifyArgument,"v_gather_scatter")->args({"to","to_index4","from","from_index4"})->unsafeOperation = true;
            // gather-scatter mask
            addExternEx<void(float *,uint4,const float *,uint4,float4),DAS_BIND_FUN(v_gather_scatter_mask)>(*this, lib, "gather_scatter_neq_mask",
                SideEffects::modifyArgument,"v_gather_scatter_mask")->args({"to","to_index4","from","from_index4","mask"})->unsafeOperation = true;
            addExternEx<void(int32_t *,uint4,const int32_t *,uint4,int4),DAS_BIND_FUN(v_gather_scatter_mask)>(*this, lib, "gather_scatter_neq_mask",
                SideEffects::modifyArgument,"v_gather_scatter_mask")->args({"to","to_index4","from","from_index4","mask"})->unsafeOperation = true;
            addExternEx<void(uint32_t *,uint4,const uint32_t *,uint4,uint4),DAS_BIND_FUN(v_gather_scatter_mask)>(*this, lib, "gather_scatter_neq_mask",
                SideEffects::modifyArgument,"v_gather_scatter_mask")->args({"to","to_index4","from","from_index4","mask"})->unsafeOperation = true;

            addExternEx<void(float *,int4,const float *,int4,float4),DAS_BIND_FUN(v_gather_scatter_mask)>(*this, lib, "gather_scatter_neq_mask",
                SideEffects::modifyArgument,"v_gather_scatter_mask")->args({"to","to_index4","from","from_index4","mask"})->unsafeOperation = true;
            addExternEx<void(int32_t *,int4,const int32_t *,int4,int4),DAS_BIND_FUN(v_gather_scatter_mask)>(*this, lib, "gather_scatter_neq_mask",
                SideEffects::modifyArgument,"v_gather_scatter_mask")->args({"to","to_index4","from","from_index4","mask"})->unsafeOperation = true;
            addExternEx<void(uint32_t *,int4,const uint32_t *,int4,uint4),DAS_BIND_FUN(v_gather_scatter_mask)>(*this, lib, "gather_scatter_neq_mask",
                SideEffects::modifyArgument,"v_gather_scatter_mask")->args({"to","to_index4","from","from_index4","mask"})->unsafeOperation = true;
            // gather-store-mask
            addExternEx<void(float4 *,const float *,uint4,float4),DAS_BIND_FUN(v_gather_store_mask)>(*this, lib, "gather_store_neq_mask",
                SideEffects::modifyArgument,"v_gather_store_mask")->args({"to","from","from_index4","mask"})->unsafeOperation = true;
            addExternEx<void(int4 *,const int32_t *,uint4,int4),DAS_BIND_FUN(v_gather_store_mask)>(*this, lib, "gather_store_neq_mask",
                SideEffects::modifyArgument,"v_gather_store_mask")->args({"to","from","from_index4","mask"})->unsafeOperation = true;
            addExternEx<void(uint4 *,const uint32_t *,uint4,uint4),DAS_BIND_FUN(v_gather_store_mask)>(*this, lib, "gather_store_neq_mask",
                SideEffects::modifyArgument,"v_gather_store_mask")->args({"to","from","from_index4","mask"})->unsafeOperation = true;

            addExternEx<void(float4 *,const float *,int4,float4),DAS_BIND_FUN(v_gather_store_mask)>(*this, lib, "gather_store_neq_mask",
                SideEffects::modifyArgument,"v_gather_store_mask")->args({"to","from","from_index4","mask"})->unsafeOperation = true;
            addExternEx<void(int4 *,const int32_t *,int4,int4),DAS_BIND_FUN(v_gather_store_mask)>(*this, lib, "gather_store_neq_mask",
                SideEffects::modifyArgument,"v_gather_store_mask")->args({"to","from","from_index4","mask"})->unsafeOperation = true;
            addExternEx<void(uint4 *,const uint32_t *,int4,uint4),DAS_BIND_FUN(v_gather_store_mask)>(*this, lib, "gather_store_neq_mask",
                SideEffects::modifyArgument,"v_gather_store_mask")->args({"to","from","from_index4","mask"})->unsafeOperation = true;            // lets make sure its all aot ready
            verifyAotReady();
            // gather-store-with stride
            addExternEx<void(float *,int32_t,const float *,uint4),DAS_BIND_FUN(v_gather_store_stride)>(*this, lib, "gather_store_stride",
                SideEffects::modifyArgument,"v_gather_store_stride")->args({"to","stride","from","from_index4"})->unsafeOperation = true;
            addExternEx<void(int32_t *,int32_t,const int32_t *,uint4),DAS_BIND_FUN(v_gather_store_stride)>(*this, lib, "gather_store_stride",
                SideEffects::modifyArgument,"v_gather_store_stride")->args({"to","stride","from","from_index4"})->unsafeOperation = true;
            addExternEx<void(uint32_t *,int32_t,const uint32_t *,uint4),DAS_BIND_FUN(v_gather_store_stride)>(*this, lib, "gather_store_stride",
                SideEffects::modifyArgument,"v_gather_store_stride")->args({"to","stride","from","from_index4"})->unsafeOperation = true;

            addExternEx<void(float *,int32_t,const float *,int4),DAS_BIND_FUN(v_gather_store_stride)>(*this, lib, "gather_store_stride",
                SideEffects::modifyArgument,"v_gather_store_stride")->args({"to","stride","from","from_index4"})->unsafeOperation = true;
            addExternEx<void(int32_t *,int32_t,const int32_t *,int4),DAS_BIND_FUN(v_gather_store_stride)>(*this, lib, "gather_store_stride",
                SideEffects::modifyArgument,"v_gather_store_stride")->args({"to","stride","from","from_index4"})->unsafeOperation = true;
            addExternEx<void(uint32_t *,int32_t,const uint32_t *,int4),DAS_BIND_FUN(v_gather_store_stride)>(*this, lib, "gather_store_stride",
                SideEffects::modifyArgument,"v_gather_store_stride")->args({"to","stride","from","from_index4"})->unsafeOperation = true;            // lets make sure its all aot ready
            // gather-store u8x4
            addExternEx<void(uint8_t *,const uint8_t *,uint4),DAS_BIND_FUN(u8x4_gather_store)>(*this, lib, "u8x4_gather_store",
                SideEffects::modifyArgument,"u8x4_gather_store")->args({"to","from","from_index4"})->unsafeOperation = true;

            addExternEx<void(uint8_t *,const uint8_t *,int4),DAS_BIND_FUN(u8x4_gather_store)>(*this, lib, "u8x4_gather_store",
                SideEffects::modifyArgument,"u8x4_gather_store")->args({"to","from","from_index4"})->unsafeOperation = true;
            // typed copy helpers (8 pixel sizes)
            addExtern<DAS_BIND_FUN(rast_copy_reversed_1)>(*this, lib, "rast_copy_reversed_1",
                SideEffects::modifyArgument,"rast_copy_reversed_1")->args({"dst","src","count"})->unsafeOperation = true;
            addExtern<DAS_BIND_FUN(rast_copy_reversed_2)>(*this, lib, "rast_copy_reversed_2",
                SideEffects::modifyArgument,"rast_copy_reversed_2")->args({"dst","src","count"})->unsafeOperation = true;
            addExternEx<void(void*,void*,int32_t),DAS_BIND_FUN(rast_copy_reversed_3)>(*this, lib, "rast_copy_reversed_3",
                SideEffects::modifyExternal,"rast_copy_reversed_3")->args({"dst","src","count"})->unsafeOperation = true;
            addExtern<DAS_BIND_FUN(rast_copy_reversed_4)>(*this, lib, "rast_copy_reversed_4",
                SideEffects::modifyArgument,"rast_copy_reversed_4")->args({"dst","src","count"})->unsafeOperation = true;
            addExternEx<void(void*,void*,int32_t),DAS_BIND_FUN(rast_copy_reversed_6)>(*this, lib, "rast_copy_reversed_6",
                SideEffects::modifyExternal,"rast_copy_reversed_6")->args({"dst","src","count"})->unsafeOperation = true;
            addExtern<DAS_BIND_FUN(rast_copy_reversed_8)>(*this, lib, "rast_copy_reversed_8",
                SideEffects::modifyArgument,"rast_copy_reversed_8")->args({"dst","src","count"})->unsafeOperation = true;
            addExternEx<void(void*,void*,int32_t),DAS_BIND_FUN(rast_copy_reversed_12)>(*this, lib, "rast_copy_reversed_12",
                SideEffects::modifyExternal,"rast_copy_reversed_12")->args({"dst","src","count"})->unsafeOperation = true;
            addExternEx<void(void*,void*,int32_t),DAS_BIND_FUN(rast_copy_reversed_16)>(*this, lib, "rast_copy_reversed_16",
                SideEffects::modifyExternal,"rast_copy_reversed_16")->args({"dst","src","count"})->unsafeOperation = true;
            addExtern<DAS_BIND_FUN(rast_copy_rect_1)>(*this, lib, "rast_copy_rect_1",
                SideEffects::modifyArgument,"rast_copy_rect_1")->args({"dst","src","dst_w","src_w","sx0","sy0","dx0","dy0","w","h"})->unsafeOperation = true;
            addExtern<DAS_BIND_FUN(rast_copy_rect_2)>(*this, lib, "rast_copy_rect_2",
                SideEffects::modifyArgument,"rast_copy_rect_2")->args({"dst","src","dst_w","src_w","sx0","sy0","dx0","dy0","w","h"})->unsafeOperation = true;
            addExternEx<void(void*,void*,int32_t,int32_t,int32_t,int32_t,int32_t,int32_t,int32_t,int32_t),DAS_BIND_FUN(rast_copy_rect_3)>(*this, lib, "rast_copy_rect_3",
                SideEffects::modifyExternal,"rast_copy_rect_3")->args({"dst","src","dst_w","src_w","sx0","sy0","dx0","dy0","w","h"})->unsafeOperation = true;
            addExtern<DAS_BIND_FUN(rast_copy_rect_4)>(*this, lib, "rast_copy_rect_4",
                SideEffects::modifyArgument,"rast_copy_rect_4")->args({"dst","src","dst_w","src_w","sx0","sy0","dx0","dy0","w","h"})->unsafeOperation = true;
            addExternEx<void(void*,void*,int32_t,int32_t,int32_t,int32_t,int32_t,int32_t,int32_t,int32_t),DAS_BIND_FUN(rast_copy_rect_6)>(*this, lib, "rast_copy_rect_6",
                SideEffects::modifyExternal,"rast_copy_rect_6")->args({"dst","src","dst_w","src_w","sx0","sy0","dx0","dy0","w","h"})->unsafeOperation = true;
            addExtern<DAS_BIND_FUN(rast_copy_rect_8)>(*this, lib, "rast_copy_rect_8",
                SideEffects::modifyArgument,"rast_copy_rect_8")->args({"dst","src","dst_w","src_w","sx0","sy0","dx0","dy0","w","h"})->unsafeOperation = true;
            addExternEx<void(void*,void*,int32_t,int32_t,int32_t,int32_t,int32_t,int32_t,int32_t,int32_t),DAS_BIND_FUN(rast_copy_rect_12)>(*this, lib, "rast_copy_rect_12",
                SideEffects::modifyExternal,"rast_copy_rect_12")->args({"dst","src","dst_w","src_w","sx0","sy0","dx0","dy0","w","h"})->unsafeOperation = true;
            addExternEx<void(void*,void*,int32_t,int32_t,int32_t,int32_t,int32_t,int32_t,int32_t,int32_t),DAS_BIND_FUN(rast_copy_rect_16)>(*this, lib, "rast_copy_rect_16",
                SideEffects::modifyExternal,"rast_copy_rect_16")->args({"dst","src","dst_w","src_w","sx0","sy0","dx0","dy0","w","h"})->unsafeOperation = true;
            // pixel format conversion
            addExtern<DAS_BIND_FUN(rast_convert_channels_u8)>(*this, lib, "rast_convert_channels_u8",
                SideEffects::modifyArgument,"rast_convert_channels_u8")->args({"dst","src","num_pixels","src_ch","dst_ch"})->unsafeOperation = true;
            addExtern<DAS_BIND_FUN(rast_convert_channels_u16)>(*this, lib, "rast_convert_channels_u16",
                SideEffects::modifyArgument,"rast_convert_channels_u16")->args({"dst","src","num_pixels","src_ch","dst_ch"})->unsafeOperation = true;
            addExtern<DAS_BIND_FUN(rast_convert_channels_f32)>(*this, lib, "rast_convert_channels_f32",
                SideEffects::modifyArgument,"rast_convert_channels_f32")->args({"dst","src","num_pixels","src_ch","dst_ch"})->unsafeOperation = true;
            addExtern<DAS_BIND_FUN(rast_convert_u8_to_f32)>(*this, lib, "rast_convert_u8_to_f32",
                SideEffects::modifyArgument,"rast_convert_u8_to_f32")->args({"dst","src","count"})->unsafeOperation = true;
            addExtern<DAS_BIND_FUN(rast_convert_f32_to_u8)>(*this, lib, "rast_convert_f32_to_u8",
                SideEffects::modifyArgument,"rast_convert_f32_to_u8")->args({"dst","src","count"})->unsafeOperation = true;
            addExtern<DAS_BIND_FUN(rast_convert_u8_to_u16)>(*this, lib, "rast_convert_u8_to_u16",
                SideEffects::modifyArgument,"rast_convert_u8_to_u16")->args({"dst","src","count"})->unsafeOperation = true;
            addExtern<DAS_BIND_FUN(rast_convert_u16_to_u8)>(*this, lib, "rast_convert_u16_to_u8",
                SideEffects::modifyArgument,"rast_convert_u16_to_u8")->args({"dst","src","count"})->unsafeOperation = true;
            addExtern<DAS_BIND_FUN(rast_convert_u16_to_f32)>(*this, lib, "rast_convert_u16_to_f32",
                SideEffects::modifyArgument,"rast_convert_u16_to_f32")->args({"dst","src","count"})->unsafeOperation = true;
            addExtern<DAS_BIND_FUN(rast_convert_f32_to_u16)>(*this, lib, "rast_convert_f32_to_u16",
                SideEffects::modifyArgument,"rast_convert_f32_to_u16")->args({"dst","src","count"})->unsafeOperation = true;
            // fill rect
            addExtern<DAS_BIND_FUN(rast_fill_rect_1)>(*this, lib, "rast_fill_rect_1",
                SideEffects::modifyArgument,"rast_fill_rect_1")->args({"dst","dst_w","dx","dy","w","h","val"})->unsafeOperation = true;
            addExtern<DAS_BIND_FUN(rast_fill_rect_4)>(*this, lib, "rast_fill_rect_4",
                SideEffects::modifyArgument,"rast_fill_rect_4")->args({"dst","dst_w","dx","dy","w","h","val"})->unsafeOperation = true;
            // alpha blit
            addExtern<DAS_BIND_FUN(rast_blit_alpha)>(*this, lib, "rast_blit_alpha",
                SideEffects::modifyArgument,"rast_blit_alpha")->args({"dst","src","dst_w","src_w","sx0","sy0","dx0","dy0","w","h","r","g","b"})->unsafeOperation = true;
            // span rasters
            addExtern<DAS_BIND_FUN(rast_hspan_u8)>(*this, lib, "rast_hspan_u8", SideEffects::modifyArgument,"rast_hspan_u8")
                ->args({"span","spanOffset","tspan","tspanOffset","uvY","dUVY","count","at","context"});
            addExtern<DAS_BIND_FUN(rast_hspan_masked_u8)>(*this, lib, "rast_hspan_masked_u8", SideEffects::modifyArgument,"rast_hspan_masked_u8")
                ->args({"span","spanOffset","tspan","tspanOffset","uvY","dUVY","count","at","context"});
            addExtern<DAS_BIND_FUN(rast_hspan_masked_solid_u8)>(*this, lib, "rast_hspan_masked_solid_u8", SideEffects::modifyArgument,"rast_hspan_masked_solid_u8")
                ->args({"solid","span","spanOffset","tspan","tspanOffset","uvY","dUVY","count","at","context"});
            // lets make sure its all aot ready
            verifyAotReady();
        }
        virtual ModuleAotType aotRequire ( TextWriter & tw ) const override {
            tw << "#include \"../modules/dasStbImage/src/aot_builtin_raster.h\"\n";
            return ModuleAotType::cpp;
        }
    };

REGISTER_DYN_MODULE(Module_Raster,Module_Raster);

}

REGISTER_MODULE_IN_NAMESPACE(Module_Raster,das);
