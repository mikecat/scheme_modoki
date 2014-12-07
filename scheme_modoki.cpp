#include <cstdio>
#include "scheme_modoki.h"
#include "scheme_modoki_core.h"
#include "global_config.h"
#include "creater.h"
#include "kumikomi_tetuduki.h"

static p_data_t taiiki_kankyo;

int run_script(stream_reader& sr,bool is_interactive) {
	int exit_code=0;
	for(;;) {
		p_data_t data;
		if(is_interactive)printf("input> ");
		data=parse(sr);
		if(data->get_type()==DT_EOF)break;
		data=evaluate(data,taiiki_kankyo);
		if(data->get_type()==DT_EOF)break;
		else if(data->get_type()==DT_EXIT) {
			exit_code=((exit_t*)&*data)->exit_code;
			break;
		}
		if(is_interactive) {
			print_data(data,global_config::get_gc().get_do_syouryaku());
			putchar('\n');
		} else {
			if(data->get_type()==DT_ERROR) {
				print_data(data,global_config::get_gc().get_do_syouryaku());
				putchar('\n');
				return 1;
			}
		}
	}
	return exit_code;
}

int main(int argc,char *argv[]) {
	std::string file_name;
	bool use_file=false;
	// コマンドライン引数を読み込む
	for(int i=1;i<argc;i++) {
		std::string cur_arg=argv[i];
		if(cur_arg=="--no-syouryaku") {
			global_config::get_gc().set_do_syouryaku(false);
		} else if(cur_arg=="--no-auto-delete") {
			p_data_config_t::set_do_auto_delete(false);
		} else if(!use_file) {
			file_name=cur_arg;
			use_file=true;
		}
	}
	// 大域環境を作成する
	taiiki_kankyo=creater_t::creater().create_kankyo_data();
	add_kumikomi_tetuduki_to_kankyo(taiiki_kankyo);
	// スクリプトを実行する
	int ret;
	if(use_file) {
		FILE* script=fopen(file_name.c_str(),"r");
		if(script!=NULL) {
			file_reader fr(script);
			ret=run_script(fr,false);
			fclose(script);
		} else {
			fprintf(stderr,"ERROR: failed to open file \"%s\"\n",file_name.c_str());
			ret=1;
		}
	} else {
		ret=run_script(global_config::get_gc().get_stdin_reader(),true);
	}
	// 大域環境を開放させる(自動削除が有効の場合)
	taiiki_kankyo=NULL;
	creater_t::creater().print_number_of_data();
	return ret;
}
