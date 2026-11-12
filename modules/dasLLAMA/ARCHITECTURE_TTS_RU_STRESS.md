# dasLLAMA architecture - text to speech, Russian stress without a model

A companion of `ARCHITECTURE_TTS_FRONT_END.md`: the Russian stress pack and every tier of the stress
pass that needs no neural model. `ARCHITECTURE_COMMON.md` (repo root) is the contract. The order the
tiers run in, and the context model behind them, is
`ARCHITECTURE_TTS_FRONT_END.md#tts-stress-context`; the file charters are
`ARCHITECTURE_TTS_FRONT_END.md#tts-front-end`.

## Mechanisms

### The Russian stress pack {#tts-ru-stress-pack}

`tts_g2p_ru.bin` opens with the header every pack shares, then holds two front-coded tables: the
stress table (a lowercase form to its stressed-vowel ordinals, the default first) and the ё table
(a form written with е to its ё spelling). A table is five u32 - keys, keys a block, blocks, the
longest key's bytes, blob bytes - then one u32 blob offset a block, then the blob. A block holds 16
keys in sorted order. An entry is a head byte - the high nibble counts the key bytes it takes from
the key before it, the low nibble its own key bytes, a nibble of 15 followed by the true count in a
byte - then its own key bytes, then its value. A block's first key takes none, so a block is read
from its own start. A key byte is one letter: the hyphen 1, а to я 2 to 33, ё 34, and the keys sort
by those bytes. A value is a run of bytes whose high bit says another follows: in the stress table
one ordinal a byte, in the ё table a mask over the key's е letters, seven bits a byte, a set bit a ё.

After the two tables a pack may carry tagged sections, each a u32 tag, a u32 byte count and that
many bytes: `ENDS`, the ending table (`#tts-ru-endings`), and `RULS`, the rule tier
(`#tts-ru-rule-tier`). The pack stays version 2: a file that ends after the ё table is a whole pack,
the reader passes over a tag it does not know, and a missing section means its tier abstains. A
section whose byte count runs past the file stops the load.

The pack is the file's bytes in one array and a few headers of offsets; the block offsets are read
in place, and the load builds nothing that scales with the tables. A lookup (`front_find`) allocates
nothing: the word is recoded once into a local buffer of `FRONT_KEY_MAX` bytes, a binary search over
the blocks' first keys finds the last block that does not start past it, and a walk of that block's
entries carries how many of the word's bytes the last key matched - an entry that keeps more than
that is skipped by its lengths alone, one that keeps fewer ends the walk, and only an entry that
keeps exactly that many has its own bytes compared; no key is rebuilt. A word with any other
character - a capital, a Latin letter - or longer than the buffer is no key, and the load refuses a
pack whose longest key is. Only a result allocates: the ordinals array (`ru_stress_variants`) and
the ё spelling; the default ordinal, a reading's presence, the case-ending stem, the ending table's
answer and the rule tier's answer are read off the file bytes.

The shipped pack has three sources. The OpenRussian tables are the dictionary: their entries keep
their readings and order. `--extra` adds the forms the dictionary lacks, a list at a time, an earlier
list's form winning: first the forms of the English and the Russian Wiktionary (a reading both
editions give goes first, then the English edition's), then the words a Russian Wikipedia article
marks in its lead sentence - names of people and places, most of them. A Wikipedia form enters only
where no other source holds it, and never changes an entry. The builder can add a fourth source,
`--ruaccent` (RUAccent's `accents.json.gz` less the machine-made forms of `accents_nn.json.gz`, its
readings last); the shipped pack is built without it. A dictionary entry changes only through an
`--amend` row: a second reading both editions give, placed after its own, its one reading replaced,
or - an `after` of `-` - the entry taken out, so that the ё table restores a е spelling that is no
form of its own.

### ё restoration {#tts-ru-yo}

The ё table holds a ё form's е spelling only where that spelling is no form of its own: все and
всё are both words, so the text decides. A е spelling Wiktionary holds only as a spelling of its ё
entry - every sense an "alternative form of" that entry - is not a form and never enters the
table, so the pair is restored. The dictionary's own restorations stand whatever the other sources
hold. A name read with е whose spelling a ё form shares (Левин) enters the table as a form from a
list ahead of the other sources, so it stays unrestored. The reader applies the table as it is,
whatever the word's case or place, and writes the pack's lowercase spelling.

### The ё a preposition decides {#tts-ru-yo-rule}

чем, нем, всем and моем are words with е and words with ё, so the ё table leaves them to the text.
One place decides them: right after a preposition that governs the prepositional case, with nothing
but spaces between, the word is the ё form. `ru_mark_words` spells it so, in the letter's own case:
чём after в, на, о, при; нём after the same four; всём after во, на, обо, при; моём after в and о.
чем with a particle joined to it (чем-то, чем-либо, чем-нибудь) follows чем's prepositions.
Three guards keep the е: не before the preposition (не о чем), a full stop after нем that a small
letter or a comma follows (на нем. яз.), and anything but spaces between the two words. A word the
text marks or already spells with ё is not the rule's. по is left out: it governs the dative too
(по всем правилам). The rule answers ahead of the rule tier and of a context pass: the grammar
leaves the word one reading there, and text that omits the ё in exactly these phrases (во всем
мире) teaches a learned rule the wrong one.

### A pick that is the ё spelling {#tts-ru-yo-pick}

A pick names a word's stressed vowel by its ordinal. ё is always stressed, so the ё reading of a
word written with е is a pick too: `RU_PICK_YO` added to the ordinal says that vowel, an е, is
read as ё. A key holds fewer vowels than `RU_PICK_YO`, so an ordinal and a ё pick never meet. The
rule tier's two sides and a context pass's pick carry either kind; `mark_pick` writes a ё pick
as the ё itself in the letter's own case, with no `+`, and leaves the word as written where that
vowel is no е. This is how a pair the ё table leaves to the text (все and всё) is decided: the
form sits in the rule tier with the е reading on one side and the ё reading on the other, its
unruled side the spelling that speech uses more often. A context model decides the same pairs
where its head is the wider one: classes 1 to 8 are the stressed vowel's ordinal, classes 9 to 16
the same vowels read as ё, and `ru_stress_class_pick` turns a class into its pick. Its catalog
lists a pair form with one class of each kind, so the sentence chooses between the two spellings
as it chooses between two stresses.

### A hyphenated word {#tts-ru-hyphenated}

`ru_mark_word` first looks the whole word up, lowercase, as one key (что-нибудь, северо-запад). A
value holds alternative readings, one stressed vowel each, so a whole key marks one vowel: the one
the dictionary marks, for a Wiktionary form with a mark on each side the last. The sides are then
read one by one as words of their own, and the side the whole key marked is settled by its mark. The
other sides take their stress from their own keys: the halves that never stand alone (нибудь,
северо, юго) are keys with the reading they have inside a compound. A word with a mark or a ё in the
text, or made of clitics alone (из-за, из-под), skips the lookup and its clitic sides take no
stress; a context pass and the rule tier are never asked about one. Any other hyphenated word that
a context model's catalog or the rule tier holds as a form (чем-то) takes that pick - the context
pass's first - in place of the whole key's default, and its sides are then read as above.

### A word the pack does not hold {#tts-ru-unknown-word}

A word with no entry of its own, and no entry for its base where it ends in -ся or -сь, is read by
five tiers in order (`ru_unknown_ordinal`): the form it is a case of (`#tts-ru-case-stem`), the
full patronymic it is short for (`#tts-ru-short-patronymic`), the abbreviation rule
(`#tts-ru-abbreviations`), the ending table (`#tts-ru-endings`), and last the shape rule - the
vowel before the last.

### The form a word is a case of {#tts-ru-case-stem}

`ru_stem_pick` sheds one case ending - -ими, -их, -у, -е, -ы, -и, -о, the longest first - and
looks the rest up as a pack form. Where the pack holds that stem with one reading and two vowels or
more, the word takes the stem's stressed vowel: Федереру reads as Федерер does. A stem the stress
table lacks but the ё table restores to -ёв gives its ё as a ё pick (`#tts-ru-yo-pick`), so a
surname and the place named after it keep the letter in every case: Королеву reads Королёву,
Горбачево Горбачёво. Another restored stem is left alone: its ё does not carry to a longer word
(пелён, but пелено is no form of it). The stem is the
word's own key buffer at a shorter length, so the lookup allocates nothing. The list holds the
endings under which a pack form keeps its stem's stress at least 97 times in 100, measured over the
pack's own forms by the builder's tables; -а, -ам, -ами, -ах, -ом, -ов and the adjective endings
fall short of that and are left to the ending table.

### A short patronymic {#tts-ru-short-patronymic}

Speech shortens a patronymic and keeps its stress: Андреич is Андреевич, Егорыч is Егорович.
`ru_patronymic_ordinal` reads -еич as -еевич and -ыч as -ович, keeps a case ending of two letters
at most (Сергеичу, Иванычем), and looks the full form up in the pack. Where the pack holds it with
one reading whose stress sits in the name's stem, the short word takes the same vowel, counted from
the word's start. The full form's key is built in a local buffer, so the lookup allocates nothing.
The ending table reads these words on the wrong vowel, which is why the tier sits ahead of it.

### Abbreviations {#tts-ru-abbreviations}

A word the pack does not hold, written in capitals from its first letter to its last and closed by
a consonant, takes its stress on the last vowel where it has two or three vowels (АЭС, ВЦИОМ,
УФСИН). The rule needs the word as written, so it sits in `ru_mark_word`, which is handed it. A
pack entry always wins, which keeps a sentence set in capitals on its words' own readings and lets
the pack hold the abbreviations the rule would misread. An abbreviation that ends in a vowel is not
the rule's: such words split both ways (НАТО, РИА against ОАО, МАГАТЭ), so the pack carries the
ones that stress the last vowel as entries.

### The ending table {#tts-ru-endings}

The `ENDS` section is one front-coded table: an ending's letters, reversed, to the stressed vowel's
position counted from the word's END, one byte (1 is the last vowel). `ru_ending_ordinal` reverses
the word's key once, then looks up its last letters as a key, the longest ending the table holds
first and one letter fewer each time, down to one; the first ending found whose position the word
has a vowel for decides. An ending is always shorter than its word. The keys are reversed so that
endings with the same last letters sit in one block and share their bytes; the lookups are exact
ones through `front_find`, so the table needs no reader of its own.

`build_g2p_ru_data.py --endings LMIN:LMAX:N:SHARE` builds the table from the stress table's own
forms - every hyphen-free form of two vowels or more, by its default reading: an ending of LMIN to
LMAX letters is kept when N forms or more carry it and SHARE of them stress the same vowel from the
end. An entry that the longest shorter kept ending of it already answers the same way is left out,
which changes no lookup. The shipped pack is built with `2:8:2:0.4`.

### The rule tier {#tts-ru-rule-tier}

The `RULS` section reads a homograph by the words around it. It opens with a front-coded table of
forms: a form to four values - its family, the pick of side 0, the pick of side 1, and its
unruled side (0 for none, 1 for side 0, 2 for side 1). A side's pick is a vowel's ordinal or a ё
pick (`#tts-ru-yo-pick`), so the table holds a stress pair (за́мок, замо́к) and a pair of
spellings (все, всё) alike. After the table come the family count, one
u32 a family and one more - where each family's rules start - and the rules. A rule is three bytes
and a key: the kind and the neighbour's slot in one byte, the verdict (the side in bit 0; bit 1, which the
builder sets on its surest rules, is not read), the key's byte count, then the key in the pack's alphabet. The slots are the words
three, two and one before the form and one and two after it. The kinds are three: the neighbour is
the key; the neighbour ends in the key and is two letters longer at least; the sentence holds no
such neighbour.

`ru_stress_rule` looks the form up, recodes the five neighbours into one local buffer, and walks
its family's rules in order: the first rule that holds decides, and the form's ordinal for that
side is the answer. Where no rule holds, the form's unruled side answers, if it names one;
otherwise the tier abstains and the pack's default stands. A neighbour is a word of the same
sentence (`ru_word_sentences`); a neighbour that is no pack key - a marked word, another script -
fires no word or ending rule. The walk allocates nothing.

The tier runs in `ru_mark_words`, for every word a context pass left without a pick, so a build
with no context model reads through it. Where a context model is loaded it answers every form its
catalog lists before the tier is asked: on the model's forms a rule is the weaker reader, so no
rule stands in front of it. `build_g2p_ru_data.py --rules <file>` packs the section from a rules file - its
families, its forms and each family's rules in their order; without the argument the pack has no
section and the tier abstains on every word.
