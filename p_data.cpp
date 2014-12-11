#include "p_data.h"
#include "creater.h"

bool p_data_config_t::do_auto_delete=true;

bool p_data_t::check_sansyo(data_t *p) const {
	return p==NULL || creater_t::creater().is_exist_data(p);
}

void p_data_t::increment_sansyo_count(data_t *p) {
	if(p!=NULL)p->sansyo_count++;
}

void p_data_t::decrement_sansyo_count(data_t *p) {
	if(p!=NULL) {
		p->sansyo_count--;
		if(do_auto_delete && p->sansyo_count<=0) {
			creater_t::creater().delete_data(p);
		}
	}
}
bool p_data_t::check_sansyo() const {return check_sansyo(sansyo);}
void p_data_t::increment_sansyo_count() {increment_sansyo_count(sansyo);}
void p_data_t::decrement_sansyo_count() {decrement_sansyo_count(sansyo);}

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
	data_t *old_sansyo=sansyo;
	sansyo=san.sansyo;
	if(!check_sansyo())return *this;
	increment_sansyo_count();
	if(check_sansyo(old_sansyo))decrement_sansyo_count(old_sansyo);
	return *this;
}

p_data_t& p_data_t::operator=(data_t* data) {
	data_t *old_sansyo=sansyo;
	sansyo=data;
	if(!check_sansyo())return *this;
	increment_sansyo_count();
	if(check_sansyo(old_sansyo))decrement_sansyo_count(old_sansyo);
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
