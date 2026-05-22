// The interpreter/JIT (wasm) radio in the playground toolbar. Three concerns:
//   1. Picking a sample with a precompiled .wasm enables the JIT radio.
//   2. Picking a sample without one disables it.
//   3. If JIT was selected when (2) happens, the radio must also be UNCHECKED
//      and interpreter re-checked — otherwise selectedEngine() keeps returning
//      'jit' and runCode() 404s on the missing artifact.
//
// SHA-256 is in the all_wasm cross-compile list (web/CMakeLists.txt); Macros
// is multi-file so deriveJitName returns null synchronously — clean pair for
// "JIT becomes available → user opts in → JIT becomes unavailable".
//
// The actual .wasm artifacts are only built by `cmake --build … all_wasm`,
// which the playground-e2e CI lane runs but local dev typically skips. So we
// stub the HEAD probe with page.route — the test asserts the radio-state
// machine, not the cross-compile pipeline.

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

// Make the HEAD fetch for any /samples/examples/*.wasm resolve to 200 OK so
// updateEngineAvailability flips the JIT radio to enabled without relying on
// build artifacts being present in the served tree.
async function stubWasmAvailable(page) {
    await page.route('**/samples/examples/*.wasm', route => {
        if (route.request().method() === 'HEAD') {
            return route.fulfill({ status: 200, body: '' });
        }
        return route.continue();
    });
}

test('JIT radio enables for samples with a precompiled .wasm', async ({ playground }) => {
    await stubWasmAvailable(playground);
    await waitDropdownsPopulated(playground);

    await playground.locator('#examples').selectOption({ label: 'SHA-256 (benchmark)' });
    await expect.poll(
        () => playground.locator(jitSel).isDisabled(),
        { timeout: 5_000 }
    ).toBe(false);
});

test('JIT radio auto-reverts to interpreter when sample has no .wasm', async ({ playground }) => {
    await stubWasmAvailable(playground);
    await waitDropdownsPopulated(playground);

    await playground.locator('#examples').selectOption({ label: 'SHA-256 (benchmark)' });
    await expect.poll(
        () => playground.locator(jitSel).isDisabled(),
        { timeout: 5_000 }
    ).toBe(false);

    // User opts into JIT.
    await playground.locator(jitSel).check();
    expect(await playground.locator(jitSel).isChecked()).toBe(true);

    // Macros is multi-file — currentJitName is null, JIT disables synchronously
    // (no HEAD fetch, so the stub above isn't consulted on this path).
    await playground.locator('#examples').selectOption({ label: 'Macros (multi-file)' });
    await expect.poll(
        () => playground.locator(jitSel).isDisabled(),
        { timeout: 5_000 }
    ).toBe(true);

    // The fix: disabled radio must NOT remain checked, and interpreter must
    // be the new active selection.
    expect(await playground.locator(jitSel).isChecked()).toBe(false);
    expect(await playground.locator(interpSel).isChecked()).toBe(true);
});
