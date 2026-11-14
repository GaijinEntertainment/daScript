options gen2
options auto_inline_functions = false
// lint-skip-file: test_lint035_sites expects each marked line

struct Session {
    n_past : int
}

struct Pool {
    @pool free : array<Session?>
}

struct Shelf {
    @pool held : array<Session?>              // LINT035
}

struct Bins {
    @pool bins : table<int; array<Session?>>  // LINT035
}

struct Ledger {
    @pool rows : array<Session?>
}

struct Tray {
    @pool kept : array<Session?>
}

struct Rack {
    tray : Tray
}

struct Duo {
    @pool held : array<Session?>
}

struct Stash {
    @pool held : array<Session?>              // LINT035
}

struct Tote {
    @scratch held : array<Session?>
}

struct Counts {
    @pool n : array<int>
}

class Base {
    @pool cached : array<Session?>
}

class Derived : Base {
    extra : int
}

struct Crate {
    @pool items : array<Session?>
}

struct BigCrate : Crate {
    size : int
}

var private g_pool : Pool
var private g_shelf : Shelf
var private g_bins : Bins
var private g_ledger : Ledger
var private g_rack : Rack
var private g_duo : Duo[2]
var private g_counts : Counts
var private g_stash : Stash
var private g_tote : Tote
var private g_derived : Derived?
var private g_big : BigCrate
var private @pool g_free : array<Session?>
var private @pool g_lost : array<Session?>      // LINT035
var private @pool g_sized : array<Session?>
var private @pool g_by_id : table<int; Session?>
var private @pool g_ints : array<int>

// ===== fills: a pool keeps what it is handed; no LINT034 =====

def give_back(var sp : Session?) : void {
    g_pool.free |> push(sp)
    g_shelf.held |> push(sp)
    g_ledger.rows |> push(sp)
    g_rack.tray.kept |> push(sp)
    g_duo[0].held |> push(sp)
    g_stash.held |> push(sp)
    g_free |> push(sp)
    g_lost |> push(sp)
    g_sized |> push(sp)
    g_by_id |> insert(1, sp)
    g_bins.bins[0] |> push(sp)
    g_derived.cached |> push(sp)
    g_big.items |> push(sp)
}

// ===== drains =====

// emptying a local instance drains no instance that outlives it
def stash_local_only : void {
    var tmp : Stash
    tmp.held |> clear()
    unsafe {
        delete tmp
    }
}

// the message's other remedy: contents that die with the call
def tote_use(var sp : Session?) : int {
    g_tote.held |> push(sp)
    return length(g_tote.held)
} finally {
    g_tote.held |> clear()
}

def pool_drain(var p : Pool) : void {
    p.free |> clear()
}

def bins_drop_one(b : int) : void {
    g_bins.bins[b] |> clear()          // one bucket emptied is not the pool drained
}

def ledger_drop : void {
    unsafe {
        delete g_ledger
    }
}

def duo_drop : void {
    unsafe {
        delete g_duo
    }
}

def rack_drop(var r : Rack) : void {
    unsafe {
        delete r
    }
}

def free_drop : void {
    g_free |> resize(0)
}

def sized_drop : void {
    var lst & = unsafe(g_sized)
    lst |> clear()
}

def by_id_drop : void {
    unsafe {
        delete g_by_id
    }
}

def derived_drop : void {
    g_derived.cached |> clear()
    g_big.items |> clear()
}

[export]
def main {
    give_back(null)
    pool_drain(g_pool)
    bins_drop_one(0)
    ledger_drop()
    rack_drop(g_rack)
    duo_drop()
    stash_local_only()
    tote_use(null)
    free_drop()
    sized_drop()
    by_id_drop()
    derived_drop()
    g_counts.n |> push(1)
    g_ints |> push(1)
    g_shelf.held |> resize(1)
}
