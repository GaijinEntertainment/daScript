"use strict";
const assert = require("node:assert/strict");
const fs = require("node:fs");
const path = require("node:path");
const crypto = require("node:crypto");
const { chromium, webkit } = require("playwright");
const { createWasmServer } = require("./_wasm_server.cjs");

(async () => {
  assert.ok(process.argv[2], "Pass the text_input WASM release directory");
  const root = path.resolve(process.argv[2]);
  const manifest = JSON.parse(
    fs.readFileSync(path.join(root, "fonts/manifest.json"), "utf8"),
  );
  for (const font of manifest.fonts) {
    assert.equal(
      crypto
        .createHash("sha256")
        .update(fs.readFileSync(path.join(root, "fonts", font.file)))
        .digest("hex"),
      font.sha256,
    );
    for (const license of font.license_files)
      assert.ok(fs.statSync(path.join(root, "fonts", license)).size > 100);
  }
  const server = createWasmServer(root);
  let browser, page;
  try {
    await new Promise((resolve) => server.listen(0, "127.0.0.1", resolve));
    const useWebKit = process.env.TEXT_INPUT_BROWSER === "webkit";
    browser = await (useWebKit
      ? webkit.launch({ headless: true })
      : chromium.launch({ channel: "chrome", headless: true }));
    const context = await browser.newContext({
      viewport: { width: 1040, height: 900 },
    });
    page = await context.newPage();
    const errors = [];
    page.on("pageerror", (error) => {
      errors.push(error.message);
      console.error(error.stack || error.message);
    });
    page.on("console", (message) => {
      if (message.type() === "error") console.error(message.text());
    });
    await page.goto(
      `http://127.0.0.1:${server.address().port}/text_input.html`,
    );
    await page.waitForFunction(
      () => globalThis.Module?.FS?.analyzePath("/text-input-state.json").exists,
      {},
      { timeout: 60000 },
    );
    const state = () =>
      page.evaluate(() =>
        JSON.parse(
          Module.FS.readFile("/text-input-state.json", { encoding: "utf8" }),
        ),
      );
    const click = async (target) => {
      const point = await page.evaluate((target) => {
        const state = JSON.parse(
          Module.FS.readFile("/text-input-state.json", { encoding: "utf8" }),
        );
        const box = state.widgets.globals[target]?.bbox;
        if (!box || box.z <= box.x || box.w <= box.y)
          throw Error("Widget is not rendered: " + target);
        const canvas = Module.canvas,
          rect = canvas.getBoundingClientRect();
        return {
          x: rect.left + ((box.x + 8) * rect.width) / canvas.clientWidth,
          y:
            rect.top +
            (((box.y + box.w) / 2) * rect.height) / canvas.clientHeight,
        };
      }, target);
      await page.mouse.click(point.x, point.y);
    };
    assert.equal((await state()).fonts_ready, true);
    assert.equal((await state()).glyphs_ready, true);
    await click("TEXT_INPUT/SINGLE");
    await page.waitForFunction(
      () => document.activeElement?.tagName === "TEXTAREA",
    );
    await page.keyboard.insertText("中文 日本語 한국어 😀");
    await page.waitForFunction(
      () =>
        JSON.parse(
          Module.FS.readFile("/text-input-state.json", { encoding: "utf8" }),
        ).single === "中文 日本語 한국어 😀",
    );
    const rendered = (await state()).widgets.globals["TEXT_INPUT/SINGLE"]
      .payload;
    assert.equal(
      Buffer.from(
        rendered.buffer.slice(0, rendered.buffer.indexOf(0)),
      ).toString("utf8"),
      "中文 日本語 한국어 😀",
    );
    const cdp = useWebKit ? null : await context.newCDPSession(page);
    if (cdp) {
      await cdp.send("Input.imeSetComposition", {
        text: "にほん",
        selectionStart: 3,
        selectionEnd: 3,
      });
      await page.waitForFunction(() => Module.dasClipboardBridge.composing);
      assert.equal(
        (await state()).single,
        "中文 日本語 한국어 😀",
        "preedit remains uncommitted",
      );
      assert.equal(
        await page.evaluate(() => {
          const host = Module.dasClipboardBridge.host;
          return (
            Number(getComputedStyle(host).opacity) > 0 &&
            host.getBoundingClientRect().width > 1
          );
        }),
        true,
        "composition must be visible at the input caret",
      );
      await page.keyboard.press("Enter");
      assert.equal(
        (await state()).submissions,
        0,
        "composition Enter must not submit",
      );
      await cdp.send("Input.insertText", { text: "日本" });
      await page.waitForFunction(() =>
        JSON.parse(
          Module.FS.readFile("/text-input-state.json", { encoding: "utf8" }),
        ).single.endsWith("日本"),
      );
      await page.keyboard.press("Enter");
      await page.waitForFunction(
        () =>
          JSON.parse(
            Module.FS.readFile("/text-input-state.json", { encoding: "utf8" }),
          ).submissions === 1,
      );
    }
    await click("TEXT_INPUT/EDITOR/unicode_editor");
    await page.waitForFunction(
      () => {
        const state = JSON.parse(
          Module.FS.readFile("/text-input-state.json", { encoding: "utf8" }),
        );
        return (
          state.editor_focused &&
          Module.dasClipboardBridge.active &&
          document.activeElement === Module.dasClipboardBridge.host
        );
      },
      {},
      { timeout: 3000 },
    );
    await page.keyboard.insertText("編集 👩‍💻");
    await page.waitForFunction(
      () =>
        JSON.parse(
          Module.FS.readFile("/text-input-state.json", { encoding: "utf8" }),
        ).editor === "編集 👩‍💻",
    );
    await page.keyboard.press("Backspace");
    await page.waitForFunction(
      () =>
        JSON.parse(
          Module.FS.readFile("/text-input-state.json", { encoding: "utf8" }),
        ).editor === "編集 ",
    );
    if (cdp) {
      await cdp.send("Input.imeSetComposition", {
        text: "かんじ",
        selectionStart: 3,
        selectionEnd: 3,
      });
      await page.waitForFunction(() => Module.dasClipboardBridge.composing);
      assert.equal(
        (await state()).editor,
        "編集 ",
        "custom preedit remains uncommitted",
      );
      assert.equal(
        await page.evaluate(
          () =>
            Number(getComputedStyle(Module.dasClipboardBridge.host).opacity) >
            0,
        ),
        true,
      );
      await cdp.send("Input.insertText", { text: "漢字" });
      await page.waitForFunction(
        () =>
          JSON.parse(
            Module.FS.readFile("/text-input-state.json", { encoding: "utf8" }),
          ).editor === "編集 漢字",
      );
      await cdp.send("Input.imeSetComposition", {
        text: "未確定",
        selectionStart: 3,
        selectionEnd: 3,
      });
      await cdp.send("Input.imeSetComposition", {
        text: "",
        selectionStart: 0,
        selectionEnd: 0,
      });
      await page.waitForFunction(
        () =>
          !Module.dasClipboardBridge.composing &&
          Number(getComputedStyle(Module.dasClipboardBridge.host).opacity) ===
            0,
      );
      assert.equal(
        (await state()).editor,
        "編集 漢字",
        "cancel leaves custom buffer unchanged",
      );
    }
    const expectedEditor = cdp ? "編集 漢字" : "編集 ";
    assert.equal(
      (await state()).single,
      "中文 日本語 한국어 😀" + (cdp ? "日本" : ""),
      "custom input stays in its owning editor",
    );
    await click("TEXT_INPUT/SECONDARY");
    await page.waitForFunction(
      () =>
        JSON.parse(
          Module.FS.readFile("/text-input-state.json", { encoding: "utf8" }),
        ).widgets.globals["TEXT_INPUT/SECONDARY"].focus === true,
      {},
      { timeout: 3000 },
    );
    await page.keyboard.insertText("neighbor");
    await page.waitForFunction(
      () =>
        JSON.parse(
          Module.FS.readFile("/text-input-state.json", { encoding: "utf8" }),
        ).secondary === "neighbor",
    );
    assert.equal(
      (await state()).editor,
      expectedEditor,
      "leaving the custom editor does not redirect input back into it",
    );
    assert.deepEqual(errors, []);
    if (process.env.TEXT_INPUT_CAPTURE)
      await page.screenshot({ path: process.env.TEXT_INPUT_CAPTURE });
    console.log(
      `PASS ${useWebKit ? "WebKit committed input" : "Chromium composition"}: bundled fonts/licenses, rendered Unicode input, custom editor and neighboring focus`,
    );
  } catch (error) {
    if (page)
      console.error(
        await page
          .evaluate(() =>
            (() => {
              const s = Module.FS.analyzePath("/text-input-state.json").exists
                ? JSON.parse(
                    Module.FS.readFile("/text-input-state.json", {
                      encoding: "utf8",
                    }),
                  )
                : {};
              return {
                single: s.single,
                editor: s.editor,
                focused: s.editor_focused,
                io: s.widgets?.io,
                bridge: {
                  active: Module.dasClipboardBridge?.active,
                  editor: Module.dasClipboardBridge?.editor,
                  composing: Module.dasClipboardBridge?.composing,
                },
                activeElement: document.activeElement?.tagName,
              };
            })(),
          )
          .catch(() => null),
      );
    throw error;
  } finally {
    if (browser) await browser.close();
    server.closeAllConnections();
    await new Promise((resolve) => server.close(resolve));
  }
})().catch((error) => {
  console.error(error);
  process.exitCode = 1;
});
