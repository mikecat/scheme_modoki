#ifndef SANSYO_COUNT_G_GUARD_E44496BE_324B_4675_AAF3_087F670507B4
#define SANSYO_COUNT_G_GUARD_E44496BE_324B_4675_AAF3_087F670507B4

class p_data_config_t {
	protected:
		static bool do_auto_delete;
	public:
		static void set_do_auto_delete(bool is_do) {
			do_auto_delete=is_do;
		}

		static bool get_do_auto_delete() {
			return do_auto_delete;
		}
};

struct data_t;

// 参照カウントの操作を行う「ポインタ」
class p_data_t: public p_data_config_t {
	private:
		data_t* sansyo;
		bool check_sansyo(data_t *p) const;
		void increment_sansyo_count(data_t *p);
		void decrement_sansyo_count(data_t *p);
		bool check_sansyo() const;
		void increment_sansyo_count();
		void decrement_sansyo_count();
	public:
		p_data_t(): sansyo() {}
		p_data_t(data_t* data);
		p_data_t(const p_data_t& san);
		~p_data_t();
		p_data_t& operator=(const p_data_t& san);
		p_data_t& operator=(data_t* data);
		bool operator==(const p_data_t& san) const;
		bool operator<(const p_data_t& san) const;
		bool is_null() const;
		data_t& operator*() const;
		data_t* const& operator->() const;
};

#endif
