#include <cstdio>
#include <cmath>
#include "kumikomi_tetuduki.h"
#include "creater.h"
#include "kumikomi_tetuduki/number_calc.h"
#include "kumikomi_tetuduki/cons_and_list.h"
#include "kumikomi_tetuduki/special_form.h"

// 手続きを引数リストに適用する
data_t* apply(const std::vector<data_t*>& args,kankyo_t* kankyo) {
	if(args.size()<2) {
		return creater_t::creater().create_argument_number_error_data(
			"apply",2,args.size(),true);
	} else {
		std::vector<data_t*> args_list;
		for(std::vector<data_t*>::const_iterator it=args.begin()+1;it+1!=args.end();it++) {
			args_list.push_back(*it);
		}
		if(args.back()->type!=DT_CONS && args.back()->type!=DT_NULL) {
			return creater_t::creater().create_error_data(
				"the last argument of apply must be a list");
		}
		for(data_t* cur_data=args.back();cur_data->type==DT_CONS;cur_data=cur_data->cons_cdr) {
			args_list.push_back(cur_data->cons_car);
		}
		return tekiyou(args[0],args_list,kankyo);
	}
}

// 終了する指示を出す
data_t* exit_func(const std::vector<data_t*>&,kankyo_t*) {
	return creater_t::creater().create_error_data("",true);
}

// 引数が#f以外なら#fを、#fなら#tを返す
data_t* not_func(const std::vector<data_t*>& args,kankyo_t*) {
	if(args.size()!=1) {
		return creater_t::creater().create_argument_number_error_data(
			"not",1,args.size(),false);
	} else {
		return creater_t::creater().create_boolean_data(
			args[0]->type==DT_BOOLEAN && !args[0]->is_true); // args[0]が#fかを判定
	}
}

// 環境に組み込み手続きを追加する
void add_kumikomi_tetuduki_to_kankyo(kankyo_t* kankyo) {
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

	kankyo->sokubaku["apply"]=creater_t::creater().create_native_func_data(apply);
	kankyo->sokubaku["exit"]=creater_t::creater().create_native_func_data(exit_func);
	kankyo->sokubaku["not"]=creater_t::creater().create_native_func_data(not_func);
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
