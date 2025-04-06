all: exe

exe: startnn parsexml socketxml socketclient
	g++ -o "startnn.out" -Wl,--no-undefined -Wl,-z,now -Wl,-z,noexecstack obj/startnn_linux.o obj/parsexml.o obj/socketxml.o obj/socketclient.o

startnn:
	g++ -c -x c++ startnn_linux.cpp -g2 -gdwarf-2 -o "obj/startnn_linux.o" -Wall -Wswitch -W"no-deprecated-declarations" -W"empty-body" -Wconversion -W"return-type" -Wparentheses -W"no-format" -Wuninitialized -W"unreachable-code" -W"unused-function" -W"unused-value" -W"unused-variable" -O0 -fno-strict-aliasing -fno-omit-frame-pointer -fthreadsafe-statics -fexceptions -frtti -std=c++11

parsexml:
	g++ -c -x c++ parsexml.cpp -g2 -gdwarf-2 -o "obj/parsexml.o" -Wall -Wswitch -W"no-deprecated-declarations" -W"empty-body" -Wconversion -W"return-type" -Wparentheses -W"no-format" -Wuninitialized -W"unreachable-code" -W"unused-function" -W"unused-value" -W"unused-variable" -O0 -fno-strict-aliasing -fno-omit-frame-pointer -fthreadsafe-statics -fexceptions -frtti -std=c++11

socketxml:
	g++ -c -x c++ socketxml.cpp -g2 -gdwarf-2 -o "obj/socketxml.o" -Wall -Wswitch -W"no-deprecated-declarations" -W"empty-body" -Wconversion -W"return-type" -Wparentheses -W"no-format" -Wuninitialized -W"unreachable-code" -W"unused-function" -W"unused-value" -W"unused-variable" -O0 -fno-strict-aliasing -fno-omit-frame-pointer -fthreadsafe-statics -fexceptions -frtti -std=c++11

socketclient:
	g++ -c -x c++ socketclient.cpp -g2 -gdwarf-2 -o "obj/socketclient.o" -Wall -Wswitch -W"no-deprecated-declarations" -W"empty-body" -Wconversion -W"return-type" -Wparentheses -W"no-format" -Wuninitialized -W"unreachable-code" -W"unused-function" -W"unused-value" -W"unused-variable" -O0 -fno-strict-aliasing -fno-omit-frame-pointer -fthreadsafe-statics -fexceptions -frtti -std=c++11
