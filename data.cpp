#include <cstdio>
#include "data.h"

void print_data(const p_data_t& data,bool do_syouryaku,bool please_syouryaku) {
	switch(data->get_type()) {
		case DT_EOF:
			printf("<EOF>");
			break;
		case DT_EXIT:
			printf("<exit code=%d>",((exit_t*)&*data)->exit_code);
			break;
		case DT_ERROR:
			printf("ERROR: %s",((error_t*)&*data)->error_mes.c_str());
			break;
		case DT_NUM:
			printf("%.15g",((num_t*)&*data)->num);
			break;
		case DT_KIGOU:
			printf("%s",((kigou_t*)&*data)->kigou.c_str());
			break;
		case DT_BOOLEAN:
			printf("#%c",((boolean_t*)&*data)->is_true?'t':'f');
			break;
		case DT_LAMBDA:
			printf("<lambda-siki>");
			break;
		case DT_CONS:
			if(!do_syouryaku || !please_syouryaku)printf("(");
			if(!((cons_t*)&*data)->cons_car.is_null()) {
				print_data(((cons_t*)&*data)->cons_car,do_syouryaku);
			} else {
				printf("<NULL(bug?)>");
			}
			if(!((cons_t*)&*data)->cons_cdr.is_null()) {
				if(!do_syouryaku) {
					printf(" . ");
				} else {
					if(((cons_t*)&*data)->cons_cdr->get_type()!=DT_NULL){
						printf(" ");
						if(((cons_t*)&*data)->cons_cdr->get_type()!=DT_CONS) {
							printf(". ");
						}
					}
				}
				print_data(((cons_t*)&*data)->cons_cdr,do_syouryaku,true);
			} else {
				printf(" . <NULL(bug?)>");
			}
			if(!do_syouryaku || !please_syouryaku)printf(")");
			break;
		case DT_NATIVE_FUNC:
			printf("<native function>");
			break;
		case DT_NULL:
			if(!do_syouryaku || !please_syouryaku)printf("()");
			break;
		case DT_KANKYO:
			printf("<kankyo>");
			break;
	}
}
