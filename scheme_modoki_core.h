#ifndef SCHEME_MODOKI_CORE_H_GUARD_89E598DB_24E2_42B4_BB04_ABDD8A9D91E2
#define SCHEME_MODOKI_CORE_H_GUARD_89E598DB_24E2_42B4_BB04_ABDD8A9D91E2

#include <string>
#include <vector>
#include "p_data.h"
#include "stream_reader.h"

// 名前の規則を用いて名前を探し、そのポインタを返す
p_data_t* namae_no_kisoku2(const std::string& namae,p_data_t& kankyo);
// 名前の規則を用いて名前を探し、そのデータを返す
p_data_t namae_no_kisoku(const std::string& namae,p_data_t& kankyo);
// 適用の規則を用いて手続きを適用する
p_data_t apply_proc(const p_data_t& proc, const std::vector<p_data_t>& args,p_data_t& kankyo);
// 記号やリストで表現された式データを評価する
p_data_t evaluate(const p_data_t& data,p_data_t& kankyo);
// 入力をパースしてquoteされた状態のリストを得る
p_data_t parse(stream_reader& sr);

// 大域環境を生成する
void make_taiiki_kankyo();
// 大域環境の開放を許可する
void delete_taiiki_kankyo();
// 大域環境でスクリプトを実行する
int run_script(stream_reader& sr,bool is_interactive);

#endif
