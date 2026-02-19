This macro converts function into generator.
Generator yields bool if its a void function (coroutine), and yields the return type otherwise (async return).
async function can wait for another async function using await(<async fn call>).
use 'return false' to immediately return from the generator.
