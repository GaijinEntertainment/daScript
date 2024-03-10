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

    private static Random rng = new Random();

    static int [] makeRandomSequence(int n) {
        int [] src = new int[n];
        for (int i = 0; i < n; i++) {
            src[i] = rng.Next();
        }
        return src;
    }

    static void sortTable(int [] tab) {
        Array.Sort(tab, (a, b) => -a.CompareTo(b));
    }

    static void Main() {
        var tab = makeRandomSequence(100000);
        profile(20, "sort", () => {
            var tabb = (int[])tab.Clone();
            sortTable(tabb);
        });
    }
}
