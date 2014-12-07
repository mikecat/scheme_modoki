#include <cstdio>
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

// 連番リストを作成する
p_data_t iota(const std::vector<p_data_t>& args,p_data_t&) {
	if(args.size()<1 || 3<args.size()) {
		char buf[16];
		sprintf(buf,"%u",(unsigned int)args.size());
		return creater_t::creater().create_error(
			std::string("invalid number of arguments for iota : expected 1, 2 or 3, got ")+buf);
	} else {
		unsigned int count=0;
		double start=0,delta=1;
		bool is_error=false;
		if(args[0]->get_type()!=DT_NUMBER) {
			is_error=true;
		} else {
			count=(unsigned int)((number_t*)&*args[0])->number;
		}
		if(args.size()>=2) {
			if(args[1]->get_type()!=DT_NUMBER) {
				is_error=true;
			} else {
				start=((number_t*)&*args[1])->number;
			}
		}
		if(args.size()>=3) {
			if(args[2]->get_type()!=DT_NUMBER) {
				is_error=true;
			} else {
				delta=((number_t*)&*args[2])->number;
			}
		}
		if(is_error) {
			return creater_t::creater().create_error("arguments of iota must be numbers");
		}
		p_data_t ret=creater_t::creater().create_null();
		for(unsigned int i=count;i>0;i--) {
			ret=creater_t::creater().create_cons(
				creater_t::creater().create_number(start+delta*(i-1)),ret);
		}
		return ret;
	}
}
