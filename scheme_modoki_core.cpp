#include <cstdlib>
#include <cctype>
#include "scheme_modoki_core.h"
#include "creater.h"

static bool is_tokusyu_keisiki(const p_data_t& data) {
	return data->get_type()==DT_NATIVE_FUNC && ((native_func_t*)&*data)->tokusyu_keisiki;
}

p_data_t* namae_no_kisoku2(const std::string& namae,p_data_t& kankyo) {
	if(kankyo==NULL) {
		return NULL;
	} else {
		std::map<std::string, p_data_t>::iterator it=((kankyo_t*)&*kankyo)->sokubaku.find(namae);
		if(it!=((kankyo_t*)&*kankyo)->sokubaku.end()) {
			return &it->second;
		} else {
			return namae_no_kisoku2(namae,((kankyo_t*)&*kankyo)->parent);
		}
	}
}

p_data_t namae_no_kisoku(const std::string& namae,p_data_t& kankyo) {
	p_data_t* ret=namae_no_kisoku2(namae,kankyo);
	if(ret==NULL) {
		return creater_t::creater().create_error_data(
			std::string("unbound variable: ")+namae);
	} else {
		return *ret;
	}
}

p_data_t apply_proc(const p_data_t& proc, const std::vector<p_data_t>& args,p_data_t& kankyo) {
	if(proc->get_type()==DT_NATIVE_FUNC) {
		return (((native_func_t*)&*proc)->native_func)(args,kankyo);
	} else if(proc->get_type()==DT_LAMBDA) {
		p_data_t new_kankyo=creater_t::creater().create_kankyo_data(((lambda_t*)&*proc)->lambda_kankyo);
		if(((lambda_t*)&*proc)->is_kahencho) {
			if(args.size()+1 < ((lambda_t*)&*proc)->karihikisu.size()) {
				return creater_t::creater().create_argument_number_error_data(
					"lambda-siki",((lambda_t*)&*proc)->karihikisu.size()-1,args.size(),true);
			}
			for(size_t i=0;i<((lambda_t*)&*proc)->karihikisu.size()-1;i++) {
				((kankyo_t*)&*new_kankyo)->sokubaku[((lambda_t*)&*proc)->karihikisu[i]]=args[i];
			}
			p_data_t args_list;
			p_data_t* args_buf=&args_list;
			for(size_t i=((lambda_t*)&*proc)->karihikisu.size()-1;i<args.size();i++) {
				*args_buf=creater_t::creater().create_cons_data(args[i],NULL);
				args_buf=&((cons_t*)&*(*args_buf))->cons_cdr;
			}
			*args_buf=creater_t::creater().create_null_data();
			((kankyo_t*)&*new_kankyo)->sokubaku[((lambda_t*)&*proc)->karihikisu.back()]=args_list;
		} else {
			if(((lambda_t*)&*proc)->karihikisu.size()!=args.size()) {
				return creater_t::creater().create_argument_number_error_data(
					"lambda-siki",((lambda_t*)&*proc)->karihikisu.size(),args.size(),false);
			}
			for(size_t i=0;i<args.size();i++) {
				((kankyo_t*)&*new_kankyo)->sokubaku[((lambda_t*)&*proc)->karihikisu[i]]=args[i];
			}
		}
		p_data_t res=NULL;
		for(std::vector<p_data_t>::const_iterator it=((lambda_t*)&*proc)->hontai.begin();
		it!=((lambda_t*)&*proc)->hontai.end();it++) {
			res=evaluate(*it,new_kankyo);
			if(res->force_return_flag)break;
		}
		return res;
	} else {
		return creater_t::creater().create_error_data(
			"attempt to apply a data that is unavailable for applying");
	}
}

p_data_t evaluate(const p_data_t& data,p_data_t& kankyo) {
	if(data->get_type()==DT_KIGOU) {
		// 名前
		return namae_no_kisoku(((kigou_t*)&*data)->kigou,kankyo);
	} else if(data->get_type()==DT_CONS) {
		// 組合せ
		p_data_t proc=evaluate(((cons_t*)&*data)->cons_car,kankyo);
		p_data_t next=((cons_t*)&*data)->cons_cdr;
		std::vector<p_data_t> args;
		bool tokusyu_keisiki;
		if(proc->force_return_flag)return proc;
		tokusyu_keisiki=is_tokusyu_keisiki(proc);
		while(next->get_type()==DT_CONS) {
			p_data_t next_arg=tokusyu_keisiki?((cons_t*)&*next)->cons_car:evaluate(((cons_t*)&*next)->cons_car,kankyo);
			if(next_arg->force_return_flag)return next_arg;
			args.push_back(next_arg);
			next=((cons_t*)&*next)->cons_cdr;
		}
		return apply_proc(proc,args,kankyo);
	} else {
		// その他のデータ
		return data;
	}
}

p_data_t parse(stream_reader& sr) {
	int in;
	do {
		in=sr.get_char();
	} while(isspace(in));
	if(in==EOF)return creater_t::creater().create_eof_data();
	if(in=='\'') {
		// クオート
		p_data_t data=parse(sr);
		return creater_t::creater().create_cons_data(
			creater_t::creater().create_kigou_data("quote"),
			creater_t::creater().create_cons_data(
				data,
				creater_t::creater().create_null_data()
			)
		);
	}
	if(in=='(') {
		// 組合せ
		std::vector<p_data_t> youso;
		bool dot_flag=false;
		p_data_t error_data=NULL;
		for(;;) {
			in=sr.get_char();
			if(in==')') {
				break;
			} else if(in==EOF) {
				return creater_t::creater().create_error_data("EOF in the list");
			} else if(!isspace(in)) {
				if(in=='.') {
					in=sr.get_char();
					if(isspace(in)) {
						dot_flag=true;
						continue;
					} else {
						sr.unget_char(in);
						if(in==')') {
							dot_flag=true;
							continue;
						}
						in='.';
					}
				}
				p_data_t cur_data;
				sr.unget_char(in);
				cur_data=parse(sr);
				if(error_data.is_null()) {
					if(cur_data->get_type()==DT_ERROR) {
						error_data=cur_data;
					} else {
						youso.push_back(cur_data);
					}
				}
			}
		}
		if(!error_data.is_null()) {
			return error_data;
		} else if(youso.empty()) {
			return creater_t::creater().create_null_data();
		} else {
			p_data_t ret;
			if(dot_flag) {
				ret=youso.back();
				for(std::vector<p_data_t>::reverse_iterator rit=youso.rbegin()+1;rit!=youso.rend();rit++) {
					ret=creater_t::creater().create_cons_data(*rit,ret);
				}
			} else {
				ret=creater_t::creater().create_null_data();
				for(std::vector<p_data_t>::reverse_iterator rit=youso.rbegin();rit!=youso.rend();rit++) {
					ret=creater_t::creater().create_cons_data(*rit,ret);
				}
			}
			return ret;
		}
	} else if(in==')') {
		return creater_t::creater().create_error_data("extra ) found");
	} else {
		// 名前
		char buf[4]={(char)in,0};
		std::string namae=buf;
		for(;;) {
			in=sr.get_char();
			if(in==EOF)break;
			else if(in==')') {
				sr.unget_char(')');
				break;
			} else if(isspace(in)) break;
			buf[0]=in;
			namae+=buf;
		}
		if(!namae.empty() &&
		((namae[0]=='-' && (namae.length()>1 && (isdigit(namae[1]) || namae[1]=='.'))) ||
		isdigit(namae[0]) ||
		(namae[0]=='.' && (namae.length()>1 && isdigit(namae[1]))))) {
			return creater_t::creater().create_num_data(atof(namae.c_str()));
		} else if(!namae.empty() && namae[0]=='#') {
			if(namae=="#t")return creater_t::creater().create_boolean_data(true);
			else if(namae=="#f")return creater_t::creater().create_boolean_data(false);
			else return creater_t::creater().create_error_data(
				"invalid name with sharp : "+namae);
		} else {
			return creater_t::creater().create_kigou_data(namae);
		}
	}
	return creater_t::creater().create_error_data("reached end of parse(bug!)");
}
