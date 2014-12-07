#ifndef DATA_H_GUARD_DE3BB54A_0B97_4EED_A48D_F23686EFCAA3
#define DATA_H_GUARD_DE3BB54A_0B97_4EED_A48D_F23686EFCAA3

#include <vector>
#include <string>
#include <map>
#include "p_data.h"

// データの種類
enum DATATYPE {
	DT_EOF, // (入力の終わり)
	DT_EXIT, // 終了指示
	DT_ERROR, // エラー
	DT_NUM, // 数値
	DT_KIGOU, // 記号
	DT_BOOLEAN, // 真偽値
	DT_LAMBDA, // ラムダ式
	DT_CONS, // consセル
	DT_NATIVE_FUNC, // 組み込み手続き
	DT_NULL, // '()
	DT_KANKYO // 環境
};

// 組み込み手続きの関数ポインタ型
typedef p_data_t(*p_native_func)(const std::vector<p_data_t>& args,p_data_t& kankyo);

// データを表す構造体(参照カウントを持つ)
struct data_t {
	// 参照カウント
	int sansyo_count;
	// このデータをもらったら、次の評価を中止して呼び出し元に戻るべきというフラグ
	bool force_return_flag;

	data_t(): sansyo_count(0), force_return_flag(false) {}
	virtual ~data_t() {}

	// このデータの種類を返す
	virtual DATATYPE get_type() const = 0;
};

struct eof_t : public data_t {
	DATATYPE get_type() const {return DT_EOF;}
};

struct exit_t: public data_t {
	DATATYPE get_type() const {return DT_EXIT;}
	int exit_code;
};

struct error_t : public data_t {
	DATATYPE get_type() const {return DT_ERROR;}
	std::string error_mes;
};

struct num_t : public data_t {
	DATATYPE get_type() const {return DT_NUM;}
	double num;
};

struct kigou_t : public data_t {
	DATATYPE get_type() const {return DT_KIGOU;}
	std::string kigou;
};

struct boolean_t : public data_t {
	DATATYPE get_type() const {return DT_BOOLEAN;}
	bool is_true;
};

struct lambda_t : public data_t {
	DATATYPE get_type() const {return DT_LAMBDA;}
	std::vector<std::string> karihikisu;
	std::vector<p_data_t> hontai;
	p_data_t lambda_kankyo;
	bool is_kahencho;
};

struct cons_t : public data_t {
	DATATYPE get_type() const {return DT_CONS;}
	p_data_t cons_car;
	p_data_t cons_cdr;
};

struct native_func_t : public data_t {
	DATATYPE get_type() const {return DT_NATIVE_FUNC;}
	p_native_func native_func;
	bool tokusyu_keisiki;
};

struct null_t : public data_t {
	DATATYPE get_type() const {return DT_NULL;}
};

struct kankyo_t : public data_t {
	DATATYPE get_type() const {return DT_KANKYO;}
	p_data_t parent;
	std::map<std::string, p_data_t> sokubaku;
};

void print_data(const p_data_t& data,bool do_syouryaku,bool please_syouryaku=false);

#endif
