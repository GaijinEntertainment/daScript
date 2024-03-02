array<string> dict_makeSrc() {
	array<string> src;
	int n = 500000;
	int modn = n;
	for (int i = 0; i < n; ++i) {
		int num = (271828183 ^ i * 119) % modn;
		src.insertLast("_" + formatInt(num,"",0));
	}
	return src;
}

int dict(array<string> &in src) {
	dictionary tab;
	int n = src.length();
	int max = 1;
	for (int i = 0; i < n; ++i) {
		string l = src[i]; // Fixed to use i instead of n to access current element
		if (tab.exists(l)) {
			int count;
			tab.get(l, count);
			count++;
			tab.set(l, count);
			max = max > count ? max : count;
		} else {
			tab.set(l, 1);
		}
	}
	return max;
}

int main(int unused)
{
    print("dictionary: ");
    array<string> src = dict_makeSrc();
    int ctime = GetSystemTime();
    int fr = dict(src);
    int time = GetSystemTime()-ctime;
    print(fr);
    print("\ntime taken (msec): ");
    print (time);
    print("\n");

    return 0;
}
