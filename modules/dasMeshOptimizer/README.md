# dasMeshOptimizer

Shared, backend-neutral bindings to meshoptimizer **1.2** (MIT). CMake downloads
upstream commit `9d9890c73011d75920af614485296d1e03e95448` through `ExternalProject_Add`
and verifies its SHA-256. Sources stay in the build directory and compile directly
into the module archives with the parent toolchain, including WASM. The upstream
license is retained as `MESHOPTIMIZER.LICENSE`.

`require meshoptimizer` exposes checked array adapters for vertex remapping,
vertex-cache optimization, vertex-fetch optimization, and vertex/index codecs.
The initial API operates on triangle lists, uint32 indices and packed byte streams
with a 4-byte-aligned stride up to 256 bytes. It rejects invalid buffer dimensions
and indices before calling the library. Decode functions return false and clear
the output on malformed encoded data.

The processing order and vertex layout belong to the caller. No glTF, rendering,
LOD, or application policy lives in this module. See `modules/dasGLTF/gltf/gltf_processed.das`
for a consumer. Translucent materials preserve their triangle order there.

The same bindings build as a native shared module and a WASM archive. The public
wrapper header supports daslang AOT/JIT. `daspkg build --wasm` includes this archive.

Tests: `tests/test_meshoptimizer.das` covers deduplication, optimization, exact
packed-vertex codec round trips, oriented triangles and corrupt-input rejection.
