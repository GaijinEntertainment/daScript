# Cadmus conversation history and assistant plan

## Product decisions

- Cadmus keeps all history locally in SQLite and retains it indefinitely.
- Every inbound message visible to the bot is recorded, even when Cadmus is not addressed.
- A voice or audio message is one user turn whose primary content is the cleaned transcript; the raw ASR transcript is retained separately.
- The Telegram message used to deliver a transcript is not an assistant turn. Genuine LLM answers are assistant turns.
- In private chats Cadmus answers every text and voice message.
- In groups Cadmus answers mentions, replies to the bot, commands, and voice transcripts that directly address `Cadmus` or `Кадмус`.
- History, retrieval, user summaries, and whole-chat summaries are always limited to the current chat. Forum topics share one history.
- Anyone in a chat may request its summaries.
- Bare `today` means the rolling previous 24 hours. An explicit date means that local calendar day.
- User summaries filter by stable Telegram user ID within the current chat.
- History starts when this feature ships; existing logs are not imported.
- Telegram edits update stored messages when reported. Ordinary deletions are not generally reported by the Bot API; supported deletion updates are marked locally.

## Personality

The assistant is named Cadmus, a quiet reference to the figure associated with bringing writing to Greece. The editable personality prompt lives in `dictation.toml`, separate from trusted runtime instructions and the dictation-cleanup prompt.

## Implementation

1. Add typed SQLite tables with migration 1 for messages/bot state and migration 2 for FTS5 search from the first shipped build.
2. Persist Telegram update offsets and enforce `(chat_id, message_id)` uniqueness for idempotency.
3. Normalize text, captions, voice, and audio into one inbound-message path; upsert edit updates.
4. Resolve the bot's own ID and username at startup for reply and mention detection.
5. Store all inbound user content. Store only genuine conversational responses as assistant turns.
6. Build assistant context from a recent chronological window plus older FTS5 matches, restricted to the current chat and a configurable context budget.
7. Implement `/summary`, `/summary yesterday`, `/summary YYYY-MM-DD`, `/summary me`, and `/summary @username [date]`, with chunked reduction for long histories.
8. Treat natural historical questions as ordinary assistant questions using the same recent-plus-FTS context.
9. Add configuration for the database path, Cadmus name and aliases, personality prompt, assistant sampling, retrieval limits, and summary limits.
10. Add tests for idempotency, edits, transcript de-duplication, direct-address rules, rolling-24-hour summaries, explicit local dates, per-user filtering, FTS retrieval, and chat isolation.
11. Update the README and release hook, enable/build dasSQLITE, and verify a staged `daspkg release` before deploying.

## Telegram setup at deployment

Commands work when typed without BotFather configuration, but Telegram's `/` menu must be registered manually.

At deployment, walk the operator through:

1. Open `@BotFather` and run `/setcommands`.
2. Select the Cadmus bot.
3. Paste the final command list, initially:

   ```text
   summary - Summarize recent conversation
   help - Show Cadmus commands and usage
   ```

4. Run `/setprivacy`, select the bot, and disable privacy mode so it can archive ordinary group messages.
5. Remove and re-add the bot to each group, or make it an administrator, so the privacy change takes effect.

Arguments such as `/summary me`, `/summary yesterday`, and `/summary 2026-07-13` are variants of the single `summary` command and do not need separate BotFather entries.
