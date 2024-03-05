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

    // Translated expLoop function
    public static double ExpLoop(int n)
    {
        double sum = 0.0;
        for (int i = 1; i <= n; i++)
        {
            sum += Math.Exp(1.0 / (1.0 + i));
        }
        return sum;
    }

    // Translated verifyExp function
    public static void VerifyExp(double f)
    {
        double t = 1e+06;
        double eps = 10.0;
        double q = Math.Abs(f - t);
        Debug.Assert(q < eps, "The result is outside the acceptable tolerance.");
    }

    static void Main() {
        profile(20, "exp loop", () => {
            double f4 = ExpLoop(1000000);
            VerifyExp(f4);
        });
    }
}
