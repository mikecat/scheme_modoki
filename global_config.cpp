#include "global_config.h"

global_config global_config::gc;

global_config::global_config(): do_syouryaku(true), stdin_reader(stdin) {}

global_config& global_config::get_gc() {
	return gc;
}

void global_config::set_do_syouryaku(bool enable) {
	do_syouryaku=enable;
}

bool global_config::get_do_syouryaku() const {
	return do_syouryaku;
}

file_reader& global_config::get_stdin_reader() {
	return stdin_reader;
}
