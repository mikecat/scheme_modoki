#ifndef SANSYO_COUNT_G_GUARD_E44496BE_324B_4675_AAF3_087F670507B4
#define SANSYO_COUNT_G_GUARD_E44496BE_324B_4675_AAF3_087F670507B4

#include <cstdio>

// 参照カウントを持つクラス
struct with_sansyo_count_t {
	int sansyo_count;
};

// 参照カウントの操作を行う「ポインタ」
template<typename T>
class sansyo_t {
	private:
		T* sansyo;
		void increment_sansyo_count() {
			if(sansyo!=NULL)sansyo->sansyo_count++;
		}
		void decrement_sansyo_count();
	public:
		sansyo_t(): sansyo(NULL) {}

		sansyo_t(T* data) {
			sansyo=data;
			increment_sansyo_count();
		}

		sansyo_t(const sansyo_t<T>& san) {
			sansyo=san.sansyo;
			increment_sansyo_count();
		}

		~sansyo_t() {
			decrement_sansyo_count();
		}

		sansyo_t& operator=(const sansyo_t<T>& san) {
			decrement_sansyo_count();
			sansyo=san.sansyo;
			increment_sansyo_count();
			return *this;
		}

		sansyo_t& operator=(T* data) {
			decrement_sansyo_count();
			sansyo=data;
			increment_sansyo_count();
		}

		bool operator==(const sansyo_t<T>& san) const {
			return sansyo==san.sansyo;
		}

		bool operator<(const sansyo_t<T>& san) const {
			return sansyo<san.sansyo;
		}

		bool is_null() const {
			return sansyo==NULL;
		}

		T& operator*() const {
			return *sansyo;
		}

		T* const& operator->() const {
			return sansyo;
		}
};

#include "creater.h"

template<typename T>
void sansyo_t<T>::decrement_sansyo_count() {
	if(sansyo!=NULL)sansyo->sansyo_count--;
}

template<>
void sansyo_t<kankyo_t>::decrement_sansyo_count();

template<>
void sansyo_t<data_t>::decrement_sansyo_count();

#endif
