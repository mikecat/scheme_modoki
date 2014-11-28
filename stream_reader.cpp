#include "stream_reader.h"

int stream_reader::get_char(void) {
	if(ungetted_chars.empty()) {
		return read_char();
	} else {
		int ret=ungetted_chars.top();
		ungetted_chars.pop();
		return ret;
	}
}

void stream_reader::unget_char(int c) {
	ungetted_chars.push(c);
}

int file_reader::read_char(void) {
	return getc(fp);
}

bool file_reader::is_eof(void) {
	return feof(fp);
}
