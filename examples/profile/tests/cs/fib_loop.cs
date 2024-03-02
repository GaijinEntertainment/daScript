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
        Console.WriteLine("\"" + category + "\"," + minT + ", " + count);
    }

    public static int FibI(int n)
    {
        int last = 0;
        int cur = 1;
        n = n - 1; // Adjust because the loop starts with the second Fibonacci number
        while (n > 0)
        {
            n = n - 1;
            int tmp = cur;
            cur = last + cur;
            last = tmp;
        }
        return cur;
    }
    static void Main() {
        profile(20, "fibbonacci loop", () => {
            int fi = FibI(6511134);
            Debug.Assert( fi==1781508648, "The result is incorrect.");
        });
    }
}

