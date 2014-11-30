#ifndef STREAM_READER_H_GUARD_C13B12B8_8933_44CD_BDB7_AC2A1E60D708
#define STREAM_READER_H_GUARD_C13B12B8_8933_44CD_BDB7_AC2A1E60D708

#include <cstdio>
#include <stack>

// 汎用のデータを順に読み出すクラス
class stream_reader {
	private:
		std::stack<int> ungetted_chars;
		virtual int read_char(void) = 0;
	public:
		int get_char(void);
		void unget_char(int c);
		virtual bool is_eof(void) = 0;
};

// ファイルを順に読み出すクラス
class file_reader : public stream_reader {
	private:
		FILE* fp;
		int read_char(void);
	public:
		file_reader(FILE* _fp): fp(_fp) {}
		bool is_eof(void);
};

#endif
