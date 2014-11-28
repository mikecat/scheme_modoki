#ifndef SCHEME_MODOKI_H_GUARD_66217EC2_CAC2_42C6_82A5_13D31B0E4EA4
#define SCHEME_MODOKI_H_GUARD_66217EC2_CAC2_42C6_82A5_13D31B0E4EA4

#include <string>
#include <map>
#include <vector>

enum DATATYPE {
	DT_EOF, // (入力の終わり)
	DT_ERROR, // エラー
	DT_NUM, // 数値
	DT_KIGOU, // 記号
	DT_BOOLEAN, // 真偽値
	DT_LAMBDA, // ラムダ式
	DT_CONS, // consセル
	DT_NATIVE_FUNC, // 組み込み手続き
	DT_NULL // '()
};

struct kankyo_t;
struct data_t;
typedef data_t* (*p_native_func)(const std::vector<data_t*>& args,kankyo_t* kankyo);

struct data_t {
	DATATYPE type;
	// DT_ERROR
	std::string error_mes;
	bool please_exit;
	// DT_NUM
	double num;
	// DT_KIGOU
	std::string kigou;
	// DT_BOOLEAN
	bool is_true;
	// DT_LAMBDA
	std::vector<std::string> karihikisu;
	std::vector<data_t*> hontai;
	kankyo_t* lambda_kankyo;
	bool is_kahencho;
	// DT_CONS
	data_t* cons_car;
	data_t* cons_cdr;
	// DT_NATIVE_FUNC
	p_native_func native_func;
	bool tokusyu_keisiki;
};

struct kankyo_t {
	kankyo_t *parent;
	std::map<std::string, data_t*> sokubaku;
	kankyo_t(kankyo_t *oya=NULL):parent(oya),sokubaku() {}
};

class creater_t {
	private:
		static creater_t cr;
		std::vector<kankyo_t*> kankyo_log;
		std::vector<data_t*> data_log;
		creater_t(){}
		~creater_t();
	public:
		kankyo_t *create_kankyo(kankyo_t* oya=NULL) {
			kankyo_t* new_kankyo=new kankyo_t(oya);
			kankyo_log.push_back(new_kankyo);
			return new_kankyo;
		}
		data_t *create_data() {
			data_t *new_data=new data_t();
			data_log.push_back(new_data);
			return new_data;
		}
		data_t *create_eof_data() {
			data_t *new_data=create_data();
			new_data->type=DT_EOF;
			return new_data;
		}
		data_t *create_error_data(const std::string& str,bool please_exit=false) {
			data_t *new_data=create_data();
			new_data->type=DT_ERROR;
			new_data->error_mes=str;
			new_data->please_exit=please_exit;
			return new_data;
		}
		data_t *create_num_data(double num) {
			data_t *new_data=create_data();
			new_data->type=DT_NUM;
			new_data->num=num;
			return new_data;
		}
		data_t *create_kigou_data(const std::string& str) {
			data_t *new_data=create_data();
			new_data->type=DT_KIGOU;
			new_data->kigou=str;
			return new_data;
		}
		data_t *create_boolean_data(bool is_true) {
			data_t *new_data=create_data();
			new_data->type=DT_BOOLEAN;
			new_data->is_true=is_true;
			return new_data;
		}
		data_t *create_lambda_data(
		const std::vector<std::string>& karihikisu,
		const std::vector<data_t*>& hontai,bool is_kahencho,kankyo_t* kankyo) {
			data_t *new_data=create_data();
			new_data->type=DT_LAMBDA;
			new_data->karihikisu=karihikisu;
			new_data->hontai=hontai;
			new_data->lambda_kankyo=kankyo;
			new_data->is_kahencho=is_kahencho;
			return new_data;
		}
		data_t *create_cons_data(data_t* car,data_t* cdr) {
			data_t *new_data=create_data();
			new_data->type=DT_CONS;
			new_data->cons_car=car;
			new_data->cons_cdr=cdr;
			return new_data;
		}
		data_t *create_native_func_data(p_native_func native_func,bool tokusyu_keisiki=false) {
			data_t *new_data=create_data();
			new_data->type=DT_NATIVE_FUNC;
			new_data->native_func=native_func;
			new_data->tokusyu_keisiki=tokusyu_keisiki;
			return new_data;
		}
		data_t *create_null_data() {
			data_t *new_data=create_data();
			new_data->type=DT_NULL;
			return new_data;
		}
		static creater_t& creater(void) {
			return cr;
		}
};

data_t** namae_no_kisoku2(const std::string& namae,kankyo_t* kankyo);
data_t* namae_no_kisoku(const std::string& namae, kankyo_t* kankyo);
data_t* tekiyou(data_t* proc, const std::vector<data_t*> args,kankyo_t* kankyo);
data_t* hyouka_data(data_t* data,kankyo_t* kankyo);

#endif
