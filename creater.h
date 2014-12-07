#ifndef CREATER_H_GUARD_85D8EF3B_8D4D_4B3A_824C_8B783B6BE20D
#define CREATER_H_GUARD_85D8EF3B_8D4D_4B3A_824C_8B783B6BE20D

#include <set>
#include "data.h"

class creater_t {
	private:
		static creater_t cr;
		std::set<data_t*> data_log;
		creater_t(){}
		~creater_t();
	public:
		// 生成し、削除されていないデータの種類ごとの数を表示する
		void print_statistics();
		// 生成し、削除されていないデータの数を取得する
		size_t get_number_of_data();

		// EOFデータを生成する
		p_data_t create_eof_data();
		// 終了指示データを生成する
		p_data_t create_exit_data(int exit_code);
		// エラーデータを生成する
		p_data_t create_error_data(const std::string& str);
		// 引数の数に関するエラーを生成する
		p_data_t create_argument_number_error_data(
			const std::string& name,size_t min_number,size_t got_number,bool is_kahencho);
		// 数値データを生成する
		p_data_t create_number_data(double number);
		// 記号データを生成する
		p_data_t create_kigou_data(const std::string& str);
		// 真偽値データを生成する
		p_data_t create_boolean_data(bool is_true);
		// ラムダ式データを生成する
		p_data_t create_lambda_data(
			const std::vector<std::string>& karihikisu,
			const std::vector<p_data_t>& hontai,bool is_kahencho,const p_data_t& kankyo);
		// consセルデータを生成する
		p_data_t create_cons_data(const p_data_t& car,const p_data_t& cdr);
		// 組込み関数データを生成する
		p_data_t create_native_func_data(p_native_func native_func,bool tokusyu_keisiki=false);
		// '()データを生成する
		p_data_t create_null_data();
		// 環境データを生成する
		p_data_t create_kankyo_data(const p_data_t& oya=NULL);

		// データが存在するか確認する
		bool is_exist_data(data_t* data);

		// データを削除する
		void delete_data(data_t* data);
		// 参照カウントが0以下のデータを削除する
		void delete_zero_sansyo();

		// 生成器のインスタンスを得る
		static creater_t& creater(void);
};

#endif
