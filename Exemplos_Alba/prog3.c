#include <errno.h>
#include <stdio.h>
#include <string.h>
int main()
{
   int pid;
   FILE *pai, *filho;

   pai = fopen("pai.txt", "w");
   filho = fopen("filho.txt", "w");

   if (( pid = fork()) < 0)
   {
     printf("erro no fork\n");
     exit(1);
   }
   if (pid == 0)
   {
     if (execl("prfilho1", "prfilho1", "parametro 1", "Parm 2", (char *) 0) < 0)
       fprintf(filho, "erro no execl = %d\n", errno);
   }
   else
     fprintf(pai, "sou o processo pai\n");

    fclose(pai);
    fclose(filho);

    return 0;
}

