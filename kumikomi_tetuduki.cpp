#include <cstdio>
#include <cmath>
#include "kumikomi_tetuduki.h"
#include "creater.h"

// 足し算 例: (+ 1 2) => 3
data_t* add(const std::vector<data_t*>& args,kankyo_t*) {
	double ret=0;
	for(std::vector<data_t*>::const_iterator it=args.begin();it!=args.end();it++) {
		if((*it)->type!=DT_NUM) {
			return creater_t::creater().create_error_data(
				"attempt to add what is not a number");
		}
		ret+=(*it)->num;
	}
	return creater_t::creater().create_num_data(ret);
}

// 引き算 例: (- 1 2) => -1
data_t* sub(const std::vector<data_t*>& args,kankyo_t*) {
	if(args.size()==0) {
		return creater_t::creater().create_argument_number_error_data(
			"sub",1,args.size(),true);
	} else if(args.size()==1) {
		if(args[0]->type==DT_NUM) {
			return creater_t::creater().create_num_data(-args[0]->num);
		} else {
			return creater_t::creater().create_error_data(
				"attempt to negate what is not a number");
		}
	} else {
		if(args[0]->type!=DT_NUM) {
			return creater_t::creater().create_error_data(
				"attempt to subtract what is not a number");
		} else {
			double ret=args[0]->num;
			for(std::vector<data_t*>::const_iterator it=args.begin()+1;it!=args.end();it++) {
				if((*it)->type!=DT_NUM) {
					return creater_t::creater().create_error_data(
						"attempt to subtract what is not a number");
				}
				ret-=(*it)->num;
			}
			return creater_t::creater().create_num_data(ret);
		}
	}
}

// 掛け算 例: (* 2 3) => 6
data_t* mul(const std::vector<data_t*>& args,kankyo_t*) {
	double ret=1;
	for(std::vector<data_t*>::const_iterator it=args.begin();it!=args.end();it++) {
		if((*it)->type!=DT_NUM) {
			return creater_t::creater().create_error_data(
				"attempt to add what is not a number");
		}
		ret*=(*it)->num;
	}
	return creater_t::creater().create_num_data(ret);
}

// 割り算 例: (/ 4 2) => 2
data_t* div_func(const std::vector<data_t*>& args,kankyo_t*) {
	if(args.size()==0) {
		return creater_t::creater().create_argument_number_error_data(
			"div",1,args.size(),true);
	} else {
		if(args[0]->type!=DT_NUM) {
			return creater_t::creater().create_error_data(
				"attempt to divide what is not a number");
		} else {
			double ret=args[0]->num;
			for(std::vector<data_t*>::const_iterator it=args.begin()+1;it!=args.end();it++) {
				if((*it)->type!=DT_NUM) {
					return creater_t::creater().create_error_data(
						"attempt to divide what is not a number");
				}
				ret/=(*it)->num;
			}
			return creater_t::creater().create_num_data(ret);
		}
	}
}

// 比較共通関数
data_t* num_cmp(const std::vector<data_t*>& args,
bool (*cmp_func)(double,double),const std::string& name) {
	if(args.size()<2) {
		return creater_t::creater().create_argument_number_error_data(
			name,2,args.size(),true);
	} else {
		bool ok=true;
		for(size_t i=0;i<args.size();i++) {
			if(args[i]->type!=DT_NUM) {
				return creater_t::creater().create_error_data(
					std::string("arguments for ")+name+" must be numbers");
			}
			if(i>0 && !cmp_func(args[i-1]->num,args[i]->num))ok=false;
		}
		return creater_t::creater().create_boolean_data(ok);
	}
}

// 比較「同じ」
bool cmp_eq(double a,double b){return a==b;}
data_t* num_eq(const std::vector<data_t*>& args,kankyo_t*) {
	return num_cmp(args,cmp_eq,"=");
}

// 比較「小さい」
bool cmp_lt(double a,double b){return a<b;}
data_t* num_lt(const std::vector<data_t*>& args,kankyo_t*) {
	return num_cmp(args,cmp_lt,"<");
}

// 比較「以下」
bool cmp_leq(double a,double b){return a<=b;}
data_t* num_leq(const std::vector<data_t*>& args,kankyo_t*) {
	return num_cmp(args,cmp_leq,"<=");
}

// 比較「大きい」
bool cmp_gt(double a,double b){return a>b;}
data_t* num_gt(const std::vector<data_t*>& args,kankyo_t*) {
	return num_cmp(args,cmp_gt,">");
}

// 比較「以上」
bool cmp_geq(double a,double b){return a>=b;}
data_t* num_geq(const std::vector<data_t*>& args,kankyo_t*) {
	return num_cmp(args,cmp_geq,">=");
}

// consセルを作成する
data_t* cons(const std::vector<data_t*>& args,kankyo_t*) {
	if(args.size()!=2) {
		return creater_t::creater().create_argument_number_error_data(
			"cons",2,args.size(),false);
	} else {
		return creater_t::creater().create_cons_data(args[0],args[1]);
	}
}

// consセルの前の要素
data_t* car(const std::vector<data_t*>& args,kankyo_t*) {
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
data_t* cdr(const std::vector<data_t*>& args,kankyo_t*) {
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
data_t* set_car(const std::vector<data_t*>& args,kankyo_t*) {
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
data_t* set_cdr(const std::vector<data_t*>& args,kankyo_t*) {
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
data_t* is_null(const std::vector<data_t*>& args,kankyo_t*) {
	if(args.size()!=1) {
		return creater_t::creater().create_argument_number_error_data(
			"null?",1,args.size(),false);
	} else {
		return creater_t::creater().create_boolean_data(args[0]->type==DT_NULL);
	}
}

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

// 引数が偶数なら#tを、偶数以外なら#fを返す
data_t* is_even(const std::vector<data_t*>& args,kankyo_t*) {
	if(args.size()!=1) {
		return creater_t::creater().create_argument_number_error_data(
			"even?",1,args.size(),false);
	} else {
		return creater_t::creater().create_boolean_data(
			args[0]->type==DT_NUM && args[0]->num==floor(args[0]->num) &&
			(int)args[0]->num%2==0
		);
	}
}

// 引数が奇数なら#tを、奇数以外なら#fを返す
data_t* is_odd(const std::vector<data_t*>& args,kankyo_t*) {
	if(args.size()!=1) {
		return creater_t::creater().create_argument_number_error_data(
			"odd?",1,args.size(),false);
	} else {
		return creater_t::creater().create_boolean_data(
			args[0]->type==DT_NUM && args[0]->num==floor(args[0]->num) &&
			(int)args[0]->num%2!=0
		);
	}
}

// 引数(未評価)を返す
data_t* quote_proc(const std::vector<data_t*>& args,kankyo_t*) {
	if(args.size()!=1) {
		return creater_t::creater().create_argument_number_error_data(
			"quote",1,args.size(),false);
	} else {
		return args[0];
	}
}

// 新規束縛の作成
data_t* define_proc(const std::vector<data_t*>& args,kankyo_t* kankyo) {
	if(args.size()!=2) {
		return creater_t::creater().create_argument_number_error_data(
			"define",2,args.size(),false);
	} else if(args[0]->type!=DT_KIGOU) {
		return creater_t::creater().create_error_data(
			"you must specify kigou for first argument of define");
	} else {
		data_t* ret_data=hyouka_data(args[1],kankyo);
		if(ret_data->type==DT_ERROR)return ret_data;
		kankyo->sokubaku[args[0]->kigou]=ret_data;
		return creater_t::creater().create_kigou_data(args[0]->kigou);
	}
}

// 代入
data_t* set_proc(const std::vector<data_t*>& args,kankyo_t* kankyo) {
	if(args.size()!=2) {
		return creater_t::creater().create_argument_number_error_data(
			"set!",2,args.size(),false);
	} else if(args[0]->type!=DT_KIGOU) {
		return creater_t::creater().create_error_data(
			"you must specify kigou for first argument of set!");
	} else {
		data_t** zittai=namae_no_kisoku2(args[0]->kigou,kankyo);
		if(zittai==NULL)return namae_no_kisoku(args[0]->kigou,kankyo);
		data_t* value=hyouka_data(args[1],kankyo);
		if(value->type==DT_ERROR)return value;
		*zittai=value;
		return value;
	}
}

// 新規手続きの作成
data_t* lambda_proc(const std::vector<data_t*>& args,kankyo_t* kankyo) {
	if(args.size()<2) {
		return creater_t::creater().create_argument_number_error_data(
			"lambda",2,args.size(),true);
	} else {
		std::vector<std::string> karihikisu_list;
		std::vector<data_t*> hontai_list;
		bool is_kahencho=false;
		bool karihikisu_valid_flag=false;
		// 仮引数リストの取得
		if(args[0]->type==DT_KIGOU || args[0]->type==DT_CONS || args[0]->type==DT_NULL) {
			data_t* cur_karihikisu=args[0];
			for(;;) {
				if(cur_karihikisu->type==DT_KIGOU) {
					karihikisu_list.push_back(cur_karihikisu->kigou);
					is_kahencho=true;
					karihikisu_valid_flag=true;
					break;
				} else if(cur_karihikisu->type==DT_NULL) {
					karihikisu_valid_flag=true;
					break;
				} else if(cur_karihikisu->type==DT_CONS) {
					if(cur_karihikisu->cons_car->type==DT_KIGOU) {
						karihikisu_list.push_back(cur_karihikisu->cons_car->kigou);
						cur_karihikisu=cur_karihikisu->cons_cdr;
					} else {
						// 不正
						break;
					}
				} else {
					// 不正
					break;
				}
			}
		}
		if(!karihikisu_valid_flag) {
			return creater_t::creater().create_error_data("invalid karihikisu for lambda");
		}
		// 本体の格納
		for(std::vector<data_t*>::const_iterator it=args.begin()+1;it!=args.end();it++) {
			hontai_list.push_back(*it);
		}
		return creater_t::creater().create_lambda_data(
			karihikisu_list,hontai_list,is_kahencho,kankyo);
	}
}

// 条件分岐
data_t* if_proc(const std::vector<data_t*>& args,kankyo_t* kankyo) {
	if(args.size()!=2 && args.size()!=3) {
		char buf[16];
		sprintf(buf,"%u",(unsigned int)args.size());
		return creater_t::creater().create_error_data(
			std::string("invalid number of arguments for if : expected 2 or 3, got ")+buf);
	} else {
		data_t* sinriti=hyouka_data(args[0],kankyo);
		if(sinriti->type==DT_ERROR)return sinriti;
		if(sinriti->type==DT_BOOLEAN && !sinriti->is_true) {
			return args.size()>=3?args[2]:creater_t::creater().create_null_data();
		} else {
			return hyouka_data(args[1],kankyo);
		}
	}
}

// 途中に1個でも#fがあれば#f、無ければ最後の値を返す
data_t* and_proc(const std::vector<data_t*>& args,kankyo_t* kankyo) {
	if(args.size()==0) {
		return creater_t::creater().create_boolean_data(true);
	} else {
		data_t* last_data=NULL;
		for(std::vector<data_t*>::const_iterator it=args.begin();it!=args.end();it++) {
			data_t* cur_data=hyouka_data(*it,kankyo);
			if(cur_data->type==DT_ERROR) {
				return cur_data;
			} else if(cur_data->type==DT_BOOLEAN && !cur_data->is_true) {
				return cur_data;
			}
			last_data=cur_data;
		}
		return last_data;
	}
}

// 途中に1個でも#f以外があればその値、無ければ#fを返す
data_t* or_proc(const std::vector<data_t*>& args,kankyo_t* kankyo) {
	for(std::vector<data_t*>::const_iterator it=args.begin();it!=args.end();it++) {
		data_t* cur_data=hyouka_data(*it,kankyo);
		if(cur_data->type==DT_ERROR) {
			return cur_data;
		} else if(cur_data->type!=DT_BOOLEAN || cur_data->is_true) {
			return cur_data;
		}
	}
	return creater_t::creater().create_boolean_data(false);
}

// 環境に組み込み手続きを追加する
void add_kumikomi_tetuduki_to_kankyo(kankyo_t* kankyo) {
	// 手続き
	kankyo->sokubaku["+"]=creater_t::creater().create_native_func_data(add);
	kankyo->sokubaku["-"]=creater_t::creater().create_native_func_data(sub);
	kankyo->sokubaku["*"]=creater_t::creater().create_native_func_data(mul);
	kankyo->sokubaku["/"]=creater_t::creater().create_native_func_data(div_func);
	kankyo->sokubaku["="]=creater_t::creater().create_native_func_data(num_eq);
	kankyo->sokubaku["<"]=creater_t::creater().create_native_func_data(num_lt);
	kankyo->sokubaku["<="]=creater_t::creater().create_native_func_data(num_leq);
	kankyo->sokubaku[">"]=creater_t::creater().create_native_func_data(num_gt);
	kankyo->sokubaku[">="]=creater_t::creater().create_native_func_data(num_geq);
	kankyo->sokubaku["cons"]=creater_t::creater().create_native_func_data(cons);
	kankyo->sokubaku["car"]=creater_t::creater().create_native_func_data(car);
	kankyo->sokubaku["cdr"]=creater_t::creater().create_native_func_data(cdr);
	kankyo->sokubaku["set-car!"]=creater_t::creater().create_native_func_data(set_car);
	kankyo->sokubaku["set-cdr!"]=creater_t::creater().create_native_func_data(set_cdr);
	kankyo->sokubaku["null?"]=creater_t::creater().create_native_func_data(is_null);
	kankyo->sokubaku["apply"]=creater_t::creater().create_native_func_data(apply);
	kankyo->sokubaku["exit"]=creater_t::creater().create_native_func_data(exit_func);
	kankyo->sokubaku["not"]=creater_t::creater().create_native_func_data(not_func);
	kankyo->sokubaku["even?"]=creater_t::creater().create_native_func_data(is_even);
	kankyo->sokubaku["odd?"]=creater_t::creater().create_native_func_data(is_odd);
	// 特殊形式
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
