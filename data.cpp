#include <cstdio>
#include "data.h"
#include "global_config.h"

void print_data(const p_data_t& data,bool please_syouryaku) {
	switch(data->get_type()) {
		case DT_EOF:
			printf("#<EOF>");
			break;
		case DT_EXIT:
			printf("#<exit code=%d>",((exit_t*)&*data)->exit_code);
			break;
		case DT_ERROR:
			printf("ERROR: %s",((error_t*)&*data)->error_mes.c_str());
			break;
		case DT_NUMBER:
			printf("%.15g",((number_t*)&*data)->number);
			break;
		case DT_KIGOU:
			printf("%s",((kigou_t*)&*data)->kigou.c_str());
			break;
		case DT_BOOLEAN:
			printf("#%c",((boolean_t*)&*data)->is_true?'t':'f');
			break;
		case DT_LAMBDA:
			printf("#<lambda-siki>");
			break;
		case DT_CONS:
			if(!global_config::get_gc().get_do_syouryaku() || !please_syouryaku)printf("(");
			if(!((cons_t*)&*data)->cons_car.is_null()) {
				print_data(((cons_t*)&*data)->cons_car);
			} else {
				printf("#<NULL(bug?)>");
			}
			if(!((cons_t*)&*data)->cons_cdr.is_null()) {
				if(!global_config::get_gc().get_do_syouryaku()) {
					printf(" . ");
				} else {
					if(((cons_t*)&*data)->cons_cdr->get_type()!=DT_NULL){
						printf(" ");
						if(((cons_t*)&*data)->cons_cdr->get_type()!=DT_CONS) {
							printf(". ");
						}
					}
				}
				print_data(((cons_t*)&*data)->cons_cdr,true);
			} else {
				printf(" . #<NULL(bug?)>");
			}
			if(!global_config::get_gc().get_do_syouryaku() || !please_syouryaku)printf(")");
			break;
		case DT_NATIVE_FUNC:
			printf("#<native function>");
			break;
		case DT_NULL:
			if(!global_config::get_gc().get_do_syouryaku() || !please_syouryaku)printf("()");
			break;
		case DT_DELAY:
			printf("#<promise>");
			break;
		case DT_CONTINUATION:
			printf("#<continuation>");
			break;
		case DT_KANKYO:
			printf("#<kankyo>");
			break;
	}
}
