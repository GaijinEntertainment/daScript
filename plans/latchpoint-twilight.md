# Latchpoint twilight graphics pass

## Implemented

- Twilight sky with a warm horizon, advected procedural clouds and distant storm illumination.
  A short jagged bolt accompanies two separated pulses in an approximately 47-second storm
  cycle whose onset varies by cycle. `L` disables both bolt and illumination completely.
- Twelve local lights: three robot sources, salvage hanging lamp, coil task lamp, workshop,
  archive, generator status, two perimeter lamps, and two door indicators. The moving salvage
  lamp has a separate 1024 shadow map; the directional light retains its 2048 map.
  Other local lights currently have no shadow maps and can leak through intervening objects.
- A wind-driven hanging fixture and 1,400 instanced dust billboards. A 20-step scattering pass
  adds restrained dusty beams for the hanging lamp and two robot inspection lights. Only the
  hanging lamp's scattering samples its shadow map. This is not volumetric global illumination.
- A 128-pixel-per-face cubemap captured from the static yard, with box-projected reflections
  and roughness-scaled mip sampling. Capture excludes moving robots and refreshes on the
  power/door state key or renderer initialization. Ordinary mipmaps approximate roughness
  filtering; they are not GGX-convolved prefiltering or a split-sum BRDF integration.
- Optional 32-step screen-space reflections with screen-edge/range fading and a probe-based
  background. This is a bounded, approximate contribution, not an energy-exact replacement
  of the indirect specular term. Off-screen objects and thin geometry remain limitations.
- Procedural rust/roughness variation and surface-gradient normal detail. Floor tread receives
  eight-layer shallow parallax with distance/footprint fading. These are shader-generated
  fields, not image-backed normal maps, and do not alter silhouettes or collision.
- More geometry: beveled casings/crates, denser spheres/cylinders, fittings, supports, bench
  cradles, pipework and archive panels. Generator grille faces the open yard, with the status
  light in a separate housing clear of its bars. Archive side-wall visuals stop behind the
  front wall to remove the reported overlap without changing gameplay collision.

## Shared additions

`geom_gen::gen_beveled_box(half_size, radius, segments)` produces indexed rounded-box geometry
with outward normals and per-face UVs. Extents and subdivision counts are bounded; radius
zero returns a scaled sharp box. Latchpoint uses a normalized instance scaled per part, so
its bevel widths scale with each part; callers needing a uniform physical bevel can supply
the final extents directly to the generator.

`shader_lingua_franca` adds the missing explicit-LOD cubemap sampling overload. It follows
the existing lowered-by-name texture stubs. The live OpenGL shader exercises this path;
Vulkan and WebGL runtime validation have not been performed in this pass.

## Controls and persistence

Outside text entry, `L` toggles storm flashes, `F6` toggles SSR, and `F7` toggles dust/scattering.
The Escape menu shows all three settings. `saves/visual-settings.json` retains them across
restart and New Game. Robot/workshop lights and probe reflections remain when SSR is off.

Live `cmd_render_settings` also accepts `reduced_flashes`, `ssr`, `dust` and deterministic
`time`; these inspection changes are not themselves written to preferences.
`cmd_lighting_state` reports source counts, probe readiness, flash intensity and lamp pose.

## Verification and limitations

- 37 daslang test cases pass, including generated cubemap shader text, rounded-mesh counts,
  normals/winding/bounds, extreme generator inputs, storm limits and disabled flashes,
  lamp movement and robot/generator light state. 59 Python tests remain green.
- Live views inspected at salvage, the main yard, generator front, coil bench and archive.
  HDR readbacks in the inspected views reported no invalid half-float values and no GL errors.
  This does not establish that every previously reported intermittent black-square glitch
  is gone. Lighting outputs are bounded, and the half-vector denominator is guarded.
- Reduced-flash keyboard behavior was checked at an active deterministic storm time and
  remained disabled through hot reload. Shadow/probe/atmosphere framebuffers validate
  completeness at creation. New targets are released on reload/shutdown; atmosphere targets
  recreate on resize, and non-float color targets follow the existing HDR fallback.
- Live host remained around its 120 Hz cap at the inspected 1280x800 view. This is not GPU
  timing, a worst-case performance result, or a simultaneous-inference benchmark. No claim
  is made that the GPU is saturated; rendering quality, not artificial load, is the target.

Gameplay dialogue and audio implementation are unchanged in this pass. The storm and wind
functions provide deterministic events/motion to hook to sound in a later audio pass.
