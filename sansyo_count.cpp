#include "sansyo_count.h"

template<>
void sansyo_t<kankyo_t>::decrement_sansyo_count() {
	if(sansyo!=NULL) {
		sansyo->sansyo_count--;
		if(sansyo->sansyo_count<=0)creater_t::creater().delete_kankyo(sansyo);
	}
}

template<>
void sansyo_t<data_t>::decrement_sansyo_count() {
	if(sansyo!=NULL) {
		sansyo->sansyo_count--;
		if(sansyo->sansyo_count<=0)creater_t::creater().delete_data(sansyo);
	}
}
