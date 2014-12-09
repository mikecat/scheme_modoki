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
	DT_NUMBER, // 数値
	DT_KIGOU, // 記号
	DT_BOOLEAN, // 真偽値
	DT_LAMBDA, // ラムダ式
	DT_CONS, // consセル
	DT_NATIVE_FUNC, // 組み込み手続き
	DT_NULL, // '()
	DT_DELAY, // 遅延オブジェクト
	DT_CONTINUATION, // 継続
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
	// このデータの名前を返す
	virtual const char* get_name() const = 0;
};

struct eof_t : public data_t {
	DATATYPE get_type() const {return DT_EOF;}
	const char* get_name() const {return "EOF";}
};

struct exit_t: public data_t {
	DATATYPE get_type() const {return DT_EXIT;}
	const char* get_name() const {return "exit";}
	int exit_code;
};

struct error_t : public data_t {
	DATATYPE get_type() const {return DT_ERROR;}
	const char* get_name() const {return "error";}
	std::string error_mes;
};

struct number_t : public data_t {
	DATATYPE get_type() const {return DT_NUMBER;}
	const char* get_name() const {return "number";}
	double number;
};

struct kigou_t : public data_t {
	DATATYPE get_type() const {return DT_KIGOU;}
	const char* get_name() const {return "kigou";}
	std::string kigou;
};

struct boolean_t : public data_t {
	DATATYPE get_type() const {return DT_BOOLEAN;}
	const char* get_name() const {return "boolean";}
	bool is_true;
};

struct lambda_t : public data_t {
	DATATYPE get_type() const {return DT_LAMBDA;}
	const char* get_name() const {return "lambda-siki";}
	std::vector<std::string> karihikisu;
	std::vector<p_data_t> hontai;
	p_data_t lambda_kankyo;
	bool is_kahencho;
};

struct cons_t : public data_t {
	DATATYPE get_type() const {return DT_CONS;}
	const char* get_name() const {return "cons-cell";}
	p_data_t cons_car;
	p_data_t cons_cdr;
};

struct native_func_t : public data_t {
	DATATYPE get_type() const {return DT_NATIVE_FUNC;}
	const char* get_name() const {return "native func";}
	p_native_func native_func;
	bool tokusyu_keisiki;
};

struct null_t : public data_t {
	DATATYPE get_type() const {return DT_NULL;}
	const char* get_name() const {return "null";}
};

struct delay_t : public data_t {
	DATATYPE get_type() const {return DT_DELAY;}
	const char* get_name() const {return "delay";}
	p_data_t expr;
	p_data_t kankyo;
};

struct continuation_t : public data_t {
	DATATYPE get_type() const {return DT_CONTINUATION;}
	const char* get_name() const {return "continuation";}
	p_data_t next_cont; // 次に実行する継続(無い場合NULL)
	p_data_t kankyo; // 評価に使用する環境
	p_data_t kankyo_for_args; // 引数を束縛する環境(無い場合NULL)
	p_data_t proc_to_apply; // 適用する手続き(無い場合NULL)
	std::vector<p_data_t> evaluated_args; // 既に評価した引数
	std::vector<p_data_t> args_to_evaluate; // 未評価の引数
};

struct kankyo_t : public data_t {
	DATATYPE get_type() const {return DT_KANKYO;}
	const char* get_name() const {return "kankyo";}
	p_data_t parent;
	std::map<std::string, p_data_t> sokubaku;
};

void print_data(const p_data_t& data,bool please_syouryaku=false);

#endif
