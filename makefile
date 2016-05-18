CC = g++
CFLAG = -std=c++11 -O2
PROCESSOS = alocador.o processo_usr.o shutdown.o substituidor.o 
SINC = fila_mensagem.o memoria_compartilhada.o semaforo.o
AUX = pagina.o utils.o

all:	main.o $(SINC) $(AUX) $(PROCESSOS)  compile clean

clean:
	rm -f *.o
compile:
	$(CC) -o sim main.o $(PROCESSOS) $(SINC) $(AUX)  -s

#####################
#	Processos
#####################
alocador.o:	src/alocador.cpp
	$(CC) $(CFLAG)  -c src/alocador.cpp -o alocador.o
shutdown.o:	src/shutdown.cpp
	$(CC) $(CFLAG)  -c src/shutdown.cpp -o shutdown.o
substituidor.o:	src/substituidor.cpp
	$(CC) $(CFLAG)  -c src/substituidor.cpp -o substituidor.o
processo_usr.o:	src/processo_usr.cpp
	$(CC) $(CFLAG)  -c src/processo_usr.cpp -o processo_usr.o
main.o:	main.cpp
	$(CC) $(CFLAG)  -c main.cpp -o main.o

#####################
#	Auxílio
#####################
pagina.o:	src/pagina.cpp
	$(CC) $(CFLAG) -c src/pagina.cpp -o pagina.o
utils.o:	src/utils.cpp
	$(CC) $(CFLAG) -c src/utils.cpp -o utils.o

#####################
#	Métodos de sincronização
#####################
fila_mensagem.o:	src/fila_mensagem.cpp
	$(CC) $(CFLAG) -c src/fila_mensagem.cpp -o fila_mensagem.o
memoria_compartilhada.o:	src/memoria_compartilhada.cpp
	$(CC) $(CFLAG) -c src/memoria_compartilhada.cpp -o memoria_compartilhada.o
semaforo.o:	src/semaforo.cpp
	$(CC) $(CFLAG) -c src/semaforo.cpp -o semaforo.o