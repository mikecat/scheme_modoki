#include <cmath>
#include "number_calc.h"
#include "../creater.h"

// 足し算 例: (+ 1 2) => 3
p_data_t add(const std::vector<p_data_t>& args,p_data_t&) {
	double ret=0;
	for(std::vector<p_data_t>::const_iterator it=args.begin();it!=args.end();it++) {
		if((*it)->get_type()!=DT_NUMBER) {
			return creater_t::creater().create_error_data(
				"attempt to add what is not a number");
		}
		ret+=((number_t*)&*(*it))->number;
	}
	return creater_t::creater().create_number_data(ret);
}

// 引き算 例: (- 1 2) => -1
p_data_t sub(const std::vector<p_data_t>& args,p_data_t&) {
	if(args.size()==0) {
		return creater_t::creater().create_argument_number_error_data(
			"sub",1,args.size(),true);
	} else if(args.size()==1) {
		if(args[0]->get_type()==DT_NUMBER) {
			return creater_t::creater().create_number_data(-((number_t*)&*args[0])->number);
		} else {
			return creater_t::creater().create_error_data(
				"attempt to negate what is not a number");
		}
	} else {
		if(args[0]->get_type()!=DT_NUMBER) {
			return creater_t::creater().create_error_data(
				"attempt to subtract what is not a number");
		} else {
			double ret=((number_t*)&*args[0])->number;
			for(std::vector<p_data_t>::const_iterator it=args.begin()+1;it!=args.end();it++) {
				if((*it)->get_type()!=DT_NUMBER) {
					return creater_t::creater().create_error_data(
						"attempt to subtract what is not a number");
				}
				ret-=((number_t*)&*(*it))->number;
			}
			return creater_t::creater().create_number_data(ret);
		}
	}
}

// 掛け算 例: (* 2 3) => 6
p_data_t mul(const std::vector<p_data_t>& args,p_data_t&) {
	double ret=1;
	for(std::vector<p_data_t>::const_iterator it=args.begin();it!=args.end();it++) {
		if((*it)->get_type()!=DT_NUMBER) {
			return creater_t::creater().create_error_data(
				"attempt to add what is not a number");
		}
		ret*=((number_t*)&*(*it))->number;
	}
	return creater_t::creater().create_number_data(ret);
}

// 割り算 例: (/ 4 2) => 2
p_data_t div_func(const std::vector<p_data_t>& args,p_data_t&) {
	if(args.size()==0) {
		return creater_t::creater().create_argument_number_error_data(
			"div",1,args.size(),true);
	} else {
		if(args[0]->get_type()!=DT_NUMBER) {
			return creater_t::creater().create_error_data(
				"attempt to divide what is not a number");
		} else {
			double ret=((number_t*)&*args[0])->number;
			for(std::vector<p_data_t>::const_iterator it=args.begin()+1;it!=args.end();it++) {
				if((*it)->get_type()!=DT_NUMBER) {
					return creater_t::creater().create_error_data(
						"attempt to divide what is not a number");
				}
				ret/=((number_t*)&*(*it))->number;
			}
			return creater_t::creater().create_number_data(ret);
		}
	}
}

// 商の整数部分
p_data_t quotient(const std::vector<p_data_t>& args,p_data_t&) {
	if(args.size()!=2) {
		return creater_t::creater().create_argument_number_error_data(
			"quotient",2,args.size(),false);
	} else if(args[0]->get_type()!=DT_NUMBER || args[1]->get_type()!=DT_NUMBER) {
		return creater_t::creater().create_error_data(
			"attempt to calculate quotient of what is not a number");
	} else {
		return creater_t::creater().create_number_data(
			trunc(((number_t*)&*args[0])->number/((number_t*)&*args[1])->number));
	}
}

// 比較共通関数
static p_data_t number_cmp(const std::vector<p_data_t>& args,
bool (*cmp_func)(double,double),const std::string& name) {
	if(args.size()<2) {
		return creater_t::creater().create_argument_number_error_data(
			name,2,args.size(),true);
	} else {
		bool ok=true;
		for(size_t i=0;i<args.size();i++) {
			if(args[i]->get_type()!=DT_NUMBER) {
				return creater_t::creater().create_error_data(
					std::string("arguments for ")+name+" must be numbers");
			}
			if(i>0 && !cmp_func(((number_t*)&*args[i-1])->number,((number_t*)&*args[i])->number)) {
				ok=false;
			}
		}
		return creater_t::creater().create_boolean_data(ok);
	}
}

// 比較「同じ」
static bool cmp_eq(double a,double b){return a==b;}
p_data_t number_eq(const std::vector<p_data_t>& args,p_data_t&) {
	return number_cmp(args,cmp_eq,"=");
}

// 比較「小さい」
static bool cmp_lt(double a,double b){return a<b;}
p_data_t number_lt(const std::vector<p_data_t>& args,p_data_t&) {
	return number_cmp(args,cmp_lt,"<");
}

// 比較「以下」
static bool cmp_leq(double a,double b){return a<=b;}
p_data_t number_leq(const std::vector<p_data_t>& args,p_data_t&) {
	return number_cmp(args,cmp_leq,"<=");
}

// 比較「大きい」
static bool cmp_gt(double a,double b){return a>b;}
p_data_t number_gt(const std::vector<p_data_t>& args,p_data_t&) {
	return number_cmp(args,cmp_gt,">");
}

// 比較「以上」
static bool cmp_geq(double a,double b){return a>=b;}
p_data_t number_geq(const std::vector<p_data_t>& args,p_data_t&) {
	return number_cmp(args,cmp_geq,">=");
}

// 引数が偶数なら#tを、偶数以外なら#fを返す
p_data_t is_even(const std::vector<p_data_t>& args,p_data_t&) {
	if(args.size()!=1) {
		return creater_t::creater().create_argument_number_error_data(
			"even?",1,args.size(),false);
	} else {
		return creater_t::creater().create_boolean_data(
			args[0]->get_type()==DT_NUMBER &&
			((number_t*)&*args[0])->number==floor(((number_t*)&*args[0])->number) &&
			(int)((number_t*)&*args[0])->number%2==0
		);
	}
}

// 引数が奇数なら#tを、奇数以外なら#fを返す
p_data_t is_odd(const std::vector<p_data_t>& args,p_data_t&) {
	if(args.size()!=1) {
		return creater_t::creater().create_argument_number_error_data(
			"odd?",1,args.size(),false);
	} else {
		return creater_t::creater().create_boolean_data(
			args[0]->get_type()==DT_NUMBER &&
			((number_t*)&*args[0])->number==floor(((number_t*)&*args[0])->number) &&
			(int)((number_t*)&*args[0])->number%2!=0
		);
	}
}

// 引数の平方根を返す
p_data_t sqrt_func(const std::vector<p_data_t>& args,p_data_t&) {
	if(args.size()!=1) {
		return creater_t::creater().create_argument_number_error_data(
			"sqrt",1,args.size(),false);
	} else if(args[0]->get_type()!=DT_NUMBER) {
		return creater_t::creater().create_error_data(
			"attempt to calculate square root of what is not a number");
	} else if(((number_t*)&*args[0])->number<0.0) {
		return creater_t::creater().create_error_data(
			"sqrt of negative value is not supported");
	} else {
		return creater_t::creater().create_number_data(sqrt(((number_t*)&*args[0])->number));
	}
}
