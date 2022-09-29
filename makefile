test: boot.bin vp.o cpu.o snd.o dmc.o main.o
	g++ *.o  -g -lz80ex -lraylib -lGL -lm -lpthread -ldl -lrt -lX11
%.o: %.cpp
	g++ -c $? -g
boot.bin: boot.z80
	pasmo boot.z80 boot.bin
