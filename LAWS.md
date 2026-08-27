# LAWS.md - Boris's rulings

Append-only intent provenance for rule-document edits in this folder's documents (the
mechanism: CLAUDE.md sec. "Boris's rulings get a `LAWS.md` sidecar"). Never groomed,
compacted, or cited as rules.

| Date | Document | The ask |
|---|---|---|
| 2026-08-22 | CLAUDE.md (comment hygiene), skills/comment_style_hygiene.md, skills/internal/make_pr.md, .claude/hooks/, .claude/settings.json | "i think the hook is wrong - and we get rid of it. u should be able to write them while u work on PR as u see fit. then, come PR or otherwise finished section - just like we do with lint - we run formatter, and if any significant amount of comments get deleted - we run rescue bot. it becomes normal part of PR like lint, REVIEW.md, TDD auditor" |
| 2026-08-22 | utils/mcp/tools/format_file.das, utils/das-fmt/dasfmt.das (tool messages) | "formatter tool should output helpful message - like run rescue thing on this, because comments were deleted too" |
| 2026-08-22 | .claude/agents/rescue-bot.md, .claude/agents/rescue-sweep-bot.md | "bot reports. im ok it not reading REVIEW.md and ARCHITECTURE.md. we decide the rescue at the end. up to us to save or not. more focused bot. TODO - split to bots: one is rescue-sweep-bot, which we'll get rid of after doing the sweep - or leave to people. one to deal with new code. i want report only - one archer -> one arrow" |
| 2026-08-22 | .claude/agents/*.md (model pins) | "bots are Opus. reviewer is Fable. [...] nah, i mean u. dragon and reviewer turn out to be fine with Opus" |
| 2026-08-22 | .claude/agents/analysis-bot.md | "we make 'analysis-bot'. it looks at the file, and comments it - in good detail - as to what it does. and then we run rescue bot on it. [...] they are lacking intent. they are emission. [...] provenance and analysis go hand in hand" |
| 2026-08-22 | CLAUDE.md (this mechanism) | "I don't know [how provenance lands in the docs]. But I'd like to be able to decide later. Which means we add LAWS.md next to whichever document we are editing - where we list 1. date. 2. which document. 3. what I asked for" |
| 2026-08-24 | REVIEW_COMMON.md (obligation-form bullet) | "to me a few rules in that file read like a statement. I'd like to propose change to REVIEW_COMMON.md - which states that rules should read like rules, not statements. [...] lets document" - no sweep of existing checklists; rewrite-on-touch |
| 2026-08-24 | REVIEW_COMMON.md (plain-English bullet) | "im an immigrant with russian origins. i studied english in russia. academic writing is not my idea of light reading." - yes to the proposed bullet: plain English, short sentences, common words, no idioms; a rule needing a second read is a defect |
| 2026-08-27 | REVIEW_COMMON.md (rule-form bullet), .claude/agents/dragon.md, .claude/agents/janitor.md (new) | "we really have 3 types of things we state in REVIEW.md. A is 'how the system should be' ... B is 'how the system should not be'. and then there is C 'how the system is allowed to change'. and they are in the conflicting formats. i was effectively expecting B and C, and the current setup is primarily A" + "plain English rule not enforced" - rules are bans and duties, statements banned outright (the 08-24 A-form license removed); dragon gets a STATEMENT verdict and bright-line idiom flagging; a janitor agent rewrites checklists, trialed on one medium REVIEW.md first |
