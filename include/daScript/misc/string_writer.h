#pragma once

namespace das {

	class VectorAllocationPolicy {
	public:
		string str() const {			// todo: replace via stringview
			return string(data.data(), data.size());
		}
		int tellp() const {
			return int(data.size());
		}
	protected:
		void append(const char * s, int l) {
			data.reserve(data.size() + l);
			data.insert(data.end(), s, s + l);
		}
		char * allocate (int l) {
			data.resize(data.size() + l);
			return data.data() + data.size() - l;
		}
		void adjust(int l, int realL) {
			data.resize(data.size() - l + realL);
		}
		virtual void output() {}
		vector<char> data;
	};

	// todo: support hex
	struct StringWriterTag {};
	extern StringWriterTag HEX;
	extern StringWriterTag DEC;
    extern StringWriterTag FIXED;
    extern StringWriterTag SCIENTIFIC;

	template <typename AllocationPolicy>
	class StringWriter : public AllocationPolicy {
	public:
		template <typename TT>
		StringWriter & write(const char * format, TT value) {
			const int size = 100;
			auto at = this->allocate(size);
			int realL = snprintf(at, size, format, value);
			this->adjust(size, realL);
			this->output();
			return *this;
		}
		StringWriter & write(const char * st, size_t len) {
			this->append(st, int(len));
			this->output();
			return *this;
		}
		StringWriter & write(const char * st) {return write(st, strlen(st));}
		StringWriter & operator << (const StringWriterTag & v ) {
			if (&v == &HEX) hex = true;
			else if (&v == &DEC) hex = false;
            else if (&v == &FIXED) fixed = true;
            else if (&v == &SCIENTIFIC) fixed = false;
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
        StringWriter & operator << (float v)				{ return write(fixed ? "%.9f" : "%g", v); }
        StringWriter & operator << (double v)				{ return write(fixed ? "%.17f" : "%g", v); }
		StringWriter & operator << (char * v)				{ return write(v); }
		StringWriter & operator << (const char * v)			{ return write(v); }
		StringWriter & operator << (const string & v)		{ return write(v.c_str(), v.length()); }
	protected:
		bool hex = false;
        bool fixed = false;
	};

	typedef StringWriter<VectorAllocationPolicy> TextWriter;

	class TextPrinter : public TextWriter {
	public:
		virtual void output() {
			int newPos = tellp();
			if (newPos != pos) {
				string st(data.data() + pos, newPos - pos);
				printf("%s", st.c_str());
				pos = newPos;
			}
		}
	protected:
		int pos = 0;
	};
}
