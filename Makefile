CXX=g++
LD=g++
RM=rm -f
CXXFLAGS=-std=c++98 -pedantic -O2 -s -static -Wall -Wextra -finput-charset=utf-8 -fexec-charset=cp932
LDFLAGS=-O2 -s -static -finput-charset=utf-8 -fexec-charset=cp932
OBJS=scheme_modoki.o kumikomi_tetuduki.o stream_reader.o creater.o p_data.o data.o \
	scheme_modoki_core.o global_config.o \
	kumikomi_tetuduki/special_form.o kumikomi_tetuduki/number_calc.o \
	kumikomi_tetuduki/cons_and_list.o kumikomi_tetuduki/system_control.o
BINNAME=scheme_modoki.exe

$(BINNAME): $(OBJS)
	$(LD) $(LDFLAGS) -o $(BINNAME) $(OBJS)

%.o: %.cpp
	$(CXX) $(CXXFLAGS) -c -o $@ $^

.PHONY: clean
clean:
	$(RM) $(OBJS) $(BINNAME)
