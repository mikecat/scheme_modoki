#include <cstdio>
#include <cmath>
#include "scheme_modoki_core.h"
#include "global_config.h"
#include "creater.h"
#include "kumikomi_tetuduki/number_calc.h"
#include "kumikomi_tetuduki/cons_and_list.h"
#include "kumikomi_tetuduki/special_form.h"
#include "kumikomi_tetuduki/system_control.h"

// データが等価か判定する
p_data_t is_eq(const std::vector<p_data_t>& args,p_data_t&) {
	if(args.size()!=2) {
		return creater_t::creater().create_argument_number_error(
			"eq?",2,args.size(),false);
	} else {
		if(args[0]->get_type()!=args[1]->get_type()) {
			return creater_t::creater().create_boolean(false);
		} else if(args[0]->get_type()==DT_EOF || args[0]->get_type()==DT_NULL) {
			return creater_t::creater().create_boolean(true);
		} else if(args[0]->get_type()==DT_NUMBER) {
			return creater_t::creater().create_boolean(
				((number_t*)&*args[0])->number==((number_t*)&*args[1])->number);
		} else if(args[0]->get_type()==DT_KIGOU) {
			return creater_t::creater().create_boolean(
				((kigou_t*)&*args[0])->kigou==((kigou_t*)&*args[1])->kigou);
		} else if(args[0]->get_type()==DT_BOOLEAN) {
			return creater_t::creater().create_boolean(
				((boolean_t*)&*args[0])->is_true==((boolean_t*)&*args[1])->is_true);
		} else {
			return creater_t::creater().create_boolean(args[0]==args[1]);
		}
	}
}

// 手続きを引数リストに適用する
p_data_t apply(const std::vector<p_data_t>& args,p_data_t& kankyo) {
	if(args.size()<2) {
		return creater_t::creater().create_argument_number_error(
			"apply",2,args.size(),true);
	} else {
		std::vector<p_data_t> args_list;
		for(std::vector<p_data_t>::const_iterator it=args.begin()+1;it+1!=args.end();it++) {
			args_list.push_back(*it);
		}
		if(args.back()->get_type()!=DT_CONS && args.back()->get_type()!=DT_NULL) {
			return creater_t::creater().create_error(
				"the last argument of apply must be a list");
		}
		for(p_data_t cur_data=args.back();
		cur_data->get_type()==DT_CONS;cur_data=((cons_t*)&*cur_data)->cons_cdr) {
			args_list.push_back(((cons_t*)&*cur_data)->cons_car);
		}
		return apply_proc(args[0],args_list,kankyo);
	}
}

// 終了する指示を出す
p_data_t exit_func(const std::vector<p_data_t>& args,p_data_t&) {
	if(args.size()==0) {
		return creater_t::creater().create_exit(0);
	} else if(args.size()==1) {
		if(args[0]->get_type()!=DT_NUMBER) {
			return creater_t::creater().create_error(
				"first argument of exit must be a number");
		}
		return creater_t::creater().create_exit((int)((number_t*)&*args[0])->number);
	} else {
		char buffer[16];
		sprintf(buffer,"%u",(unsigned int)args.size());
		return creater_t::creater().create_error(
			std::string("invalid number of arguments for exit : expected 0 or 1, got ")+buffer);
	}
}

// 引数が#f以外なら#fを、#fなら#tを返す
p_data_t not_func(const std::vector<p_data_t>& args,p_data_t&) {
	if(args.size()!=1) {
		return creater_t::creater().create_argument_number_error(
			"not",1,args.size(),false);
	} else {
		// args[0]が#fかを判定
		return creater_t::creater().create_boolean(
			args[0]->get_type()==DT_BOOLEAN && !((boolean_t*)&*args[0])->is_true);
	}
}

// データを読み込む
p_data_t read_func(const std::vector<p_data_t>& args,p_data_t&) {
	if(args.size()==0) {
		return parse(global_config::get_gc().get_stdin_reader());
	} else if(args.size()==1) {
		return creater_t::creater().create_error("port for read is not supported");
	} else {
		char buf[16];
		sprintf(buf,"%u",(unsigned int)args.size());
		return creater_t::creater().create_error(
			std::string("invalid number of argument for read : expected 0 or 1, got ")+buf);
	}
}

// データを出力する
p_data_t write_func(const std::vector<p_data_t>& args,p_data_t&) {
	if(args.size()==1) {
		print_data(args[0]);
		return creater_t::creater().create_null();
	} else if(args.size()==2) {
		return creater_t::creater().create_error("port for write is not supported");
	} else {
		char buf[16];
		sprintf(buf,"%u",(unsigned int)args.size());
		return creater_t::creater().create_error(
			std::string("invalid number of argument for write : expected 1 or 2, got ")+buf);
	}
}

// 環境に組み込み手続きを追加する
void add_kumikomi_tetuduki_to_kankyo(p_data_t& kankyo) {
	std::map<std::string, p_data_t>& sokubaku=((kankyo_t*)&*kankyo)->sokubaku;
	// 数値計算 (number_calc.cpp)
	sokubaku["+"]=creater_t::creater().create_native_func(add);
	sokubaku["-"]=creater_t::creater().create_native_func(sub);
	sokubaku["*"]=creater_t::creater().create_native_func(mul);
	sokubaku["/"]=creater_t::creater().create_native_func(div_func);
	sokubaku["quotient"]=creater_t::creater().create_native_func(quotient);
	sokubaku["="]=creater_t::creater().create_native_func(number_eq);
	sokubaku["<"]=creater_t::creater().create_native_func(number_lt);
	sokubaku["<="]=creater_t::creater().create_native_func(number_leq);
	sokubaku[">"]=creater_t::creater().create_native_func(number_gt);
	sokubaku[">="]=creater_t::creater().create_native_func(number_geq);
	sokubaku["even?"]=creater_t::creater().create_native_func(is_even);
	sokubaku["odd?"]=creater_t::creater().create_native_func(is_odd);
	sokubaku["sqrt"]=creater_t::creater().create_native_func(sqrt_func);

	// リスト操作 (cons_and_list.cpp)
	sokubaku["cons"]=creater_t::creater().create_native_func(cons);
	sokubaku["car"]=creater_t::creater().create_native_func(car);
	sokubaku["cdr"]=creater_t::creater().create_native_func(cdr);
	sokubaku["set-car!"]=creater_t::creater().create_native_func(set_car);
	sokubaku["set-cdr!"]=creater_t::creater().create_native_func(set_cdr);
	sokubaku["null?"]=creater_t::creater().create_native_func(is_null);
	sokubaku["list"]=creater_t::creater().create_native_func(list);
	sokubaku["length"]=creater_t::creater().create_native_func(length);

	// システム操作 (system_control.cpp)
	sokubaku["set-enable-syouryaku"]=
		creater_t::creater().create_native_func(set_enable_syouryaku);
	sokubaku["get-enable-syouryaku"]=
		creater_t::creater().create_native_func(get_enable_syouryaku);
	sokubaku["print-statistics"]=
		creater_t::creater().create_native_func(print_statistics);
	sokubaku["get-number-of-data"]=
		creater_t::creater().create_native_func(get_number_of_data);
	sokubaku["set-enable-auto-delete"]=
		creater_t::creater().create_native_func(set_enable_auto_delete);
	sokubaku["get-enable-auto-delete"]=
		creater_t::creater().create_native_func(get_enable_auto_delete);
	sokubaku["delete-zero-sansyo"]=
		creater_t::creater().create_native_func(delete_zero_sansyo);

	// その他の手続き
	sokubaku["eq?"]=creater_t::creater().create_native_func(is_eq);
	sokubaku["apply"]=creater_t::creater().create_native_func(apply);
	sokubaku["exit"]=creater_t::creater().create_native_func(exit_func);
	sokubaku["not"]=creater_t::creater().create_native_func(not_func);
	sokubaku["read"]=creater_t::creater().create_native_func(read_func);
	sokubaku["write"]=creater_t::creater().create_native_func(write_func);

	// 特殊形式 (special_form.cpp)
	sokubaku["quote"]=creater_t::creater().create_native_func(quote_proc,true);
	sokubaku["define"]=creater_t::creater().create_native_func(define_proc,true);
	sokubaku["set!"]=creater_t::creater().create_native_func(set_proc,true);
	sokubaku["lambda"]=creater_t::creater().create_native_func(lambda_proc,true);
	sokubaku["if"]=creater_t::creater().create_native_func(if_proc,true);
	sokubaku["and"]=creater_t::creater().create_native_func(and_proc,true);
	sokubaku["or"]=creater_t::creater().create_native_func(or_proc,true);
	sokubaku["cond"]=creater_t::creater().create_native_func(cond_proc,true);
	sokubaku["let"]=creater_t::creater().create_native_func(let_proc,true);
	sokubaku["let*"]=creater_t::creater().create_native_func(let_star_proc,true);
	sokubaku["begin"]=creater_t::creater().create_native_func(begin_proc,true);

	// その他
	sokubaku["else"]=creater_t::creater().create_boolean(true);
}
