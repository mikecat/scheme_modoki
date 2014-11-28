#ifndef CREATER_H_GUARD_85D8EF3B_8D4D_4B3A_824C_8B783B6BE20D
#define CREATER_H_GUARD_85D8EF3B_8D4D_4B3A_824C_8B783B6BE20D

#include "scheme_modoki.h"

class creater_t {
	private:
		static creater_t cr;
		std::vector<kankyo_t*> kankyo_log;
		std::vector<data_t*> data_log;
		creater_t(){}
		~creater_t();
	public:
		// 環境データを生成する
		kankyo_t *create_kankyo(kankyo_t* oya=NULL);
		// 適当なデータを生成する
		data_t *create_data();
		// EOFデータを生成する
		data_t *create_eof_data();
		// エラーまたは終了指示データを生成する
		data_t *create_error_data(const std::string& str,bool please_exit=false);
		// 引数の数に関するエラーを生成する
		data_t *create_argument_number_error_data(
			const std::string& name,size_t min_number,size_t got_number,bool is_kahencho);
		// 数値データを生成する
		data_t *create_num_data(double num);
		// 記号データを生成する
		data_t *create_kigou_data(const std::string& str);
		// 真偽値データを生成する
		data_t *create_boolean_data(bool is_true);
		// ラムダ式データを生成する
		data_t *create_lambda_data(
			const std::vector<std::string>& karihikisu,
			const std::vector<data_t*>& hontai,bool is_kahencho,kankyo_t* kankyo);
		// consセルデータを生成する
		data_t *create_cons_data(data_t* car,data_t* cdr);
		// 組込み関数データを生成する
		data_t *create_native_func_data(p_native_func native_func,bool tokusyu_keisiki=false);
		// '()データを生成する
		data_t *create_null_data();
		// 生成器のインスタンスを得る
		static creater_t& creater(void);
};

#endif
