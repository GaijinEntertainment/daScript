options gen2

require math
require strings
require daslib/linq
require daslib/linq_boost
require daslib/linq_fold
require daslib/decs
require daslib/decs_boost
require dastest/testing_boost public

struct Sound {
    id   : int
    x    : float
    rank : int
}

[decs_template(prefix = "ds_")]
struct DecsSound {
    id : int
    x : float
}

[decs_template(prefix = "dc_")]
struct DecsCar {
    id : int
    dealer_id : int
    name : string
}

[decs_template(prefix = "dd_")]
struct DecsDealer {
    id : int
    name : string
}

def make_sounds() : array<Sound> {
    return <- [
        Sound(id = 1, x = 3.0, rank = 1),
        Sound(id = 2, x = 1.0, rank = 2),
        Sound(id = 3, x = 4.0, rank = 3),
        Sound(id = 4, x = 1.0, rank = 4),
        Sound(id = 5, x = 5.0, rank = 5)
    ]
}

[test]
def test_order_take_select_array(t : T?) {
    t |> run("plan_order_family: take + terminal _select") @(tt : T?) {
        let sounds <- make_sounds()
        // Closest 3 by |x|; return their ids.
        unsafe {
            let ids <- _fold(each(sounds)._order_by(abs(_.x)).take(3)._select(_.id).to_array())
            // Sounds with smallest |x|: id 2 (1.0), id 4 (1.0), id 1 (3.0).
            tt |> equal(length(ids), 3)
            tt |> equal(true, ids[0] == 2 || ids[0] == 4)
            tt |> equal(true, ids[2] == 1)
        }
    }
}

[test]
def test_where_order_take_select_array(t : T?) {
    t |> run("plan_order_family: where + take + terminal _select") @(tt : T?) {
        let sounds <- make_sounds()
        unsafe {
            let ids <- _fold(each(sounds)._where(_.rank >= 2)._order_by(_.x).take(2)._select(_.id).to_array())
            // After filter rank>=2: ids 2,3,4,5 (x = 1,4,1,5). Top 2 by x: ids 2 (1.0), 4 (1.0).
            tt |> equal(length(ids), 2)
            for (id in ids) {
                tt |> equal(true, id == 2 || id == 4)
            }
        }
    }
}

[test]
def test_where_order_bare_select_array(t : T?) {
    t |> run("plan_order_family: where + bare order + terminal _select") @(tt : T?) {
        let sounds <- make_sounds()
        unsafe {
            let ranks <- _fold(each(sounds)._where(_.id != 3)._order_by(_.x)._select(_.rank).to_array())
            // After filter id!=3: ids 1,2,4,5 (x = 3,1,1,5). Sorted by x: (2 or 4, 2 or 4, 1, 5).
            tt |> equal(length(ranks), 4)
            tt |> equal(ranks[3], 5)
        }
    }
}

[test]
def test_order_take_select_decs(t : T?) {
    t |> run("plan_decs_order_family: take + terminal _select") @(tt : T?) {
        restart()
        create_entities(5) $(eid : EntityId; i : int; var cmp : ComponentMap) {
            apply_decs_template(cmp, DecsSound(id = i + 1, x = float((i % 2) * 5 + 1)))
        }
        unsafe {
            let ids <- _fold(from_decs_template(type<DecsSound>)._order_by(_.x).take(2)._select(_.id).to_array())
            tt |> equal(length(ids), 2)
        }
        restart()
    }
}

[test]
def test_reverse_take_select_array(t : T?) {
    t |> run("plan_reverse: where + reverse + take + terminal _select") @(tt : T?) {
        let sounds <- make_sounds()
        unsafe {
            let ids <- _fold(each(sounds)._where(_.rank > 0).reverse().take(2)._select(_.id).to_array())
            // After filter (all 5), reverse: 5,4,3,2,1. take 2: 5,4.
            tt |> equal(length(ids), 2)
            tt |> equal(ids[0], 5)
            tt |> equal(ids[1], 4)
        }
    }
}

[test]
def test_reverse_select_first_array(t : T?) {
    t |> run("plan_reverse: where + reverse + _select + first") @(tt : T?) {
        let sounds <- make_sounds()
        unsafe {
            let id = _fold(each(sounds)._where(_.rank > 0).reverse()._select(_.id).first())
            // Reverse of ids 1..5 is 5,4,3,2,1; first = 5.
            tt |> equal(id, 5)
        }
    }
}

[test]
def test_reverse_take_select_decs(t : T?) {
    t |> run("plan_decs_reverse: reverse + take + terminal _select") @(tt : T?) {
        restart()
        create_entities(4) $(eid : EntityId; i : int; var cmp : ComponentMap) {
            apply_decs_template(cmp, DecsSound(id = i + 1, x = float(i)))
        }
        unsafe {
            let ids <- _fold(from_decs_template(type<DecsSound>).reverse().take(2)._select(_.id).to_array())
            tt |> equal(length(ids), 2)
        }
        restart()
    }
}

[test]
def test_join_select_to_array(t : T?) {
    t |> run("plan_decs_join: join + terminal _select") @(tt : T?) {
        restart()
        create_entities(3) $(eid : EntityId; i : int; var cmp : ComponentMap) {
            apply_decs_template(cmp, DecsCar(id = i + 1, dealer_id = i % 2 + 1, name = "Car{i}"))
        }
        create_entities(2) $(eid : EntityId; i : int; var cmp : ComponentMap) {
            apply_decs_template(cmp, DecsDealer(id = i + 1, name = "Dealer{i}"))
        }
        unsafe {
            let names <- _fold(from_decs_template(type<DecsCar>) |> _join(from_decs_template(type<DecsDealer>),
                                                                          $(l, r) => l.dealer_id == r.id,
                                                                          $(l, r) => (CarName = l.name, DealerName = r.name))
                                                                 |> _select(_.CarName)
                                                                 |> to_array())
            // 3 cars × 1 matching dealer each = 3 results.
            tt |> equal(length(names), 3)
        }
        restart()
    }
}

[test]
def test_zip_3arg_sum(t : T?) {
    t |> run("plan_zip: 3-arg zip + sum") @(tt : T?) {
        let a <- [1, 2, 3, 4]
        let b <- [10, 20, 30, 40]
        unsafe {
            let total = _fold(each(a) |> zip(each(b), $(x, y : int) => x * y) |> sum())
            // 1*10 + 2*20 + 3*30 + 4*40 = 10 + 40 + 90 + 160 = 300.
            tt |> equal(total, 300)
        }
    }
}

[test]
def test_zip_3arg_to_array(t : T?) {
    t |> run("plan_zip: 3-arg zip + to_array") @(tt : T?) {
        let a <- [1, 2, 3]
        let b <- [10, 20, 30]
        unsafe {
            let r <- _fold(each(a) |> zip(each(b), $(x, y : int) => x + y) |> to_array())
            tt |> equal(length(r), 3)
            tt |> equal(r[0], 11)
            tt |> equal(r[1], 22)
            tt |> equal(r[2], 33)
        }
    }
}
