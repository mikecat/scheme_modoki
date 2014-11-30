#ifndef SANSYO_COUNT_G_GUARD_E44496BE_324B_4675_AAF3_087F670507B4
#define SANSYO_COUNT_G_GUARD_E44496BE_324B_4675_AAF3_087F670507B4

// 参照カウントを持つクラス
struct with_sansyo_count_t {
	int sansyo_count;
};

// 参照カウントの操作を行う「ポインタ」
template<typename T>
class sansyo_t {
	private:
		T* sansyo;
	public:
		sansyo_t(): sansyo(NULL) {}

		sansyo_t(T* data) {
			sansyo=data;
			if(sansyo!=NULL)sansyo->sansyo_count++;
		}

		sansyo_t(const sansyo_t<T>& san) {
			sansyo=san.sansyo;
			if(sansyo!=NULL)sansyo->sansyo_count++;
		}

		~sansyo_t() {
			if(sansyo!=NULL)sansyo->sansyo_count--;
		}

		sansyo_t& operator=(const sansyo_t<T>& san) {
			if(sansyo!=NULL)sansyo->sansyo_count--;
			sansyo=san.sansyo;
			if(sansyo!=NULL)sansyo->sansyo_count++;
			return *this;
		}

		sansyo_t& operator=(T* data) {
			if(sansyo!=NULL)sansyo->sansyo_count--;
			sansyo=data;
			if(sansyo!=NULL)sansyo->sansyo_count++;
		}

		bool operator==(const sansyo_t<T>& san) const {
			return sansyo==san.sansyo;
		}

		bool operator<(const sansyo_t<T>& san) const {
			return sansyo<san.sansyo;
		}

		T& operator*() const {
			return *sansyo;
		}
};

#endif
