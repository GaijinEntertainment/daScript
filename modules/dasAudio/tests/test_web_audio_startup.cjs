// MINIAUDIO_H=/path/to/fetched/miniaudio.h EMCC=emcc node this-file.cjs
// Requires Playwright and a browser. BROWSER_EXECUTABLE optionally selects one.
const assert = require("node:assert/strict");
const fs = require("node:fs");
const os = require("node:os");
const path = require("node:path");
const http = require("node:http");
const { execFileSync } = require("node:child_process");
const { chromium } = require("playwright");
(async () => {
  assert.ok(
    process.env.MINIAUDIO_H,
    "MINIAUDIO_H must name the fetched upstream header",
  );
  const root = fs.mkdtempSync(path.join(os.tmpdir(), "audio-startup-"));
  let server, browser;
  try {
    const header = path.join(root, "miniaudio.h");
    fs.copyFileSync(process.env.MINIAUDIO_H, header);
    const patch = path.join(__dirname, "../patches/miniaudio_memory64.cmake");
    const apply = () =>
      execFileSync("cmake", [`-DMINIAUDIO_H=${header}`, "-P", patch], {
        stdio: "inherit",
      });
    const invalid = path.join(root, "invalid.h");
    fs.writeFileSync(invalid, "/* unsupported upstream layout */");
    assert.throws(
      () =>
        execFileSync("cmake", [`-DMINIAUDIO_H=${invalid}`, "-P", patch], {
          stdio: "pipe",
        }),
      /Command failed/,
      "unknown header layout must fail patching",
    );
    apply();
    const once = fs.readFileSync(header);
    apply();
    assert.deepEqual(fs.readFileSync(header), once, "patch is idempotent");
    execFileSync(
      process.env.EMCC || "emcc",
      [
        path.join(__dirname, "web_audio_startup.c"),
        "-I",
        root,
        "-DMA_ENABLE_AUDIO_WORKLETS",
        "-sMEMORY64=2",
        "-pthread",
        "-sAUDIO_WORKLET=1",
        "-sWASM_WORKERS=1",
        "-sALLOW_MEMORY_GROWTH=1",
        "-sEXIT_RUNTIME=0",
        "-O1",
        "-o",
        path.join(root, "probe.js"),
      ],
      { stdio: "inherit" },
    );
    server = http.createServer((req, res) => {
      res.setHeader("Cross-Origin-Opener-Policy", "same-origin");
      res.setHeader("Cross-Origin-Embedder-Policy", "require-corp");
      if (req.url === "/") {
        res.setHeader("Content-Type", "text/html");
        res.end(
          '<button onclick="Module._start_probe(window.channels,window.type)">Start</button><script>var Module={};</script><script src="/probe.js"></script>',
        );
      } else {
        const file = path.join(root, path.basename(req.url));
        if (!fs.existsSync(file)) {
          res.statusCode = 404;
          res.end();
          return;
        }
        res.setHeader(
          "Content-Type",
          file.endsWith(".wasm") ? "application/wasm" : "text/javascript",
        );
        res.end(fs.readFileSync(file));
      }
    });
    await new Promise((r) => server.listen(0, "127.0.0.1", r));
    browser = await chromium.launch({
      executablePath: process.env.BROWSER_EXECUTABLE,
      headless: false,
      ignoreDefaultArgs: ["--mute-audio"],
      args: [
        "--use-fake-device-for-media-stream",
        "--use-fake-ui-for-media-stream",
      ],
    });
    for (const [channels, type] of [
      [1, 1],
      [2, 1],
      [4, 1],
      [1, 2],
      [1, 3],
    ]) {
      const page = await browser.newPage();
      const errors = [];
      page.on("pageerror", (e) => errors.push(String(e)));
      await page.addInitScript(
        ({ channels, type }) => {
          window.channels = channels;
          window.type = type;
          window.created = [];
          const Native = AudioWorkletNode;
          window.AudioWorkletNode = new Proxy(Native, {
            construct(T, args) {
              created.push({
                name: args[1],
                channels: args[2]?.outputChannelCount,
              });
              return new T(...args);
            },
          });
          const add = Worklet.prototype.addModule;
          Worklet.prototype.addModule = function (...args) {
            return new Promise((r) => setTimeout(r, 500)).then(() =>
              add.apply(this, args),
            );
          };
        },
        { channels, type },
      );
      await page.goto(`http://127.0.0.1:${server.address().port}/`);
      await page.waitForFunction(() => Module.calledRun);
      await page.getByText("Start", { exact: true }).click();
      await page.waitForFunction(() => Module._callback_count() > 20);
      const nodes = await page.evaluate(() => created);
      assert.deepEqual(
        nodes.find((n) => n.name === "miniaudio")?.channels,
        [channels],
        "async worklet retains requested channels after init returns",
      );
      assert.deepEqual(errors, []);
      await page.close();
    }
    console.log(
      "PASS: delayed audio startup preserves mono/stereo/four-channel output and fake-input capture/duplex",
    );
  } finally {
    if (browser) await browser.close();
    if (server) await new Promise((r) => server.close(r));
    fs.rmSync(root, { recursive: true, force: true });
  }
})().catch((e) => {
  console.error(e);
  process.exitCode = 1;
});
