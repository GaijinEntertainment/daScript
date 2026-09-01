// node --test  (run from this directory)
import test from 'node:test';
import assert from 'node:assert/strict';
import { execFile } from 'node:child_process';
import { promisify } from 'node:util';
import { fileURLToPath } from 'node:url';
import { Artifacts } from './runner.mjs';

const run = promisify(execFile);
const RUNNER = fileURLToPath(new URL('./runner.mjs', import.meta.url));

// A stubbed page: goto() replays a scripted sequence and records what it was
// asked for. No playwright, no browser, no network — the driving loop's own
// recovery is what is under test, not anything Chromium does.
function stubbed(outcomes) {
    const artifacts = new Artifacts(null, { navRetryMs: 0 });
    const calls = [];
    artifacts.page = {
        async goto(url) {
            calls.push(url);
            const next = outcomes.shift();
            if (next instanceof Error) throw next;
            return next;
        },
    };
    return { artifacts, calls };
}

// Every retry here prints the production line, and a nightly log is grepped for
// exactly that string to count how often the edge dropped a connection. Test
// output on stdout would be counted as production drops, so no case may let the
// line escape: drive every retry through this, and assert on what it captured.
async function captureStderr(fn) {
    const written = [];
    const real = process.stderr.write.bind(process.stderr);
    process.stderr.write = (chunk) => { written.push(String(chunk)); return true; };
    try {
        await fn();
    } finally {
        process.stderr.write = real;
    }
    return written;
}

const ARTIFACT_URL = 'https://run.daslang.io/api/build/artifact/a/b/sample.html';
const drop = () => new Error(`page.goto: net::ERR_CONNECTION_CLOSED at ${ARTIFACT_URL}`);
const refused = () => new Error(`page.goto: net::ERR_CONNECTION_REFUSED at ${ARTIFACT_URL}`);

test('a dropped navigation is retried, and the retry asks for the same url', async () => {
    const { artifacts, calls } = stubbed([drop(), {}]);
    await captureStderr(() => artifacts.load(ARTIFACT_URL));
    assert.deepEqual(calls, [ARTIFACT_URL, ARTIFACT_URL]);
});

test('the retry is spent once — a second drop fails the row', async () => {
    const { artifacts, calls } = stubbed([drop(), drop(), {}]);
    await captureStderr(() =>
        assert.rejects(artifacts.load(ARTIFACT_URL), /ERR_CONNECTION_CLOSED/));
    assert.equal(calls.length, 2);
});

test('a navigation failure that is not a transport drop is never retried', async () => {
    const { artifacts, calls } = stubbed([refused(), {}]);
    await assert.rejects(artifacts.load(ARTIFACT_URL), /ERR_CONNECTION_REFUSED/);
    assert.equal(calls.length, 1);
});

test('a clean navigation costs exactly one attempt', async () => {
    const { artifacts, calls } = stubbed([{}]);
    await artifacts.load(ARTIFACT_URL);
    assert.equal(calls.length, 1);
});

// The verdict merges artifacts.pageErrors (verifyWasm), so an error raised by
// the attempt that never landed would fail a row the retry rescued.
test('page errors from the dropped attempt do not survive into the retry', async () => {
    const artifacts = new Artifacts(null, { navRetryMs: 0 });
    let attempt = 0;
    artifacts.page = {
        async goto() {
            attempt++;
            artifacts.pageErrors.push(`error from attempt ${attempt}`);
            if (attempt === 1) throw drop();
            return {};
        },
    };
    await captureStderr(() => artifacts.load(ARTIFACT_URL));
    assert.deepEqual(artifacts.pageErrors, ['error from attempt 2']);
});

// The line is the only measurement of how often the edge drops a connection, so
// losing it, or losing the net error inside it, must fail something.
test('the retry names the sample and the net error on stderr', async () => {
    const { artifacts } = stubbed([drop(), {}]);
    const written = await captureStderr(() =>
        artifacts.load(ARTIFACT_URL, 'OpenGL: cube swarm (instancing)'));
    assert.equal(written.length, 1);
    assert.match(written[0], /OpenGL: cube swarm \(instancing\).*artifact navigation dropped.*ERR_CONNECTION_CLOSED/);
});

test('the retry waits before asking again', async () => {
    const artifacts = new Artifacts(null, { navRetryMs: 120 });
    let attempt = 0;
    artifacts.page = { async goto() { attempt++; if (attempt === 1) throw drop(); return {}; } };
    const t0 = Date.now();
    await captureStderr(() => artifacts.load(ARTIFACT_URL));
    assert.ok(Date.now() - t0 >= 100, `retried after ${Date.now() - t0}ms, expected a wait`);
});

// main() is behind an entry-point check so `node --test` can import this module.
// If that check ever reads false when the nightly spawns the file, the verify
// step exits 0 having verified nothing — green by construction, forever.
// `--help` returns before any fetch or browser launch, so this costs no network.
test('invoked directly, runner.mjs still reaches main()', async () => {
    const { stdout } = await run(process.execPath, [RUNNER, '--help']);
    assert.match(stdout, /^dasweb-verify browser leg/);
});
