# PR babysitting has ONE tool: daslang utils/internal/pr-babysit/main.das -- --pr <N> --watch
# (CI + latest-review tip match + unresolved threads; exit code IS the verdict — run it BARE
# in a background Bash, never piped). Deny hand-rolled PR watchers wherever they are launched:
# any `gh pr checks` inside a Monitor, and a backgrounded/looping `gh pr checks --watch` in
# Bash. A one-shot `gh pr checks <N>` peek from Bash stays allowed.
if ((.tool_input.command // "") | test("gh pr checks"))
   and ((.tool_name == "Monitor")
        or ((.tool_input.command // "") | test("--watch|while |until |sleep ")))
then
  {
    hookSpecificOutput: {
      hookEventName: "PreToolUse",
      permissionDecision: "deny",
      permissionDecisionReason: "Hand-rolled PR watching. The babysit driver is `daslang utils/internal/pr-babysit/main.das -- --pr <N> --watch` — it watches CI, the latest review against the tip, and unresolved threads, and its exit code is the verdict (0 merge-ready, 2 CI red, 3 review needs action, 5 timeout). Run it BARE in a background Bash, never piped. See skills/internal/babysit.md."
    }
  }
else
  empty
end
