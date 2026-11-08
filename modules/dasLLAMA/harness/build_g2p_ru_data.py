#!/usr/bin/env python3
# Pack the Russian stress data into tts_g2p_ru.bin for dasllama_g2p_ru.das:
#   - the stress table: every inflected form of the OpenRussian dictionary (nouns.csv, adjectives.csv,
#     verbs.csv, others.csv - the `accented` and inflection columns, the apostrophe after the stressed
#     vowel), lowercase and bare, with the ordinal of its stressed vowel; a form two lemmas or cases
#     stress apart carries every ordinal, the first one the default (a frequency table, when given,
#     puts the most frequent first),
#   - the ё table: every form spelled with ё whose е spelling is not itself a form, keyed by that е
#     spelling - the restoration a text written without ё needs.
# Provenance and licence: THIRD_PARTY_NOTICES.md beside the module. Both tables are byte-sorted
# string tables (offset columns + one key blob + one value blob), looked up by binary search on the
# mapped bytes - nothing is parsed at load.
#
#   python build_g2p_ru_data.py --openrussian <dir of the four csv files> --out tts_g2p_ru.bin
#                               [--frequency <tsv: word \t stressed vowel ordinal \t count>]
import argparse, csv, os, re, struct, sys

sys.path.insert(0, os.path.dirname(os.path.abspath(__file__)))
from build_g2p_data import string_table, write_pack  # noqa: E402

MAGIC = 0x55524754  # 'TGRU'
VERSION = 1
VOWELS = "аеёиоуыэюя"
SKIP_COLUMNS = {"bare", "translations_en", "translations_de", "gender", "partner", "animate", "indeclinable",
                "sg_only", "pl_only", "aspect"}
FORM_RE = re.compile(r"[а-яё'\-]+")

# The inflections the dictionary's `others` table leaves out - the pronouns and determiners come as a
# bare headword - and the future of быть, which it writes as есть. Apostrophe after the stressed vowel.
EXTRA_FORMS = """
я меня' мне мной мно'ю ты тебя' тебе' тобо'й тобо'ю он его' него' ему' нему' им ним нём она' её неё ей ней е'ю не'ю
мы нас нам на'ми вы вас вам ва'ми они' их них и'ми ни'ми себя' себе' собо'й собо'ю
кто кого' кому' кем ком что чего' чему' чем чём никто' никого' никому' нике'м ничто' ничего' ничему' ниче'м ниче'м
не'кого не'кому не'чего не'чему кто'-то кто'-нибудь что'-то что'-нибудь
э'тот э'та э'то э'ти э'того э'той э'тому э'тим э'том э'ту э'тих э'тими тот та то те того' той тому' тем том ту тех те'ми
весь вся всё все всего' всей всему' всем всю всех все'ми
мой моя' моё мои' моего' мое'й моему' мои'м моём мою' мои'х мои'ми
твой твоя' твоё твои' твоего' твое'й твоему' твои'м твоём твою' твои'х твои'ми
свой своя' своё свои' своего' свое'й своему' свои'м своём свою' свои'х свои'ми
наш на'ша на'ше на'ши на'шего на'шей на'шему на'шим на'шем на'шу на'ших на'шими
ваш ва'ша ва'ше ва'ши ва'шего ва'шей ва'шему ва'шим ва'шем ва'шу ва'ших ва'шими
сам сама' само' са'ми самого' само'й самому' сами'м само'м саму' сами'х сами'ми
оди'н одна' одно' одни' одного' одно'й одному' одни'м одно'м одну' одни'х одни'ми
бу'ду бу'дешь бу'дет бу'дем бу'дете бу'дут бу'дь бу'дьте
"""


def stressed_ordinal(form):
    """the 1-based ordinal of the stressed vowel in an apostrophe-marked form; the one vowel of a
    monosyllable; the ё of a form that has one; 0 when the form carries no stress"""
    bare = form.replace("'", "")
    vowels = [ch for ch in bare if ch in VOWELS]
    if "'" in form:
        return sum(1 for ch in form[: form.index("'")] if ch in VOWELS)
    if len(vowels) == 1:
        return 1
    if "ё" in bare:
        return next(i + 1 for i, ch in enumerate(vowels) if ch == "ё")
    return 0


def add_form(forms, form):
    """one apostrophe-marked form into the table; a form with no stress to read is skipped"""
    ordinal = stressed_ordinal(form)
    if ordinal <= 0:
        return
    bare = form.replace("'", "")
    ordinals = forms.setdefault(bare, [])
    if ordinal not in ordinals:
        ordinals.append(ordinal)


def read_forms(openrussian_dir):
    """bare form -> ordered list of stressed ordinals, in file order (nouns, adjectives, verbs, others),
    then the supplementary forms"""
    forms = {}
    lemmas = 0
    for name in ("nouns.csv", "adjectives.csv", "verbs.csv", "others.csv"):
        with open(os.path.join(openrussian_dir, name), encoding="utf8") as f:
            for row in csv.DictReader(f, delimiter="\t"):
                lemmas += 1
                for column, value in row.items():
                    if column in SKIP_COLUMNS or column is None or not isinstance(value, str):
                        continue
                    for form in re.split(r"[,;/]\s*", value.strip()):
                        form = form.strip().strip("*").lower()
                        if not form or not FORM_RE.fullmatch(form):
                            continue
                        add_form(forms, form)
    for form in EXTRA_FORMS.split():
        add_form(forms, form)
    return forms, lemmas


def apply_frequency(forms, path):
    """a homograph's variants reordered most frequent first, by a word \t ordinal \t count table"""
    counts = {}
    with open(path, encoding="utf8") as f:
        for line in f:
            parts = line.rstrip("\n").split("\t")
            if len(parts) >= 3:
                counts[(parts[0], int(parts[1]))] = int(parts[2])
    reordered = 0
    for word, ordinals in forms.items():
        if len(ordinals) < 2:
            continue
        ranked = sorted(ordinals, key=lambda o: -counts.get((word, o), 0))
        if ranked != ordinals:
            forms[word] = ranked
            reordered += 1
    return reordered


def main():
    ap = argparse.ArgumentParser()
    ap.add_argument("--openrussian", required=True, help="directory holding nouns.csv, adjectives.csv, verbs.csv, others.csv")
    ap.add_argument("--out", required=True)
    ap.add_argument("--frequency", help="word \\t stressed vowel ordinal \\t count: orders a homograph's variants")
    a = ap.parse_args()
    forms, lemmas = read_forms(a.openrussian)
    reordered = apply_frequency(forms, a.frequency) if a.frequency else 0
    homographs = sum(1 for v in forms.values() if len(v) > 1)
    yo = {}
    clashes = 0
    for word in forms:
        if "ё" not in word:
            continue
        plain = word.replace("ё", "е")
        if plain in forms:
            clashes += 1
        else:
            yo[plain] = word
    stress_entries = sorted((k.encode("utf8"), bytes([len(v)] + v)) for k, v in forms.items())
    yo_entries = sorted((k.encode("utf8"), v.encode("utf8")) for k, v in yo.items())
    sections = [("stress", string_table(stress_entries)), ("yo", string_table(yo_entries))]
    written = write_pack(a.out, MAGIC, VERSION, "OpenRussian dictionary (nouns, adjectives, verbs, others)", sections)
    with open(a.out + ".LICENSE", "w", encoding="utf8") as f:
        f.write(f"{os.path.basename(a.out)} - derived data, see the dasLLAMA THIRD_PARTY_NOTICES.md\n\n"
                "OpenRussian dictionary data (https://github.com/Badestrand/russian-dictionary, openrussian.org), "
                "Creative Commons Attribution-ShareAlike 4.0 International: every inflected form's stressed vowel, "
                "and the ё spellings a text written without ё needs\n")
    print(f"wrote {a.out}: {written} bytes")
    for name, sec in sections:
        print(f"  {name:8s} {len(sec):9d} bytes")
    print(f"{lemmas} lemmas, {len(forms)} forms, {homographs} with more than one stress ({reordered} reordered by frequency), "
          f"{len(yo)} ё restorations ({clashes} ё forms whose е spelling is a form of its own, left to the text)")


if __name__ == "__main__":
    main()
