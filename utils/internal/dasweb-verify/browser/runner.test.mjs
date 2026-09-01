// node --test  (run from this directory)
import test from 'node:test';
import assert from 'node:assert/strict';
import { Artifacts } from './runner.mjs';

// A stubbed page: goto() replays a scripted sequence and records what it was
// asked for. No playwright, no browser, no network — the driving loop's own
// recovery is what is under test, not anything Chromium does.
function stubbed(outcomes) {
    const artifacts = new Artifacts(null, {});
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

const URL = 'https://run.daslang.io/api/build/artifact/a/b/sample.html';
const drop = () => new Error(`page.goto: net::ERR_CONNECTION_CLOSED at ${URL}`);
const refused = () => new Error(`page.goto: net::ERR_CONNECTION_REFUSED at ${URL}`);

test('a dropped navigation is retried, and the retry asks for the same url', async () => {
    const { artifacts, calls } = stubbed([drop(), {}]);
    await artifacts.load(URL);
    assert.deepEqual(calls, [URL, URL]);
});

test('the retry is spent once — a second drop fails the row', async () => {
    const { artifacts, calls } = stubbed([drop(), drop(), {}]);
    await assert.rejects(artifacts.load(URL), /ERR_CONNECTION_CLOSED/);
    assert.equal(calls.length, 2);
});

test('a navigation failure that is not a transport drop is never retried', async () => {
    const { artifacts, calls } = stubbed([refused(), {}]);
    await assert.rejects(artifacts.load(URL), /ERR_CONNECTION_REFUSED/);
    assert.equal(calls.length, 1);
});

test('a clean navigation costs exactly one attempt', async () => {
    const { artifacts, calls } = stubbed([{}]);
    await artifacts.load(URL);
    assert.equal(calls.length, 1);
});

// The verdict merges artifacts.pageErrors (verifyWasm), so an error raised by
// the attempt that never landed would fail a row the retry rescued.
test('page errors from the dropped attempt do not survive into the retry', async () => {
    const artifacts = new Artifacts(null, {});
    let attempt = 0;
    artifacts.page = {
        async goto() {
            attempt++;
            artifacts.pageErrors.push(`error from attempt ${attempt}`);
            if (attempt === 1) throw drop();
            return {};
        },
    };
    await artifacts.load(URL);
    assert.deepEqual(artifacts.pageErrors, ['error from attempt 2']);
});
