#ifndef SPECIAL_FORM_H_GUARD_E0BC3500_9AA0_4786_8160_0763B25B1B2C
#define SPECIAL_FORM_H_GUARD_E0BC3500_9AA0_4786_8160_0763B25B1B2C

#include <vector>
#include "../p_data.h"

// 引数(未評価)を返す
p_data_t quote_proc(const std::vector<p_data_t>& args,p_data_t&,p_data_t&);
// 新規束縛の作成
p_data_t define_proc(const std::vector<p_data_t>& args,p_data_t& kankyo,p_data_t& cont);
// 代入
p_data_t set_proc(const std::vector<p_data_t>& args,p_data_t& kankyo,p_data_t& cont);
// 新規手続きの作成
p_data_t lambda_proc(const std::vector<p_data_t>& args,p_data_t& kankyo,p_data_t&);
// 条件分岐
p_data_t if_proc(const std::vector<p_data_t>& args,p_data_t& kankyo,p_data_t& cont);
// 途中に1個でも#fがあれば#f、無ければ最後の値を返す
p_data_t and_proc(const std::vector<p_data_t>& args,p_data_t& kankyo,p_data_t& cont);
// 途中に1個でも#f以外があればその値、無ければ#fを返す
p_data_t or_proc(const std::vector<p_data_t>& args,p_data_t& kankyo,p_data_t& cont);
// 各引数のリストの最初の値が真なら、そのリストの式を評価し、それを返す
p_data_t cond_proc(const std::vector<p_data_t>& args,p_data_t& kankyo,p_data_t& cont);
// 束縛を作ってから、本体を評価する
p_data_t let_proc(const std::vector<p_data_t>& args,p_data_t& kankyo,p_data_t& cont);
// 束縛を順番に作ってから、本体を評価する
p_data_t let_star_proc(const std::vector<p_data_t>& args,p_data_t& kankyo,p_data_t& cont);
// 引数を順に評価する
p_data_t begin_proc(const std::vector<p_data_t>& args,p_data_t& kankyo,p_data_t& cont);
// 遅延オブジェクトを生成する
p_data_t delay_proc(const std::vector<p_data_t>& args,p_data_t& kankyo,p_data_t&);

#endif
