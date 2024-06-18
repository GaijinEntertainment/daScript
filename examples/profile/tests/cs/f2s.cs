using System;
using System.Diagnostics;
using System.Timers;
using System.Collections.Generic;
using System.Numerics;

class HelloWorld {

    delegate void MyBlock();

    const int TOTAL_NUMBERS = 10000;
    const int TOTAL_TIMES = 100;

    static void profile ( int count, string category, MyBlock f ) {
        double minT = 1e+06;
        for ( int i = 0; i < count; i++ ) {
            DateTime start = DateTime.Now;
            f();
            DateTime end = DateTime.Now;
            TimeSpan elapsed = end - start;
            double dt = ((double)elapsed.TotalMilliseconds) / 1000.0;
            minT = Math.Min(minT, dt);
        }
        Console.WriteLine($"\"{category}\", {minT}, {count}");
    }


    static int test_f2i(float[] nums)
    {
        int summ = 0;
        for ( int i = 0; i != TOTAL_TIMES; ++i ) {
            foreach (var n in nums) {
                summ += n.ToString().Length;
            }
        }
        return summ;
    }

    static float mk_float ( int i ) {
        return ((float)i) + ((float)i) / ((float)TOTAL_NUMBERS);
    }

    static float[] Init()
    {
        var nums = new float[TOTAL_NUMBERS];
        for ( int i = 0; i != TOTAL_NUMBERS; ++i ) {
            nums[i] = mk_float(i);
        }
        return nums;
    }

    public static bool IsRunningOnMono()
    {
        return Type.GetType("Mono.Runtime") != null;
    }

    static void Main() {
        var nums = Init();
        var count = IsRunningOnMono() ? 1 : 20;
        profile(count, "float2string", () => {
            test_f2i(nums);
        });
    }
}
