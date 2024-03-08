using System;
using System.Diagnostics;
using System.Timers;
using System.Collections.Generic;
using System.Runtime.InteropServices;

class HelloWorld {

    [DllImport ("addOne.dll")]
    private static extern int AddOne ( int n );

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


    public static int testAdds()
    {
        int count = 0;
        for ( int i = 0; i < 10000000; i++ ) {
            count = AddOne(count);
        }
        return count;
    }

    static void Main() {
        profile(20, "native loop", () => {
            int count = testAdds();
            Debug.Assert(10000000==count);
        });
    }
}
