#include <cstdio>
#include "special_form.h"

// 引数(未評価)を返す
p_data_t quote_proc(const std::vector<p_data_t>& args,p_kankyo_t&) {
	if(args.size()!=1) {
		return creater_t::creater().create_argument_number_error_data(
			"quote",1,args.size(),false);
	} else {
		return args[0];
	}
}

// 新規束縛の作成
p_data_t define_proc(const std::vector<p_data_t>& args,p_kankyo_t& kankyo) {
	if(args.size()<2) {
		return creater_t::creater().create_argument_number_error_data(
			"define",2,args.size(),true);
	} else if(args[0]->type==DT_CONS) {
		p_data_t name=args[0]->cons_car;
		if(name->type!=DT_KIGOU) {
			return creater_t::creater().create_error_data(
				"you must specify kigou for first argument of define");
		} else {
			std::vector<p_data_t> lambda_args=args;
			lambda_args[0]=args[0]->cons_cdr;
			p_data_t lambda_data=lambda_proc(lambda_args,kankyo);
			kankyo->sokubaku[name->kigou]=lambda_data;
			return creater_t::creater().create_kigou_data(name->kigou);
		}
	} else if(args[0]->type==DT_KIGOU) {
		if(args.size()!=2) {
			return creater_t::creater().create_argument_number_error_data(
				"define",2,args.size(),false);
		}
		p_data_t ret_data=hyouka_data(args[1],kankyo);
		if(ret_data->type==DT_ERROR)return ret_data;
		kankyo->sokubaku[args[0]->kigou]=ret_data;
		return creater_t::creater().create_kigou_data(args[0]->kigou);
	} else {
		return creater_t::creater().create_error_data(
			"you must specify kigou for first argument of define");
	}
}

// 代入
p_data_t set_proc(const std::vector<p_data_t>& args,p_kankyo_t& kankyo) {
	if(args.size()!=2) {
		return creater_t::creater().create_argument_number_error_data(
			"set!",2,args.size(),false);
	} else if(args[0]->type!=DT_KIGOU) {
		return creater_t::creater().create_error_data(
			"you must specify kigou for first argument of set!");
	} else {
		p_data_t* zittai=namae_no_kisoku2(args[0]->kigou,kankyo);
		if(zittai==NULL)return namae_no_kisoku(args[0]->kigou,kankyo);
		p_data_t value=hyouka_data(args[1],kankyo);
		if(value->type==DT_ERROR)return value;
		*zittai=value;
		return value;
	}
}

// 新規手続きの作成
p_data_t lambda_proc(const std::vector<p_data_t>& args,p_kankyo_t& kankyo) {
	if(args.size()<2) {
		return creater_t::creater().create_argument_number_error_data(
			"lambda",2,args.size(),true);
	} else {
		std::vector<std::string> karihikisu_list;
		std::vector<p_data_t> hontai_list;
		bool is_kahencho=false;
		bool karihikisu_valid_flag=false;
		// 仮引数リストの取得
		if(args[0]->type==DT_KIGOU || args[0]->type==DT_CONS || args[0]->type==DT_NULL) {
			p_data_t cur_karihikisu=args[0];
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
		for(std::vector<p_data_t>::const_iterator it=args.begin()+1;it!=args.end();it++) {
			hontai_list.push_back(*it);
		}
		return creater_t::creater().create_lambda_data(
			karihikisu_list,hontai_list,is_kahencho,kankyo);
	}
}

// 条件分岐
p_data_t if_proc(const std::vector<p_data_t>& args,p_kankyo_t& kankyo) {
	if(args.size()!=2 && args.size()!=3) {
		char buf[16];
		sprintf(buf,"%u",(unsigned int)args.size());
		return creater_t::creater().create_error_data(
			std::string("invalid number of arguments for if : expected 2 or 3, got ")+buf);
	} else {
		p_data_t sinriti=hyouka_data(args[0],kankyo);
		if(sinriti->type==DT_ERROR)return sinriti;
		if(sinriti->type==DT_BOOLEAN && !sinriti->is_true) {
			if(args.size()>=3) {
				return hyouka_data(args[2],kankyo);
			} else {
				return creater_t::creater().create_null_data();
			}
		} else {
			return hyouka_data(args[1],kankyo);
		}
	}
}

// 途中に1個でも#fがあれば#f、無ければ最後の値を返す
p_data_t and_proc(const std::vector<p_data_t>& args,p_kankyo_t& kankyo) {
	if(args.size()==0) {
		return creater_t::creater().create_boolean_data(true);
	} else {
		p_data_t last_data=NULL;
		for(std::vector<p_data_t>::const_iterator it=args.begin();it!=args.end();it++) {
			p_data_t cur_data=hyouka_data(*it,kankyo);
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
p_data_t or_proc(const std::vector<p_data_t>& args,p_kankyo_t& kankyo) {
	for(std::vector<p_data_t>::const_iterator it=args.begin();it!=args.end();it++) {
		p_data_t cur_data=hyouka_data(*it,kankyo);
		if(cur_data->type==DT_ERROR) {
			return cur_data;
		} else if(cur_data->type!=DT_BOOLEAN || cur_data->is_true) {
			return cur_data;
		}
	}
	return creater_t::creater().create_boolean_data(false);
}
