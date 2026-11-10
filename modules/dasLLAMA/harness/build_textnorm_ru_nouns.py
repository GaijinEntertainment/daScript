#!/usr/bin/env python3
# Write dasllama/dasllama_textnorm_ru_nouns.das for dasllama_textnorm_ru.das: the frequent nouns of the
# OpenRussian dictionary (nouns.csv, its rows in frequency order) whose gender or case an ending cannot
# tell - every feminine noun, every neuter noun, and the masculine nouns in -а or -я - one string a
# noun in a list a gender. The normalizer derives a noun's singular cases from its nominative by its
# own ending rules, so a string holds the nominative, then the genitive singular and the genitive
# plural only where those rules do not give them ("-" for a genitive plural the noun does not claim).
# A noun is left out when its derived forms spell a form of a more frequent noun of another gender,
# a neuter in -мя, an adjective declined as a noun, a numeral, a name or a plural-only noun. Beside
# them: the masculine nouns in a consonant, -ь or -й of the first --masculine-rank rows, and every
# noun form a verb, an adjective or another word of the dictionary (verbs.csv, adjectives.csv,
# others.csv) shares - what tells a noun beside a number from a verb.
# Provenance and licence: THIRD_PARTY_NOTICES.md beside the module.
#
#   python -I -X utf8 build_textnorm_ru_nouns.py --nouns <openrussian nouns.csv> --out <.das> [--rank 5000]
import argparse, csv, os, re

SOURCE = ("OpenRussian dictionary data (https://github.com/Badestrand/russian-dictionary, openrussian.org), "
          "Creative Commons Attribution-ShareAlike 4.0 International")
CASES = ("sg_nom", "sg_gen", "sg_dat", "sg_acc", "sg_inst", "sg_prep")
VOWELS = "аеёиоуыэюя"
HUSHING = ("ж", "ч", "ш", "щ", "ц")
VELAR_HUSHING = ("г", "к", "х", "ж", "ч", "ш", "щ")
ADJECTIVE_ENDINGS = ("ая", "яя", "ое", "ее", "ой", "ый", "ий")
NUMERALS = {"пять", "шесть", "семь", "восемь", "девять", "десять", "двадцать", "тридцать", "сорок", "пятьдесят",
            "шестьдесят", "семьдесят", "восемьдесят", "девяносто", "сто", "одиннадцать", "двенадцать", "тринадцать",
            "четырнадцать", "пятнадцать", "шестнадцать", "семнадцать", "восемнадцать", "девятнадцать"}
GENDERS = {"f": "FEMININE", "n": "NEUTER", "m": "MASCULINE"}
LINE_WIDTH = 120


def fold(s):
    return s.replace("ё", "е")


def variants(cell):
    return [fold(v.strip().replace("'", "").replace("*", "").lower()) for v in cell.split(",") if v.strip()]


def first(cell):
    v = variants(cell)
    return v[0] if v else ""


# dasllama_textnorm_ru.das `noun_genitive_singular`, `noun_genitive_plural` and `noun_case` (singular)
def genitive_singular(one):
    stem = one[:-1]
    if one.endswith("а"):
        return stem + ("и" if stem.endswith(VELAR_HUSHING) else "ы")
    if one.endswith(("я", "ь")):
        return stem + "и"
    if one.endswith("о"):
        return stem + "а"
    if one.endswith("е"):
        return stem + ("а" if stem.endswith(HUSHING) else "я")
    return one


def genitive_plural(one):
    stem = one[:-1]
    if one.endswith(("ия", "ие")):
        return stem + "й"
    if one.endswith("ь"):
        return stem + "ей"
    if one.endswith(("а", "о")):
        return stem
    if one.endswith("я"):
        return stem + ("й" if stem[-1:] in VOWELS else "ь")
    return one


def singular_cases(one, few, feminine_soft):
    if one == few:
        return [one] * 6
    soft = one.endswith(("ь", "й", "я", "е"))
    fem = one.endswith(("а", "я"))
    stem = one[:-1] if (soft or fem or one.endswith("о")) else one
    i_stem = stem.endswith("и")
    acc = stem + ("ю" if soft else "у") if fem else one
    if feminine_soft:
        return [one, few, stem + "и", acc, one + "ю", stem + "и"]
    dat = stem + (("и" if i_stem else "е") if fem else ("ю" if soft else "у"))
    if fem:
        instr = stem + ("ей" if soft or stem.endswith(HUSHING) else "ой")
    else:
        instr = stem + (("ём" if one.endswith("ь") else "ем") if soft else "ом")
    prep = stem + ("и" if i_stem else "е")
    return [one, few, dat, acc, fold(instr), prep]


def gender_of(r, one):
    if r["gender"] or r["indeclinable"] == "1":
        return r["gender"]
    if one.endswith(("а", "я")) and r["animate"] != "1":
        return "f"
    return "n" if one.endswith(("о", "е")) else ""


def selected(rows, rank):
    for n, r in enumerate(rows[:rank]):
        one = first(r["sg_nom"]) or fold(r["bare"].lower())
        gender = gender_of(r, one)
        if gender not in GENDERS or r["pl_only"] == "1" or r["bare"][:1].isupper() or one in NUMERALS:
            continue
        if not re.fullmatch(r"[а-я]{2,}", one) or one.endswith(ADJECTIVE_ENDINGS) or one.endswith("мя"):
            continue
        if gender == "m" and r["indeclinable"] != "1" and not (one.endswith(("а", "я")) and r["animate"] == "1"):
            continue
        if r["indeclinable"] == "1" and (len(one) < 4 or one.endswith("ия") or not re.search(f"[{VOWELS}]", one)):
            continue
        yield n, gender, one, r


def other_words(nouns_path):
    """every form of the verbs, the adjectives and the other words beside nouns.csv: a noun form among them is a homograph"""
    words = set()
    for name, skip in (("verbs.csv", {"accented", "translations_en", "translations_de", "aspect", "partner"}),
                       ("adjectives.csv", {"accented", "translations_en", "translations_de"}),
                       ("others.csv", {"accented", "translations_en", "translations_de"})):
        with open(os.path.join(os.path.dirname(nouns_path), name), encoding="utf8") as f:
            for r in csv.DictReader(f, delimiter="\t"):
                for key, cell in r.items():
                    if key not in skip and isinstance(cell, str) and cell:
                        words.update(variants(cell))
    return words


def masculine_genitive(one):
    return one[:-1] + "я" if one.endswith(("ь", "й")) else one + "а"


def masculine_nouns(rows, rank, homographs, noun_homographs):
    """the masculine nouns in a consonant, -ь or -й as their nominative, the genitive after it where -а or -я does not give it; their homograph forms go to `noun_homographs`"""
    out = []
    for r in rows[:rank]:
        one = first(r["sg_nom"]) or fold(r["bare"].lower())
        few = first(r["sg_gen"])
        if r["gender"] != "m" or r["indeclinable"] == "1" or r["pl_only"] == "1" or r["bare"][:1].isupper() or not few:
            continue
        if not re.fullmatch(r"[а-я]{3,}", one) or one.endswith(("а", "я") + ADJECTIVE_ENDINGS) or one in NUMERALS:
            continue
        noun_homographs.update(form for form in singular_cases(one, few, False) if form in homographs)
        out.append(one if few == masculine_genitive(one) else f"{one} {few}")
    return out


def write_list(out, name, entries):
    out.write(f"\nlet {name} <- [\n")
    line = "   "
    for k, entry in enumerate(entries):
        item = f' "{entry}"' + ("," if k + 1 < len(entries) else "")
        if len(line) + len(item) > LINE_WIDTH:
            out.write(line + "\n")
            line = "   "
        line += item
    out.write(line + "\n]\n")


def main():
    ap = argparse.ArgumentParser()
    ap.add_argument("--nouns", required=True, help="OpenRussian nouns.csv, with verbs.csv, adjectives.csv and others.csv beside it")
    ap.add_argument("--out", required=True)
    ap.add_argument("--rank", type=int, default=5000, help="read the first N rows, the most frequent nouns")
    ap.add_argument("--masculine-rank", type=int, default=3000, help="the first N rows the masculine consonant nouns come from")
    a = ap.parse_args()
    with open(a.nouns, encoding="utf8") as f:
        rows = list(csv.DictReader(f, delimiter="\t"))
    homographs = other_words(a.nouns)
    noun_homographs = set()
    owners = {}
    for n, r in enumerate(rows):
        for key in CASES + ("pl_gen",):
            for v in variants(r[key]):
                owners.setdefault(v, []).append((n, r["gender"]))
    lists = {g: [] for g in GENDERS}
    dropped = genless = 0
    for n, gender, one, r in selected(rows, a.rank):
        indeclinable = r["indeclinable"] == "1"
        real_few = one if indeclinable else first(r["sg_gen"])
        real_many = one if indeclinable else first(r["pl_gen"])
        few = genitive_singular(one) if not indeclinable else one
        tokens = [one]
        if few != real_few and real_few:
            few = real_few
            tokens.append(real_few)
        forms = singular_cases(one, few, gender == "f" and one.endswith("ь"))

        def taken(form):
            return any(m < n and g != gender for m, g in owners.get(form, []))
        if any(taken(x) for x in forms):
            dropped += 1
            continue
        many = real_many if real_many and not taken(real_many) else ""
        if not many:
            genless += 1
        if indeclinable:
            tokens = [one, one, one]
        elif len(tokens) == 2 or many != genitive_plural(one):
            tokens.append(many or "-")
        lists[gender].append(" ".join(tokens))
        noun_homographs.update(form for form in forms + [many] if form in homographs)
    masculine = masculine_nouns(rows, a.masculine_rank, homographs, noun_homographs)
    with open(a.out, "w", encoding="utf8", newline="\n") as out:
        out.write("options gen2\noptions _comment_hygiene = true\noptions _dasllama_internal = true\n"
                  "options _ascii_strings = false\n\nmodule dasllama_textnorm_ru_nouns shared public\n\n")
        out.write(f"//! Generated by `harness/build_textnorm_ru_nouns.py` from the {SOURCE}; regenerate it, never edit it\n")
        out.write("//! One string a noun: its nominative, then its genitive singular and its genitive plural where the "
                  "ending rules of `dasllama_textnorm_ru.das` do not derive them (- for none)\n")
        for g, name in GENDERS.items():
            write_list(out, f"RU_{name}_NOUNS", lists[g])
        out.write("\n//! The masculine nouns in a consonant, -ь or -й: the nominative, then the genitive where -а or -я does not give it\n")
        write_list(out, "RU_MASCULINE_CONSONANT_NOUNS", masculine)
        out.write("\n//! The forms of the nouns above that are a verb's, an adjective's or another word's too (были, стали, жила, дома)\n")
        write_list(out, "RU_NOUN_HOMOGRAPHS", sorted(noun_homographs))
    total = sum(len(v) for v in lists.values())
    print(f"wrote {a.out}: {total} nouns ({', '.join(f'{len(v)} {g}' for g, v in lists.items())}), "
          f"{dropped} left out for a form a more frequent noun of another gender owns, {genless} with no genitive plural; "
          f"{len(masculine)} masculine consonant nouns, {len(noun_homographs)} homographs")


if __name__ == "__main__":
    main()
