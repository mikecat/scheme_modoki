#include <cstdio>
#include "scheme_modoki.h"
#include "scheme_modoki_core.h"
#include "creater.h"
#include "kumikomi_tetuduki.h"

static bool do_syouryaku=true;

void set_do_syouryaku(bool enable) {
	do_syouryaku=enable;
}

bool get_do_syouryaku() {
	return do_syouryaku;
}

int main(int argc,char *argv[]) {
	for(int i=1;i<argc;i++) {
		std::string cur_arg=argv[i];
		if(cur_arg=="--no-syouryaku") {
			do_syouryaku=false;
		} else if(cur_arg=="--no-auto-delete") {
			p_data_config_t::set_do_auto_delete(false);
		}
	}
	int exit_code=0;
	p_data_t taiiki_kankyo=creater_t::creater().create_kankyo_data();
	add_kumikomi_tetuduki_to_kankyo(taiiki_kankyo);
	file_reader fr(stdin);
	for(;;) {
		p_data_t data;
		printf("input> ");
		data=parse(fr);
		if(data->get_type()==DT_EOF)break;
		data=evaluate(data,taiiki_kankyo);
		if(data->get_type()==DT_EOF)break;
		else if(data->get_type()==DT_EXIT) {
			exit_code=((exit_t*)&*data)->exit_code;
			break;
		}
		print_data(data,do_syouryaku);
		putchar('\n');
	}
	return exit_code;
}
