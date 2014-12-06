#include <cstdio>
#include <cmath>
#include "sansyo_count.h"
#include "creater.h"
#include "kumikomi_tetuduki/number_calc.h"
#include "kumikomi_tetuduki/cons_and_list.h"
#include "kumikomi_tetuduki/special_form.h"

// データが等価か判定する
p_data_t is_eq(const std::vector<p_data_t>& args,p_data_t&) {
	if(args.size()!=2) {
		return creater_t::creater().create_argument_number_error_data(
			"eq?",2,args.size(),false);
	} else {
		if(args[0]->type!=args[1]->type) {
			return creater_t::creater().create_boolean_data(false);
		} else if(args[0]->type==DT_EOF || args[0]->type==DT_NULL) {
			return creater_t::creater().create_boolean_data(true);
		} else if(args[0]->type==DT_NUM) {
			return creater_t::creater().create_boolean_data(args[0]->num==args[1]->num);
		} else if(args[0]->type==DT_KIGOU) {
			return creater_t::creater().create_boolean_data(args[0]->kigou==args[1]->kigou);
		} else if(args[0]->type==DT_BOOLEAN) {
			return creater_t::creater().create_boolean_data(args[0]->is_true==args[1]->is_true);
		} else {
			return creater_t::creater().create_boolean_data(args[0]==args[1]);
		}
	}
}

// 手続きを引数リストに適用する
p_data_t apply(const std::vector<p_data_t>& args,p_data_t& kankyo) {
	if(args.size()<2) {
		return creater_t::creater().create_argument_number_error_data(
			"apply",2,args.size(),true);
	} else {
		std::vector<p_data_t> args_list;
		for(std::vector<p_data_t>::const_iterator it=args.begin()+1;it+1!=args.end();it++) {
			args_list.push_back(*it);
		}
		if(args.back()->type!=DT_CONS && args.back()->type!=DT_NULL) {
			return creater_t::creater().create_error_data(
				"the last argument of apply must be a list");
		}
		for(p_data_t cur_data=args.back();cur_data->type==DT_CONS;cur_data=cur_data->cons_cdr) {
			args_list.push_back(cur_data->cons_car);
		}
		return apply_proc(args[0],args_list,kankyo);
	}
}

// 終了する指示を出す
p_data_t exit_func(const std::vector<p_data_t>&,p_data_t&) {
	return creater_t::creater().create_error_data("",true);
}

// 引数が#f以外なら#fを、#fなら#tを返す
p_data_t not_func(const std::vector<p_data_t>& args,p_data_t&) {
	if(args.size()!=1) {
		return creater_t::creater().create_argument_number_error_data(
			"not",1,args.size(),false);
	} else {
		return creater_t::creater().create_boolean_data(
			args[0]->type==DT_BOOLEAN && !args[0]->is_true); // args[0]が#fかを判定
	}
}

// 省略できるconsセルの表示を省略するかを設定する
p_data_t set_enable_syouryaku(const std::vector<p_data_t>& args,p_data_t&) {
	if(args.size()!=1) {
		return creater_t::creater().create_argument_number_error_data(
			"set-enable-syouryaku",1,args.size(),false);
	}
	set_do_syouryaku(!(args[0]->type==DT_BOOLEAN && !args[0]->is_true));
	return args[0];
}

// 省略できるconsセルの表示を省略するかを取得する
p_data_t get_enable_syouryaku(const std::vector<p_data_t>& args,p_data_t&) {
	if(args.size()!=0) {
		return creater_t::creater().create_argument_number_error_data(
			"get-enable-syouryaku",0,args.size(),false);
	}
	return creater_t::creater().create_boolean_data(get_do_syouryaku());
}

// 生成され、削除されていない環境とデータの数を表示する
p_data_t print_number_of_data(const std::vector<p_data_t>& args,p_data_t&) {
	if(args.size()!=0) {
		return creater_t::creater().create_argument_number_error_data(
			"print-number-of-kankyo-and-data",0,args.size(),false);
	}
	creater_t::creater().print_number_of_data();
	return creater_t::creater().create_null_data();
}

// 環境とデータの自動削除の有効/無効を設定する
p_data_t set_enable_auto_delete(const std::vector<p_data_t>& args,p_data_t&) {
	if(args.size()!=1) {
		return creater_t::creater().create_argument_number_error_data(
			"set-enable-auto-delete",1,args.size(),false);
	}
	p_data_config_t::set_do_auto_delete(!(args[0]->type==DT_BOOLEAN && !args[0]->is_true));
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

// 環境に組み込み手続きを追加する
void add_kumikomi_tetuduki_to_kankyo(p_data_t& kankyo) {
	// 数値計算 (number_calc.cpp)
	kankyo->sokubaku["+"]=creater_t::creater().create_native_func_data(add);
	kankyo->sokubaku["-"]=creater_t::creater().create_native_func_data(sub);
	kankyo->sokubaku["*"]=creater_t::creater().create_native_func_data(mul);
	kankyo->sokubaku["/"]=creater_t::creater().create_native_func_data(div_func);
	kankyo->sokubaku["quotient"]=creater_t::creater().create_native_func_data(quotient);
	kankyo->sokubaku["="]=creater_t::creater().create_native_func_data(num_eq);
	kankyo->sokubaku["<"]=creater_t::creater().create_native_func_data(num_lt);
	kankyo->sokubaku["<="]=creater_t::creater().create_native_func_data(num_leq);
	kankyo->sokubaku[">"]=creater_t::creater().create_native_func_data(num_gt);
	kankyo->sokubaku[">="]=creater_t::creater().create_native_func_data(num_geq);
	kankyo->sokubaku["even?"]=creater_t::creater().create_native_func_data(is_even);
	kankyo->sokubaku["odd?"]=creater_t::creater().create_native_func_data(is_odd);
	kankyo->sokubaku["sqrt"]=creater_t::creater().create_native_func_data(sqrt_func);

	// リスト操作 (cons_and_list.cpp)
	kankyo->sokubaku["cons"]=creater_t::creater().create_native_func_data(cons);
	kankyo->sokubaku["car"]=creater_t::creater().create_native_func_data(car);
	kankyo->sokubaku["cdr"]=creater_t::creater().create_native_func_data(cdr);
	kankyo->sokubaku["set-car!"]=creater_t::creater().create_native_func_data(set_car);
	kankyo->sokubaku["set-cdr!"]=creater_t::creater().create_native_func_data(set_cdr);
	kankyo->sokubaku["null?"]=creater_t::creater().create_native_func_data(is_null);

	// その他の手続き
	kankyo->sokubaku["eq?"]=creater_t::creater().create_native_func_data(is_eq);
	kankyo->sokubaku["apply"]=creater_t::creater().create_native_func_data(apply);
	kankyo->sokubaku["exit"]=creater_t::creater().create_native_func_data(exit_func);
	kankyo->sokubaku["not"]=creater_t::creater().create_native_func_data(not_func);
	kankyo->sokubaku["set-enable-syouryaku"]=
		creater_t::creater().create_native_func_data(set_enable_syouryaku);
	kankyo->sokubaku["get-enable-syouryaku"]=
		creater_t::creater().create_native_func_data(get_enable_syouryaku);
	kankyo->sokubaku["print-number-of-data"]=
		creater_t::creater().create_native_func_data(print_number_of_data);
	kankyo->sokubaku["set-enable-auto-delete"]=
		creater_t::creater().create_native_func_data(set_enable_auto_delete);
	kankyo->sokubaku["get-enable-auto-delete"]=
		creater_t::creater().create_native_func_data(get_enable_auto_delete);
	kankyo->sokubaku["delete-zero-sansyo"]=
		creater_t::creater().create_native_func_data(delete_zero_sansyo);

	// 特殊形式 (special_form.cpp)
	kankyo->sokubaku["quote"]=creater_t::creater().create_native_func_data(quote_proc,true);
	kankyo->sokubaku["define"]=creater_t::creater().create_native_func_data(define_proc,true);
	kankyo->sokubaku["set!"]=creater_t::creater().create_native_func_data(set_proc,true);
	kankyo->sokubaku["lambda"]=creater_t::creater().create_native_func_data(lambda_proc,true);
	kankyo->sokubaku["if"]=creater_t::creater().create_native_func_data(if_proc,true);
	kankyo->sokubaku["and"]=creater_t::creater().create_native_func_data(and_proc,true);
	kankyo->sokubaku["or"]=creater_t::creater().create_native_func_data(or_proc,true);

	// その他
	kankyo->sokubaku["else"]=creater_t::creater().create_boolean_data(true);
}
