#ifndef CONS_AND_LIST_H_GUARD_5150F043_822B_45B9_BDB2_B685D8616C38
#define CONS_AND_LIST_H_GUARD_5150F043_822B_45B9_BDB2_B685D8616C38

#include <vector>
#include "../p_data.h"

// consセルを作成する
p_data_t cons(const std::vector<p_data_t>& args,p_data_t&);
// consセルの前の要素
p_data_t car(const std::vector<p_data_t>& args,p_data_t&);
// consセルの後ろの要素
p_data_t cdr(const std::vector<p_data_t>& args,p_data_t&);
// consセルの前の要素を設定する
p_data_t set_car(const std::vector<p_data_t>& args,p_data_t&);
// consセルの後ろの要素を設定する
p_data_t set_cdr(const std::vector<p_data_t>& args,p_data_t&);
// 引数が'()かを判定する
p_data_t is_null(const std::vector<p_data_t>& args,p_data_t&);
// 引数のリストを作成する
p_data_t list(const std::vector<p_data_t>& args,p_data_t&);

#endif
