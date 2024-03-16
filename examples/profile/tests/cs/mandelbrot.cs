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

    struct float2 {
        public float x;
        public float y;
        public float2 ( float x, float y ) {
            this.x = x;
            this.y = y;
        }

        public static float2 operator + ( float2 a, float2 b ) {
            return new float2(a.x + b.x, a.y + b.y);
        }

        public static float length2 ( float2 a ) {
            return (float) Math.Sqrt(a.x * a.x + a.y * a.y);
        }
    }

    static int level ( float2 c ) {
        int l = 0;
        float2 z = c;
        while ( float2.length2(z) < 2.0 && l < 255 ) {
            z = new float2(z.x * z.x - z.y * z.y, z.x * z.y + z.y * z.x) + c;
            l++;
        }
        return l - 1;
    }

    static int test() {
        float xmin = -2.0f;
        float xmax = 2.0f;
        float ymin = -2.0f;
        float ymax = 2.0f;
        int N = 64;
        float dx = (xmax - xmin) / N;
        float dy = (ymax - ymin) / N;
        int S = 0;
        float2 xy = new float2(xmin, ymin);
        for (int i = 0; i < N; i++) {
            xy.y = ymin;
            for (int j = 0; j < N; j++) {
                S += level(xy);
                xy.y += dy;
            }
            xy.x += dx;
        }
        return S;
    }

    static void Main() {
        profile(20, "mandelbrot", () => {
            int result = test();
            Debug.Assert(result == 107701);
        });
    }
}
