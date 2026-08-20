// Serves ../control.html and fixtures/ through route interception on a fake origin.
// No model, no JIT, no listening server.

const base = require('@playwright/test');
const fs = require('fs');
const path = require('path');

const PAGE_HTML = fs.readFileSync(path.join(__dirname, '..', 'control.html'), 'utf8');

function fx(name) {
    return JSON.parse(fs.readFileSync(path.join(__dirname, 'fixtures', name + '.json'), 'utf8'));
}

// SSE captures keep their exact wire framing
function raw(name) {
    return fs.readFileSync(path.join(__dirname, 'fixtures', name + '.txt'), 'utf8');
}

// GET pathname -> state key
const GET_MAP = {
    '/v1/stats': 'stats',
    '/v1/streams': 'streams',
    '/config': 'config',
    '/exchange': 'exchange',
    '/exchange/matches': 'matches',
    '/bench': 'bench',
    '/v1/images': 'images',
    '/catalog': 'catalog',
};

async function mockServer(page, opts = {}) {
    // GET keys hold a JSON object, a function (served-count) => object | {status, json}
    // for sequenced responses, or {__delay, json} to hold a response back.
    const state = {
        stats: opts.stats ?? fx('stats'),
        streams: opts.streams ?? fx('streams_idle'),
        config: opts.config ?? fx('config'),
        exchange: opts.exchange ?? fx('exchange'),
        matches: opts.matches ?? { reachable: false },
        bench: opts.bench ?? fx('bench_idle'),
        images: opts.images ?? fx('images'),
        catalog: opts.catalog ?? fx('catalog_idle'),
        // POST pathname -> {status, json}; the chat route instead streams `sse`
        // back verbatim as text/event-stream.
        responses: opts.responses ?? {},
        sse: opts.sse ?? null,
    };
    const posts = [];
    const calls = new Map();   // GET state key -> served count

    await page.route('**/*', async route => {
        const req = route.request();
        const url = new URL(req.url());
        if (url.hostname !== 'dasllama-control.test') return route.abort();   // must match baseURL in playwright.config.js
        const p = url.pathname;

        if (req.method() === 'GET') {
            if (p === '/') return route.fulfill({ contentType: 'text/html', body: PAGE_HTML });
            const key = GET_MAP[p];
            if (!key) return route.fulfill({ status: 404, contentType: 'application/json', body: '{}' });
            const n = calls.get(key) || 0;
            calls.set(key, n + 1);
            let v = state[key];
            if (typeof v === 'function') v = v(n);
            if (v && typeof v.__delay === 'number') {
                await new Promise(res => setTimeout(res, v.__delay));
                v = v.json;
            }
            if (v && typeof v.status === 'number') {
                return route.fulfill({ status: v.status, contentType: 'application/json', body: JSON.stringify(v.json ?? {}) });
            }
            return route.fulfill({ contentType: 'application/json', body: JSON.stringify(v) });
        }

        posts.push({ path: p, body: req.postData() });
        if (p === '/v1/chat/completions' && state.sse) {
            return route.fulfill({ contentType: 'text/event-stream', body: state.sse });
        }
        const r = state.responses[p] ?? { status: 200, json: {} };
        return route.fulfill({ status: r.status ?? 200, contentType: 'application/json', body: JSON.stringify(r.json ?? {}) });
    });

    return posts;
}

// The page has three independent load chains (stats poll, loadExchange().then(loadConfig),
// bench probe). Returning on the badge alone lets a test act before the config form and
// exchange policy exist — so also wait out the chains whose fixture will render (a fixture
// served as an error is deliberate: its test owns the not-loaded state).
async function openControl(page, opts = {}) {
    const posts = await mockServer(page, opts);
    await page.goto('/');
    await base.expect(page.locator('#badge')).not.toHaveText('connecting');
    const errShaped = v => v && typeof v.status === 'number' && v.status >= 400;
    if (!errShaped(opts.exchange)) {
        await base.expect(page.locator('#ex-current')).not.toHaveText('loading…');
    }
    if (!errShaped(opts.exchange) && !errShaped(opts.config)) {
        await base.expect(page.locator('#cfg-file')).toContainText('saves to');
    }
    return { posts };
}

const lastJson = posts => JSON.parse(posts[posts.length - 1].body);

module.exports = { test: base.test, expect: base.expect, fx, raw, openControl, lastJson };
