#ifndef SCHEME_MODOKI_H_GUARD_66217EC2_CAC2_42C6_82A5_13D31B0E4EA4
#define SCHEME_MODOKI_H_GUARD_66217EC2_CAC2_42C6_82A5_13D31B0E4EA4

#include <string>
#include <vector>
#include "p_data.h"

// 省略できるconsセルの表示を省略するかを設定する
void set_do_syouryaku(bool enable);
// 省略できるconsセルの表示を省略するかを取得する
bool get_do_syouryaku();

// 名前の規則を用いて名前を探し、そのポインタを返す
p_data_t* namae_no_kisoku2(const std::string& namae,p_data_t& kankyo);
// 名前の規則を用いて名前を探し、そのデータを返す
p_data_t namae_no_kisoku(const std::string& namae,p_data_t& kankyo);
// 適用の規則を用いて手続きを適用する
p_data_t apply_proc(const p_data_t& proc, const std::vector<p_data_t>& args,p_data_t& kankyo);
// 記号やリストで表現された式データを評価する
p_data_t evaluate(const p_data_t& data,p_data_t& kankyo);

#endif
