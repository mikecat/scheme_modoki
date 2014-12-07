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
		}
	}
	return exit_code;
}

int main(int argc,char *argv[]) {
	// コマンドライン引数を読み込む
	for(int i=1;i<argc;i++) {
		std::string cur_arg=argv[i];
		if(cur_arg=="--no-syouryaku") {
			global_config::get_gc().set_do_syouryaku(false);
		} else if(cur_arg=="--no-auto-delete") {
			p_data_config_t::set_do_auto_delete(false);
		}
	}
	// 大域環境を作成する
	taiiki_kankyo=creater_t::creater().create_kankyo_data();
	add_kumikomi_tetuduki_to_kankyo(taiiki_kankyo);
	// スクリプトを実行
	int ret=run_script(global_config::get_gc().get_stdin_reader(),true);
	// 大域環境を開放させる(自動削除が有効の場合)
	taiiki_kankyo=NULL;
	return ret;
}
