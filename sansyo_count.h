#ifndef SANSYO_COUNT_G_GUARD_E44496BE_324B_4675_AAF3_087F670507B4
#define SANSYO_COUNT_G_GUARD_E44496BE_324B_4675_AAF3_087F670507B4

#include <cstdio>

// 参照カウントを持つクラス
struct with_sansyo_count_t {
	int sansyo_count;
	with_sansyo_count_t(): sansyo_count(0) {}
};

// 参照カウントの操作を行う「ポインタ」
template<typename T>
class sansyo_t {
	private:
		T* sansyo;
		bool check_sansyo() const;
		void increment_sansyo_count() {
			if(sansyo!=NULL)sansyo->sansyo_count++;
		}
		void decrement_sansyo_count();
	public:
		sansyo_t(): sansyo(NULL) {}

		sansyo_t(T* data) {
			sansyo=data;
			if(!check_sansyo())return;
			increment_sansyo_count();
		}

		sansyo_t(const sansyo_t<T>& san) {
			sansyo=san.sansyo;
			if(!check_sansyo())return;
			increment_sansyo_count();
		}

		~sansyo_t() {
			if(!check_sansyo())return;
			decrement_sansyo_count();
		}

		sansyo_t& operator=(const sansyo_t<T>& san) {
			if(!check_sansyo())return *this;
			decrement_sansyo_count();
			sansyo=san.sansyo;
			if(!check_sansyo())return *this;
			increment_sansyo_count();
			return *this;
		}

		sansyo_t& operator=(T* data) {
			if(!check_sansyo())return *this;
			decrement_sansyo_count();
			sansyo=data;
			if(!check_sansyo())return *this;
			increment_sansyo_count();
		}

		bool operator==(const sansyo_t<T>& san) const {
			return sansyo==san.sansyo;
		}

		bool operator<(const sansyo_t<T>& san) const {
			return sansyo<san.sansyo;
		}

		bool is_null() const {
			if(!check_sansyo())return true;
			return sansyo==NULL;
		}

		T& operator*() const {
			if(!check_sansyo()) {
				static T tmp;
				return tmp;
			}
			return *sansyo;
		}

		T* const& operator->() const {
			if(!check_sansyo())return NULL;
			return sansyo;
		}
};

#include "creater.h"

template<typename T>
bool sansyo_t<T>::check_sansyo() const {
	return true; // 一般のオブジェクトに対しては判定不能
}

template<>
bool sansyo_t<kankyo_t>::check_sansyo() const;

template<>
bool sansyo_t<data_t>::check_sansyo() const;

template<typename T>
void sansyo_t<T>::decrement_sansyo_count() {
	if(sansyo!=NULL)sansyo->sansyo_count--;
}

template<>
void sansyo_t<kankyo_t>::decrement_sansyo_count();

template<>
void sansyo_t<data_t>::decrement_sansyo_count();

#endif
