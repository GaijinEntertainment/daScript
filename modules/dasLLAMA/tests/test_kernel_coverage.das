// The KERNEL COVERAGE census verdict (REVIEW_GPU_KERNEL_CLASSES.md):
// every test process of a run writes its dispatch census into the
// folder DASLLAMA_KERNEL_CENSUS_DIR names (`_kernel_census.das`), and
// tests/run.das runs this file last over the merged files. The census
// rows are cells of the blocks that load their carriers
// (`_kernel_coverage_cells.das`); the verdicts and the declared blind
// spots are `_kernel_census.das`'s. This file loads no model, and its
// verdicts take no arm: they judge what the run's cells dispatched,
// whatever arms the run named. Skips: a verdict skips where
// DASLLAMA_KERNEL_CENSUS_DIR is unset (a bare run of this file) or a
// narrowed run wrote no census file of the home; the Metal verdict
// where das_metal is not built; the Vulkan verdict where the vulkan
// module is not built or the GPU tier is not armed. A whole run's
// folder that holds no file of the home fails. The floors assert only
// under DASLLAMA_KERNEL_CENSUS_WHOLE, which the runner sets on a whole
// run.
options gen2
options persistent_heap
options stack = 524288   // every dasLLAMA program root takes this budget (options stack does not unify up from libs)
options _dasllama_internal = true

require dastest/testing_boost public
require dasllama/dasllama_env   // nolint:STYLE030,LINT019 - g_env_test, inside the GPU halves
require _gpu_tier_armed   // nolint:STYLE030,LINT019 - the vulkan half's tier skip
require ?vulkan dasllama/dasllama_math_vulkan   // nolint:STYLE030,LINT019 - its [init] arms the tier that skip reads
require _kernel_census   // nolint:STYLE030,LINT019 - with_kernel_census, the verdicts

[test(description = "The Metal census verdict over a run's merged census files: at least 40 distinct kernels dispatched on a whole run, and each declared blind-spot entry names a compiled kernel that no cell of the run dispatches", standalone = "the census verdict: tests/run.das runs it last, a child of its own, over the merged census files every other process of the run wrote")]
def test_metal_kernel_coverage(tst : T?) {
    tst |> run("Metal kernel coverage census: every process's census merged") <| @(t : T?) {
        static_if (typeinfo builtin_module_exists(das_metal)) {
            let folder = g_env_test.kernel_census_dir
            let ran_whole = g_env_test.kernel_census_whole
            with_kernel_census(t, folder, CENSUS_METAL, ran_whole) $(cov, whole) {
                metal_census_verdict(t, cov, whole)
            }
        } else {
            t |> skip("das_metal is not built on this platform; nothing to census")
        }
    }
}

[test(description = "The Vulkan serving census verdict over a run's merged census files dispatches at least 24 distinct kernels on a whole run; every declared blind spot names a seeded kernel with zero dispatches, and a conditional one counts a dispatch where its driver pick holds and every carrier row of it ran", standalone = "the census verdict: tests/run.das runs it last, a child of its own, over the merged census files every other process of the run wrote")]
def test_vulkan_kernel_coverage_serving(tst : T?) {
    tst |> run("Vulkan serving census: every process's census merged") <| @(t : T?) {
        static_if (typeinfo builtin_module_exists(vulkan)) {
            return if (!gpu_tier_armed(t))
            let folder = g_env_test.kernel_census_dir
            let ran_whole = g_env_test.kernel_census_whole
            with_kernel_census(t, folder, CENSUS_VULKAN, ran_whole) $(cov, whole) {
                vulkan_census_verdict(t, cov, whole)
            }
        } else {
            t |> skip("vulkan is not built on this platform; nothing to census")
        }
    }
}
