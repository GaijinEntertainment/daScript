using System;
using System.Diagnostics;
using System.Timers;
using System.Collections.Generic;

class HelloWorld {

    delegate void MyBlock();

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

    public static int FibR(int n)
    {
        if (n < 2)
        {
            return n;
        }
        return FibR(n - 1) + FibR(n - 2);
    }

    static void Main() {
        profile(20, "fibbonacci recursive", () => {
            int fi = FibR(31);
            Debug.Assert( fi==1346269, "The result is incorrect.");
        });
    }
}

