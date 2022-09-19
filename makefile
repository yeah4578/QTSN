test: cpu.o vp.o snd.o dmc.o main.o
	g++ *.o  -g -lz80ex -lraylib -lGL -lm -lpthread -ldl -lrt -lX11
%.o: %.c
	g++ -c $? -g
