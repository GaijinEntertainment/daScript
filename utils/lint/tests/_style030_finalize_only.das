options gen2
// Helper for style030_unused_require.das: a module whose only value is a
// lifecycle hook. [finalize] here stands in for the whole hook set the rule
// skips ([init]/[finalize]/[before_reload]/[after_reload]/[live_command]) -
// they share one annotation-name check.
module _style030_finalize_only shared private

var private touched = 0

[finalize]
def shutdown_hook {
    touched = 0
}
