#ifndef CONS_AND_LIST_H_GUARD_5150F043_822B_45B9_BDB2_B685D8616C38
#define CONS_AND_LIST_H_GUARD_5150F043_822B_45B9_BDB2_B685D8616C38

#include <vector>
struct data_t;
struct kankyo_t;

// consセルを作成する
data_t* cons(const std::vector<data_t*>& args,kankyo_t*);
// consセルの前の要素
data_t* car(const std::vector<data_t*>& args,kankyo_t*);
// consセルの後ろの要素
data_t* cdr(const std::vector<data_t*>& args,kankyo_t*);
// consセルの前の要素を設定する
data_t* set_car(const std::vector<data_t*>& args,kankyo_t*);
// consセルの後ろの要素を設定する
data_t* set_cdr(const std::vector<data_t*>& args,kankyo_t*);
// 引数が'()かを判定する
data_t* is_null(const std::vector<data_t*>& args,kankyo_t*);

#endif
