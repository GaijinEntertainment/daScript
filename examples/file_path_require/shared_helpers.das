// shared_helpers.das — module imported via `require ./shared_helpers.das`
// from main.das and from project.das_project. Demonstrates that a single
// .das file can be reused by both the project bootstrap and ordinary
// scripts.

options gen2
module shared_helpers

[export]
def shared_greeting() : string {
    return "Hello from shared_helpers.das"
}
