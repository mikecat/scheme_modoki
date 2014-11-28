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

data_t** namae_no_kisoku2(const std::string& namae,kankyo_t* kankyo);
data_t* namae_no_kisoku(const std::string& namae, kankyo_t* kankyo);
data_t* tekiyou(data_t* proc, const std::vector<data_t*> args,kankyo_t* kankyo);
data_t* hyouka_data(data_t* data,kankyo_t* kankyo);

#endif
