#!/usr/bin/env python3
# MLX (mlx-lm, Apple GPU) text bench - the MLX reference column of the LLM board, in llama-bench's
# shape: a prompt of random token ids timed as prefill (pp<P>), then <G> generated tokens timed as
# decode (tg<G>), each the mean and standard deviation of --reps trials after one untimed warm-up
# trial; with --npl N > 1 a batch of N such prompts decoded together (tg<G>@<N>, the aggregate
# rate over all streams, as llama-batched-bench's S_TG). Rates come from mlx-lm's own generation
# stats (prompt_tps, generation_tps). End-of-sequence ids are cleared so every trial runs the
# whole count.
#
#   benchmarks/asr/.venv-mlx/bin/python mlx_lm_bench.py --model mlx-community/Qwen3.8-27B-4bit \
#       -p 512 -n 128 -r 5 [--npl 4]
#
# Prints one `ROW\t<test>\t<tok/s>\t<stddev>` line a test and one VERSIONS line.
import argparse
import statistics


def mean_sd(vals):
    return statistics.fmean(vals), (statistics.stdev(vals) if len(vals) > 1 else 0.0)


def main():
    ap = argparse.ArgumentParser()
    ap.add_argument("--model", required=True)
    ap.add_argument("-p", "--plen", type=int, default=512)
    ap.add_argument("-n", "--ngen", type=int, default=128)
    ap.add_argument("-r", "--reps", type=int, default=5)
    ap.add_argument("--npl", type=int, default=1)
    args = ap.parse_args()

    from importlib.metadata import version
    import mlx.core as mx
    from mlx_lm import batch_generate, load, stream_generate

    print(f"VERSIONS\tmlx-lm={version('mlx-lm')}\tmlx={version('mlx')}\t{args.model}", flush=True)
    mx.random.seed(0)
    model, tokenizer, config = load(args.model, return_config=True)
    tokenizer._eos_token_ids = set()
    vocab = config.get("vocab_size") or config["text_config"]["vocab_size"]
    prompts = mx.random.randint(0, vocab, (max(args.npl, 1), args.plen)).tolist()

    def single():
        last = None
        for last in stream_generate(model, tokenizer, prompts[0], max_tokens=args.ngen):
            pass
        return last

    single()
    pp, tg = [], []
    for _ in range(args.reps):
        r = single()
        pp.append(r.prompt_tps)
        tg.append(r.generation_tps)
    for tag, vals in ((f"pp{args.plen}", pp), (f"tg{args.ngen}", tg)):
        m, sd = mean_sd(vals)
        print(f"ROW\t{tag}\t{m:.3f}\t{sd:.3f}", flush=True)

    if args.npl > 1:
        def batch():
            return batch_generate(model, tokenizer, prompts, max_tokens=args.ngen).stats

        batch()
        rates = [batch().generation_tps for _ in range(args.reps)]
        m, sd = mean_sd(rates)
        print(f"ROW\ttg{args.ngen}@{args.npl}\t{m:.3f}\t{sd:.3f}", flush=True)


if __name__ == "__main__":
    main()
