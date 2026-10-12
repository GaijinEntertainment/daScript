"use strict";
const fs = require("node:fs");
const os = require("node:os");
const path = require("node:path");
const { spawnSync } = require("node:child_process");
const sdk = path.resolve(__dirname, "../../..");
function requiredPath(name, directory) {
  if (!process.env[name]) throw Error(`Set ${name} to an explicit SDK path`);
  const value = path.resolve(process.env[name]);
  const entry = fs.statSync(value);
  if (directory ? !entry.isDirectory() : !entry.isFile())
    throw Error(`${name} has the wrong file type`);
  return value;
}
function run(command, args, env = process.env) {
  const result = spawnSync(command, args, { cwd: sdk, env, stdio: "inherit" });
  if (result.error) throw result.error;
  if (result.status !== 0)
    throw Error(
      `${path.basename(command)} failed (${result.status ?? result.signal})`,
    );
}
const daslang = requiredPath("DASLANG_BIN", false);
const libraries = requiredPath("DASLANG_WASM_LIB_DIR", true);
const temporary = fs.mkdtempSync(path.join(os.tmpdir(), "dasimgui-browser-"));
try {
  run("emcc", ["--version"]);
  for (const fixture of [
    { root: "modules/dasImgui/tests/_fixtures/browser_input" },
    { root: "modules/dasImgui/examples/features/text_input" },
  ]) {
    run(daslang, [
      "-project_root",
      sdk,
      "-no-module-cache",
      path.join(sdk, "utils/daspkg/main.das"),
      "--",
      "release",
      "wasm",
      "--root",
      fixture.root,
      "--out",
      temporary,
      "--wasm-lib-dir",
      libraries,
    ]);
  }
  const normal = { ...process.env, TEXT_INPUT_BROWSER: "chromium" };
  run(
    process.execPath,
    [
      path.join(__dirname, "browser_input.cjs"),
      path.join(temporary, "browser_input"),
    ],
    normal,
  );
  run(
    process.execPath,
    [
      path.join(__dirname, "text_input_browser.cjs"),
      path.join(temporary, "text_input"),
    ],
    normal,
  );
  run(
    process.execPath,
    [
      path.join(__dirname, "text_input_browser.cjs"),
      path.join(temporary, "text_input"),
    ],
    { ...normal, TEXT_INPUT_BROWSER: "webkit" },
  );
} finally {
  fs.rmSync(temporary, { recursive: true, force: true });
}
