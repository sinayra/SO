#include <stdio.h>

int main(){
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
     fprintf(filho, "sou o processo filho\n");
   else
   {
     fprintf(pai,"sou o processo pai\n");
     sleep(20);
     fprintf(pai,"Imprimi isso depois do sleep\n");
   }

   fclose(pai);
   fclose(filho);

   return 0;
}

