//! A table or set keyed by a distinct type must reject a key of
//! another distinct over the same underlying type (30251, 30341 three
//! times).
options gen2

expect 30251
expect 30341 : 3

typedef distinct Id = int
typedef distinct OtherId = int

[export]
def main {
    var tab : table<Id; int>
    tab[OtherId(1)] = 1             // 30251: the key must be Id, not the other distinct over int
    key_exists(tab, OtherId(1))     // 30341: no key_exists over table<Id> with an OtherId
    var arr : array<Id>
    arr |> push(OtherId(2))         // 30341: an array of Id takes no OtherId
    var set : table<Id>
    set |> insert(OtherId(3))       // 30341: a set of Id takes no OtherId
}
