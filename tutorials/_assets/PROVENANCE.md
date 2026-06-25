# tutorials/_assets — shared tutorial assets (all CC0)

Every asset here is **CC0 / public domain** (free to use, no attribution required). Sources are
recorded for provenance and so the set can be regenerated.

## brick/ — ambientCG "Bricks031" (CC0)
- Source: https://ambientcg.com/view?id=Bricks031 (downloaded `Bricks031_1K-JPG.zip`)
- Kept maps: `_Color` (albedo), `_NormalGL` (OpenGL-convention normal), `_Roughness`, `_AmbientOcclusion`.
- Seamless / tileable. Used for the cube (tut 8) and as a general PBR material.

## hdri/cannon_2k.hdr — Poly Haven "Cannon" by Greg Zaal (CC0)
- Source: https://polyhaven.com/a/cannon
- 2K equirectangular Radiance HDR. Soft overcast outdoor light — forgiving IBL / skybox reflections.
- Used for environment reflections (skybox tut 6, teapot tut 14) and image-based lighting.

## cat/ — Poly Haven "Concrete Cat Statue" (CC0)
- Source: https://polyhaven.com/a/concrete_cat_statue
- `concrete_cat_statue.obj` (6863 verts / 11950 tris) converted from the 2K glTF by `gltf_to_obj.py`
  (a dependency-free glTF→OBJ extractor — re-run after re-downloading the glTF + .bin to regenerate).
- `textures/concrete_cat_statue_{diff,nor_gl,arm}_2k.jpg` — diffuse, OpenGL normal, and ARM
  (Ambient-occlusion / Roughness / Metallic packed in R/G/B). Organic form with deep concavities —
  good for normal mapping + screen-space AO (deferred tut 10).
