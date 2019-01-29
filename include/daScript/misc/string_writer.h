#pragma once

namespace das {

	using namespace std;

	struct VectorAllocationPolicy {
		vector<char> data;
		string str() const {			// todo: replace via stringview
			return data.data();
		}
		void append(const char * s, int l) {
			data.reserve(data.size() + l + 1);
			data.insert(data.end(), s, s + l + 1);
		}
		char * alloocate (int l) {
			data.reserve(data.size() + l + 1);
			char * at = data.data();
			data.resize(data.size() + l + 1);
			return at;
		}
		void adjust(int l, int realL) {
			data.resize(data.size() - l + realL);
		}
	};

	// todo: support hex
	struct StringWriterTag {};
	extern StringWriterTag HEX;
	extern StringWriterTag DEC;

	template <typename AllocationPolicy>
	class StringWriter : public AllocationPolicy {
	public:
		template <typename TT>
		StringWriter & write(const char * format, TT value) {
			const int size = 100;
			auto at = allocate(size);
			int realL = _snprintf(at, size, format, TT);
			adjust(size, realL);
			return *this;
		}
		StringWriter & write(const char * st) {
			auto len = strlen(st);
			append(st, len);
			return *this;
		}
		StringWriter & operator << (const StringWriterTag & v ) { 
			if (v == HEX) hex = true;
			else if (v == DEC) hex = false;
			return *this;
		}
		StringWriter & operator << (char v)					{ return write("%c", v); }
		StringWriter & operator << (unsigned char v)		{ return write("%c", v); }
		StringWriter & operator << (bool v)					{ return write(v ? "true" : "false"); }
		StringWriter & operator << (int v)					{ return write(hex ? "%x" : "%d", v); }
		StringWriter & operator << (long v)					{ return write(hex ? "%lx" : "%ld", v); }
		StringWriter & operator << (long long v)			{ return write(hex ? "%llx" : "%lld", v); }
		StringWriter & operator << (unsigned v)				{ return write(hex ? "%x" : "%u", v); }
		StringWriter & operator << (unsigned long v)		{ return write(hex ? "%lx" : "%lu", v); }
		StringWriter & operator << (unsigned long long v)	{ return write(hex ? "%llx" : "%llu", v); }
		StringWriter & operator << (float v)				{ return write("%f", v); }
		StringWriter & operator << (double v)				{ return write("%f", v); }
		StringWriter & operator << (long double v)			{ return write("%Lf", v); }
		StringWriter & operator << (char * v)				{ return write(v); }
		StringWriter & operator << (const char * v)			{ return write(v); }
		StringWriter & operator << (const string & v)		{ return write(v, v.length()); }
	protected:
		bool hex = false;
	};

	typedef StringWriter<VectorAllocationPolicy> TextWriter;
}
