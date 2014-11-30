#include "sansyo_count.h"

template<>
bool sansyo_t<kankyo_t>::check_sansyo() const {
	return sansyo==NULL || creater_t::creater().is_exist_kankyo(sansyo);
}

template<>
bool sansyo_t<data_t>::check_sansyo() const {
	return sansyo==NULL || creater_t::creater().is_exist_data(sansyo);
}

template<>
void sansyo_t<kankyo_t>::decrement_sansyo_count() {
	if(sansyo!=NULL) {
		sansyo->sansyo_count--;
		if(sansyo->sansyo_count<=0) {
			creater_t::creater().delete_kankyo(sansyo);
			sansyo=NULL;
		}
	}
}

template<>
void sansyo_t<data_t>::decrement_sansyo_count() {
	if(sansyo!=NULL) {
		sansyo->sansyo_count--;
		if(sansyo->sansyo_count<=0) {
			creater_t::creater().delete_data(sansyo);
			sansyo=NULL;
		}
	}
}
