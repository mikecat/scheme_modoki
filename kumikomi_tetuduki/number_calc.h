#ifndef NUMBER_CALC_H_GUARD_0CC22007_3B46_4D34_AA45_6D0EF5E9777D
#define NUMBER_CALC_H_GUARD_0CC22007_3B46_4D34_AA45_6D0EF5E9777D

#include "../sansyo_count.h"

// 足し算 例: (+ 1 2) => 3
p_data_t add(const std::vector<p_data_t>& args,p_data_t&);
// 引き算 例: (- 1 2) => -1
p_data_t sub(const std::vector<p_data_t>& args,p_data_t&);
// 掛け算 例: (* 2 3) => 6
p_data_t mul(const std::vector<p_data_t>& args,p_data_t&);
// 割り算 例: (/ 4 2) => 2
p_data_t div_func(const std::vector<p_data_t>& args,p_data_t&);
// 商の整数部分
p_data_t quotient(const std::vector<p_data_t>& args,p_data_t&);
// 比較「同じ」
p_data_t num_eq(const std::vector<p_data_t>& args,p_data_t&);
// 比較「小さい」
p_data_t num_lt(const std::vector<p_data_t>& args,p_data_t&);
// 比較「以下」
p_data_t num_leq(const std::vector<p_data_t>& args,p_data_t&);
// 比較「大きい」
p_data_t num_gt(const std::vector<p_data_t>& args,p_data_t&);
// 比較「以上」
p_data_t num_geq(const std::vector<p_data_t>& args,p_data_t&);
// 引数が偶数なら#tを、偶数以外なら#fを返す
p_data_t is_even(const std::vector<p_data_t>& args,p_data_t&);
// 引数が奇数なら#tを、奇数以外なら#fを返す
p_data_t is_odd(const std::vector<p_data_t>& args,p_data_t&);
// 引数の平方根を返す
p_data_t sqrt_func(const std::vector<p_data_t>& args,p_data_t&);

#endif
