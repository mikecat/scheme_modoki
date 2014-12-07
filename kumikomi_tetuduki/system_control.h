#ifndef SYSTEM_CONTROL_H_GUARD_6406AA72_E3D6_4F38_AD5E_636B48C79E7C
#define SYSTEM_CONTROL_H_GUARD_6406AA72_E3D6_4F38_AD5E_636B48C79E7C

#include <vector>
#include "../p_data.h"

// 省略できるconsセルの表示を省略するかを設定する
p_data_t set_enable_syouryaku(const std::vector<p_data_t>& args,p_data_t&);
// 省略できるconsセルの表示を省略するかを取得する
p_data_t get_enable_syouryaku(const std::vector<p_data_t>& args,p_data_t&);
// 生成され、削除されていない環境とデータの数を表示する
p_data_t print_statistics(const std::vector<p_data_t>& args,p_data_t&);
// 環境とデータの自動削除の有効/無効を設定する
p_data_t set_enable_auto_delete(const std::vector<p_data_t>& args,p_data_t&);
// 環境とデータの自動削除の有効/無効を取得する
p_data_t get_enable_auto_delete(const std::vector<p_data_t>& args,p_data_t&);
// 参照カウントが0以下の環境とデータを削除する
p_data_t delete_zero_sansyo(const std::vector<p_data_t>& args,p_data_t&) ;

#endif
