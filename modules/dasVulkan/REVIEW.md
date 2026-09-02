# dasVulkan Code Review Checklist

**Read `REVIEW_COMMON.md` (repo root) first - its contract binds this checklist.** Architecture doc:
`ARCHITECTURE.md`. Planned work: `ROADMAP.md`. A tutorial - a `tutorials/<NN_name>/` unit, and any
`record_*.das` recording driver wherever the diff puts it - answers to the `tutorials/` subfolder's
checklist. A generator source or a committed generator report, wherever the diff puts it, answers
to the `generator/` subfolder's checklist. A `[test]` file, wherever the diff puts it, answers to
the `tests/` subfolder's checklist.

**Weakening `REVIEW.das` (beside this file) is a defect:** dropping a check, adding a name to
its ignore set, or changing a finding text so it no longer names what failed.

**A diff that adds a feature bit to a `create_device_storage_8_16*` creator in
`daslib/vulkan_boost.das` also adds or updates that bit's probe, in the same change.** A bit the
creator always enables is added to the `storage_8_16_supported` probe; a bit the creator enables
only when the device reports support gets its own `<capability>_supported` probe. A bit with no
probe either fails `vkCreateDevice` or lets a kernel use a feature the device never enabled.

**A diff to `utils/vulkan2rst.das` that puts a public helper of a module it documents into a
catch-all `group_by_regex` group is a defect - give the helper a group that names what it
does.** A catch-all group hides the helper from the `Uncategorized` check in
`.github/workflows/doc.yml`.

**A diff that adds or edits a call to raw `vkCreateDevice`, wherever the diff puts it, appends
`VK_KHR_portability_subset` to that call's extension list when the device advertises it - or
calls a `create_device*` boost creator instead, which appends it.** The Vulkan spec fails
`vkCreateDevice` on a device that advertises the extension without enabling it.

**A diff that puts the result of a `create_*` call into a local the function neither returns
nor moves into a container it deletes declares that local `var inscope`.** A plain
`var x <- create_*()` leaks: the wrapper owns a raw Vulkan handle, and nothing frees it without
the scope-exit `finalize`.
