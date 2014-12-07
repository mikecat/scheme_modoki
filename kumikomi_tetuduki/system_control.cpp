#include "system_control.h"
#include "../creater.h"
#include "../global_config.h"

// 省略できるconsセルの表示を省略するかを設定する
p_data_t set_enable_syouryaku(const std::vector<p_data_t>& args,p_data_t&) {
	if(args.size()!=1) {
		return creater_t::creater().create_argument_number_error_data(
			"set-enable-syouryaku",1,args.size(),false);
	}
	global_config::get_gc().set_do_syouryaku(
		!(args[0]->get_type()==DT_BOOLEAN && !((boolean_t*)&*args[0])->is_true)
	);
	return args[0];
}

// 省略できるconsセルの表示を省略するかを取得する
p_data_t get_enable_syouryaku(const std::vector<p_data_t>& args,p_data_t&) {
	if(args.size()!=0) {
		return creater_t::creater().create_argument_number_error_data(
			"get-enable-syouryaku",0,args.size(),false);
	}
	return creater_t::creater().create_boolean_data(global_config::get_gc().get_do_syouryaku());
}

// 生成され、削除されていない環境とデータの数を表示する
p_data_t print_statistics(const std::vector<p_data_t>& args,p_data_t&) {
	if(args.size()!=0) {
		return creater_t::creater().create_argument_number_error_data(
			"print-statistics",0,args.size(),false);
	}
	creater_t::creater().print_statistics();
	return creater_t::creater().create_null_data();
}

// 環境とデータの自動削除の有効/無効を設定する
p_data_t set_enable_auto_delete(const std::vector<p_data_t>& args,p_data_t&) {
	if(args.size()!=1) {
		return creater_t::creater().create_argument_number_error_data(
			"set-enable-auto-delete",1,args.size(),false);
	}
	p_data_config_t::set_do_auto_delete(
		!(args[0]->get_type()==DT_BOOLEAN && !((boolean_t*)&*args[0])->is_true));
	return args[0];
}

// 環境とデータの自動削除の有効/無効を取得する
p_data_t get_enable_auto_delete(const std::vector<p_data_t>& args,p_data_t&) {
	if(args.size()!=0) {
		return creater_t::creater().create_argument_number_error_data(
			"get-enable-auto-delete",0,args.size(),false);
	}
	return creater_t::creater().create_boolean_data(p_data_config_t::get_do_auto_delete());
}

// 参照カウントが0以下の環境とデータを削除する
p_data_t delete_zero_sansyo(const std::vector<p_data_t>& args,p_data_t&) {
	if(args.size()!=0) {
		return creater_t::creater().create_argument_number_error_data(
			"delete-zero-sansyo",0,args.size(),false);
	}
	creater_t::creater().delete_zero_sansyo();
	return creater_t::creater().create_null_data();
}
