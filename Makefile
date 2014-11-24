build = Release
objects = main.o
CXX = clang++

CommonOptsAll = -Wall -Wextra -std=c++11
CommonDebugOpts = -g -O0
CommonReleaseOpts = -s -O3
CommonOpts = $(CommonOptsAll) $(Common$(build)Opts)

CompileOptsAll = -c
CompileOptsRelease =  
CompileOptsDebug =
CompileOpts = $(CommonOpts) $(CompileOptsAll) $(CompileOpts$(build))

LinkerOptsAll = -lnetsnmp
LinkerOptsRelease = 
LinkerOptsDebug = 
LinkerOpts = $(CommonOpts) $(LinkerOptsAll) $(LinkerOpts$(build))

.PHONY: clean

snmpwalktranslate : $(objects)
	$(CXX) -o ./snmpwalktranslate $(objects) $(LinkerOpts)

clean :
	-rm snmpwalktranslate $(objects)

main.o : main.cxx
	$(CXX) -o main.o ./main.cxx $(CompileOpts)
