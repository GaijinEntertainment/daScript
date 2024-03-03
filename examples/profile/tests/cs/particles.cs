using System;
using System.Diagnostics;
using System.Timers;
using System.Collections.Generic;
using System.Numerics;

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

    struct Vector3
    {
        public float x;
        public float y;
        public float z;

        public Vector3(float x, float y, float z)
        {
            this.x = x;
            this.y = y;
            this.z = z;
        }

        public static Vector3 operator +(Vector3 a, Vector3 b)
        {
            return new Vector3(a.x + b.x, a.y + b.y, a.z + b.z);
        }
    }

    struct NObject
    {
        public Vector3 position;
        public Vector3 velocity;

        // Constructor to easily initialize an NObject
        public NObject(Vector3 position, Vector3 velocity)
        {
            this.position = position;
            this.velocity = velocity;
        }
    }

    static void TestSimI(NObject[] objects)
    {
        for (int i = 0; i < objects.Length; i++)
        {
            objects[i].position += objects[i].velocity;
        }
    }

    static void TestSim2I(NObject[] objects, int count)
    {
        for (int i = 0; i < count; i++)
        {
            TestSimI(objects);
        }
    }

    static NObject[] Init()
    {
        var objects = new NObject[50000];
        int i = 0;
        for (; i < objects.Length; i++)
        {
            objects[i] = new NObject(new Vector3(i, i + 1, i + 2), new Vector3(1.0f, 2.0f, 3.0f));
        }
        Debug.Assert(i == objects.Length, "Initialization count mismatch.");
        return objects;
    }

    static void Main() {
        var objects = Init();
        profile(20, "particles kinematics, inlined", () => {
            TestSim2I(objects, 100);
        });
        float f = 0;
        for (int i = 0; i < objects.Length; i++)
        {
            f += objects[i].position.x + objects[i].position.y + objects[i].position.z;
        }
        Debug.Assert(f != 0.123f, "The result is not correct.");
    }
}
