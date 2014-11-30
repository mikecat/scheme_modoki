#ifndef NUMBER_CALC_H_GUARD_0CC22007_3B46_4D34_AA45_6D0EF5E9777D
#define NUMBER_CALC_H_GUARD_0CC22007_3B46_4D34_AA45_6D0EF5E9777D

#include <vector>
struct data_t;
struct kankyo_t;

// 足し算 例: (+ 1 2) => 3
data_t* add(const std::vector<data_t*>& args,kankyo_t*);
// 引き算 例: (- 1 2) => -1
data_t* sub(const std::vector<data_t*>& args,kankyo_t*);
// 掛け算 例: (* 2 3) => 6
data_t* mul(const std::vector<data_t*>& args,kankyo_t*);
// 割り算 例: (/ 4 2) => 2
data_t* div_func(const std::vector<data_t*>& args,kankyo_t*);
// 商の整数部分
data_t* quotient(const std::vector<data_t*>& args,kankyo_t*);
// 比較「同じ」
data_t* num_eq(const std::vector<data_t*>& args,kankyo_t*);
// 比較「小さい」
data_t* num_lt(const std::vector<data_t*>& args,kankyo_t*);
// 比較「以下」
data_t* num_leq(const std::vector<data_t*>& args,kankyo_t*);
// 比較「大きい」
data_t* num_gt(const std::vector<data_t*>& args,kankyo_t*);
// 比較「以上」
data_t* num_geq(const std::vector<data_t*>& args,kankyo_t*);
// 引数が偶数なら#tを、偶数以外なら#fを返す
data_t* is_even(const std::vector<data_t*>& args,kankyo_t*);
// 引数が奇数なら#tを、奇数以外なら#fを返す
data_t* is_odd(const std::vector<data_t*>& args,kankyo_t*);
// 引数の平方根を返す
data_t* sqrt_func(const std::vector<data_t*>& args,kankyo_t*);

#endif
