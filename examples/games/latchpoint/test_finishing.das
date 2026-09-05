options gen2
options persistent_heap
require dastest/testing_boost public
require lp_runtime
require lp_audio
require lp_geology

[test]
def test_archive_payoff(suite : T?) {
    suite |> run("archive inspection is gated and records its payoff only once") @(t : T?) {
        delete world
        world = WorldState()
        t |> success(!runtime_physical("inspect archive record").ok)
        world.archive_open = true
        t |> success(runtime_physical("inspect archive record").ok)
        t |> success(world.archive_inspected)
        let events = length(world.events)
        t |> success(runtime_physical("inspect archive record").ok)
        t |> equal(length(world.events), events)
        let saved = runtime_snapshot()
        t |> success(runtime_load(saved))
        t |> success(world.archive_inspected)
        avatar = float3(-4.0, 9.8, 1.65)
        t |> success(physical_at_avatar() != "inspect archive record")
        avatar = float3(-4.0, 12.0, 1.65)
        t |> equal(physical_at_avatar(), "inspect archive record")
    }
}

[test]
def test_finishing_materials(suite : T?) {
    suite |> run("voice coloration is subtle, bounded and bypassable") @(t : T?) {
        let original <- mechanical_pcm(0.2, 210.0, 0.3, 0.2)
        for (resident in range(3)) {
            var samples := original
            voice_processing = true
            process_robot_voice(samples, EFFECT_RATE, 1, resident)
            var error = 0.0
            for (sample, dry in samples, original) {
                t |> success(abs(sample) <= 1.0)
                error += abs(sample - dry)
            }
            t |> success(error > 0.0 && error / float(length(samples)) < 0.03)
            samples := original
            voice_processing = false
            process_robot_voice(samples, EFFECT_RATE, 1, resident)
            t |> equal(samples, original)
        }
        voice_processing = true
    }
    suite |> run("outcrop variants have bounded vertices and flat valid faces") @(t : T?) {
        for (seed in fixed_array(1.0, 3.7)) {
            let mesh <- fractured_outcrop(seed)
            t |> success(length(mesh.indices) > 100)
            for (vertex in mesh.vertices) {
                t |> success(abs(vertex.xyz.x) <= 1.0 && abs(vertex.xyz.y) <= 1.0 && abs(vertex.xyz.z) <= 1.0)
                t |> success(abs(length(vertex.normal) - 1.0) < 0.0001)
            }
        }
    }
}
