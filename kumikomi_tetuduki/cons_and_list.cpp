#include "cons_and_list.h"
#include "../creater.h"

// consセルを作成する
p_data_t cons(const std::vector<p_data_t>& args,p_kankyo_t&) {
	if(args.size()!=2) {
		return creater_t::creater().create_argument_number_error_data(
			"cons",2,args.size(),false);
	} else {
		return creater_t::creater().create_cons_data(args[0],args[1]);
	}
}

// consセルの前の要素
p_data_t car(const std::vector<p_data_t>& args,p_kankyo_t&) {
	if(args.size()!=1) {
		return creater_t::creater().create_argument_number_error_data(
			"car",1,args.size(),false);
	} else {
		if(args[0]->type!=DT_CONS) {
			return creater_t::creater().create_error_data(
				"requested car of what is not a cons-cell");
		} else {
			return args[0]->cons_car;
		}
	}
}

// consセルの後ろの要素
p_data_t cdr(const std::vector<p_data_t>& args,p_kankyo_t&) {
	if(args.size()!=1) {
		return creater_t::creater().create_argument_number_error_data(
			"cdr",1,args.size(),false);
	} else {
		if(args[0]->type!=DT_CONS) {
			return creater_t::creater().create_error_data(
				"requested cdr of what is not a cons-cell");
		} else {
			return args[0]->cons_cdr;
		}
	}
}

// consセルの前の要素を設定する
p_data_t set_car(const std::vector<p_data_t>& args,p_kankyo_t&) {
	if(args.size()!=2) {
		return creater_t::creater().create_argument_number_error_data(
			"set-car!",2,args.size(),false);
	} else if(args[0]->type!=DT_CONS) {
		return creater_t::creater().create_error_data(
			"attempt to set car of what is not a cons-cell");
	} else {
		args[0]->cons_car=args[1];
		return args[1];
	}
}

// consセルの後ろの要素を設定する
p_data_t set_cdr(const std::vector<p_data_t>& args,p_kankyo_t&) {
	if(args.size()!=2) {
		return creater_t::creater().create_argument_number_error_data(
			"set-cdr!",2,args.size(),false);
	} else if(args[0]->type!=DT_CONS) {
		return creater_t::creater().create_error_data(
			"attempt to set cdr of what is not a cons-cell");
	} else {
		args[0]->cons_cdr=args[1];
		return args[1];
	}
}

// 引数が'()かを判定する
p_data_t is_null(const std::vector<p_data_t>& args,p_kankyo_t&) {
	if(args.size()!=1) {
		return creater_t::creater().create_argument_number_error_data(
			"null?",1,args.size(),false);
	} else {
		return creater_t::creater().create_boolean_data(args[0]->type==DT_NULL);
	}
}
