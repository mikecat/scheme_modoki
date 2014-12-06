#include "p_data.h"
#include "creater.h"

bool p_data_config_t::do_auto_delete=true;

bool p_data_t::check_sansyo() const {
	return sansyo==NULL || creater_t::creater().is_exist_data(sansyo);
}

void p_data_t::increment_sansyo_count() {
	if(sansyo!=NULL)sansyo->sansyo_count++;
}

void p_data_t::decrement_sansyo_count() {
	if(sansyo!=NULL) {
		sansyo->sansyo_count--;
		if(do_auto_delete && sansyo->sansyo_count<=0) {
			creater_t::creater().delete_data(sansyo);
			sansyo=NULL;
		}
	}
}

p_data_t::p_data_t(data_t* data) {
	sansyo=data;
	if(!check_sansyo())return;
	increment_sansyo_count();
}

p_data_t::p_data_t(const p_data_t& san) {
	sansyo=san.sansyo;
	if(!check_sansyo())return;
	increment_sansyo_count();
}

p_data_t::~p_data_t() {
	if(!check_sansyo())return;
	decrement_sansyo_count();
}

p_data_t& p_data_t::operator=(const p_data_t& san) {
	if(!check_sansyo())return *this;
	decrement_sansyo_count();
	sansyo=san.sansyo;
	if(!check_sansyo())return *this;
	increment_sansyo_count();
	return *this;
}

p_data_t& p_data_t::operator=(data_t* data) {
	if(!check_sansyo())return *this;
	decrement_sansyo_count();
	sansyo=data;
	if(!check_sansyo())return *this;
	increment_sansyo_count();
	return *this;
}

bool p_data_t::operator==(const p_data_t& san) const {
	return sansyo==san.sansyo;
}

bool p_data_t::operator<(const p_data_t& san) const {
	return sansyo<san.sansyo;
}

bool p_data_t::is_null() const {
	if(!check_sansyo())return true;
	return sansyo==NULL;
}

data_t& p_data_t::operator*() const {
	if(!check_sansyo()) {
		static null_t tmp;
		return tmp;
	}
	return *sansyo;
}

data_t* const& p_data_t::operator->() const {
	if(!check_sansyo())return NULL;
	return sansyo;
}
