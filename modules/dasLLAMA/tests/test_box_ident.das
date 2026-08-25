options gen2
options indenting = 4
options stack = 524288

// model-free: the hardware-keyed box tag (box_ident) - the fleet table maps every recorded
// box's CPU brand to the store tag its records already carry, the override always wins, and
// unknown hardware gets its own slug, never another box's store (the m1-default landmine).

require dastest/testing_boost public
require ../performance/box_ident.das

[test]
def test_box_ident(t : T?) {
    t |> run("fleet table: recorded CPU brands map to their store tags") @(t : T?) {
        t |> equal("m1", box_tag_for_cpu("Apple M1 Max"))
        t |> equal("m3air", box_tag_for_cpu("Apple M3"))
        t |> equal("m4", box_tag_for_cpu("Apple M4 Pro"))
        t |> equal("m5", box_tag_for_cpu("Apple M5 Max"))
        t |> equal("zen2", box_tag_for_cpu("AMD Ryzen Threadripper 3990X 64-Core Processor"))
        t |> equal("zen4", box_tag_for_cpu("AMD Ryzen 7 PRO 8700GE w/ Radeon 780M Graphics"))
    }
    t |> run("unknown hardware slugs from the brand - never a fleet tag") @(t : T?) {
        t |> equal("apple-m6-ultra", box_tag_for_cpu("Apple M6 Ultra"))
        t |> equal("intel-r-xeon-r-w9-3495x", box_tag_for_cpu("Intel(R) Xeon(R) w9-3495X"))
        t |> equal("unknown-box", box_tag_for_cpu(""))
    }
    t |> run("the DASLLAMA_BOX override wins over any probe") @(t : T?) {
        t |> equal("zen4", derive_box_name("zen4"))
        t |> equal("custom-tag", derive_box_name("custom-tag"))
    }
    t |> run("an unset env derives the tag from THIS box's probed CPU brand") @(t : T?) {
        // the landmine this file exists for: an unset env must resolve through the probe +
        // table on every box (a reintroduced literal default breaks the equality anywhere
        // the hardware is not that literal's box)
        let cpu = probe_cpu_brand()
        let derived = derive_box_name("")
        t |> success(!empty(derived), "derived tag is non-empty ('{derived}' from '{cpu}')")
        t |> equal(box_tag_for_cpu(cpu), derived)
    }
}
