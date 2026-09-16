#pragma once
#include "daScript/simulate/aot.h"
namespace das {
uint32_t mo_remap(TArray<uint32_t>& remap, const TArray<uint32_t>& indices, const TArray<uint8_t>& vertices, uint32_t stride, Context*, LineInfoArg*);
void mo_remap_vertices(TArray<uint8_t>& output, const TArray<uint8_t>& vertices, const TArray<uint32_t>& remap, uint32_t stride, uint32_t count, Context*, LineInfoArg*);
void mo_remap_indices(TArray<uint32_t>& output, const TArray<uint32_t>& indices, const TArray<uint32_t>& remap, Context*, LineInfoArg*);
void mo_cache(TArray<uint32_t>& indices, uint32_t count, Context*, LineInfoArg*);
uint32_t mo_fetch(TArray<uint32_t>& indices, TArray<uint8_t>& vertices, uint32_t stride, Context*, LineInfoArg*);
void mo_encode_vertices(TArray<uint8_t>& output, const TArray<uint8_t>& vertices, uint32_t stride, Context*, LineInfoArg*);
bool mo_decode_vertices(TArray<uint8_t>& output, const TArray<uint8_t>& encoded, uint32_t count, uint32_t stride, Context*, LineInfoArg*);
void mo_encode_indices(TArray<uint8_t>& output, const TArray<uint32_t>& indices, uint32_t count, Context*, LineInfoArg*);
bool mo_decode_indices(TArray<uint32_t>& output, const TArray<uint8_t>& encoded, uint32_t count, Context*, LineInfoArg*);
}
