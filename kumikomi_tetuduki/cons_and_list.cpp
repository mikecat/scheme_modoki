#include "cons_and_list.h"
#include "../creater.h"

// consセルを作成する
p_data_t cons(const std::vector<p_data_t>& args,p_data_t&) {
	if(args.size()!=2) {
		return creater_t::creater().create_argument_number_error(
			"cons",2,args.size(),false);
	} else {
		return creater_t::creater().create_cons(args[0],args[1]);
	}
}

// consセルの前の要素
p_data_t car(const std::vector<p_data_t>& args,p_data_t&) {
	if(args.size()!=1) {
		return creater_t::creater().create_argument_number_error(
			"car",1,args.size(),false);
	} else {
		if(args[0]->get_type()!=DT_CONS) {
			return creater_t::creater().create_error(
				"requested car of what is not a cons-cell");
		} else {
			return ((cons_t*)&*args[0])->cons_car;
		}
	}
}

// consセルの後ろの要素
p_data_t cdr(const std::vector<p_data_t>& args,p_data_t&) {
	if(args.size()!=1) {
		return creater_t::creater().create_argument_number_error(
			"cdr",1,args.size(),false);
	} else {
		if(args[0]->get_type()!=DT_CONS) {
			return creater_t::creater().create_error(
				"requested cdr of what is not a cons-cell");
		} else {
			return ((cons_t*)&*args[0])->cons_cdr;
		}
	}
}

// consセルの前の要素を設定する
p_data_t set_car(const std::vector<p_data_t>& args,p_data_t&) {
	if(args.size()!=2) {
		return creater_t::creater().create_argument_number_error(
			"set-car!",2,args.size(),false);
	} else if(args[0]->get_type()!=DT_CONS) {
		return creater_t::creater().create_error(
			"attempt to set car of what is not a cons-cell");
	} else {
		((cons_t*)&*args[0])->cons_car=args[1];
		return args[1];
	}
}

// consセルの後ろの要素を設定する
p_data_t set_cdr(const std::vector<p_data_t>& args,p_data_t&) {
	if(args.size()!=2) {
		return creater_t::creater().create_argument_number_error(
			"set-cdr!",2,args.size(),false);
	} else if(args[0]->get_type()!=DT_CONS) {
		return creater_t::creater().create_error(
			"attempt to set cdr of what is not a cons-cell");
	} else {
		((cons_t*)&*args[0])->cons_cdr=args[1];
		return args[1];
	}
}

// 引数が'()かを判定する
p_data_t is_null(const std::vector<p_data_t>& args,p_data_t&) {
	if(args.size()!=1) {
		return creater_t::creater().create_argument_number_error(
			"null?",1,args.size(),false);
	} else {
		return creater_t::creater().create_boolean(args[0]->get_type()==DT_NULL);
	}
}

// 引数のリストを作成する
p_data_t list(const std::vector<p_data_t>& args,p_data_t&) {
	p_data_t cur=creater_t::creater().create_null();
	for(std::vector<p_data_t>::const_reverse_iterator it=args.rbegin();it!=args.rend();it++) {
		cur=creater_t::creater().create_cons(*it,cur);
	}
	return cur;
}

// リストの長さを取得する
p_data_t length(const std::vector<p_data_t>& args,p_data_t&) {
	if(args.size()!=1) {
		return creater_t::creater().create_argument_number_error(
			"length",1,args.size(),false);
	} else {
		unsigned int count=0;
		p_data_t cur;
		for(cur=args[0];cur->get_type()==DT_CONS;cur=((cons_t*)&*cur)->cons_cdr)count++;
		if(cur->get_type()!=DT_NULL) {
			return creater_t::creater().create_error("the first argument of length must be a list");
		} else {
			return creater_t::creater().create_number(count);
		}
	}
}
