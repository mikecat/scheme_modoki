#include <cstdio>
#include <cstdlib>
#include <cctype>
#include "sansyo_count.h"
#include "creater.h"
#include "kumikomi_tetuduki.h"
#include "stream_reader.h"

static bool do_syouryaku=true;

void set_do_syouryaku(bool enable) {
	do_syouryaku=enable;
}

bool get_do_syouryaku() {
	return do_syouryaku;
}

bool is_space_chars(int c) {
	return c==' ' || c=='\t' || c=='\r' || c=='\n';
}

bool is_tokusyu_keisiki(const p_data_t& data) {
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
			if(res->get_type()==DT_ERROR)break;
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
		if(proc->get_type()==DT_ERROR)return proc;
		tokusyu_keisiki=is_tokusyu_keisiki(proc);
		while(next->get_type()==DT_CONS) {
			p_data_t next_arg=tokusyu_keisiki?((cons_t*)&*next)->cons_car:evaluate(((cons_t*)&*next)->cons_car,kankyo);
			if(next_arg->get_type()==DT_ERROR)return next_arg;
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
	} while(is_space_chars(in));
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
			} else if(!is_space_chars(in)) {
				if(in=='.') {
					in=sr.get_char();
					if(is_space_chars(in)) {
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
			} else if(is_space_chars(in)) break;
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

void print_data(p_data_t& data,bool do_syouryaku,bool please_syouryaku=false) {
	switch(data->get_type()) {
		case DT_EOF:
			break;
		case DT_ERROR:
			printf("ERROR: %s",((error_t*)&*data)->error_mes.c_str());
			break;
		case DT_NUM:
			printf("%.15g",((num_t*)&*data)->num);
			break;
		case DT_KIGOU:
			printf("%s",((kigou_t*)&*data)->kigou.c_str());
			break;
		case DT_BOOLEAN:
			printf("#%c",((boolean_t*)&*data)->is_true?'t':'f');
			break;
		case DT_LAMBDA:
			printf("<lambda-siki>");
			break;
		case DT_CONS:
			if(!do_syouryaku || !please_syouryaku)printf("(");
			if(!((cons_t*)&*data)->cons_car.is_null()) {
				print_data(((cons_t*)&*data)->cons_car,do_syouryaku);
			} else {
				printf("<NULL(bug?)>");
			}
			if(!((cons_t*)&*data)->cons_cdr.is_null()) {
				if(!do_syouryaku) {
					printf(" . ");
				} else {
					if(((cons_t*)&*data)->cons_cdr->get_type()!=DT_NULL){
						printf(" ");
						if(((cons_t*)&*data)->cons_cdr->get_type()!=DT_CONS) {
							printf(". ");
						}
					}
				}
				print_data(((cons_t*)&*data)->cons_cdr,do_syouryaku,true);
			} else {
				printf(" . <NULL(bug?)>");
			}
			if(!do_syouryaku || !please_syouryaku)printf(")");
			break;
		case DT_NATIVE_FUNC:
			printf("<native function>");
			break;
		case DT_NULL:
			if(!do_syouryaku || !please_syouryaku)printf("()");
			break;
		case DT_KANKYO:
			printf("<kankyo>");
			break;
	}
}

int main(int argc,char *argv[]) {
	for(int i=1;i<argc;i++) {
		std::string cur_arg=argv[i];
		if(cur_arg=="--no-syouryaku") {
			do_syouryaku=false;
		} else if(cur_arg=="--no-auto-delete") {
			p_data_config_t::set_do_auto_delete(false);
		}
	}
	p_data_t taiiki_kankyo=creater_t::creater().create_kankyo_data();
	add_kumikomi_tetuduki_to_kankyo(taiiki_kankyo);
	file_reader fr(stdin);
	for(;;) {
		p_data_t data;
		printf("input> ");
		data=parse(fr);
		if(data->get_type()==DT_EOF || (data->get_type()==DT_ERROR && ((error_t*)&*data)->please_exit))break;
		data=evaluate(data,taiiki_kankyo);
		if(data->get_type()==DT_EOF || (data->get_type()==DT_ERROR && ((error_t*)&*data)->please_exit))break;
		print_data(data,do_syouryaku);
		putchar('\n');
	}
	return 0;
}
