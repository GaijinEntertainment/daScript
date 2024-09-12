import os
import subprocess
import json
import platform
import time

lua = ["Lua 5.4.6", "lua", ["lua.exe"]]
luajit_joff = ["LuaJIT2.1 -joff", "lua", ["luajit.exe", "-joff"]]
quickjs = ["QuickJS", "js", ["qjs.exe"]]
quirrel = ["Quirrel 4.7.1", "quirrel", ["sq-64.exe"]]
squirrel = ["Squirrel 3.1", "quirrel", ["sq3-64.exe"]]
daslang_int =  ["Daslang (interperter)", None, None]
luau = ["Luau", "luau", ["luau.exe"]]
mono_int = ["Mono --interpreter", None, None]

featured_lang = daslang_int[0]

benchmarks_interp = [
  ("n-bodies", [
    [lua, "nbodies.lua"],
    [luau, "nbodies.luau"],
    [luajit_joff, "nbodies.lua"],
    [quickjs, "nbodies.js"],
    [quirrel, "nbodies.nut"],
    [squirrel, "nbodies.nut"],
  ]),
  ("particles kinematics", [
    [lua, "particles.lua"],
    [luau, "particles.luau"],
    [luajit_joff, "particles.lua"],
    [quickjs, "particles.js"],
    [quirrel, "particles.nut"],
    [squirrel, "particles.nut"],
  ]),
  ("exp loop", [
    [lua, "exp.lua"],
    [luau, "exp.luau"],
    [luajit_joff, "exp.lua"],
    [quickjs, "exp.js"],
    [quirrel, "exp.nut"],
    [squirrel, "exp.nut"],
  ]),
  ("dictionary", [
    [lua, "dict.lua"],
    [luau, "dict.luau"],
    [luajit_joff, "dict.lua"],
    [quickjs, "dict.js"],
    [quirrel, "dict.nut"],
    [squirrel, "dict.nut"],
  ]),
  ("fibonacci recursive", [
    [lua, "fib_recursive.lua"],
    [luau, "fib_recursive.luau"],
    [luajit_joff, "fib_recursive.lua"],
    [quickjs, "fib_recursive.js"],
    [quirrel, "fib_recursive.nut"],
    [squirrel, "fib_recursive.nut"],
  ]),
  ("fibonacci loop", [
    [lua, "fib_loop.lua"],
    [luau, "fib_loop.luau"],
    [luajit_joff, "fib_loop.lua"],
    [quickjs, "fib_loop.js"],
    [quirrel, "fib_loop.nut"],
    [squirrel, "fib_loop.nut"],
  ]),
  ("primes loop", [
    [lua, "primes.lua"],
    [luau, "primes.luau"],
    [luajit_joff, "primes.lua"],
    [quickjs, "primes.js"],
    [quirrel, "primes.nut"],
    [squirrel, "primes.nut"],
  ]),
  ("float2string", [
    [quirrel, "f2s.nut"],
    [squirrel, "f2s.nut"],
    [daslang_int,"f2s.das"],
    [lua,"f2s.lua"],
    [luajit_joff,"f2s.lua"],
    [luau,"f2s.luau"],
    [mono_int,"f2s.cs"],
    [quickjs,"f2s.js"],
  ]),
  ("mandelbrot", [
    [daslang_int,"mandelbrot.das"],
    [lua,"mandelbrot.lua"],
    [luajit_joff,"mandelbrot.lua"],
    [luau,"mandelbrot.luau"],
    [mono_int,"mandelbrot.cs"],
  ]),
  ("native loop", [
    [daslang_int,"native.das"],
    [luajit_joff,"native.lua"],
    [mono_int,"native.cs"],
    [quirrel,"native.nut"],
  ]),
  ("queen", [
    [daslang_int,"queen.das"],
    [quirrel, "queen.nut"],
    [squirrel, "queen.nut"],
    [lua,"queen.lua"],
    [luajit_joff,"queen.lua"],
    [luau,"queen.luau"],
  ]),
  ("sha256", [
    [daslang_int,"sha256.das"],
    [luajit_joff,"sha256.lua"],
    [luau,"sha256.luau"],
  ]),
  ("sort", [
    [daslang_int,"table-sort.das"],
    [quirrel, "table-sort.nut"],
    [squirrel, "table-sort.nut"],
    [lua,"table-sort.lua"],
    [luajit_joff,"table-sort.lua"],
    [luau,"table-sort.luau"],
    [mono_int,"table-sort.cs"],
  ]),
  ("spectral norm", [
    [daslang_int,"spectral-norm.das"],
    [quirrel, "spectral-norm.nut"],
    [squirrel, "spectral-norm.nut"],
    [lua,"spectral-norm.lua"],
    [luajit_joff,"spectral-norm.lua"],
    [luau,"spectral-norm.luau"],
    [mono_int,"spectral-norm.cs"],
    [quickjs,"spectral-norm.js"],
  ]),
  ("string2float", [
    [daslang_int,"f2i.das"],
    [quirrel, "f2i.nut"],
    [squirrel, "f2i.nut"],
    [lua,"f2i.lua"],
    [luajit_joff,"f2i.lua"],
    [luau,"f2i.luau"],
    [mono_int,"f2i.cs"],
    [quickjs,"f2i.js"],
  ]),
  ("tree", [
    [daslang_int,"tree.das"],
    [quirrel, "tree.nut"],
    [squirrel, "tree.nut"],
    [lua,"tree.lua"],
    [luajit_joff,"tree.lua"],
    [luau,"tree.luau"],
    [mono_int,"tree.cs"],
    [quickjs,"tree.js"],
  ]),
]


class pushd:
    def __init__(self, path):
        self.olddir = os.getcwd()
        if path != '':
            os.chdir(os.path.normpath(path))
    def __enter__(self):
        pass
    def __exit__(self, type, value, traceback):
        os.chdir(self.olddir)

def isnumeric(f):
  try:
    float(f)
  except ValueError:
    return False
  return True

def run_tests(benchmarks, results_file_name=None, perform_tests=None, perform_tests_by_name=None):
  res = {}
  for test_name, benchs in benchmarks:
    if perform_tests_by_name is not None and test_name not in perform_tests_by_name:
      continue
    rs = {}
    res[test_name] = rs
    for data, file in benchs:
      if perform_tests is not None and data not in perform_tests:
        continue
      lang_name, folder, cmds = data
      cmds = cmds + [file]
      folder = f"tests/{folder}"
      print(cmds, folder)
      try:
        with pushd(folder):
          proc = subprocess.run(cmds, capture_output=True,text=True, check=True)
          out = proc.stdout
      except subprocess.CalledProcessError as e:
        print("Error", e, f"\nin {folder} performing {cmds}")
        continue
      fullout = out
      out = out.splitlines()
      if len(out)==0:
        print(f"Error in {cmds}, no correct output, got {fullout}, expected <test name>, <testres in float seconds>, <number of tests>")
        continue
      out = out[0].split(",")
      out = [str(o).strip() for o in out]
      possible_vals = [o for o in out if isnumeric(o)]
      if len(possible_vals) > 0:
        possible_val = possible_vals[0]
      elif len(out) >1:
        possible_val = out[0]
      else:
        possible_val = out[0]
      val = float(possible_val) if isnumeric(possible_val) else possible_val
      rs[lang_name] = val
  with open(results_file_name, "w") as f:
    json.dump(res, f, indent=2)
  return res

def read_results(results_file_name=None):
  data = None
  if not os.path.exists(results_file_name) or results_file_name is None:
    print(results_file_name, "doesnt exists!")
    return data
  with open(results_file_name, "r") as f:
    data = json.load(f)
  return data

def get_current_test_info():
  return [
    "Platform: " + platform.platform() +"(" +platform.release()+ ")",
    "Architecture: " + platform.machine(),
    "Processor: " + platform.processor(),
    time.ctime()
  ]

def convert_results_to_html(data, fname, test_info, baseline_lang):
  if data is None:
    print("no data")
    return
  out = []
  for testname, results in data.items():
    out.append(f"<h3>{testname}</h3>")
    out.append(f'<table class="chart" style="width: 600px;">')
    results = list(results.items())
    results = [r for r in results if isnumeric(r[1])]
    results.sort(key=lambda x: 1/x[1], reverse=True)
    maxvalue = results[-1][1]
    baseline_value = next((x for x in results if x[0]==baseline_lang), [0,0])[1] if baseline_lang else 0
    for lang, res in results:
      relative = int(res/maxvalue*100)
      ratio = "" if baseline_value==0 else f"({round(res/baseline_value,2)})"
      res = round(res, 3)
      chart = 'chart-bar featured' if lang == featured_lang else 'chart-bar'
      s = f'  <tr><th>{lang}</th><td><div class="{chart}"  style="width: {relative}%;">{res}s&nbsp{ratio}</div></td></tr>'
      out.append(s)
    out.append("</table>")
  for line in test_info:
    out.append(f"<small>{line}</small><br/>")
  with open(fname, "wt") as f:
    f.writelines(line + '\n' for line in out)

def tests(results_fname, html_fname, benchmarks_cfg = None, test_info=None, perform_tests_for_langs=None, perform_tests_by_name=None, baseline_lang=daslang_int[0]):
  if benchmarks_cfg:
    run_tests(benchmarks_cfg, results_fname, perform_tests_for_langs, perform_tests_by_name)
  results = read_results(results_fname)
  if test_info is None:
    test_info = get_current_test_info()
  if html_fname:
    convert_results_to_html(results, html_fname, test_info, baseline_lang)

if __name__ == "__main__":
  perform_tests_for_langs = [lua, luau, luajit_joff, quickjs, quirrel, squirrel] #daslang, mono_int
  test_info = ["Tested on AMD Ryzen Threadripper 3990X 64-Core Processor",
    "CLANG 17.0.6, AOT CLANG 17.0.6, JIT LLVM 16.0.6",
    "Fri Mar 15 23:33:28 2024"]

  #tests("results_interp.json", "results_interp.html", benchmarks_cfg = None, test_info=test_info, perform_tests_for_langs = perform_tests_for_langs, perform_tests_by_name = None)
  tests("results_aotjit.json", "results_aoitjit.html", benchmarks_cfg = None, test_info=test_info, perform_tests_for_langs = perform_tests_for_langs, perform_tests_by_name = None, baseline_lang="C++")
