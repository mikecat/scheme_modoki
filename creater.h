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
		kankyo_t *create_kankyo(kankyo_t* oya=NULL);
		data_t *create_data();
		data_t *create_eof_data();
		data_t *create_error_data(const std::string& str,bool please_exit=false);
		data_t *create_argument_number_error_data(
			const std::string& name,size_t min_number,size_t got_number,bool is_kahencho);
		data_t *create_num_data(double num);
		data_t *create_kigou_data(const std::string& str);
		data_t *create_boolean_data(bool is_true);
		data_t *create_lambda_data(
			const std::vector<std::string>& karihikisu,
			const std::vector<data_t*>& hontai,bool is_kahencho,kankyo_t* kankyo);
		data_t *create_cons_data(data_t* car,data_t* cdr);
		data_t *create_native_func_data(p_native_func native_func,bool tokusyu_keisiki=false);
		data_t *create_null_data();
		static creater_t& creater(void);
};

#endif
