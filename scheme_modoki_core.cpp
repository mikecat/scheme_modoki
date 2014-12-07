#include <cstdlib>
#include <cctype>
#include "scheme_modoki_core.h"
#include "creater.h"
#include "global_config.h"
#include "kumikomi_tetuduki.h"

static p_data_t taiiki_kankyo;

static bool is_tokusyu_keisiki(const p_data_t& data) {
	return data->get_type()==DT_NATIVE_FUNC && ((native_func_t*)&*data)->tokusyu_keisiki;
}

p_data_t* namae_no_kisoku2(const std::string& namae,p_data_t& kankyo) {
	if(kankyo==NULL) {
		// 見つからないし、親の環境も無い
		return NULL;
	} else {
		std::map<std::string, p_data_t>::iterator it=((kankyo_t*)&*kankyo)->sokubaku.find(namae);
		if(it!=((kankyo_t*)&*kankyo)->sokubaku.end()) {
			// この環境で見つかった
			return &it->second;
		} else {
			// この環境で見つからないので、親に行く
			return namae_no_kisoku2(namae,((kankyo_t*)&*kankyo)->parent);
		}
	}
}

p_data_t namae_no_kisoku(const std::string& namae,p_data_t& kankyo) {
	p_data_t* ret=namae_no_kisoku2(namae,kankyo);
	if(ret==NULL) {
		return creater_t::creater().create_error(
			std::string("unbound variable: ")+namae);
	} else {
		return *ret;
	}
}

p_data_t apply_proc(const p_data_t& proc, const std::vector<p_data_t>& args,p_data_t& kankyo) {
	if(proc->get_type()==DT_NATIVE_FUNC) {
		// 組み込み手続きの場合、そのまま関数として呼び出し、結果を返す
		return (((native_func_t*)&*proc)->native_func)(args,kankyo);
	} else if(proc->get_type()==DT_LAMBDA) {
		// ラムダ式の適用
		p_data_t new_kankyo=creater_t::creater().create_kankyo(((lambda_t*)&*proc)->lambda_kankyo);
		// 引数の準備
		if(((lambda_t*)&*proc)->is_kahencho) {
			// 可変長引数
			if(args.size()+1 < ((lambda_t*)&*proc)->karihikisu.size()) {
				return creater_t::creater().create_argument_number_error(
					"lambda-siki",((lambda_t*)&*proc)->karihikisu.size()-1,args.size(),true);
			}
			// 最後の1個前までの仮引数を新しい環境内で束縛する
			for(size_t i=0;i<((lambda_t*)&*proc)->karihikisu.size()-1;i++) {
				((kankyo_t*)&*new_kankyo)->sokubaku[((lambda_t*)&*proc)->karihikisu[i]]=args[i];
			}
			// 最後の仮引数が束縛されるリストを作成する
			p_data_t args_list;
			p_data_t* args_buf=&args_list;
			for(size_t i=((lambda_t*)&*proc)->karihikisu.size()-1;i<args.size();i++) {
				*args_buf=creater_t::creater().create_cons(args[i],NULL);
				args_buf=&((cons_t*)&*(*args_buf))->cons_cdr;
			}
			*args_buf=creater_t::creater().create_null();
			((kankyo_t*)&*new_kankyo)->sokubaku[((lambda_t*)&*proc)->karihikisu.back()]=args_list;
		} else {
			// 引数の数が決まっている場合
			if(((lambda_t*)&*proc)->karihikisu.size()!=args.size()) {
				return creater_t::creater().create_argument_number_error(
					"lambda-siki",((lambda_t*)&*proc)->karihikisu.size(),args.size(),false);
			}
			// そのまま束縛していく
			for(size_t i=0;i<args.size();i++) {
				((kankyo_t*)&*new_kankyo)->sokubaku[((lambda_t*)&*proc)->karihikisu[i]]=args[i];
			}
		}
		// 本体を順に評価する
		p_data_t res=NULL;
		for(std::vector<p_data_t>::const_iterator it=((lambda_t*)&*proc)->hontai.begin();
		it!=((lambda_t*)&*proc)->hontai.end();it++) {
			res=evaluate(*it,new_kankyo);
			if(res->force_return_flag)break;
		}
		return res;
	} else {
		return creater_t::creater().create_error(
			"attempt to apply a data that is unavailable for applying");
	}
}

p_data_t evaluate(const p_data_t& data,p_data_t& kankyo) {
	if(data->get_type()==DT_KIGOU) { // 名前
		return namae_no_kisoku(((kigou_t*)&*data)->kigou,kankyo);
	} else if(data->get_type()==DT_CONS) { // 組合せ
		// 適用する関数を評価する
		p_data_t proc=evaluate(((cons_t*)&*data)->cons_car,kankyo);
		// 関数の引数のリストの要素を評価する
		p_data_t next=((cons_t*)&*data)->cons_cdr;
		std::vector<p_data_t> args;
		bool tokusyu_keisiki;
		if(proc->force_return_flag)return proc;
		tokusyu_keisiki=is_tokusyu_keisiki(proc);
		// リストが続いている間、引数を取る
		while(next->get_type()==DT_CONS) {
			p_data_t next_arg=tokusyu_keisiki?
				((cons_t*)&*next)->cons_car : evaluate(((cons_t*)&*next)->cons_car,kankyo);
			if(next_arg->force_return_flag)return next_arg;
			args.push_back(next_arg);
			next=((cons_t*)&*next)->cons_cdr;
		}
		// リストが'()で終わっているかをチェックする
		if(next->get_type()!=DT_NULL) {
			return creater_t::creater().create_error("list required for applying a function");
		}
		// 適用した結果を返す
		return apply_proc(proc,args,kankyo);
	} else { // その他のデータ
		return data;
	}
}

p_data_t parse(stream_reader& sr) {
	int in;
	// 空白を読み飛ばす
	do {
		in=sr.get_char();
	} while(isspace(in));
	if(in==EOF)return creater_t::creater().create_eof();
	if(in=='\'') {
		// クオート
		p_data_t data=parse(sr);
		return creater_t::creater().create_cons(
			creater_t::creater().create_kigou("quote"),
			creater_t::creater().create_cons(
				data,
				creater_t::creater().create_null()
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
				return creater_t::creater().create_error("EOF in the list");
			} else if(!isspace(in)) {
				// ドットの場合、単独のドットかを判定する
				if(in=='.') {
					in=sr.get_char();
					if(isspace(in)) {
						// 単独のドットである
						dot_flag=true;
						continue;
					} else {
						sr.unget_char(in);
						if(in==')') {
							dot_flag=true;
							continue;
						}
						// 単独のドットではない(0.2を表す.2など)
						in='.';
					}
				}
				// 組み合わせの次の要素をパースする
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
			// エラーが返ってきたらそのエラーを返す
			return error_data;
		} else if(youso.empty()) {
			// 空リスト
			return creater_t::creater().create_null();
		} else {
			p_data_t ret;
			if(dot_flag) {
				// ドットがあったので、最後に'()を入れない
				ret=youso.back();
				for(std::vector<p_data_t>::reverse_iterator rit=youso.rbegin()+1;rit!=youso.rend();rit++) {
					ret=creater_t::creater().create_cons(*rit,ret);
				}
			} else {
				// ドットが無いので、最後に'()を入れる
				ret=creater_t::creater().create_null();
				for(std::vector<p_data_t>::reverse_iterator rit=youso.rbegin();rit!=youso.rend();rit++) {
					ret=creater_t::creater().create_cons(*rit,ret);
				}
			}
			return ret;
		}
	} else if(in==')') {
		// 対応しない閉じカッコ
		return creater_t::creater().create_error("extra ) found");
	} else {
		// 名前
		char buf[4]={(char)in,0};
		std::string namae=buf;
		// 名前を読み込む
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
			// 数値
			return creater_t::creater().create_number(atof(namae.c_str()));
		} else if(!namae.empty() && namae[0]=='#') {
			// 先頭に#が付いた名前
			if(namae=="#t")return creater_t::creater().create_boolean(true);
			else if(namae=="#f")return creater_t::creater().create_boolean(false);
			else return creater_t::creater().create_error(
				"invalid name with sharp : "+namae);
		} else {
			// その他のシンボル
			return creater_t::creater().create_kigou(namae);
		}
	}
	return creater_t::creater().create_error("reached end of parse(bug!)");
}

void make_taiiki_kankyo() {
	taiiki_kankyo=creater_t::creater().create_kankyo();
	add_kumikomi_tetuduki_to_kankyo(taiiki_kankyo);
}

void delete_taiiki_kankyo() {
	taiiki_kankyo=NULL;
}

int run_script(stream_reader& sr,bool is_interactive) {
	int exit_code=0;
	for(;;) {
		p_data_t data;
		if(is_interactive)printf("input> ");
		data=parse(sr);
		if(data->get_type()==DT_EOF)break;
		data=evaluate(data,taiiki_kankyo);
		if(data->get_type()==DT_EOF)break;
		else if(data->get_type()==DT_EXIT) {
			exit_code=((exit_t*)&*data)->exit_code;
			break;
		}
		if(is_interactive) {
			print_data(data);
			putchar('\n');
		} else {
			if(data->get_type()==DT_ERROR) {
				print_data(data);
				putchar('\n');
				return 1;
			}
		}
	}
	return exit_code;
}
