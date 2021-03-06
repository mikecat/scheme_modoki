#include <cstdio>
#include <vector>
#include "creater.h"

creater_t creater_t::cr;

creater_t::~creater_t() {
	while(!data_log.empty()) {
		std::set<data_t*>::iterator it=data_log.begin();
		data_t* p=*it;
		data_log.erase(it);
		delete p;
	}
}

void creater_t::print_statistics() {
	std::map<DATATYPE, std::pair<const char*, unsigned int> > data_counter;
	for(std::set<data_t*>::iterator it=data_log.begin();it!=data_log.end();it++) {
		if(data_counter.find((*it)->get_type())==data_counter.end()) {
			data_counter[(*it)->get_type()]=
				std::make_pair<const char*, unsigned int>((*it)->get_name(),1);
		} else {
			data_counter[(*it)->get_type()].second++;
		}
	}
	printf("total number of data = %u\n",(unsigned int)data_log.size());
	for(std::map<DATATYPE, std::pair<const char*, unsigned int> >::iterator it=data_counter.begin();
	it!=data_counter.end();it++) {
		printf("number of %s = %u\n",it->second.first,it->second.second);
	}
}

size_t creater_t::get_number_of_data() {
	return data_log.size();
}

p_data_t creater_t::copy_data(const p_data_t& data) {
	data_t *new_data;
	switch(data->get_type()) {
		case DT_EOF:
			new_data=new eof_t;
			*((eof_t*)&*new_data)=*((eof_t*)&*data);
			break;
		case DT_EXIT:
			new_data=new exit_t;
			*((exit_t*)&*new_data)=*((exit_t*)&*data);
			break;
		case DT_ERROR:
			new_data=new error_t;
			*((error_t*)&*new_data)=*((error_t*)&*data);
			break;
		case DT_NUMBER:
			new_data=new number_t;
			*((number_t*)&*new_data)=*((number_t*)&*data);
			break;
		case DT_KIGOU:
			new_data=new kigou_t;
			*((kigou_t*)&*new_data)=*((kigou_t*)&*data);
			break;
		case DT_BOOLEAN:
			new_data=new boolean_t;
			*((boolean_t*)&*new_data)=*((boolean_t*)&*data);
			break;
		case DT_CONS:
			new_data=new cons_t;
			*((cons_t*)&*new_data)=*((cons_t*)&*data);
			break;
		case DT_NATIVE_FUNC:
			new_data=new native_func_t;
			*((native_func_t*)&*new_data)=*((native_func_t*)&*data);
			break;
		case DT_NULL:
			new_data=new null_t;
			*((null_t*)&*new_data)=*((null_t*)&*data);
			break;
		case DT_DELAY:
			new_data=new delay_t;
			*((delay_t*)&*new_data)=*((delay_t*)&*data);
			break;
		case DT_CONTINUATION:
			new_data=new continuation_t;
			*((continuation_t*)&*new_data)=*((continuation_t*)&*data);
			break;
		case DT_KANKYO:
			new_data=new kankyo_t;
			*((kankyo_t*)&*new_data)=*((kankyo_t*)&*data);
			break;
		default:
			return NULL;
	}
	data_log.insert(new_data);
	return new_data;
}

p_data_t creater_t::create_eof() {
	eof_t *new_data=new eof_t;
	data_log.insert(new_data);
	new_data->force_return_flag=true;
	return new_data;
}

p_data_t creater_t::create_exit(int exit_code) {
	exit_t *new_data=new exit_t;
	data_log.insert(new_data);
	new_data->force_return_flag=true;
	new_data->exit_code=exit_code;
	return new_data;
}

p_data_t creater_t::create_error(const std::string& str) {
	error_t *new_data=new error_t;
	data_log.insert(new_data);
	new_data->force_return_flag=true;
	new_data->error_mes=str;
	return new_data;
}

p_data_t creater_t::create_argument_number_error(
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
	return create_error(message);
}

p_data_t creater_t::create_number(double number) {
	number_t *new_data=new number_t;
	data_log.insert(new_data);
	new_data->number=number;
	return new_data;
}

p_data_t creater_t::create_kigou(const std::string& str) {
	kigou_t *new_data=new kigou_t;
	data_log.insert(new_data);
	new_data->kigou=str;
	return new_data;
}

p_data_t creater_t::create_boolean(bool is_true) {
	boolean_t *new_data=new boolean_t;
	data_log.insert(new_data);
	new_data->is_true=is_true;
	return new_data;
}

p_data_t creater_t::create_lambda(
const std::vector<std::string>& karihikisu,
const std::vector<p_data_t>& hontai,bool is_kahencho,const p_data_t& kankyo) {
	lambda_t *new_data=new lambda_t;
	data_log.insert(new_data);
	new_data->karihikisu=karihikisu;
	new_data->hontai=hontai;
	new_data->lambda_kankyo=kankyo;
	new_data->is_kahencho=is_kahencho;
	return new_data;
}

p_data_t creater_t::create_cons(const p_data_t& car,const p_data_t& cdr) {
	cons_t *new_data=new cons_t;
	data_log.insert(new_data);
	new_data->cons_car=car;
	new_data->cons_cdr=cdr;
	return new_data;
}

p_data_t creater_t::create_native_func(p_native_func native_func,bool tokusyu_keisiki) {
	native_func_t* new_data=new native_func_t;
	data_log.insert(new_data);
	new_data->native_func=native_func;
	new_data->tokusyu_keisiki=tokusyu_keisiki;
	return new_data;
}

p_data_t creater_t::create_null() {
	null_t *new_data=new null_t;
	data_log.insert(new_data);
	return new_data;
}

p_data_t creater_t::create_delay(const p_data_t& expr,const p_data_t kankyo) {
	delay_t *new_data=new delay_t;
	data_log.insert(new_data);
	new_data->expr=expr;
	new_data->kankyo=kankyo;
	return new_data;
}

p_data_t creater_t::create_continuation(const p_data_t& next_continuation,
bool need_apply,const p_data_t& kankyo,
const std::vector<p_data_t>& evaluated_elements,
const std::vector<p_data_t>& elements_to_evaluate) {
	continuation_t *new_data=new continuation_t;
	data_log.insert(new_data);
	new_data->next_continuation=next_continuation;
	new_data->need_apply=need_apply;
	new_data->kankyo=kankyo;
	new_data->evaluated_elements=evaluated_elements;
	new_data->elements_to_evaluate=elements_to_evaluate;
	return new_data;
}

p_data_t creater_t::create_empty_continuation() {
	continuation_t *new_data=new continuation_t;
	data_log.insert(new_data);
	new_data->next_continuation=NULL;
	new_data->need_apply=false;
	new_data->kankyo=NULL;
	return new_data;
}

p_data_t creater_t::create_kankyo(const p_data_t& oya) {
	kankyo_t *new_data=new kankyo_t;
	data_log.insert(new_data);
	new_data->parent=oya;
	return new_data;
}

bool creater_t::is_exist_data(data_t* data) {
	return data_log.find(data)!=data_log.end();
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

void creater_t::delete_zero_sansyo() {
	std::vector<data_t*> data_to_delete;
	for(std::set<data_t*>::iterator it=data_log.begin();it!=data_log.end();it++) {
		if((*it)->sansyo_count<=0)data_to_delete.push_back(*it);
	}
	for(std::vector<data_t*>::iterator it=data_to_delete.begin();
	it!=data_to_delete.end();it++) {
		data_log.erase(*it);
		delete *it;
	}
}

creater_t& creater_t::creater(void) {
	return cr;
}
