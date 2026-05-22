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

// Deterministic HEAD-probe world: 200 for sha256.wasm only, 404 for every
// other sample's .wasm regardless of what's actually present in the served
// tree. A narrower `route('**/samples/examples/sha256.wasm', ...)` would let
// the startup probe for the default Hello world sample hit a real hello.wasm
// (if a local build has staged any), and JIT would be enabled from page
// load — the "JIT enables on sha256 select" test would then pass even if
// the sample-switch path stopped working.
async function stubSha256WasmAvailable(page) {
    await page.route('**/samples/examples/*.wasm', route => {
        if (route.request().method() !== 'HEAD') return route.continue();
        const ok = route.request().url().endsWith('/sha256.wasm');
        return route.fulfill({ status: ok ? 200 : 404, body: '' });
    });
}

test('JIT radio enables for samples with a precompiled .wasm', async ({ playground }) => {
    await stubSha256WasmAvailable(playground);
    await waitDropdownsPopulated(playground);

    // The `playground` fixture's navigate-before-route ordering means the
    // startup HEAD probe for the default sample's .wasm has already happened
    // against the real http.server. Force a fresh probe by selecting a sample
    // the stub returns 404 for — that establishes the "JIT disabled" baseline
    // we need so the SHA-256 transition actually proves the sample-switch
    // path works.
    await playground.locator('#examples').selectOption({ label: 'Functions' });
    await expect.poll(
        () => playground.locator(jitSel).isDisabled(),
        { timeout: 5_000 }
    ).toBe(true);

    // Switching to SHA-256 must flip the radio to enabled — that transition
    // is what the test name asserts.
    await playground.locator('#examples').selectOption({ label: 'SHA-256 (benchmark)' });
    await expect.poll(
        () => playground.locator(jitSel).isDisabled(),
        { timeout: 5_000 }
    ).toBe(false);
});

test('JIT radio auto-reverts to interpreter when sample has no .wasm', async ({ playground }) => {
    await stubSha256WasmAvailable(playground);
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
