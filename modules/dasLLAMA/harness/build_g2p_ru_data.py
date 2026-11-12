#!/usr/bin/env python3
# Pack the Russian stress data into tts_g2p_ru.bin for dasllama_g2p_ru.das:
#   - the stress table: every inflected form of the OpenRussian dictionary (nouns.csv, adjectives.csv,
#     verbs.csv, others.csv - the `accented` and inflection columns, the apostrophe after the stressed
#     vowel), lowercase and bare, with the ordinal of its stressed vowel; a form two lemmas or cases
#     stress apart carries every ordinal, the first one the default (a frequency table, when given,
#     puts the most frequent first); then the forms of the `--extra` lists the dictionary lacks, and
#     the `--amend` rows over the dictionary's own entries,
#   - the ё table: every form spelled with ё whose е spelling is not itself a form, keyed by that е
#     spelling - the restoration a text written without ё needs,
#   - with --endings, the ending table: an ending -> the stressed vowel counted from the word's end, read
#     off the stress table's own forms - what the reader asks for a word the pack does not hold,
#   - with --rules, the rule tier: the forms a rules file lists, each with its family and the ordinal of
#     either side, and every family's rules in the order they are tried.
# Provenance and licence: THIRD_PARTY_NOTICES.md beside the module. The file shape - the front-coded
# key blocks, the key alphabet, the value runs, the tagged sections - is
# ARCHITECTURE_TTS_RU_STRESS.md#tts-ru-stress-pack.
#
#   python build_g2p_ru_data.py --openrussian <dir of the four csv files> --out tts_g2p_ru.bin
#                               [--frequency <tsv: word \t stressed vowel ordinal \t count>]...
#                               [--extra <tsv: form \t ordinals \t source>]...
#                               [--amend <tsv: form \t ordinals before \t ordinals after (or -) \t source>]...
#                               [--endings LMIN:LMAX:N:SHARE] [--rules <rules file>]
#                               [--ruaccent <dir>]    (an optional third source; the shipped pack is built without it)
#   python build_g2p_ru_data.py --dump tts_g2p_ru.bin --out tables.txt [--dump-sections]
import argparse, collections, csv, dataclasses, gzip, json, os, re, struct, sys

sys.path.insert(0, os.path.dirname(os.path.abspath(__file__)))
from build_g2p_data import write_pack  # noqa: E402

MAGIC = 0x55524754  # 'TGRU'
VERSION = 2
PACK_HEAD = struct.calcsize("<IIH")   # the magic, the version, the source note's byte count
FRONT_HEAD = struct.Struct("<5I")     # a front-coded table's counts: keys, keys a block, blocks, longest key, blob bytes
SECTION_HEAD = struct.Struct("<II")   # a section's tag and byte count
BLOCK_KEYS = 16
KEY_MAX = 64   # FRONT_KEY_MAX of dasllama_g2p_pack.das: a longer form is left out of the pack
VALUE_BITS = 7       # the value bits of a value-run byte
VALUE_MORE = 0x80    # the bit over them, set where another byte of the run follows
LEN_BITS = 4         # an entry's head byte holds two lengths of this many bits: the shared prefix's over the suffix's
LEN_ESCAPE = 15      # the head length that says the length is a byte of its own after the head
VOWELS = "аеёиоуыэюя"
ALPHABET = "-" + "".join(chr(c) for c in range(0x430, 0x450)) + "ё"
KEY_CODE = {ch: i + 1 for i, ch in enumerate(ALPHABET)}
SKIP_COLUMNS = {"bare", "translations_en", "translations_de", "gender", "partner", "animate", "indeclinable",
                "sg_only", "pl_only", "aspect"}
FORM_RE = re.compile(r"[а-яё'\-]+")
EXTRA_FORM_RE = re.compile(r"[а-яё]+(-[а-яё]+)*")
ENDINGS_TAG = 0x53444E45   # 'ENDS'
RULES_TAG = 0x534C5552     # 'RULS'
RULE_WORD, RULE_ENDING, RULE_EDGE = 0, 1, 2   # RuRuleKind of dasllama_g2p_ru.das
RULE_KINDS = {"w": RULE_WORD, "e2": RULE_ENDING, "e3": RULE_ENDING}
RULE_SLOTS = {"-3": 0, "-2": 1, "-1": 2, "+1": 3, "+2": 4}
RULE_SLOTS_BEHIND = 3   # the slots of the words behind; the slots after them are the words ahead
RULE_SLOT_BITS = 3      # a rule's first byte holds the neighbour's slot in these low bits, its kind above them
RULE_HEAD = 3           # a rule's kind, verdict and key length
SOURCE_NAMES = {"enwiktionary": "English Wiktionary", "ruwiktionary": "Russian Wiktionary", "ruaccent": "RUAccent dictionary",
                "wikipedia": "Russian Wikipedia article leads"}

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


def tsv_rows(path, columns):
    """the rows of a tsv that carry at least `columns` fields; a line opening with `#` or `form\\t` is a header"""
    with open(path, encoding="utf8") as f:
        for line in f:
            parts = line.rstrip("\n").split("\t")
            if len(parts) >= columns and not line.startswith("#") and parts[0] != "form":
                yield parts


def ordinal_list(text, form):
    """a comma separated ordinal list of `form`, every ordinal one of its vowels"""
    ordinals = [int(x) for x in text.split(",")]
    vowels = sum(1 for ch in form if ch in VOWELS)
    assert ordinals and len(set(ordinals)) == len(ordinals) and all(0 < o <= vowels for o in ordinals), (form, text)
    return ordinals


def apply_frequency(forms, paths):
    """a homograph's variants reordered most frequent first, by word \\t ordinal \\t count tables; a word
    takes its counts from the first table that names it"""
    counts = {}
    for path in paths:
        table = {}
        for parts in tsv_rows(path, 3):
            table[(parts[0], int(parts[1]))] = int(parts[2])
        named = {word for word, _ in counts}
        counts.update((key, n) for key, n in table.items() if key[0] not in named)
    reordered = 0
    for word, ordinals in forms.items():
        if len(ordinals) < 2:
            continue
        ranked = sorted(ordinals, key=lambda o: -counts.get((word, o), 0))
        if ranked != ordinals:
            forms[word] = ranked
            reordered += 1
    return reordered


def yo_keys(forms):
    """the е spellings of the table's ё forms"""
    return {word.replace("ё", "е") for word in forms if "ё" in word}


def apply_extra(forms, path, sources, agreed, variants):
    """the forms of another source the table lacks: form \\t ordinals (comma separated, the default first)
    \\t source [\\t how many of the leading ordinals two of the row's sources give]. A form the table holds,
    one the ё table answers, and a `variants` spelling (a ё word written with е) are left alone, so an
    earlier list's form wins. Answers (added, skipped)."""
    answered_by_yo = yo_keys(forms)
    extra = {}
    skipped = 0
    for parts in tsv_rows(path, 3):
        form = parts[0]
        assert EXTRA_FORM_RE.fullmatch(form), form
        if form in forms or form in answered_by_yo or form in variants:
            skipped += 1
            continue
        extra[form] = ordinal_list(parts[1], form)
        agreed[form] = int(parts[3]) if len(parts) > 3 else 0
        sources.update(parts[2].split("+"))
    forms.update(extra)
    return len(extra), skipped


class RuAccent:
    """RUAccent's dictionary files in the pack's convention. `readings`: form -> the ordinal accents.json.gz
    marks, less the forms accents_nn.json.gz lists (its machine-made entries); `variants`: form -> the
    ordinals of omographs.json.gz; `yo_words`: е spelling -> ё spelling; `context`: the е spellings
    yo_homographs.json.gz leaves to the context; `skips`: entries left out, by reason."""

    def __init__(self, directory):
        def load(name):
            with gzip.open(os.path.join(directory, name), "rt", encoding="utf8") as f:
                return json.load(f)
        self.skips = collections.Counter()
        machine = set(load("accents_nn.json.gz"))
        self.readings = {}
        for key, marked in load("accents.json.gz").items():
            if key in machine:
                self.skips["accents: machine-made (accents_nn)"] += 1
                continue
            ordinal = self.ordinal(key, marked, "accents")
            if ordinal:
                self.readings[key] = ordinal
        self.variants = {}
        for key, marks in load("omographs.json.gz").items():
            ordinals = []
            for marked in marks:
                ordinal = self.ordinal(key, marked, "omographs")
                if ordinal and ordinal not in ordinals:
                    ordinals.append(ordinal)
            if ordinals:
                self.variants[key] = ordinals
        spelled = lambda word: all(ch in KEY_CODE for ch in word)
        self.context = set(load("yo_homographs.json.gz"))
        self.yo_words = {}
        for plain, word in load("yo_words.json.gz").items():
            if spelled(plain) and spelled(word) and "ё" in word and word.replace("ё", "е") == plain:
                self.yo_words[plain] = word
            else:
                self.skips["yo_words: not a ё spelling of its key in the pack's alphabet"] += 1

    def ordinal(self, key, marked, name):
        """the ordinal of the vowel a `+` stands before, 0 with the entry counted into `skips`"""
        at = marked.find("+")
        why = ("letters outside the pack's alphabet" if not all(ch in KEY_CODE for ch in key) else
               "no mark" if at < 0 else
               "two marks" if marked.count("+") > 1 else
               "a mark not before a vowel" if marked[at + 1: at + 2] not in VOWELS or at + 1 >= len(marked) else
               "the marked word is not its key" if marked.replace("+", "") != key else None)
        if why:
            self.skips[f"{name}: {why}"] += 1
            return 0
        return sum(1 for ch in marked[:at] if ch in VOWELS) + 1


def apply_ruaccent(forms, ra, dictionary, shipped_yo, agreed, variants, restores):
    """RUAccent's readings over the table: a form no source holds enters with its reading; on a form an
    `--extra` list added, a reading the list also gives moves ahead of the readings one source gives and a
    new one goes last; a dictionary entry is left alone. A ё word's е spelling is no form: the е spellings of
    `yo_words` bring their ё form instead. Then omographs.json.gz's readings after every added form's own.
    Answers the counts."""
    counts = collections.Counter()
    answered_by_yo = yo_keys(forms)
    for key, ordinal in ra.readings.items():
        if key in dictionary:
            counts["held by the dictionary (left alone)"] += 1
        elif key in forms:
            counts["shared with an extra list"] += 1
            ordinals = forms[key]
            if ordinal not in ordinals:
                ordinals.append(ordinal)
                counts["shared: a reading the list lacks, added last"] += 1
            elif ordinals.index(ordinal) > agreed[key]:
                ordinals.remove(ordinal)
                ordinals.insert(agreed[key], ordinal)
                counts["shared: the agreed reading moved ahead"] += 1
        elif "ё" in key:
            counts["spelled with ё (the ё is its own mark)"] += 1
        elif key in shipped_yo or key in variants or (key in answered_by_yo and key not in ra.context):
            counts["a ё word's е spelling (left to the ё table)"] += 1
        else:
            forms[key] = [ordinal]
            counts["added: forms no other source holds"] += 1
    for plain in restores:
        word = ra.yo_words[plain]
        if word not in forms:
            vowels = [ch for ch in word if ch in VOWELS]
            forms[word] = [ra.readings.get(word) or max(i + 1 for i, ch in enumerate(vowels) if ch == "ё")]
            counts["added: ё forms of yo_words"] += 1
    counts["omographs: readings added after an added form's own"] = add_variants(forms, ra, lambda key: key not in dictionary)
    return counts


def add_variants(forms, ra, wanted):
    """omographs.json.gz's readings after the readings of every form `wanted` names; answers the readings added"""
    added = 0
    for key, ordinals in ra.variants.items():
        if key in forms and wanted(key):
            for ordinal in ordinals:
                if ordinal not in forms[key]:
                    forms[key].append(ordinal)
                    added += 1
    return added


def apply_amend(forms, path, sources):
    """the dictionary's own entries rewritten: form \\t ordinals before \\t ordinals after \\t source. A row
    whose `before` is not the entry the table holds stops the build. An `after` of `-` takes the entry out: a е
    spelling that is no form, so the ё table restores it. Answers the rows applied."""
    amended = 0
    for form, before, after, source in (parts[:4] for parts in tsv_rows(path, 4)):
        assert forms.get(form) == ordinal_list(before, form), (form, before, forms.get(form))
        if after == "-":
            del forms[form]
        else:
            forms[form] = ordinal_list(after, form)
        sources.update(source.split("+"))
        amended += 1
    return amended


def yo_table(forms):
    """е spelling -> ё spelling, for every ё form whose е spelling is no form of its own; answers the
    table and the count of ё forms left to the text"""
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
    return yo, clashes


def key_bytes(word):
    """a key in the pack's alphabet, one byte a letter"""
    return bytes(KEY_CODE[ch] for ch in word)


def run_bytes(groups):
    """7-bit groups as a value run: the high bit of a byte says another follows"""
    assert groups and all(0 <= g < VALUE_MORE for g in groups), groups
    return bytes(g | (VALUE_MORE if i + 1 < len(groups) else 0) for i, g in enumerate(groups))


def yo_mask(plain, word):
    """the ё spelling as a bit mask over the е letters of its key (bit k: the k-th е is a ё), low 7 bits first"""
    mask = 0
    k = 0
    for a, b in zip(plain, word):
        if a == "е":
            mask |= (b == "ё") << k
            k += 1
    groups = [mask & (VALUE_MORE - 1)]
    while mask >> VALUE_BITS:
        mask >>= VALUE_BITS
        groups.append(mask & (VALUE_MORE - 1))
    return groups


def front_table(entries, block_keys=BLOCK_KEYS):
    """entries: sorted list of (key bytes, value run) -> the packed table: blocks of `block_keys` keys, a
    block's first key whole and every next one as the bytes it adds to the key before it."""
    blob = bytearray()
    offsets = []
    prev = b""
    for i, (key, value) in enumerate(entries):
        shared = 0
        if i % block_keys == 0:
            offsets.append(len(blob))
        else:
            while shared < len(prev) and prev[shared] == key[shared]:
                shared += 1
        suffix = len(key) - shared
        assert 0 < suffix < 256 and shared < 256 and (i == 0 or prev < key), key
        blob.append(min(shared, LEN_ESCAPE) << LEN_BITS | min(suffix, LEN_ESCAPE))
        if shared >= LEN_ESCAPE:
            blob.append(shared)
        if suffix >= LEN_ESCAPE:
            blob.append(suffix)
        blob += key[shared:]
        blob += value
        prev = key
    key_max = max((len(key) for key, _ in entries), default=0)
    assert key_max <= KEY_MAX, f"a key of {key_max} letters: the reader's lookup buffer holds {KEY_MAX}"
    return (FRONT_HEAD.pack(len(entries), block_keys, len(offsets), key_max, len(blob))
            + struct.pack(f"<{len(offsets)}I", *offsets) + blob)


def read_front_table(data, o):
    """the (key bytes, value groups) entries of the table at `o`, and the offset past it"""
    n, block_keys, n_blocks, _key_max, blob_len = FRONT_HEAD.unpack_from(data, o)
    o += FRONT_HEAD.size + 4 * n_blocks
    end = o + blob_len
    entries = []
    key = b""
    for i in range(n):
        head = data[o]
        o += 1
        shared, suffix = head >> LEN_BITS, head & LEN_ESCAPE
        if shared == LEN_ESCAPE:
            shared = data[o]
            o += 1
        if suffix == LEN_ESCAPE:
            suffix = data[o]
            o += 1
        assert i % block_keys or shared == 0
        key = key[:shared] + data[o: o + suffix]
        o += suffix
        groups = []
        while True:
            groups.append(data[o] & (VALUE_MORE - 1))
            o += 1
            if data[o - 1] < VALUE_MORE:
                break
        entries.append((key, groups))
    assert o == end
    return entries, end


def ending_table(forms, lmin, lmax, n_min, share):
    """ending -> the stressed vowel counted from the word's end (1 = the last vowel), read off the default reading of
    every hyphen-free form of two vowels or more. An ending of lmin..lmax letters, shorter than its word, is kept
    when `n_min` forms or more carry it and `share` of them stress the same vowel from the end; an entry the longest
    shorter kept ending of it answers the same way is left out, so the reader's longest-ending walk is unchanged."""
    vowels = lambda w: sum(1 for ch in w if ch in VOWELS)
    words = [(w, vowels(w) - o[0] + 1) for w, o in forms.items() if "-" not in w and vowels(w) >= 2]
    table = {}
    for n in range(lmin, lmax + 1):
        counts = collections.Counter((w[-n:], pos) for w, pos in words if len(w) > n)
        totals = collections.Counter()
        best = {}
        for (ending, pos), carried in counts.items():
            totals[ending] += carried
            if ending not in best or (carried, -pos) > (best[ending][0], -best[ending][1]):
                best[ending] = (carried, pos)
        for ending in sorted(best):
            top, pos = best[ending]
            if totals[ending] < n_min or top < share * totals[ending]:
                continue
            shorter = n - 1
            while shorter >= lmin and ending[-shorter:] not in table:
                shorter -= 1
            if shorter < lmin or table[ending[-shorter:]] != pos:
                table[ending] = pos
    return table


def ending_ordinal(table, lmin, lmax, word):
    """the reader's walk: the stressed vowel's ordinal by the longest ending of `word` the table holds whose vowel
    the word has; 0 where there is none"""
    vowels = sum(1 for ch in word if ch in VOWELS)
    for n in range(min(lmax, len(word) - 1), lmin - 1, -1):
        pos = table.get(word[-n:])
        if pos and pos <= vowels:
            return vowels - pos + 1
    return 0


def section(tag, payload):
    """a tagged section after the two tables: the tag, the payload's byte count, the payload"""
    return SECTION_HEAD.pack(tag, len(payload)) + payload


def endings_section(table):
    """the ending table as a front-coded table: an ending's letters reversed -> its position, one byte"""
    return section(ENDINGS_TAG, front_table(sorted((key_bytes(e[::-1]), bytes([pos])) for e, pos in table.items())))


def read_rules(path):
    """a rules file: `family \\t id \\t name`, `word \\t form \\t family id \\t ordinal of side 0 \\t ordinal of side 1
    [\\t unruled]` (the side read where no rule holds: 1 for side 0, 2 for side 1, 0 or absent to abstain) and
    `rule \\t family id \\t feature \\t side \\t sure`, a family's rules in the order they are tried. A feature is
    `w<pos>=<word>` (the neighbour is the word), `w<pos>=<s>` or `w<pos>=</s>` (the sentence holds no such neighbour),
    `e2<pos>=` and `e3<pos>=` (the neighbour's last two or three letters, the neighbour two letters longer at least);
    <pos> is -3, -2, -1, +1 or +2. Answers (family ids in order, form -> (family, ordinal, ordinal, unruled),
    family -> rules)."""
    families, forms, rules = [], {}, collections.defaultdict(list)
    for parts in tsv_rows(path, 3):
        if parts[0] == "family":
            families.append(parts[1])
        elif parts[0] == "word":
            form, family, sides = parts[1], parts[2], ordinal_list(f"{parts[3]},{parts[4]}", parts[1])
            unruled = int(parts[5]) if len(parts) > 5 else 0
            assert EXTRA_FORM_RE.fullmatch(form) and family in families and form not in forms and unruled in (0, 1, 2), parts
            forms[form] = (family, sides[0], sides[1], unruled)
        else:
            assert parts[0] == "rule" and parts[1] in families and parts[3] in "01" and parts[4] in "01", parts
            name, _, value = parts[2].partition("=")
            kind = name[:-2]
            slot = RULE_SLOTS[name[-2:]]
            if value in ("<s>", "</s>"):
                assert kind == "w", parts
                kind, value = RULE_EDGE, ""
            else:
                assert kind in RULE_KINDS and len(value) == {"w": len(value), "e2": 2, "e3": 3}[kind], parts
                kind = RULE_KINDS[kind]
            rules[parts[1]].append((kind << RULE_SLOT_BITS | slot, int(parts[3]) | int(parts[4]) << 1, key_bytes(value)))
    assert len(families) < VALUE_MORE, "a form's value holds its family in seven bits"
    return families, forms, rules


def rules_section(families, forms, rules):
    """the rule tier: a front-coded table form -> family (from 1), the ordinal of side 0, the ordinal of side 1, the
    unruled side; then the family count, one u32 a family and one past the last - where its rules start - and the rules, each a kind
    byte (the feature's kind over the neighbour's slot), a verdict byte (the side, bit 1 a sure rule), the key's
    byte count and the key"""
    number = {family: i + 1 for i, family in enumerate(families)}
    table = front_table(sorted((key_bytes(form), run_bytes([number[family], a, b, u])) for form, (family, a, b, u) in forms.items()))
    body = bytearray()
    offsets = []
    for family in families:
        offsets.append(len(body))
        for kind, verdict, key in rules[family]:
            body += bytes([kind, verdict, len(key)]) + key
    offsets.append(len(body))
    return section(RULES_TAG, table + struct.pack(f"<I{len(offsets)}I", len(families), *offsets) + bytes(body))


def dump_sections(data, o, spell):
    """the tagged sections from `o` as text lines: `ending \\t ending \\t position from the end`, `rule-form \\t form \\t
    family \\t ordinals` and `rule \\t family \\t feature \\t side \\t sure`; `spell` turns key bytes into letters"""
    lines = []
    slots = {v: k for k, v in RULE_SLOTS.items()}
    while o + SECTION_HEAD.size <= len(data):
        tag, size = SECTION_HEAD.unpack_from(data, o)
        body = o + SECTION_HEAD.size
        o = body + size
        if tag == ENDINGS_TAG:
            entries, _ = read_front_table(data, body)
            lines += [f"ending\t{spell(key)[::-1]}\t{groups[0]}" for key, groups in entries]
        elif tag == RULES_TAG:
            entries, at = read_front_table(data, body)
            lines += [f"rule-form\t{spell(key)}\t{groups[0]}\t{groups[1]},{groups[2]}\t{groups[3]}" for key, groups in entries]
            (n,) = struct.unpack_from("<I", data, at)
            offsets_at = at + 4
            offsets = struct.unpack_from(f"<{n + 1}I", data, offsets_at)
            base = offsets_at + 4 * (n + 1)
            for family in range(n):
                r = base + offsets[family]
                while r < base + offsets[family + 1]:
                    packed, verdict, letters = data[r], data[r + 1], data[r + 2]
                    kind, slot = packed >> RULE_SLOT_BITS, packed & ((1 << RULE_SLOT_BITS) - 1)
                    value = spell(data[r + RULE_HEAD: r + RULE_HEAD + letters])
                    name = f"e{letters}" if kind == RULE_ENDING else "w"
                    value = ("<s>" if slot < RULE_SLOTS_BEHIND else "</s>") if kind == RULE_EDGE else value
                    lines.append(f"rule\t{family + 1}\t{name}{slots[slot]}={value}\t{verdict & 1}\t{verdict >> 1}")
                    r += RULE_HEAD + letters
    return lines


def dump_pack(path, out, sections=False):
    """a pack's two tables as sorted text: `stress \\t form \\t ordinals` and `yo \\t е spelling \\t ё spelling`; with
    `sections`, the ending table and the rule tier after them, in the pack's order"""
    data = open(path, "rb").read()
    magic, version, note_len = struct.unpack_from("<IIH", data, 0)
    assert magic == MAGIC and version == VERSION, (hex(magic), version)
    stress, o = read_front_table(data, PACK_HEAD + note_len)
    yo, o = read_front_table(data, o)
    spell = lambda key: "".join(ALPHABET[b - 1] for b in key)
    extra = dump_sections(data, o, spell) if sections else []
    lines = [f"stress\t{spell(key)}\t{','.join(map(str, groups))}" for key, groups in stress]
    for key, groups in yo:
        mask = sum(g << (VALUE_BITS * i) for i, g in enumerate(groups))
        letters = list(spell(key))
        for k, at in enumerate(i for i, ch in enumerate(letters) if ch == "е"):
            if mask >> k & 1:
                letters[at] = "ё"
        lines.append(f"yo\t{spell(key)}\t{''.join(letters)}")
    with open(out, "w", encoding="utf8", newline="\n") as f:
        f.write("\n".join(sorted(lines) + extra) + "\n")
    print(f"wrote {out}: {len(stress)} forms, {len(yo)} ё restorations" + (f", {len(extra)} section lines" if sections else ""))


@dataclasses.dataclass
class Tables:
    """what a build packs and prints"""
    forms: dict           # bare form -> its stressed ordinals, the default first
    yo: dict              # е spelling -> ё spelling
    sources: set          # the names of the sources past the dictionary
    dictionary: set       # the forms the dictionary gave
    extra_forms: set      # the forms the `--extra` lists gave
    lemmas: int
    extra: list           # (added, skipped) an `--extra` list
    ruaccent: collections.Counter   # RUAccent's counts by what was done
    reordered: int        # homographs the frequency tables reordered
    amended: int          # `--amend` rows applied
    too_long: list        # forms left out, longer than KEY_MAX
    clashes: int          # ё forms whose е spelling is a form of its own
    context_kept: list    # yo_homographs keys the dictionary's own ё table restores


def build_tables(openrussian, frequency=(), extra=(), amend=(), ruaccent=None, yo_guard=True):
    """the pack's two tables. Order: the dictionary; the `--extra` lists; RUAccent's readings; the frequency
    tables, which order every homograph the lists and RUAccent brought; the `--amend` rows, so a row's
    `after` is the entry as written; omographs.json.gz's readings after a dictionary entry's own. `yo_guard`
    off counts a ё word's е spelling as a form wherever a list gives it - the measurement's control."""
    forms, lemmas = read_forms(openrussian)
    dictionary = set(forms)
    shipped_yo, _ = yo_table(forms)
    sources = set()
    ra = RuAccent(ruaccent) if ruaccent else None
    restores = {k for k in ra.yo_words if k not in ra.context and k not in dictionary and k not in shipped_yo} if ra else set()
    variants = restores if yo_guard else set()
    agreed = {}
    extra_counts = [apply_extra(forms, path, sources, agreed, variants) for path in extra]
    extra_forms = set(forms) - dictionary
    ra_counts = apply_ruaccent(forms, ra, dictionary, shipped_yo, agreed, variants, restores) if ra else collections.Counter()
    reordered = apply_frequency(forms, frequency)
    amended = sum(apply_amend(forms, path, sources) for path in amend)
    if ra:
        sources.add("ruaccent")
        ra_counts["omographs: readings added after a dictionary entry's own"] = add_variants(forms, ra, lambda key: key in dictionary)
        ra_counts.update(ra.skips)
    too_long = [form for form in forms if len(form) > KEY_MAX]
    for form in too_long:
        del forms[form]
    yo, clashes = yo_table(forms)
    yo.update((key, word) for key, word in shipped_yo.items() if key in yo)   # the dictionary's own spelling of a key stands
    context_kept = sorted(k for k in yo if ra and k in ra.context and k in shipped_yo)
    for key in [k for k in yo if ra and k in ra.context and k not in shipped_yo]:
        del yo[key]
        ra_counts["ё restorations left to the context (yo_homographs)"] += 1
    return Tables(forms=forms, yo=yo, sources=sources, dictionary=dictionary, extra_forms=extra_forms, lemmas=lemmas,
                  extra=extra_counts, ruaccent=ra_counts, reordered=reordered, amended=amended, too_long=too_long,
                  clashes=clashes, context_kept=context_kept)


def main():
    ap = argparse.ArgumentParser()
    ap.add_argument("--openrussian", help="directory holding nouns.csv, adjectives.csv, verbs.csv, others.csv")
    ap.add_argument("--out", required=True)
    ap.add_argument("--frequency", action="append", default=[],
                    help="word \\t stressed vowel ordinal \\t count: orders a homograph's variants; repeatable, a word "
                         "takes its counts from the first table that names it")
    ap.add_argument("--extra", action="append", default=[],
                    help="form \\t ordinals \\t source: the forms of another source the dictionary lacks; repeatable")
    ap.add_argument("--amend", action="append", default=[],
                    help="form \\t ordinals before \\t ordinals after \\t source: a dictionary entry rewritten; repeatable")
    ap.add_argument("--ruaccent", help="directory of RUAccent's dictionary files (accents.json.gz, accents_nn.json.gz, "
                    "omographs.json.gz, yo_words.json.gz, yo_homographs.json.gz): an optional third source of forms and "
                    "readings; the shipped pack is NOT built with it")
    ap.add_argument("--endings", help="LMIN:LMAX:N:SHARE - pack the ending table for the words the pack does not hold: "
                    "endings of LMIN to LMAX letters that N forms or more carry, SHARE of them stressed on the same vowel "
                    "from the end; without it the pack has no such table and the reader falls to the shape rule")
    ap.add_argument("--rules", help="a rules file (families, forms, rules - see read_rules): the rule tier's section; "
                    "without it the pack has no rules and the rule tier abstains")
    ap.add_argument("--block-keys", type=int, default=BLOCK_KEYS, help="keys a front-coded block")
    ap.add_argument("--dump", help="write this pack's tables to --out as sorted text instead of building one")
    ap.add_argument("--dump-sections", action="store_true", help="with --dump: the ending table and the rule tier too")
    a = ap.parse_args()
    if a.dump:
        dump_pack(a.dump, a.out, a.dump_sections)
        return
    t = build_tables(a.openrussian, a.frequency, a.extra, a.amend, a.ruaccent)
    forms, yo, sources = t.forms, t.yo, t.sources
    stress_entries = sorted((key_bytes(k), run_bytes(v)) for k, v in forms.items())
    yo_entries = sorted((key_bytes(k), run_bytes(yo_mask(k, v))) for k, v in yo.items())
    sections = [("stress", front_table(stress_entries, a.block_keys)), ("yo", front_table(yo_entries, a.block_keys))]
    notes = []
    if a.endings:
        lmin, lmax, n_min, share = a.endings.split(":")
        endings = ending_table(forms, int(lmin), int(lmax), int(n_min), float(share))
        sections.append(("endings", endings_section(endings)))
        notes.append(f"{len(endings)} endings of {lmin} to {lmax} letters ({n_min} forms or more, share {share})")
    if a.rules:
        families, rule_forms, rules = read_rules(a.rules)
        sections.append(("rules", rules_section(families, rule_forms, rules)))
        notes.append(f"{sum(len(r) for r in rules.values())} rules in {len(families)} families over {len(rule_forms)} forms")
    wiktionary = sorted(SOURCE_NAMES[s] for s in sources if s.endswith("wiktionary"))
    source = "OpenRussian dictionary (nouns, adjectives, verbs, others)"
    if sources:
        source += "; " + ", ".join(sorted(SOURCE_NAMES.get(s, s) for s in sources))
    written = write_pack(a.out, MAGIC, VERSION, source, sections)
    with open(a.out + ".LICENSE", "w", encoding="utf8") as f:
        f.write(f"{os.path.basename(a.out)} - derived data, see the dasLLAMA THIRD_PARTY_NOTICES.md\n\n"
                "OpenRussian dictionary data (https://github.com/Badestrand/russian-dictionary, openrussian.org), "
                "Creative Commons Attribution-ShareAlike 4.0 International: every inflected form's stressed vowel, "
                "and the ё spellings a text written without ё needs\n")
        if wiktionary:
            f.write(f"\n{' and '.join(wiktionary)} (https://en.wiktionary.org, https://ru.wiktionary.org), through the "
                    "machine-readable extracts of https://kaikki.org (Tatu Ylonen, \"Wiktextract: Wiktionary as "
                    "Machine-Readable Structured Data\", LREC 2022, pp. 1317-1325), Creative Commons "
                    "Attribution-ShareAlike 4.0 International and the GNU Free Documentation License: the stressed "
                    "vowel of the forms the OpenRussian tables leave out, and the readings they lack\n")
        if "wikipedia" in sources:
            f.write("\nRussian Wikipedia (https://ru.wikipedia.org), Creative Commons Attribution-ShareAlike 4.0 "
                    "International and the GNU Free Documentation License: the stressed vowel a lead sentence marks in the "
                    "names and words no source above holds\n")
        if "ruaccent" in sources:
            f.write("\nRUAccent's dictionary files (Denis Petrov, https://github.com/Den4ikAI/ruaccent, "
                    "https://huggingface.co/ruaccent/accentuator), MIT License: the stressed vowel of the forms neither "
                    "source above holds, further readings, and ё spellings\n")
    print(f"wrote {a.out}: {written} bytes")
    for name, sec in sections:
        print(f"  {name:8s} {len(sec):9d} bytes")
    for path, (added, skipped) in zip(a.extra, t.extra):
        print(f"extra {os.path.basename(path)}: {added} forms added, {skipped} left alone")
    for what, n in sorted(t.ruaccent.items()):
        print(f"ruaccent {what}: {n}")
    if t.context_kept:
        print(f"ruaccent yo_homographs the dictionary's own ё table restores, kept: {len(t.context_kept)}")
    if a.amend:
        print(f"amended: {t.amended} dictionary entries")
    if t.too_long:
        print(f"left out, longer than {KEY_MAX} letters: {len(t.too_long)} forms")
    for note in notes:
        print(note)
    homographs = sum(1 for v in forms.values() if len(v) > 1)
    print(f"{t.lemmas} lemmas, {len(forms)} forms ({len(t.dictionary)} of the dictionary, {len(t.extra_forms)} of the extra lists), "
          f"{homographs} with more than one stress ({t.reordered} reordered by frequency), "
          f"{len(yo)} ё restorations ({t.clashes} ё forms whose е spelling is a form of its own, left to the text)")


if __name__ == "__main__":
    main()
