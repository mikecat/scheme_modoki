#ifndef SPECIAL_FORM_H_GUARD_E0BC3500_9AA0_4786_8160_0763B25B1B2C
#define SPECIAL_FORM_H_GUARD_E0BC3500_9AA0_4786_8160_0763B25B1B2C

#include "../sansyo_count.h"

// 引数(未評価)を返す
p_data_t quote_proc(const std::vector<p_data_t>& args,p_data_t&);
// 新規束縛の作成
p_data_t define_proc(const std::vector<p_data_t>& args,p_data_t& kankyo);
// 代入
p_data_t set_proc(const std::vector<p_data_t>& args,p_data_t& kankyo);
// 新規手続きの作成
p_data_t lambda_proc(const std::vector<p_data_t>& args,p_data_t& kankyo);
// 条件分岐
p_data_t if_proc(const std::vector<p_data_t>& args,p_data_t& kankyo);
// 途中に1個でも#fがあれば#f、無ければ最後の値を返す
p_data_t and_proc(const std::vector<p_data_t>& args,p_data_t& kankyo);
// 途中に1個でも#f以外があればその値、無ければ#fを返す
p_data_t or_proc(const std::vector<p_data_t>& args,p_data_t& kankyo);

#endif
