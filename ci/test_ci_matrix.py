#!/usr/bin/env python3
"""Pins the per-PR and nightly job matrices ci_matrix.py emits, and the workflow shapes that
consume them."""
import json
import os
import re
import subprocess
import sys
import unittest

HERE = os.path.dirname(os.path.abspath(__file__))
sys.path.insert(0, HERE)
import ci_matrix  # noqa: E402

WORKFLOWS = os.path.join(os.path.dirname(HERE), ".github", "workflows")


def names(cells):
    # the fast-math cell is a Release/none cell too, so its flag has to reach the name; a
    # sanitizer cell is one job per phase, so the phase does too
    return sorted("%s-%s-%s-%s%s%s" % (c["target"], c["architecture"], c["cmake_preset"], c["sanitizers"],
                                       "-fastmath" if c.get("fast_math") == "ON" else "",
                                       "" if c.get("phase", "all") == "all" else "-" + c["phase"])
                  for c in cells)


PR_BUILD = sorted([
    "linux-64-Debug-none", "linux-64-Release-none",
    "linux_arm-64-Debug-none", "linux_arm-64-Release-none",
    "darwin15-arm64-Debug-none", "darwin15-arm64-Release-none",
    "darwin26-arm64-Debug-none", "darwin26-arm64-Release-none",
    "windows-32-Release-none", "windows-64-Release-none",
])
NIGHTLY_ONLY_BUILD = sorted(
    ["windows-64-Debug-none-compile", "linux-64-Release-none-fastmath"]
    + ["linux-64-Release-%s-%s" % (san, phase) for san in ("asan", "tsan", "ubsan")
       for phase in ("tests", "slow", "backend")])


class BuildMatrix(unittest.TestCase):
    def test_per_pr_cells(self):
        self.assertEqual(names(ci_matrix.build_cells(False)), PR_BUILD)

    def test_nightly_adds_exactly_the_slow_cells(self):
        release = [n for n in PR_BUILD if "-Release-" in n]
        self.assertEqual(names(ci_matrix.build_cells(True)), sorted(release + NIGHTLY_ONLY_BUILD))

    def test_nightly_only_cells_are_marked(self):
        marked = names(c for c in ci_matrix.build_cells(True) if c.get("nightly_only") == "ON")
        self.assertEqual(marked, NIGHTLY_ONLY_BUILD)
        self.assertFalse(any(c.get("nightly_only") for c in ci_matrix.build_cells(False)))

    def test_every_cell_names_a_runner_and_a_generator(self):
        for cell in ci_matrix.build_cells(True):
            self.assertTrue(cell.get("runner"), cell)
            self.assertEqual(cell.get("cmake_generator"), "Ninja", cell)
            self.assertEqual(cell.get("build_system"), "cmake", cell)

    def test_windows_release_64_is_the_llvm_free_gate(self):
        cell = [c for c in ci_matrix.build_cells(False)
                if c["target"] == "windows" and c["architecture"] == 64 and c["cmake_preset"] == "Release"][0]
        self.assertEqual((cell["llvm_disabled"], cell["jit_disabled"]), ("ON", "ON"))

    def test_release_cells_carry_archive_fields(self):
        for cell in ci_matrix.build_cells(False):
            self.assertIn("release_target", cell, cell)
            self.assertIn("release_arch", cell, cell)
            self.assertIn("archive_ext", cell, cell)

    def test_sanitizer_cells_carry_no_archive_fields(self):
        for cell in ci_matrix.build_cells(True):
            if cell["sanitizers"] != "none":
                self.assertNotIn("release_target", cell, cell)
                self.assertEqual(cell["build_name"], "linux_" + cell["sanitizers"])

    def test_only_sanitizer_cells_split_into_phases(self):
        # build.yml gates its three test steps on the phase; a cell that ran none of them would be
        # a build with no signal, and a non-sanitizer cell split would multiply the per-PR matrix.
        # windows 64 Debug is the one cell that runs none of the three: its signal is the compile
        for cell in ci_matrix.build_cells("schedule"):
            if cell["sanitizers"] != "none":
                self.assertIn(cell["phase"], ("tests", "slow", "backend"), cell)
            elif (cell["target"], cell["cmake_preset"]) == ("windows", "Debug"):
                self.assertEqual(cell["phase"], "compile", cell)
            else:
                self.assertEqual(cell["phase"], "all", cell)

    def test_the_compile_only_cell_has_a_step_that_compiles(self):
        # a phase no step's condition names is a job that configures and goes green
        text = open(os.path.join(WORKFLOWS, "build_matrix.yml"), encoding="utf-8").read()
        block = re.search(r"- name: \"Compile\"\n((?:      .*\n)+)", text)
        self.assertTrue(block, "build_matrix.yml has no Compile step")
        self.assertIn("if: matrix.phase == 'compile'", block.group(1))
        self.assertIn("cmake --build ./build", block.group(1))
        self.assertLess(text.index('- name: "Compile"'), text.index('- name: "Save sccache objects"'))

    def test_every_sccache_slot_is_bounded(self):
        # sccache's default bound is 10G a slot; an unbounded slot grows to it and evicts the
        # slots the nightly cells save once a day
        for name in ("build_matrix.yml", "extended_checks.yml"):
            text = open(os.path.join(WORKFLOWS, name), encoding="utf-8").read()
            self.assertGreaterEqual(text.count('echo "SCCACHE_CACHE_SIZE='), 2, name)

    def test_half_speed_sweeps_carry_raised_watchdogs(self):
        # the sanitizer cells run their sweeps at half speed and lose the default whole-suite
        # watchdog to runner variance; every other cell keeps the defaults, so a hang still
        # fails fast where the sweep is quick
        for cell in ci_matrix.build_cells(True):
            slow = cell["sanitizers"] != "none"
            for flag in ("-DDAS_TEST_TIMEOUT=3600", "-DDAS_TEST_TIMEOUT_ISOLATED=7200",
                         "-DDAS_TEST_TIMEOUT_AOT=3600"):
                (self.assertIn if slow else self.assertNotIn)(flag, cell["cmake_extra"], cell)
        for cell in ci_matrix.build_cells(False):
            self.assertNotIn("DAS_TEST_TIMEOUT", cell["cmake_extra"], cell)


class ExtendedMatrix(unittest.TestCase):
    def test_per_pr_is_two_linux_roles(self):
        cells = ci_matrix.extended_cells(False)
        self.assertEqual([(c["target"], c["role"]) for c in cells], [("linux", "core"), ("linux", "modules")])

    def test_nightly_is_one_full_job_on_linux_and_two_halves_on_windows(self):
        cells = ci_matrix.extended_cells(True)
        self.assertEqual([(c["target"], c["role"]) for c in cells],
                         [("linux", "all"), ("windows", "all_core"), ("windows", "all_modules")])

    def test_every_role_names_a_group_and_the_two_halves_make_the_whole(self):
        with open(os.path.join(HERE, "CMakeLists.txt"), encoding="utf-8") as f:
            text = f.read()
        for nightly in (False, True):
            for cell in ci_matrix.extended_cells(nightly):
                self.assertIn("das_lane_group(run_extended_%s " % cell["role"], text)
        self.assertIn("set(_extended_all_modules ${_extended_all})\n"
                      "list(REMOVE_ITEM _extended_all_modules ${DAS_EXTENDED_CORE} ${DAS_EXTENDED_NIGHTLY})\n"
                      "set(_extended_all_core ${_extended_all})\n"
                      "list(REMOVE_ITEM _extended_all_core ${_extended_all_modules})\n", text)
        self.assertRegex(text, r"das_lane_group\(run_extended_all_core \"[^\"]+\" \$\{_extended_all_core\}\)")
        self.assertRegex(text, r"das_lane_group\(run_extended_all_modules \"[^\"]+\" \$\{_extended_all_modules\}\)")

    def test_only_a_lane_that_runs_the_aot_subset_builds_it(self):
        """Every dastest target depends on dastest_cache_guard, so a binary it hashes is a binary
        every extended cell builds. The subset binary is hashed by a guard of its own, over a
        folder inside the cache: the main guard's drop takes it too, and the slot carries it."""
        tree = os.path.dirname(HERE)
        with open(os.path.join(tree, "CMakeLists.txt"), encoding="utf-8") as f:
            top = f.read()
        hashed = top.split("set(_das_test_binaries)", 1)[1].split("endforeach()", 1)[0]
        self.assertIn("foreach(_t daslang ", hashed)
        self.assertNotIn("test_aot", hashed)
        self.assertIn("add_dependencies(dastest_aot_cache_guard dastest_cache_guard test_aot_subset)", top)
        self.assertIn("-DDIR=${DAS_TEST_AOT_CACHE_DIR}", top)
        # a configure without the AOT tests has no subset binary to guard
        self.assertIn("    if(TARGET test_aot_subset)\n        set(_aot_identity ", top)
        self.assertIn("-E sha256sum $<TARGET_FILE:test_aot_subset> > ${_aot_identity}\n", top)
        # the guard script reads the last identity under this one name
        self.assertIn("-E copy ${_aot_identity} ${DAS_TEST_AOT_CACHE_DIR}/binary_identity.txt)\n", top)
        with open(os.path.join(tree, "cmake", "das_lane.cmake"), encoding="utf-8") as f:
            self.assertIn("set(DAS_TEST_AOT_CACHE_DIR ${DAS_TEST_CACHE_DIR}/aot_subset)\n", f.read())
        with open(os.path.join(tree, "tests", "CMakeLists.txt"), encoding="utf-8") as f:
            run = f.read().split("add_das_target(run_tests_aot_subset", 1)[1].split(")\n", 1)[0]
        self.assertIn("--cache-dir ${DAS_TEST_AOT_CACHE_DIR}", run)
        self.assertIn("DEPENDS test_aot_subset dastest_aot_cache_guard", run)
        self.assertNotIn("${DAS_TEST_CACHE_DIR}", run)

    def test_the_macos_only_check_rides_the_build_lane(self):
        # the extended checks have no macOS cell, so the one check cmake gives to APPLE alone has
        # to run where a macOS job still builds per PR
        text = open(os.path.join(os.path.dirname(HERE), "ci", "CMakeLists.txt"), encoding="utf-8").read()
        block = text.split("set(DAS_BUILD_LANE_APPLE", 1)[1].split("das_lane_group(run_build ", 1)
        self.assertIn("run_tests_llvm_vector_math", block[0])
        self.assertIn("${DAS_BUILD_LANE_APPLE}", block[1].split(")", 1)[0])

    def test_every_cell_names_a_runner(self):
        for nightly in (False, True):
            for cell in ci_matrix.extended_cells(nightly):
                self.assertTrue(cell.get("runner"), cell)
                self.assertIn("architecture", cell, cell)


class WorkflowShapes(unittest.TestCase):
    """The workflows consume the matrices through pre_job; the role split must leave every step
    running in some nightly job of each platform."""

    def read(self, name):
        with open(os.path.join(WORKFLOWS, name), encoding="utf-8") as f:
            return f.read()

    def test_both_workflows_read_their_matrix_from_pre_job(self):
        for name, job in (("build_matrix.yml", "matrix"), ("extended_checks.yml", "pre_job")):
            text = self.read(name)
            self.assertIn("matrix: ${{ fromJSON(needs.%s.outputs.matrix) }}" % job, text, name)
            self.assertIn("ci/ci_matrix.py ", text, name)

    def test_only_nightly_yml_carries_a_cron(self):
        # a workflow with a cron of its own is one a #nightly PR never arms
        for name in sorted(os.listdir(WORKFLOWS)):
            if not name.endswith(".yml") or name == "nightly.yml":
                continue
            text = self.read(name)
            self.assertFalse(re.search(r"^  schedule:", text, re.M), "%s carries a cron of its own" % name)
            self.assertFalse("uses: ./.github/workflows/nightly_issue.yml" in text, "%s files its own nightly issue" % name)

    def test_role_conditions_only_exclude(self):
        # `matrix.role == 'core'` would drop the step from the nightly `all` job; the only admitted
        # spelling names the role a step does NOT run in
        text = self.read("extended_checks.yml")
        refs = re.findall(r"matrix\.role\s*(==|!=)\s*'([^']*)'", text)
        self.assertTrue(refs, "extended_checks.yml carries no role conditions")
        for op, role in refs:
            self.assertEqual(op, "!=", "matrix.role %s '%s'" % (op, role))
            self.assertIn(role, ("core", "modules", "all_modules"), "matrix.role %s '%s'" % (op, role))

    def test_one_job_of_a_slot_saves_it(self):
        # jobs of one platform share a slot key; the module-suite job of each pair leaves the save
        # and the prune to the other, so two jobs never write one key
        text = self.read("extended_checks.yml")
        steps = text.split("- name:")
        conditioned = [step.split("\n", 1)[0].strip() for step in steps if "matrix.role !=" in step]
        self.assertEqual(conditioned, ['"Save sccache objects"', '"Prune older sccache slots"'])
        for step in steps:
            if "matrix.role !=" in step:
                self.assertIn("matrix.role != 'modules' && matrix.role != 'all_modules'", step)
        for nightly in (False, True):
            by_target = {}
            for cell in ci_matrix.extended_cells(nightly):
                by_target.setdefault(cell["target"], []).append(cell["role"])
            for target, roles in by_target.items():
                savers = [r for r in roles if r not in ("modules", "all_modules")]
                self.assertEqual(len(savers), 1, "%s: %s" % (target, roles))

    def test_every_cache_restore_runs_on_the_cron_too(self):
        # the nightly cells are the slow ones; a restore gated off `schedule` is the old cold-build
        # policy coming back
        for name in ("build.yml", "build_matrix.yml", "nightly.yml", "extended_checks.yml"):
            text = self.read(name)
            for block in re.findall(r"- name: \"(?:Restore|Cache) [^\"]*\"\n((?:      .*\n)+)", text):
                self.assertNotIn("github.event_name != 'schedule'", block, name)

    def test_each_sanitizer_slot_has_one_writer(self):
        # tests and backend share a slot the tests job writes; slow compiles the full test_aot,
        # which is in no other job's objects, so it keeps a slot of its own
        text = self.read("build_matrix.yml")
        self.assertIn("github.ref == 'refs/heads/master' && matrix.phase != 'backend'", text)
        self.assertEqual(text.count("${{ matrix.phase == 'slow' && '-slow' || '' }}@"), 4)
        self.assertNotIn("matrix.nightly_only != 'ON'", text)

    def test_the_three_test_steps_gate_on_the_phase(self):
        text = self.read("build_matrix.yml")
        self.assertIn("(matrix.phase == 'all' || matrix.phase == 'tests')", text)
        self.assertIn("inputs.lane == 'build_nightly' && matrix.phase == 'slow'", text)
        self.assertIn("inputs.lane == 'build_nightly' && matrix.phase == 'backend'", text)

    def test_the_fast_math_cell_runs_the_per_pr_lane_narrowed_and_no_nightly_step(self):
        cells = [c for c in ci_matrix.build_cells(True) if c["fast_math"] == "ON"]
        self.assertEqual([(c["phase"], c.get("nightly_only")) for c in cells], [("all", "ON")])
        step = self.read("build_matrix.yml").split('- name: "Nightly Tests"', 1)[1].split("- name:", 1)[0]
        self.assertIn("      if: matrix.cmake_preset == 'Release' && matrix.sanitizers == 'none'"
                      " && matrix.fast_math != 'ON' && inputs.lane == 'build_nightly'\n", step)
        tests_cmake = self.read_tests_cmake()
        self.assertIn("        elseif(DAS_FAST_MATH)\n"
                      "            set(_DAS_JIT_TEST_DIRS ${_DAS_FAST_MATH_JIT_DIRS})\n", tests_cmake)
        self.assertIn("    if(DAS_FAST_MATH)\n"
                      "        set(_aot_subset_test_dirs ${_DAS_FAST_MATH_AOT_DIRS})\n", tests_cmake)
        self.assertIn("set(_DAS_FAST_MATH_AOT_DIRS ${PROJECT_SOURCE_DIR}/tests/language"
                      " ${PROJECT_SOURCE_DIR}/tests/math)\n", tests_cmake)

        with open(os.path.join(os.path.dirname(HERE), "tests", "aot", "CMakeLists.txt"), encoding="utf-8") as f:
            aot_cmake = f.read()
        self.assertIn("if(DAS_FAST_MATH)\n    list(APPEND AOT_SUBSET_DASLIB_MODULE_FILES"
                      " daslib/math_boost.das daslib/color_mapping.das)\n", aot_cmake)
        self.assertIn("if(DAS_FAST_MATH)\n    target_sources(test_aot_subset PRIVATE"
                      " ${MATH_AOT_GENERATED_SRC} ${MATH_MODULES_AOT_GENERATED_SRC})\n"
                      "    add_dependencies(test_aot_subset test_aot_math test_aot_math_modules)\n", aot_cmake)

    def read_tests_cmake(self):
        with open(os.path.join(os.path.dirname(HERE), "tests", "CMakeLists.txt"), encoding="utf-8") as f:
            return f.read()

    def test_the_fast_math_jit_sweep_takes_every_folder_with_a_fast_math_arm(self):
        """A JIT run on a fast-math host turns the JIT's own fast-math on, and a test says what it
        expects there through HOST_FAST_MATH. A folder with such a test that the fast-math sweep
        leaves out is an arm no JIT run reaches."""
        listed = re.search(r"set\(_DAS_FAST_MATH_JIT_DIRS\)\nforeach\(_dir ([a-z_ ]+)\)\n"
                           r"    list\(APPEND _DAS_FAST_MATH_JIT_DIRS \$\{PROJECT_SOURCE_DIR\}/tests/\$\{_dir\}\)\n",
                           self.read_tests_cmake())
        self.assertIsNotNone(listed)
        tests = os.path.join(os.path.dirname(HERE), "tests")
        with_arm = set()
        for folder, _, files in os.walk(tests):
            if "_generated" in folder:
                continue
            for name in files:
                if not name.endswith(".das"):
                    continue
                with open(os.path.join(folder, name), encoding="utf-8", errors="replace") as f:
                    if "HOST_FAST_MATH" in f.read():
                        with_arm.add(os.path.relpath(folder, tests).replace(os.sep, "/").split("/")[0])
        self.assertEqual(sorted(listed.group(1).split()), sorted(with_arm))

    def test_slots_are_saved_under_a_run_key_and_restored_by_prefix(self):
        # a delete-then-save refresh left the key empty for the length of a build, and a run
        # cancelled between the two left it empty for good; a run-unique key with a prefix restore
        # never has a gap, and the prune after the save keeps one entry per slot
        for name in ("build.yml", "build_matrix.yml", "nightly.yml", "extended_checks.yml"):
            text = self.read(name)
            self.assertNotIn("Refresh sccache slot", text, name)
            restores = re.findall(r"- name: \"Restore sccache objects\"\n((?:      .*\n)+)", text)
            saves = re.findall(r"- name: \"Save sccache objects\"\n((?:      .*\n)+)", text)
            prunes = text.count('- name: "Prune older sccache slots"')
            self.assertTrue(restores, name)
            self.assertEqual(len(saves), len(restores), name)
            self.assertEqual(prunes, len(saves), name)
            for block in restores:
                self.assertRegex(block, r"key: sccache-[^\n]+@\$\{\{ github\.run_id \}\}\n", name)
                self.assertRegex(block, r"restore-keys: sccache-[^\n]+@\n", name)
            for block in saves:
                self.assertRegex(block, r"key: sccache-[^\n]+@\$\{\{ github\.run_id \}\}\n", name)

    def test_every_workflow_restores_sccache_by_prefix(self):
        # a consumer in another workflow that restores a slot by its exact old name would go cold
        # the moment the writer moved to run keys
        for name in sorted(os.listdir(WORKFLOWS)):
            if not name.endswith(".yml"):
                continue
            text = self.read(name)
            for block in re.findall(r"uses: actions/cache/restore@v4\n((?:      .*\n)+)", text):
                if "key: sccache-" not in block:
                    continue
                self.assertRegex(block, r"key: sccache-[^\n]+@\$\{\{ github\.run_id \}\}\n", name)
                self.assertRegex(block, r"restore-keys: sccache-[^\n]+@\n", name)

    def test_master_push_builds_are_not_cancelled_by_the_next_merge(self):
        text = self.read("build.yml")
        self.assertIn("cancel-in-progress: ${{ github.event_name == 'pull_request' }}", text)

    def test_the_slot_is_saved_after_the_lane_compiled(self):
        # Build only configures; the lane targets compile, so a save placed before them lands
        # an empty slot
        text = self.read("build_matrix.yml")
        self.assertLess(text.index('- name: "Nightly Backend Sweeps"'), text.index('- name: "Save sccache objects"'))

    # the checks that run only on the nightly cron, pinned by name: moving another off the
    # per-PR path is a deliberate edit here, with its preflight mirror or platform reason stated
    NIGHTLY_ONLY = {
        "run_examples",
        "dry_run_tutorials",
        "check_doc_verify",
        "check_nano_arm",
        "check_ast_verify_tree",
        "run_coverage",
        "run_tests_llvm_tune_slow",
    }

    def test_nightly_only_checks_are_exactly_the_pinned_set(self):
        """extended_checks.yml runs one group target per role; ci/CMakeLists.txt assigns the
        checks, so the nightly-only set is read there rather than from step conditions."""
        text = open(os.path.join(os.path.dirname(HERE), "ci", "CMakeLists.txt"), encoding="utf-8").read()
        body = text.split("set(DAS_EXTENDED_NIGHTLY", 1)[1].split("das_lane_group", 1)[0]
        named = set(re.findall(r"\b(?:check|run|dry)_[a-z_0-9]+", body))
        self.assertEqual(named, self.NIGHTLY_ONLY)

    def test_the_windows_cell_leaves_out_only_checks_a_linux_cell_runs(self):
        """A check a windows configure drops must stay in a list the linux cells build, or no cell runs it."""
        text = self.cmake_lists()

        def names(list_name):
            body = text.split("set(" + list_name, 1)[1].split(")", 1)[0]
            return set(re.findall(r"\b(?:check|run|dry)_[a-z_0-9]+", body))

        dropped = names("DAS_EXTENDED_HOST_INDEPENDENT")
        self.assertTrue(dropped)
        self.assertEqual(dropped - names("DAS_EXTENDED_CORE") - names("DAS_EXTENDED_MODULES"), set())
        removal = "list(REMOVE_ITEM _extended_all ${DAS_EXTENDED_HOST_INDEPENDENT})"
        self.assertIn(removal, text)
        # the nearest `if` or `endif` above the removal is the windows guard, so only windows drops them
        guards = re.findall(r"^\s*(if|endif)\(([^)]*)\)", text[:text.index(removal)], flags=re.M)
        self.assertEqual(guards[-1], ("if", "WIN32"))

    def cmake_lists(self):
        with open(os.path.join(HERE, "CMakeLists.txt"), encoding="utf-8") as f:
            return f.read()

    def test_every_ci_python_test_is_a_target_of_an_extended_list(self):
        """das_lane_group skips a name that is no target and only prints a status line, so a test file
        with no das_lane_python, or a target in no list, stops running with nothing red."""
        text = self.cmake_lists()
        for name in sorted(os.listdir(HERE)):
            if not (name.startswith("test_") and name.endswith(".py")):
                continue
            lane = re.search(r'das_lane_python\((\w+)[^)]*"ci/' + re.escape(name) + r'[ "]', text)
            self.assertIsNotNone(lane, f"ci/{name} is no das_lane_python target")
            lists = "\n".join(line for line in text.splitlines()
                              if "das_lane_python(" not in line and not line.lstrip().startswith("#"))
            self.assertRegex(lists, r"\b" + lane.group(1) + r"\b", f"{lane.group(1)} is in no check list")

    # every job that drives a lane through ninja, with the build directory its configure names
    TIMED_JOBS = {
        "build_matrix.yml": ["build"],
        "extended_checks.yml": ["build"],
        "nightly.yml": ["build", "build", "build-mingw", "build-clangcl"],
    }

    def test_each_ninja_lane_prints_its_time_by_target_before_the_slot_is_saved(self):
        step = '- name: "Build time by target"'
        save = '- name: "Save sccache objects"'
        for workflow, build_dirs in self.TIMED_JOBS.items():
            text = self.read(workflow)
            found = [m.start() for m in re.finditer(re.escape(step), text)]
            self.assertEqual(len(found), len(build_dirs), workflow)
            for at, build_dir in zip(found, build_dirs):
                body = text[at:text.index(save, at)]
                self.assertEqual(body.count("- name:"), 1, f"{workflow}: the step is the last before the save")
                self.assertIn("if: ${{ !cancelled() }}", body)
                self.assertIn(f"run: python3 ci/ninja_log_report.py {build_dir}/.ninja_log", body)
                job = text[text.rfind(save, 0, at) + 1:at]
                self.assertRegex(job, r"-B\s*(\./)?" + re.escape(build_dir) + r"(?![-\w])",
                                 f"{workflow}: the job configures {build_dir}")
                self.assertIn("cmake --build", job)


class CommandLine(unittest.TestCase):
    def run_tool(self, *args):
        tool = os.path.join(HERE, "ci_matrix.py")
        return subprocess.run([sys.executable, tool, *args], capture_output=True, text=True)

    def test_emits_one_json_line_the_workflow_can_fromjson(self):
        for kind in ("build", "extended"):
            out = self.run_tool(kind)
            self.assertEqual(out.returncode, 0, out.stderr)
            self.assertEqual(out.stdout.count("\n"), 1)
            self.assertIn("include", json.loads(out.stdout))

    def test_rejects_an_unknown_matrix(self):
        self.assertEqual(self.run_tool("release").returncode, 2)

    def test_rejects_a_missing_lane(self):
        self.assertEqual(self.run_tool().returncode, 2)

    def test_each_kind_emits_its_own_cell_shape(self):
        build = json.loads(self.run_tool("build").stdout)["include"]
        extended = json.loads(self.run_tool("extended").stdout)["include"]
        self.assertTrue(all("sanitizers" in c and "role" not in c for c in build))
        self.assertTrue(all("role" in c and "sanitizers" not in c for c in extended))


if __name__ == "__main__":
    unittest.main()
