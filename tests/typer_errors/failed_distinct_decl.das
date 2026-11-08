//! A distinct of a distinct, and a distinct over a non-workhorse
//! type, must not compile (30297 twice).
options gen2

expect 30297 : 2

typedef distinct Id = int
typedef distinct Id2 = Id           // 30297: distinct of distinct
typedef distinct Bad = array<int>   // 30297: non-workhorse underlying
