#include <cstdio>
#include "special_form.h"
#include "../creater.h"
#include "../scheme_modoki_core.h"

// 引数(未評価)を返す
p_data_t quote_proc(const std::vector<p_data_t>& args,p_data_t&) {
	if(args.size()!=1) {
		return creater_t::creater().create_argument_number_error(
			"quote",1,args.size(),false);
	} else {
		return args[0];
	}
}

// 新規束縛の作成
p_data_t define_proc(const std::vector<p_data_t>& args,p_data_t& kankyo) {
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
			p_data_t lambda_data=lambda_proc(lambda_args,kankyo);
			((kankyo_t*)&*kankyo)->sokubaku[((kigou_t*)&*name)->kigou]=lambda_data;
			return creater_t::creater().create_kigou(((kigou_t*)&*name)->kigou);
		}
	} else if(args[0]->get_type()==DT_KIGOU) {
		if(args.size()!=2) {
			return creater_t::creater().create_argument_number_error(
				"define",2,args.size(),false);
		}
		p_data_t ret_data=evaluate(args[1],kankyo);
		if(ret_data->force_return_flag)return ret_data;
		((kankyo_t*)&*kankyo)->sokubaku[((kigou_t*)&*args[0])->kigou]=ret_data;
		return creater_t::creater().create_kigou(((kigou_t*)&*args[0])->kigou);
	} else {
		return creater_t::creater().create_error(
			"you must specify kigou for first argument of define");
	}
}

// 代入
p_data_t set_proc(const std::vector<p_data_t>& args,p_data_t& kankyo) {
	if(args.size()!=2) {
		return creater_t::creater().create_argument_number_error(
			"set!",2,args.size(),false);
	} else if(args[0]->get_type()!=DT_KIGOU) {
		return creater_t::creater().create_error(
			"you must specify kigou for first argument of set!");
	} else {
		p_data_t* zittai=namae_no_kisoku2(((kigou_t*)&*args[0])->kigou,kankyo);
		if(zittai==NULL)return namae_no_kisoku(((kigou_t*)&*args[0])->kigou,kankyo);
		p_data_t value=evaluate(args[1],kankyo);
		if(value->force_return_flag)return value;
		*zittai=value;
		return value;
	}
}

// 新規手続きの作成
p_data_t lambda_proc(const std::vector<p_data_t>& args,p_data_t& kankyo) {
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
p_data_t if_proc(const std::vector<p_data_t>& args,p_data_t& kankyo) {
	if(args.size()!=2 && args.size()!=3) {
		char buf[16];
		sprintf(buf,"%u",(unsigned int)args.size());
		return creater_t::creater().create_error(
			std::string("invalid number of arguments for if : expected 2 or 3, got ")+buf);
	} else {
		p_data_t sinriti=evaluate(args[0],kankyo);
		if(sinriti->force_return_flag)return sinriti;
		if(sinriti->get_type()==DT_BOOLEAN && !((boolean_t*)&*sinriti)->is_true) {
			if(args.size()>=3) {
				return evaluate(args[2],kankyo);
			} else {
				return creater_t::creater().create_null();
			}
		} else {
			return evaluate(args[1],kankyo);
		}
	}
}

// 途中に1個でも#fがあれば#f、無ければ最後の値を返す
p_data_t and_proc(const std::vector<p_data_t>& args,p_data_t& kankyo) {
	if(args.size()==0) {
		return creater_t::creater().create_boolean(true);
	} else {
		p_data_t last_data=NULL;
		for(std::vector<p_data_t>::const_iterator it=args.begin();it!=args.end();it++) {
			p_data_t cur_data=evaluate(*it,kankyo);
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
p_data_t or_proc(const std::vector<p_data_t>& args,p_data_t& kankyo) {
	for(std::vector<p_data_t>::const_iterator it=args.begin();it!=args.end();it++) {
		p_data_t cur_data=evaluate(*it,kankyo);
		if(cur_data->force_return_flag) {
			return cur_data;
		} else if(cur_data->get_type()!=DT_BOOLEAN || ((boolean_t*)&*cur_data)->is_true) {
			return cur_data;
		}
	}
	return creater_t::creater().create_boolean(false);
}

// 各引数のリストの最初の値が真なら、そのリストの式を評価し、それを返す
p_data_t cond_proc(const std::vector<p_data_t>& args,p_data_t& kankyo) {
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
		// 引数を順番に評価する
		for(std::vector<p_data_t>::const_iterator it=args.begin();it!=args.end();it++) {
			p_data_t cur=*it;
			p_data_t cur_val=evaluate(((cons_t*)&*cur)->cons_car,kankyo);
			if(cur_val->force_return_flag)return cur_val;
			if(cur_val->get_type()!=DT_BOOLEAN || ((boolean_t*)&*cur_val)->is_true) {
				for(;;) {
					cur=((cons_t*)&*cur)->cons_cdr;
					if(cur->get_type()!=DT_CONS)break;
					cur_val=evaluate(((cons_t*)&*cur)->cons_car,kankyo);
					if(cur_val->force_return_flag)break;
				}
				return cur_val;
			}
		}
		return creater_t::creater().create_null();
	}
}

// 束縛を作ってから、本体を評価する
p_data_t let_proc(const std::vector<p_data_t>& args,p_data_t& kankyo) {
	if(args.size()==0) {
		return creater_t::creater().create_argument_number_error(
			"let",1,args.size(),true);
	} else {
		// 引数をチェックする
		p_data_t cur;
		for(cur=args[0];cur->get_type()==DT_CONS;cur=((cons_t*)&*cur)->cons_cdr) {
			p_data_t current_data=((cons_t*)&*cur)->cons_car;
			if(current_data->get_type()!=DT_CONS ||
			((cons_t*)&*current_data)->cons_car->get_type()!=DT_KIGOU ||
			((cons_t*)&*current_data)->cons_cdr->get_type()!=DT_CONS ||
			((cons_t*)&*((cons_t*)&*current_data)->cons_cdr)->cons_cdr->get_type()!=DT_NULL) {
				return creater_t::creater().create_error("invalid argument(s) for let");
			}
		}
		if(cur->get_type()!=DT_NULL) {
			return creater_t::creater().create_error("first argument of let must be a list");
		}
		// 環境を作成して束縛を実行する
		p_data_t new_kankyo=creater_t::creater().create_kankyo(kankyo);
		for(cur=args[0];cur->get_type()==DT_CONS;cur=((cons_t*)&*cur)->cons_cdr) {
			p_data_t current_data=((cons_t*)&*cur)->cons_car;
			p_data_t current_name=((cons_t*)&*current_data)->cons_car;
			p_data_t current_value=evaluate(
				((cons_t*)&*((cons_t*)&*current_data)->cons_cdr)->cons_car,kankyo);
			if(current_value->force_return_flag)return current_value;
			((kankyo_t*)&*new_kankyo)->sokubaku[((kigou_t*)&*current_name)->kigou]=current_value;
		}
		// 評価を実行する
		cur=creater_t::creater().create_number(0);
		for(std::vector<p_data_t>::const_iterator it=args.begin()+1;it!=args.end();it++) {
			cur=evaluate(*it,new_kankyo);
			if(cur->force_return_flag)break;
		}
		return cur;
	}
}

// 束縛を順番に作ってから、本体を評価する
p_data_t let_star_proc(const std::vector<p_data_t>& args,p_data_t& kankyo) {
	if(args.size()==0) {
		return creater_t::creater().create_argument_number_error(
			"let*",1,args.size(),true);
	} else {
		// 引数をチェックする
		p_data_t cur;
		for(cur=args[0];cur->get_type()==DT_CONS;cur=((cons_t*)&*cur)->cons_cdr) {
			p_data_t current_data=((cons_t*)&*cur)->cons_car;
			if(current_data->get_type()!=DT_CONS ||
			((cons_t*)&*current_data)->cons_car->get_type()!=DT_KIGOU ||
			((cons_t*)&*current_data)->cons_cdr->get_type()!=DT_CONS ||
			((cons_t*)&*((cons_t*)&*current_data)->cons_cdr)->cons_cdr->get_type()!=DT_NULL) {
				return creater_t::creater().create_error("invalid argument(s) for let*");
			}
		}
		if(cur->get_type()!=DT_NULL) {
			return creater_t::creater().create_error("first argument of let* must be a list");
		}
		// 環境を作成して束縛を実行する
		p_data_t current_kankyo=kankyo;
		for(cur=args[0];cur->get_type()==DT_CONS;cur=((cons_t*)&*cur)->cons_cdr) {
			p_data_t new_kankyo=creater_t::creater().create_kankyo(current_kankyo);
			p_data_t current_data=((cons_t*)&*cur)->cons_car;
			p_data_t current_name=((cons_t*)&*current_data)->cons_car;
			p_data_t current_value=evaluate(
				((cons_t*)&*((cons_t*)&*current_data)->cons_cdr)->cons_car,current_kankyo);
			if(current_value->force_return_flag)return current_value;
			((kankyo_t*)&*new_kankyo)->sokubaku[((kigou_t*)&*current_name)->kigou]=current_value;
			current_kankyo=new_kankyo;
		}
		// 評価を実行する
		cur=creater_t::creater().create_number(0);
		for(std::vector<p_data_t>::const_iterator it=args.begin()+1;it!=args.end();it++) {
			cur=evaluate(*it,current_kankyo);
			if(cur->force_return_flag)break;
		}
		return cur;
	}
}
