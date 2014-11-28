#include <cstdio>
#include <cstdlib>
#include <cctype>
#include "scheme_modoki.h"
#include "creater.h"
#include "kumikomi_tetuduki.h"
#include "stream_reader.h"

bool is_space_chars(int c) {
	return c==' ' || c=='\t' || c=='\r' || c=='\n';
}

bool is_tokusyu_keisiki(data_t* data) {
	return data->type==DT_NATIVE_FUNC && data->tokusyu_keisiki;
}

data_t** namae_no_kisoku2(const std::string& namae,kankyo_t* kankyo) {
	if(kankyo==NULL) {
		return NULL;
	} else {
		std::map<std::string, data_t*>::iterator it=kankyo->sokubaku.find(namae);
		if(it!=kankyo->sokubaku.end()) {
			return &it->second;
		} else {
			return namae_no_kisoku2(namae,kankyo->parent);
		}
	}
}

data_t* namae_no_kisoku(const std::string& namae,kankyo_t* kankyo) {
	data_t** ret=namae_no_kisoku2(namae,kankyo);
	if(ret==NULL) {
		return creater_t::creater().create_error_data(
			std::string("unbound variable: ")+namae);
	} else {
		return *ret;
	}
}

data_t* tekiyou(data_t* proc, const std::vector<data_t*> args,kankyo_t* kankyo) {
	if(proc->type==DT_NATIVE_FUNC) {
		return (proc->native_func)(args,kankyo);
	} else if(proc->type==DT_LAMBDA) {
		kankyo_t* new_kankyo=creater_t::creater().create_kankyo(proc->lambda_kankyo);
		if(proc->is_kahencho) {
			if(args.size()+1 < proc->karihikisu.size()) {
				return creater_t::creater().create_error_data(
					"arguments too few for lambda-siki");
			}
			for(size_t i=0;i<proc->karihikisu.size()-1;i++) {
				new_kankyo->sokubaku[proc->karihikisu[i]]=args[i];
			}
			data_t* args_list;
			data_t** args_buf=&args_list;
			for(size_t i=proc->karihikisu.size()-1;i<args.size();i++) {
				*args_buf=creater_t::creater().create_cons_data(args[i],NULL);
				args_buf=&(*args_buf)->cons_cdr;
			}
			*args_buf=creater_t::creater().create_null_data();
			new_kankyo->sokubaku[proc->karihikisu.back()]=args_list;
		} else {
			if(proc->karihikisu.size()!=args.size()) {
				return creater_t::creater().create_error_data(
					"invalid number of arguments for lambda-siki");
			}
			for(size_t i=0;i<args.size();i++) {
				new_kankyo->sokubaku[proc->karihikisu[i]]=args[i];
			}
		}
		data_t* res=NULL;
		for(std::vector<data_t*>::iterator it=proc->hontai.begin();
		it!=proc->hontai.end();it++) {
			res=hyouka_data(*it,new_kankyo);
			if(res->type==DT_ERROR)break;
		}
		return res;
	} else {
		return creater_t::creater().create_error_data(
			"attempt to tekiyou a data that is unavailable for tekiyou");
	}
}

data_t* hyouka_data(data_t* data,kankyo_t* kankyo) {
	if(data->type==DT_KIGOU) {
		// 名前
		return namae_no_kisoku(data->kigou,kankyo);
	} else if(data->type==DT_CONS) {
		// 組合せ
		data_t* proc=hyouka_data(data->cons_car,kankyo);
		data_t* next=data->cons_cdr;
		std::vector<data_t*> args;
		bool tokusyu_keisiki;
		if(proc->type==DT_ERROR)return proc;
		tokusyu_keisiki=is_tokusyu_keisiki(proc);
		while(next->type==DT_CONS) {
			data_t* next_arg=tokusyu_keisiki?next->cons_car:hyouka_data(next->cons_car,kankyo);
			if(next_arg->type==DT_ERROR)return next_arg;
			args.push_back(next_arg);
			next=next->cons_cdr;
		}
		return tekiyou(proc,args,kankyo);
	} else {
		// その他のデータ
		return data;
	}
}

data_t* hyouka(stream_reader& sr,kankyo_t* kankyo,bool quote_mode=false) {
	int in;
	do {
		in=sr.get_char();
	} while(is_space_chars(in));
	if(in==EOF)return creater_t::creater().create_eof_data();
	if(in=='\'') {
		// クオート
		data_t* data=hyouka(sr,kankyo,true);
		if(quote_mode) {
			return creater_t::creater().create_cons_data(
				creater_t::creater().create_kigou_data("quote"),
				creater_t::creater().create_cons_data(
					data,
					creater_t::creater().create_null_data()
				)
			);
		} else {
			return data;
		}
	}
	if(in=='(') {
		// 組合せ
		std::vector<data_t*> youso;
		bool tokusyu_keisiki=false;
		bool dot_flag=false;
		data_t* error_data=NULL;
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
				data_t* cur_data;
				sr.unget_char(in);
				cur_data=hyouka(sr,kankyo,tokusyu_keisiki?true:quote_mode);
				if(cur_data->type==DT_ERROR) {
					if(error_data==NULL)error_data=cur_data;
				} else if(error_data==NULL) {
					if(youso.empty()) {
						tokusyu_keisiki=is_tokusyu_keisiki(cur_data);
					}
					youso.push_back(cur_data);
				}
			}
		}
		if(error_data!=NULL) {
			return error_data;
		} else if(youso.empty()) {
			return creater_t::creater().create_null_data();
		} else {
			if(quote_mode) {
				data_t* ret;
				if(dot_flag) {
					ret=youso.back();
					for(std::vector<data_t*>::reverse_iterator rit=youso.rbegin()+1;rit!=youso.rend();rit++) {
						ret=creater_t::creater().create_cons_data(*rit,ret);
					}
				} else {
					ret=creater_t::creater().create_null_data();
					for(std::vector<data_t*>::reverse_iterator rit=youso.rbegin();rit!=youso.rend();rit++) {
						ret=creater_t::creater().create_cons_data(*rit,ret);
					}
				}
				return ret;
			} else {
				data_t* proc=youso[0];
				youso.erase(youso.begin());
				return tekiyou(proc,youso,kankyo);
			}
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
		} else if(quote_mode) {
			return creater_t::creater().create_kigou_data(namae);
		} else {
			return namae_no_kisoku(namae,kankyo);
		}
	}
	return creater_t::creater().create_error_data("reached end of hyouka(bug!)");
}

void print_data(data_t& data,bool do_syouryaku,bool please_syouryaku=false) {
	switch(data.type) {
		case DT_EOF:
			break;
		case DT_ERROR:
			printf("ERROR: %s",data.error_mes.c_str());
			break;
		case DT_NUM:
			printf("%.15g",data.num);
			break;
		case DT_KIGOU:
			printf("%s",data.kigou.c_str());
			break;
		case DT_BOOLEAN:
			printf("#%c",data.is_true?'t':'f');
			break;
		case DT_LAMBDA:
			printf("<lambda-siki>");
			break;
		case DT_CONS:
			if(!do_syouryaku || !please_syouryaku)printf("(");
			if(data.cons_car!=NULL) {
				print_data(*data.cons_car,do_syouryaku);
			} else {
				printf("<NULL(bug?)>");
			}
			if(data.cons_cdr!=NULL) {
				if(!do_syouryaku) {
					printf(" . ");
				} else {
					if(data.cons_cdr->type!=DT_NULL){
						printf(" ");
						if(data.cons_cdr->type!=DT_CONS) {
							printf(". ");
						}
					}
				}
				print_data(*data.cons_cdr,do_syouryaku,true);
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
	}
}

int main(int argc,char *argv[]) {
	kankyo_t* taiiki_kankyo=creater_t::creater().create_kankyo();
	file_reader fr(stdin);
	bool do_syouryaku=true;
	for(int i=1;i<argc;i++) {
		if(std::string(argv[i])=="--no-syouryaku")do_syouryaku=false;
	}
	add_kumikomi_tetuduki_to_kankyo(taiiki_kankyo);
	for(;;) {
		data_t* data;
		printf("input> ");
		data=hyouka(fr,taiiki_kankyo);
		if(data->type==DT_EOF || (data->type==DT_ERROR && data->please_exit))break;
		print_data(*data,do_syouryaku);
		putchar('\n');
	}
	return 0;
}
