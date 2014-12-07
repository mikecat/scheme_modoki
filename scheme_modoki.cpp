#include <cstdio>
#include "scheme_modoki_core.h"
#include "global_config.h"
#include "creater.h"

int main(int argc,char *argv[]) {
	std::string file_name;
	bool use_file=false;
	bool print_statistics_on_end=false;
	// コマンドライン引数を読み込む
	for(int i=1;i<argc;i++) {
		std::string cur_arg=argv[i];
		if(cur_arg=="--no-syouryaku") {
			global_config::get_gc().set_do_syouryaku(false);
		} else if(cur_arg=="--no-auto-delete") {
			p_data_config_t::set_do_auto_delete(false);
		} else if(cur_arg=="--print-statistics-on-end") {
			print_statistics_on_end=true;
		} else if(!use_file) {
			file_name=cur_arg;
			use_file=true;
		}
	}
	// 大域環境を作成する
	make_taiiki_kankyo();
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
	delete_taiiki_kankyo();
	if(print_statistics_on_end)creater_t::creater().print_statistics();
	return ret;
}
