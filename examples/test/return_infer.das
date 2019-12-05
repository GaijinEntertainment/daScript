options indenting=2
options log_infer_passes = true

// from squad config

struct SoldierConfig
  sClass : string
  tier : int = 1
  level : int = 1

struct SquadConfig
  id : string = "unknown"
  locId : string = "unknown"
  campaignName : string
  size : int = 7  //base squad size, can be increased by researches
  maxClassesDefault : int //limit for not listed classes
  maxClasses : table<string ; int>
  startSoldiers : array<SoldierConfig>
  startVehicle : string
  unlockCost : int
  newClass : string
  newPerk : string
  newWeapon : string

// from army effects

struct ArmyEffects
  squad_size : table<string; int>
  squad_class_limit : table<string; table<string; int> >
  soldier_drop : table<string; table<string; int> >
  weapon_drop : table<string; float>

// from item generation

struct ItemGeneration
  items : array< tuple<string; float> >
  amount : int2

def ItemGeneration(var items2 : array< tuple<string; float> >; amount : int2 = int2(1,1))
  var res = new ItemGeneration
  res.items <- items2
  res.amount = amount
  return res

def ItemGeneration(var items2 : array<string>; amount : int2 = int2(1,1))
  var res = new ItemGeneration
  for item in  items2
    push(res.items, [[auto item, 1.]])

  res.amount = amount
  return res

def ItemGeneration(item : string)
  var res = new ItemGeneration
  push(res.items, [[auto item, 1.]])
  res.amount = int2(1,1)
  return res

// end item generation

// hacking random pick

def random_pick_idx_w(source)
  return 0

def random_pick_w(source)
  let idx = random_pick_idx_w(source)
  return source[0]._0

// end hack random pick

def getEffect(tbl; blk1; blk2; var defValue :auto(R) delete const&)
  var result = defValue
  //find(tbl, blk1) <| $(res : table<string; R>?)
  //  if res != null
  //    find(*res, blk2) <| $(eff : R?)
  //      if eff != null
  //        result = *eff
  return result

def getEffect(tbl; blk; var defValue :auto(R) delete const&)
  var result = defValue
  //find(tbl, blk) <| $(eff : R?)
  //  if eff != null
  //    result = *eff
  return result

def move_to_ptr(var v : auto(V))
  var p = new V
  deref(p) <- v
  return p

struct Crate
  armies : array<string>
  dropAfterBattleChance : int
  squadId : lambda<(armySquadsConfig : array<SquadConfig>; sClass : string; effects : ArmyEffects): string>
  soldiersTier : lambda<(effects : ArmyEffects): int>
  itemWeights : lambda<(effects : ArmyEffects):array<ItemGeneration?> >

let
  defaultSoldierTier = lambda() <| $(effects : ArmyEffects) : int
    return random_pick_w([{ auto[] 1 => 640.; 2 => 270.; 3 => 70.; 4 => 19.; 5 => 1.}])

def simpleWeights(var weights : array<ItemGeneration?>)
  return lambda() <| $(effects : ArmyEffects)
    return <-clone_to_move(weights)

let
  crates_schemes <- {{
    "after_battle_normandy_axis" => move_to_ptr([[ Crate
      armies <- [{ auto[] "normandy_axis"; "tunisia_axis" }],
      dropAfterBattleChance = 1,
      soldiersTier = defaultSoldierTier,
      itemWeights = simpleWeights([{ auto[]
        ItemGeneration([{ auto[]
          "repair_kit" => 3.;
          "small_medkit" => 1.;
          "m24_grenade" => 1.;
          "m24_smoke_grenade_short_time" => 0.5;
          "__noitem__" => 1.}],
          int2(1, 3));
        ItemGeneration([{ auto[]
          "mauser_c96" => 4.;
          "p08_luger" => 3.;
          "p38_walther" => 3.;
          "axe" => 10.;
          "gewehr_43" => 20.;
          "kar98k_kriegsmodell" => 20.;
          "mp_35" => 20.;
          "mg_34" => 2.;
          "fg_42_model_i" => 1.;
          "mp_40" => 2.;
          "stg_44" => 1.;
          "rpzb43_panzerschreck" => 1.;
          "kar98k_kriegsmodell_with_scope_mount" => 2.;
          "luftwaffendrilling_m30" => 1.}],
          int2(4, 8));
        ItemGeneration([{ auto[]
          "rifle_noweap" => 1.;
          "mgun_noweap" => 1.;
          "assault_noweap" => 1.;
          "sniper_noweap" => 1.;
          "anti_tank_noweap" => 1.}],
          int2(4, 4))
      }])
    ]]);
    "after_battle_moscow_axis" => move_to_ptr([[ Crate
      armies <- [{ auto[] "moscow_axis" }],
      dropAfterBattleChance = 1,
      soldiersTier = defaultSoldierTier,
      itemWeights = lambda() <| $(effects : ArmyEffects) {
        return <-[{ auto[]
        ItemGeneration([{ auto[]
          "repair_kit" => 3.;
          "small_medkit" => 1.;
          "m24_grenade" => 1.;
          "explosion_pack" => 1.;
          "m24_smoke_grenade_short_time" => 1.;
          "__noitem__" => 1.}],
          int2(1, 3));
        ItemGeneration([{ auto[]
          "mauser_c96" => 4.;
          "p08_luger" => 3.;
          "p38_walther" => 3.;
          "axe" => 10.;
          "gewehr_41" => 20.;
          "kar98k" => 20.;
          "mp_35" => 20.;
          "mg_34" => 2.;
          "gewehr_42_handheld" => 1.;
          "mp_40" => 2.;
          "pzb_38" => 1.;
          "kar98k_with_scope_mount" => float(getEffect(effects.weapon_drop, "kar98k_with_scope_mount", 0)) * 2.;
          "luftwaffendrilling_m30" => float(getEffect(effects.weapon_drop, "luftwaffendrilling_m30", 0))}],
          int2(4, 8));
        ItemGeneration([{ auto[]
          "rifle_noweap" => 1.;
          "mgun_noweap" => 1.;
          "assault_noweap" => 1.;
          "sniper_noweap" => 1.;
          "anti_tank_noweap" => 1.}],
          int2(4, 4))
        }];
      }
    ]])
}}
