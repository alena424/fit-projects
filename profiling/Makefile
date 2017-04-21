###############################################################################################
#											      #						
# Makefile for compiling deviation.cbp using AteamMathLib.cbp and AteamMathLin.h from ../src/ #
#											      #
# @authors Alena Tesarova, Peter Uhrin, Daniel Uhricek, Jan Sorm			      #
# @date 21.04.2017									      #
# @brief 2nd project for IVS -- profiling of Standard Deviation				      #								
#											      #
###############################################################################################

all: deviation

deviation: deviation.cpp ../src/AteamMathLib.cpp ../src/AteamMathLib.h
	g++ -o $@ $^

clean: 
	rm -rf *.o deviation	
