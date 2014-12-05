#ifndef SCHEME_MODOKI_H_GUARD_66217EC2_CAC2_42C6_82A5_13D31B0E4EA4
#define SCHEME_MODOKI_H_GUARD_66217EC2_CAC2_42C6_82A5_13D31B0E4EA4

#include <string>
#include <map>
#include <vector>
#include "sansyo_count.h"

// データの種類
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
typedef sansyo_t<kankyo_t> p_kankyo_t;
typedef sansyo_t<data_t> p_data_t;

typedef p_data_t(*p_native_func)(const std::vector<p_data_t>& args,p_kankyo_t& kankyo);

// データを表す構造体
struct data_t : public with_sansyo_count_t {
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
	std::vector<p_data_t> hontai;
	p_kankyo_t lambda_kankyo;
	bool is_kahencho;
	// DT_CONS
	p_data_t cons_car;
	p_data_t cons_cdr;
	// DT_NATIVE_FUNC
	p_native_func native_func;
	bool tokusyu_keisiki;
};

// 環境を表す構造体
struct kankyo_t : public with_sansyo_count_t {
	p_kankyo_t parent;
	std::map<std::string, p_data_t> sokubaku;
	kankyo_t(p_kankyo_t oya=NULL):parent(oya),sokubaku() {}
};

// 省略できるconsセルの表示を省略するかを設定する
void set_do_syouryaku(bool enable);
// 省略できるconsセルの表示を省略するかを取得する
bool get_do_syouryaku();

// 名前の規則を用いて名前を探し、そのポインタを返す
p_data_t* namae_no_kisoku2(const std::string& namae,p_kankyo_t& kankyo);
// 名前の規則を用いて名前を探し、そのデータを返す
p_data_t namae_no_kisoku(const std::string& namae,p_kankyo_t& kankyo);
// 適用の規則を用いて手続きを適用する
p_data_t apply_proc(const p_data_t& proc, const std::vector<p_data_t>& args,p_kankyo_t& kankyo);
// 記号やリストで表現された式データを評価する
p_data_t evaluate(const p_data_t& data,p_kankyo_t& kankyo);

#endif
