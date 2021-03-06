#include <cstdio>
#include "special_form.h"
#include "../creater.h"
#include "../scheme_modoki_core.h"

// 引数(未評価)を返す
p_data_t quote_proc(const std::vector<p_data_t>& args,p_data_t&,p_data_t&) {
	if(args.size()!=1) {
		return creater_t::creater().create_argument_number_error(
			"quote",1,args.size(),false);
	} else {
		return args[0];
	}
}

// 新規束縛の作成
p_data_t define_proc(const std::vector<p_data_t>& args,p_data_t& kankyo,p_data_t& cont) {
	if(args.size()<2) {
		return creater_t::creater().create_argument_number_error(
			"define",2,args.size(),true);
	} else if(args[0]->get_type()==DT_CONS) {
		p_data_t name=((cons_t*)&*args[0])->cons_car;
		if(name->get_type()!=DT_KIGOU) {
			return creater_t::creater().create_error(
				"you must specify kigou for first argument of define");
		} else {
			std::vector<p_data_t> lambda_args=args;
			lambda_args[0]=((cons_t*)&*args[0])->cons_cdr;
			p_data_t lambda_data=lambda_proc(lambda_args,kankyo,cont);
			((kankyo_t*)&*kankyo)->sokubaku[((kigou_t*)&*name)->kigou]=lambda_data;
			return creater_t::creater().create_kigou(((kigou_t*)&*name)->kigou);
		}
	} else if(args[0]->get_type()==DT_KIGOU) {
		if(args.size()!=2) {
			return creater_t::creater().create_argument_number_error(
				"define",2,args.size(),false);
		}
		std::vector<p_data_t> evaluated;
		std::vector<p_data_t> to_evaluate;
		evaluated.push_back(creater_t::creater().create_native_func(define_proc,true));
		evaluated.push_back(args[0]);
		p_data_t ret_data=evaluate(args[1],kankyo,creater_t::creater().create_continuation(
			cont,true,kankyo,evaluated,to_evaluate));
		if(ret_data->force_return_flag)return ret_data;
		((kankyo_t*)&*kankyo)->sokubaku[((kigou_t*)&*args[0])->kigou]=ret_data;
		return creater_t::creater().create_kigou(((kigou_t*)&*args[0])->kigou);
	} else {
		return creater_t::creater().create_error(
			"you must specify kigou for first argument of define");
	}
}

// 代入
p_data_t set_proc(const std::vector<p_data_t>& args,p_data_t& kankyo,p_data_t& cont) {
	if(args.size()!=2) {
		return creater_t::creater().create_argument_number_error(
			"set!",2,args.size(),false);
	} else if(args[0]->get_type()!=DT_KIGOU) {
		return creater_t::creater().create_error(
			"you must specify kigou for first argument of set!");
	} else {
		p_data_t* zittai=namae_no_kisoku2(((kigou_t*)&*args[0])->kigou,kankyo);
		if(zittai==NULL)return namae_no_kisoku(((kigou_t*)&*args[0])->kigou,kankyo);
		std::vector<p_data_t> evaluated;
		std::vector<p_data_t> to_evaluate;
		evaluated.push_back(creater_t::creater().create_native_func(set_proc,true));
		evaluated.push_back(args[0]);
		p_data_t value=evaluate(args[1],kankyo,creater_t::creater().create_continuation(
			cont,true,kankyo,evaluated,to_evaluate));
		if(value->force_return_flag)return value;
		*zittai=value;
		return value;
	}
}

// 新規手続きの作成
p_data_t lambda_proc(const std::vector<p_data_t>& args,p_data_t& kankyo,p_data_t&) {
	if(args.size()<2) {
		return creater_t::creater().create_argument_number_error(
			"lambda",2,args.size(),true);
	} else {
		std::vector<std::string> karihikisu_list;
		std::vector<p_data_t> hontai_list;
		bool is_kahencho=false;
		bool karihikisu_valid_flag=false;
		// 仮引数リストの取得
		if(args[0]->get_type()==DT_KIGOU ||
		args[0]->get_type()==DT_CONS || args[0]->get_type()==DT_NULL) {
			p_data_t cur_karihikisu=args[0];
			for(;;) {
				if(cur_karihikisu->get_type()==DT_KIGOU) {
					karihikisu_list.push_back(((kigou_t*)&*cur_karihikisu)->kigou);
					is_kahencho=true;
					karihikisu_valid_flag=true;
					break;
				} else if(cur_karihikisu->get_type()==DT_NULL) {
					karihikisu_valid_flag=true;
					break;
				} else if(cur_karihikisu->get_type()==DT_CONS) {
					if(((cons_t*)&*cur_karihikisu)->cons_car->get_type()==DT_KIGOU) {
						karihikisu_list.push_back(
							((kigou_t*)&*((cons_t*)&*cur_karihikisu)->cons_car)->kigou);
						cur_karihikisu=((cons_t*)&*cur_karihikisu)->cons_cdr;
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
			return creater_t::creater().create_error("invalid karihikisu for lambda");
		}
		// 本体の格納
		for(std::vector<p_data_t>::const_iterator it=args.begin()+1;it!=args.end();it++) {
			hontai_list.push_back(*it);
		}
		return creater_t::creater().create_lambda(
			karihikisu_list,hontai_list,is_kahencho,kankyo);
	}
}

// 条件分岐
p_data_t if_proc(const std::vector<p_data_t>& args,p_data_t& kankyo,p_data_t& cont) {
	if(args.size()!=2 && args.size()!=3) {
		char buf[16];
		sprintf(buf,"%u",(unsigned int)args.size());
		return creater_t::creater().create_error(
			std::string("invalid number of arguments for if : expected 2 or 3, got ")+buf);
	} else {
		std::vector<p_data_t> evaluated;
		std::vector<p_data_t> to_evaluate;
		evaluated.push_back(creater_t::creater().create_native_func(if_proc,true));
		to_evaluate.push_back(args[1]);
		if(args.size()>=3)to_evaluate.push_back(args[2]);
		p_data_t sinriti=evaluate(args[0],kankyo,creater_t::creater().create_continuation(
			cont,true,kankyo,evaluated,to_evaluate));
		if(sinriti->force_return_flag)return sinriti;
		if(sinriti->get_type()==DT_BOOLEAN && !((boolean_t*)&*sinriti)->is_true) {
			if(args.size()>=3) {
				return evaluate(args[2],kankyo,cont);
			} else {
				return creater_t::creater().create_null();
			}
		} else {
			return evaluate(args[1],kankyo,cont);
		}
	}
}

// 途中に1個でも#fがあれば#f、無ければ最後の値を返す
p_data_t and_proc(const std::vector<p_data_t>& args,p_data_t& kankyo,p_data_t& cont) {
	if(args.size()==0) {
		return creater_t::creater().create_boolean(true);
	} else {
		std::vector<p_data_t> evaluated;
		std::vector<p_data_t> to_evaluate=args;
		evaluated.push_back(creater_t::creater().create_native_func(and_proc,true));
		p_data_t last_data=NULL;
		while(!to_evaluate.empty()) {
			p_data_t cur_expr=*to_evaluate.begin();
			to_evaluate.erase(to_evaluate.begin());
			p_data_t cur_data=evaluate(cur_expr,kankyo,creater_t::creater().create_continuation(
				cont,true,kankyo,evaluated,to_evaluate));
			if(cur_data->force_return_flag) {
				return cur_data;
			} else if(cur_data->get_type()==DT_BOOLEAN && !((boolean_t*)&*cur_data)->is_true) {
				return cur_data;
			}
			last_data=cur_data;
		}
		return last_data;
	}
}

// 途中に1個でも#f以外があればその値、無ければ#fを返す
p_data_t or_proc(const std::vector<p_data_t>& args,p_data_t& kankyo,p_data_t& cont) {
	std::vector<p_data_t> evaluated;
	std::vector<p_data_t> to_evaluate=args;
	evaluated.push_back(creater_t::creater().create_native_func(or_proc,true));
	while(!to_evaluate.empty()) {
		p_data_t cur_expr=*to_evaluate.begin();
		to_evaluate.erase(to_evaluate.begin());
		p_data_t cur_data=evaluate(cur_expr,kankyo,creater_t::creater().create_continuation(
			cont,true,kankyo,evaluated,to_evaluate));
		if(cur_data->force_return_flag) {
			return cur_data;
		} else if(cur_data->get_type()!=DT_BOOLEAN || ((boolean_t*)&*cur_data)->is_true) {
			return cur_data;
		}
	}
	return creater_t::creater().create_boolean(false);
}

// 各引数のリストの最初の値が真なら、そのリストの式を評価し、それを返す
p_data_t cond_proc(const std::vector<p_data_t>& args,p_data_t& kankyo,p_data_t& cont) {
	if(args.size()==0) {
		return creater_t::creater().create_argument_number_error(
			"cond",1,args.size(),true);
	} else {
		// 引数をチェックする
		for(std::vector<p_data_t>::const_iterator it=args.begin();it!=args.end();it++) {
			p_data_t cur=*it;
			while(cur->get_type()==DT_CONS)cur=((cons_t*)&*cur)->cons_cdr;
			if((*it)->get_type()==DT_NULL || cur->get_type()!=DT_NULL) {
				return creater_t::creater().create_error("arguments of cond must be non-empty lists");
			}
		}
		// 継続用のリストを作成する
		p_data_t args_list=creater_t::creater().create_null();
		for(std::vector<p_data_t>::const_reverse_iterator it=args.rbegin();it!=args.rend();it++) {
			args_list=creater_t::creater().create_cons(*it,args_list);
		}
		// 引数を順番に評価する
		for(std::vector<p_data_t>::const_iterator it=args.begin();it!=args.end();it++) {
			std::vector<p_data_t> evaluated;
			std::vector<p_data_t> to_evaluate;
			evaluated.push_back(creater_t::creater().create_native_func(if_proc,true));
			// (cond (x a b) (c d) (e f)) -> (if x (begin a b) (cond (c d) (e f)))
			p_data_t cur=*it;
			args_list=((cons_t*)&*args_list)->cons_cdr;
			// 継続用begin
			to_evaluate.push_back(creater_t::creater().create_cons(
				creater_t::creater().create_native_func(begin_proc,true),
				((cons_t*)&*cur)->cons_cdr));
			// 継続用cond
			if(args_list->get_type()==DT_CONS) {
				to_evaluate.push_back(creater_t::creater().create_cons(
					creater_t::creater().create_native_func(cond_proc,true),
					args_list));
			}
			p_data_t cur_val=evaluate(((cons_t*)&*cur)->cons_car,kankyo,
				creater_t::creater().create_continuation(
					cont,true,kankyo,evaluated,to_evaluate));
			if(cur_val->force_return_flag)return cur_val;
			if(cur_val->get_type()!=DT_BOOLEAN || ((boolean_t*)&*cur_val)->is_true) {
				evaluated.clear();
				to_evaluate.clear();
				for(;;) {
					cur=((cons_t*)&*cur)->cons_cdr;
					if(cur->get_type()!=DT_CONS)break;
					to_evaluate.push_back(((cons_t*)&*cur)->cons_car);
				}
				while(!to_evaluate.empty()) {
					p_data_t cur_expr=*to_evaluate.begin();
					to_evaluate.erase(to_evaluate.begin());
					cur_val=evaluate(cur_expr,kankyo,creater_t::creater().create_continuation(
						cont,false,kankyo,evaluated,to_evaluate));
					if(cur_val->force_return_flag)break;
				}
				return cur_val;
			}
		}
		return creater_t::creater().create_null();
	}
}

// letとlet*の共通の処理を行う関数
static p_data_t let_check_arguments(const std::vector<p_data_t>& args,const std::string& name) {
	if(args.size()==0) {
		return creater_t::creater().create_argument_number_error(
			name,1,args.size(),true);
	} else {
		// 引数をチェックする
		p_data_t cur;
		for(cur=args[0];cur->get_type()==DT_CONS;cur=((cons_t*)&*cur)->cons_cdr) {
			p_data_t current_data=((cons_t*)&*cur)->cons_car;
			if(current_data->get_type()!=DT_CONS ||
			((cons_t*)&*current_data)->cons_car->get_type()!=DT_KIGOU ||
			((cons_t*)&*current_data)->cons_cdr->get_type()!=DT_CONS ||
			((cons_t*)&*((cons_t*)&*current_data)->cons_cdr)->cons_cdr->get_type()!=DT_NULL) {
				return creater_t::creater().create_error(
					std::string("invalid argument(s) for ")+name);
			}
		}
		if(cur->get_type()!=DT_NULL) {
			return creater_t::creater().create_error(
				std::string("first argument of ")+name+" must be a list");
		}
		return creater_t::creater().create_null(); // dummy
	}
}

// 束縛を作ってから、本体を評価する
p_data_t let_proc(const std::vector<p_data_t>& args,p_data_t& kankyo,p_data_t& cont) {
	p_data_t check=let_check_arguments(args,"let");
	if(check->get_type()==DT_ERROR)return check;
	// ラムダ式を作成する
	std::vector<p_data_t> evaluated;
	std::vector<p_data_t> to_evaluate;
	std::vector<std::string> lambda_args;
	std::vector<p_data_t> lambda_expr;
	for(p_data_t cur=args[0];cur->get_type()==DT_CONS;cur=((cons_t*)&*cur)->cons_cdr) {
		// 次に作成する束縛の情報を得る
		p_data_t current_data=((cons_t*)&*cur)->cons_car;
		p_data_t current_name=((cons_t*)&*current_data)->cons_car;
		p_data_t current_value=((cons_t*)&*((cons_t*)&*current_data)->cons_cdr)->cons_car;
		// 情報を格納する
		lambda_args.push_back(((kigou_t*)&*current_name)->kigou);
		to_evaluate.push_back(current_value);
	}
	// ラムダ式を作成する
	lambda_expr.insert(lambda_expr.end(),args.begin()+1,args.end());
	evaluated.push_back(creater_t::creater().create_lambda(lambda_args,lambda_expr,false,kankyo));
	// 引数の評価を実行する
	while(!to_evaluate.empty()) {
		p_data_t cur_expr=*to_evaluate.begin();
		to_evaluate.erase(to_evaluate.begin());
		p_data_t cur_res=evaluate(cur_expr,kankyo,creater_t::creater().create_continuation(
			cont,true,kankyo,evaluated,to_evaluate));
		if(cur_res->force_return_flag)return cur_res;
		evaluated.push_back(cur_res);
	}
	// ラムダ式を適用する
	p_data_t lambda_siki=*evaluated.begin();
	evaluated.erase(evaluated.begin());
	return apply_proc(lambda_siki,evaluated,kankyo,cont);
}

struct karizitu_t {
	p_data_t kari;
	p_data_t zitu;
	karizitu_t(){}
	karizitu_t(const p_data_t& k,const p_data_t& z):kari(k),zitu(z){}
};

// 束縛を順番に作ってから、本体を評価する
p_data_t let_star_proc(const std::vector<p_data_t>& args,p_data_t& kankyo,p_data_t& cont) {
	p_data_t check=let_check_arguments(args,"let*");
	if(check->get_type()==DT_ERROR)return check;
	if(args[0]->get_type()==DT_NULL) {
		// 引数が無いので、そのまま評価する
		std::vector<p_data_t> evaluated;
		std::vector<p_data_t> to_evaluate;
		p_data_t new_kankyo=creater_t::creater().create_kankyo(kankyo);
		to_evaluate.insert(to_evaluate.end(),args.begin()+1,args.end());
		p_data_t ret=args[0];
		while(!to_evaluate.empty()) {
			p_data_t cur_expr=*to_evaluate.begin();
			to_evaluate.erase(to_evaluate.begin());
			p_data_t cur_res=evaluate(cur_expr,new_kankyo,creater_t::creater().create_continuation(
				cont,false,new_kankyo,evaluated,to_evaluate));
			if(cur_res->force_return_flag)return cur_res;
			ret=cur_res;
		}
		return ret;
	} else {
		// lambdaで書きなおす
		p_data_t lambda_hyogen=creater_t::creater().create_null();
		for(std::vector<p_data_t>::const_reverse_iterator it=args.rbegin();it!=args.rend()-1;it++) {
			lambda_hyogen=creater_t::creater().create_cons(*it,lambda_hyogen);
		}
		std::vector<karizitu_t> lambda_arguments;
		for(p_data_t cur=args[0];cur->get_type()==DT_CONS;cur=((cons_t*)&*cur)->cons_cdr) {
			// 次に作成する束縛の情報を得る
			p_data_t current_data=((cons_t*)&*cur)->cons_car;
			p_data_t current_name=((cons_t*)&*current_data)->cons_car;
			p_data_t current_value=((cons_t*)&*((cons_t*)&*current_data)->cons_cdr)->cons_car;
			// 情報を格納する
			lambda_arguments.push_back(karizitu_t(current_name,current_value));
		}
		// 仮引数と実引数を追加する
		bool first_flag=true;
		for(std::vector<karizitu_t>::reverse_iterator it=lambda_arguments.rbegin();
		it!=lambda_arguments.rend();it++) {
			// lambda式を作成する
			lambda_hyogen=creater_t::creater().create_cons(
				creater_t::creater().create_native_func(lambda_proc,true),
				creater_t::creater().create_cons(
					creater_t::creater().create_cons(it->kari,creater_t::creater().create_null()),
					first_flag?lambda_hyogen:creater_t::creater().create_cons(
						lambda_hyogen,creater_t::creater().create_null())));
			first_flag=false;
			// 実引数を追加して評価させる
			lambda_hyogen=creater_t::creater().create_cons(lambda_hyogen,
				creater_t::creater().create_cons(it->zitu,creater_t::creater().create_null()));
		}
		// 評価させる
		return evaluate(lambda_hyogen,kankyo,cont);
	}
}

// 引数を順に評価する
p_data_t begin_proc(const std::vector<p_data_t>& args,p_data_t& kankyo,p_data_t& cont) {
	if(args.size()==0) {
		return creater_t::creater().create_number(0);
	} else {
		std::vector<p_data_t> evaluated; // dummy
		std::vector<p_data_t> to_evaluate=args;
		p_data_t ret;
		while(!to_evaluate.empty()) {
			p_data_t cur_expr=*to_evaluate.begin();
			to_evaluate.erase(to_evaluate.begin());
			ret=evaluate(cur_expr,kankyo,creater_t::creater().create_continuation(
				cont,false,kankyo,evaluated,to_evaluate));
			if(ret->force_return_flag)break;
		}
		return ret;
	}
}

// 遅延オブジェクトを生成する
p_data_t delay_proc(const std::vector<p_data_t>& args,p_data_t& kankyo,p_data_t&) {
	if(args.size()!=1) {
		return creater_t::creater().create_argument_number_error(
			"delay",1,args.size(),false);
	} else {
		return creater_t::creater().create_delay(args[0],kankyo);
	}
}
