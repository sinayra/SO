# Trabalho de SO 1/2016
Sinayra Pascoal Cotts Moreira 		10/0020666

*******************************************
*
*Resumo
*
*******************************************
	Este trabalho realiza uma simulação de gerência de memória paginada. Ele é separado em 4 grandes módulos: alocador, substituidor, processo de usuário e shutdown. Todos os três primeiros processos são criados pela função principal do programa, a main(). Ao final da execução de todos os processos de usuário, a main() encerra execução do alocador e substituidor e executa função de shutdown().

	Shutdown: Este processo cria todos os processos que entrarão em execução e aguarda até que todos os processos de usuário termine. Após isso, ele imprime na tela um resumo dos eventos que ocorreram, imprimindo também a configuração final da memória.

	Processo de usuário: Este processo abre um arquivo texto, onde neste haverá uma única linha indicando todas as páginas que deverão ser referenciadas. Para cada página, é enviada uma requisição de referência dela ao alocador e, em seguida, aguarda resposta do mesmo. Se a resposta for positiva, então a página foi referenciada; caso contrário, processo reenvia requisição. Este processo termina sua execução quando todas suas páginas forem referenciadas.

	Alocador: Recebe do processo de usuário uma requisição de alocação de página. O alocador então inicializa contador de page faults deste processo de usuário e verifica se a página já está referenciada ou não. Se já estiver referenciada, atualiza o tempo de referência. Se não estiver referenciada, verifica-se o estado da memória: se estiver cheia, executa algoritmo de substituição do processo substituidor. Após isso, aumenta-se o contador de page fault e insere a referência da página solicitada do processo que a requisitou. Por fim, envia uma mensagem ao processo que requisitou a alocação uma mensagem de sucesso.

	Substituidor: Verifica de 0,5s em 0,5s o estado da memória. Se o número de frames livres for maior ou igual que o parâmetro MAX_OCUPACAO, executa algoritmo de substituição. Este algoritmo leva em conta o tempo de referência das páginas, retirando da memória os frames que foram referenciados no passado mais distante.



*******************************************
*
*Para executar este trabalho:
*
*******************************************
	O trabalho foi feito no Ubuntu 15.10, versão 5.2.1 20151010 do gcc. O próprio programa principal encarrega-se de criar os processos e colocá-los em execução.

	Para compilar, execute o comando
		make

	Para executar, execute o comando:
		./sim
		Ele perguntará quantos processos entrarão em execução. Certifique-se que, para a quantidade de processos que queira executar, tenha os arquivos pag_processo_X correspondentes.
Por exemplo, caso queira executar 2 processos, é necessário ter os arquivos pag_processo_0 e pag_processo_1.
