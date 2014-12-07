#ifndef SCHEME_MODOKI_H_GUARD_66217EC2_CAC2_42C6_82A5_13D31B0E4EA4
#define SCHEME_MODOKI_H_GUARD_66217EC2_CAC2_42C6_82A5_13D31B0E4EA4

#include "stream_reader.h"

// 省略できるconsセルの表示を省略するかを設定する
void set_do_syouryaku(bool enable);
// 省略できるconsセルの表示を省略するかを取得する
bool get_do_syouryaku();

// 大域環境でスクリプトを実行する
int run_script(stream_reader& sr,bool is_interactive);

#endif
