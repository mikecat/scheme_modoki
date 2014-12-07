#ifndef GLOBAL_CONFIG_H_GUARD_7542E416_3579_4B33_96C3_7197FD598806
#define GLOBAL_CONFIG_H_GUARD_7542E416_3579_4B33_96C3_7197FD598806

#include "stream_reader.h"

class global_config {
	private:
		static global_config gc;

		bool do_syouryaku;
		file_reader stdin_reader;

		global_config();
		~global_config(){}
	public:
		// インスタンスを取得する
		static global_config& get_gc();
		// 省略できるconsセルを省略するかを設定/取得する
		void set_do_syouryaku(bool enable);
		bool get_do_syouryaku() const;
		// 標準入力を読み込むクラスを得る
		file_reader& get_stdin_reader();
};

#endif
