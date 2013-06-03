/* 
*/

#include <stdio.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <unistd.h>
#include <stdlib.h> 
#include <string.h>
#include <ctype.h>
#include <errno.h>

#define MAX_TIROS 20
#define SEMILLA_DEFECTO 1
#define TIROS_DEFECTO 10


int main(int argc, char * argv[])
{   
   int semilla=SEMILLA_DEFECTO, jugadores=0, tiros=TIROS_DEFECTO;
   int status,lanzamiento,suma=0,i,j,ganador=0;
   pid_t childpid, *hijos;
   char nombre[]="Jugador_", nombre_arch[15];
   FILE *fd;
   
   for (i = 1; i<argc ; i++) 
   {
     if (!strcmp(argv[i],"-h"))
     {
       printf("help\n"); // poner VERGUERO!!!!!!!!!!!!!!!!!!!
       return 0;
     } else 
     { 
       if (!strcmp(argv[i],"-j"))
       {
	  i++;
	  if (argc==i)
	    printf("mal pase de parametros");
	  else
	    if (!(jugadores=strtol(argv[i],NULL,10))) printf("j debe ser un # > 0");
	} else 
	{
	  if (!strcmp(argv[i],"-n"))
	  {
	    i++;
	    if (argc==i)
	      printf("mal pase de parametros");
	    else{
	      if (!(tiros=strtol(argv[i],NULL,10))) printf("n debe ser un # > 0");
	      if (tiros>20){
		printf("n debe ser <=20");
		return 0;
	      }
	    }
	  } else 
	  {
	    if (!strcmp(argv[i],"-s"))
	    {
	      i++;
	      if (argc==i)
		printf("mal pase de parametros");
	      else
		if (!(semilla=strtol(argv[i],NULL,10))) printf("s debe ser un # > 0");
	    } else
	    {
	      printf("opcion invalida");
	      return 1;
	    }
	  }
	}
     }
   }
   if (jugadores==0) return 1;
   
   printf("Jugadores=%d\nSemilla=%d\nTiradas=%d\n",jugadores,semilla,tiros);
   
   hijos = (pid_t *) malloc(sizeof(pid_t)*jugadores);
   for (i = 0; i < jugadores; i++) 
   {
     
      if ((childpid = fork())<0){ /* chequea error en llamada */ 
	    perror("fork:");
	    exit(1);
	  }   
	  
      // Codigo que ejecutaran los hijos
      if (childpid == 0) 
      {
	sprintf(nombre_arch,"%s%d",nombre,i+1);
	srand(semilla+i);
  
	if (fd= fopen(nombre_arch,"w"))
	{
	  fprintf(fd,"%s\n",nombre_arch);
	  j=0;
	  while (j<tiros)
	  {
	    lanzamiento= (rand() % 11) + 2;
	    fprintf(fd,"tirada %d: %d\n",j+1,lanzamiento);
	    suma=suma + lanzamiento;
	    j++;
	  }
	} else 
	    fprintf(stderr,"No se pudo crear el archivo %s\nERROR:%s\n",nombre_arch,strerror(errno));
	exit(suma);
      } else 
      {
	*hijos=childpid;
	hijos=hijos+1;
      }
   }
   hijos=hijos-jugadores;
   for (i = 0; i < jugadores; i++)
   {
     waitpid(*hijos,&status,0);
     if (WIFEXITED(status))
     {
       ganador=WEXITSTATUS(status);
       printf("jugador %d: %d\n",i+1,ganador);
       
     }
     hijos=hijos+1;
   }
   
return 0;}
