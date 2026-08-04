// The interpreter/JIT (wasm) radio in the playground toolbar.
//
// Phase 2 of plans/dasweb_backend.md retires the precompiled-artifact path:
// wasm builds move to the on-demand build service, and until phase 3 serves
// real builds the radio is PERMANENTLY disabled (updateEngineAvailability
// short-circuits to disableJit()). These tests pin that state — including
// across sample switches that used to enable it — and the un-check invariant:
// a disabled radio must never remain checked (selectedEngine() would keep
// returning 'jit' and runCode() would 404).
//
// When phase 3 lands, restore the transition tests from git history
// (pre-phase-2 versions asserted enable-on-sha256 via a stubbed HEAD probe).

const { test, expect } = require('./fixtures.js');

async function waitDropdownsPopulated(page) {
    await page.waitForFunction(
        () => document.getElementById('examples').options.length > 1,
        null,
        { timeout: 10_000 }
    );
}

const jitSel = 'input[name=engine][value=jit]';
const interpSel = 'input[name=engine][value=interpreter]';

test('JIT radio stays disabled regardless of sample', async ({ playground }) => {
    await waitDropdownsPopulated(playground);

    expect(await playground.locator(jitSel).isDisabled()).toBe(true);

    // SHA-256 used to enable it (precompiled-artifact era) — it must not now.
    await playground.locator('#examples').selectOption({ label: 'SHA-256 (benchmark)' });
    await expect.poll(
        () => playground.locator(jitSel).isDisabled(),
        { timeout: 5_000 }
    ).toBe(true);

    await playground.locator('#examples').selectOption({ label: 'Macros (multi-file)' });
    await expect.poll(
        () => playground.locator(jitSel).isDisabled(),
        { timeout: 5_000 }
    ).toBe(true);
});

test('interpreter stays the active engine', async ({ playground }) => {
    await waitDropdownsPopulated(playground);

    await playground.locator('#examples').selectOption({ label: 'SHA-256 (benchmark)' });
    await expect.poll(
        () => playground.locator(interpSel).isChecked(),
        { timeout: 5_000 }
    ).toBe(true);
    expect(await playground.locator(jitSel).isChecked()).toBe(false);
});
