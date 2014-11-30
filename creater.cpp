#include <cstdio>
#include "sansyo_count.h"

creater_t creater_t::cr;

creater_t::~creater_t() {
	print_number_of_kankyo_and_data();
	while(!kankyo_log.empty()) {
		std::set<kankyo_t*>::iterator it=kankyo_log.begin();
		kankyo_t* p=*it;
		kankyo_log.erase(it);
		delete p;
	}
	while(!data_log.empty()) {
		std::set<data_t*>::iterator it=data_log.begin();
		data_t* p=*it;
		data_log.erase(it);
		delete p;
	}
}

void creater_t::print_number_of_kankyo_and_data() {
	printf("number of kankyo = %u\n",(unsigned int)kankyo_log.size());
	printf("number of data   = %u\n",(unsigned int)data_log.size());
}

p_kankyo_t creater_t::create_kankyo(const p_kankyo_t& oya) {
	kankyo_t* new_kankyo=new kankyo_t(oya);
	kankyo_log.insert(new_kankyo);
	return new_kankyo;
}

data_t* creater_t::create_raw_data() {
	data_t *new_data=new data_t();
	data_log.insert(new_data);
	return new_data;
}

p_data_t creater_t::create_data() {
	return create_raw_data();
}

p_data_t creater_t::create_eof_data() {
	data_t *new_data=create_raw_data();
	new_data->type=DT_EOF;
	return new_data;
}

p_data_t creater_t::create_error_data(const std::string& str,bool please_exit) {
	data_t *new_data=create_raw_data();
	new_data->type=DT_ERROR;
	new_data->error_mes=str;
	new_data->please_exit=please_exit;
	return new_data;
}

p_data_t creater_t::create_argument_number_error_data(
const std::string& name,size_t min_number,size_t got_number,bool is_kahencho) {
	std::string message="invalid number of arguments for "+name+" : expected ";
	char buffer[16];
	if(is_kahencho) {
		message+=" at least ";
	}
	sprintf(buffer,"%u",(unsigned int)min_number);
	message+=buffer;
	message+=", got ";
	sprintf(buffer,"%u",(unsigned int)got_number);
	message+=buffer;
	return create_error_data(message,false);
}

p_data_t creater_t::create_num_data(double num) {
	data_t *new_data=create_raw_data();
	new_data->type=DT_NUM;
	new_data->num=num;
	return new_data;
}

p_data_t creater_t::create_kigou_data(const std::string& str) {
	data_t *new_data=create_raw_data();
	new_data->type=DT_KIGOU;
	new_data->kigou=str;
	return new_data;
}

p_data_t creater_t::create_boolean_data(bool is_true) {
	data_t *new_data=create_raw_data();
	new_data->type=DT_BOOLEAN;
	new_data->is_true=is_true;
	return new_data;
}

p_data_t creater_t::create_lambda_data(
const std::vector<std::string>& karihikisu,
const std::vector<p_data_t>& hontai,bool is_kahencho,const p_kankyo_t& kankyo) {
	data_t *new_data=create_raw_data();
	new_data->type=DT_LAMBDA;
	new_data->karihikisu=karihikisu;
	new_data->hontai=hontai;
	new_data->lambda_kankyo=kankyo;
	new_data->is_kahencho=is_kahencho;
	return new_data;
}

p_data_t creater_t::create_cons_data(const p_data_t& car,const p_data_t& cdr) {
	data_t *new_data=create_raw_data();
	new_data->type=DT_CONS;
	new_data->cons_car=car;
	new_data->cons_cdr=cdr;
	return new_data;
}

p_data_t creater_t::create_native_func_data(p_native_func native_func,bool tokusyu_keisiki) {
	data_t* new_data=create_raw_data();
	new_data->type=DT_NATIVE_FUNC;
	new_data->native_func=native_func;
	new_data->tokusyu_keisiki=tokusyu_keisiki;
	return new_data;
}

p_data_t creater_t::create_null_data() {
	data_t *new_data=create_raw_data();
	new_data->type=DT_NULL;
	return new_data;
}
bool creater_t::is_exist_kankyo(kankyo_t* kankyo) {
	return kankyo_log.find(kankyo)!=kankyo_log.end();
}

bool creater_t::is_exist_data(data_t* data) {
	return data_log.find(data)!=data_log.end();
}

void creater_t::delete_kankyo(kankyo_t* kankyo) {
	if(kankyo->sansyo_count<=0) {
		std::set<kankyo_t*>::iterator it=kankyo_log.find(kankyo);
		if(it!=kankyo_log.end()) {
			kankyo_log.erase(it);
		}
		delete kankyo;
	}
}

void creater_t::delete_data(data_t* data) {
	if(data->sansyo_count<=0) {
		std::set<data_t*>::iterator it=data_log.find(data);
		if(it!=data_log.end()) {
			data_log.erase(it);
		}
		delete data;
	}
}

creater_t& creater_t::creater(void) {
	return cr;
}
