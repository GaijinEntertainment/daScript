This is latest (at 2019 Jan) LuaJIT2.1.0Beta3 with only one diff (to allow detailed profiling)
Built with LLVM8.0 (clang-cl)

diff --git a/src/lib_os.c b/src/lib_os.c
index ffbc3fd..145ebd4 100644
--- a/src/lib_os.c
+++ b/src/lib_os.c
@@ -122,11 +122,27 @@ LJLIB_CF(os_exit)
   return 0;  /* Unreachable. */
 }
 
+#include <windows.h>
+
+LARGE_INTEGER profileGetTime () {
+    LARGE_INTEGER  t0;
+    QueryPerformanceCounter(&t0);
+    return t0;
+}
+
+double profileGetTimeSec ( LARGE_INTEGER minT ) {
+    LARGE_INTEGER freq;
+    QueryPerformanceFrequency(&freq);
+    return ((double)minT.QuadPart) / ((double)freq.QuadPart);
+}
+
 LJLIB_CF(os_clock)
 {
-  setnumV(L->top++, ((lua_Number)clock())*(1.0/(lua_Number)CLOCKS_PER_SEC));
-  return 1;
+//setnumV(L->top++, ((lua_Number)clock())*(1.0/(lua_Number)CLOCKS_PER_SEC));
+setnumV(L->top++, (lua_Number)profileGetTimeSec(profileGetTime()));
+return 1;
 }
+__declspec(dllexport) int addOne(int a) {return a+1;}
 
 /* ------------------------------------------------------------------------ */
 
