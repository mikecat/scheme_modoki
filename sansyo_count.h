#ifndef SANSYO_COUNT_G_GUARD_E44496BE_324B_4675_AAF3_087F670507B4
#define SANSYO_COUNT_G_GUARD_E44496BE_324B_4675_AAF3_087F670507B4

#include <vector>
#include <string>
#include <map>

// データの種類
enum DATATYPE {
	DT_EOF, // (入力の終わり)
	DT_ERROR, // エラー
	DT_NUM, // 数値
	DT_KIGOU, // 記号
	DT_BOOLEAN, // 真偽値
	DT_LAMBDA, // ラムダ式
	DT_CONS, // consセル
	DT_NATIVE_FUNC, // 組み込み手続き
	DT_NULL, // '()
	DT_KANKYO
};

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
		bool check_sansyo() const;
		void increment_sansyo_count();
		void decrement_sansyo_count();
	public:
		p_data_t(): sansyo(NULL) {}
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

typedef p_data_t(*p_native_func)(const std::vector<p_data_t>& args,p_data_t& kankyo);

// データを表す構造体(参照カウントを持つ)
struct data_t {
	int sansyo_count;
	data_t(): sansyo_count(0) {}

	DATATYPE type;
	// DT_ERROR
	std::string error_mes;
	bool please_exit;
	// DT_NUM
	double num;
	// DT_KIGOU
	std::string kigou;
	// DT_BOOLEAN
	bool is_true;
	// DT_LAMBDA
	std::vector<std::string> karihikisu;
	std::vector<p_data_t> hontai;
	p_data_t lambda_kankyo;
	bool is_kahencho;
	// DT_CONS
	p_data_t cons_car;
	p_data_t cons_cdr;
	// DT_NATIVE_FUNC
	p_native_func native_func;
	bool tokusyu_keisiki;
	// DT_KANKYO
	p_data_t parent;
	std::map<std::string, p_data_t> sokubaku;
};

#endif
