all: trabalho_pratico algoritmo1 algoritmo2

trabalho_pratico: trabalho_pratico.o smpl.o rand.o
	$(LINK.c) -o $@ -Bstatic trabalho_pratico.o smpl.o rand.o -lm

algoritmo1: main_algoritmo1.o smpl.o rand.o
	$(LINK.c) -o $@ -Bstatic main_algoritmo1.o smpl.o rand.o -lm

algoritmo2: main_algoritmo2.o smpl.o rand.o
	$(LINK.c) -o $@ -Bstatic main_algoritmo2.o smpl.o rand.o -lm

smpl.o: smpl.c smpl.h
	$(COMPILE.c)  -g smpl.c

trabalho_pratico.o: trabalho_pratico.c smpl.h
	$(COMPILE.c) -g  trabalho_pratico.c

main_algoritmo1.o: main_algoritmo1.c smpl.h
	$(COMPILE.c) -g  main_algoritmo1.c

main_algoritmo2.o: main_algoritmo2.c smpl.h
	$(COMPILE.c) -g  main_algoritmo2.c

rand.o: rand.c
	$(COMPILE.c) -g rand.c

clean:
	$(RM) *.o algoritmo1 algoritmo2 trabalho_pratico

