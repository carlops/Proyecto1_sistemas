/* 
*/

#include <stdio.h>
#include <sys/types.h>
#include <unistd.h>
#include <stdlib.h> 
#include <string.h>
#include <ctype.h>
#include <pthread.h>
#include <errno.h>

#define MAX_TIROS 20
#define SEMILLA_DEFECTO 1
#define TIROS_DEFECTO 10

#define TEXTO_AYUDA "juegodedados_h: imprime una secuencia de numeros aleatorios\nSINTAXIS:\njuegodedados_p [-n i] [-s x] [-h] \n-n i: tamano de la secuencia (por defecto, 10)\n-s x: semilla (por defecto, 1)\n-j y: numero de jugadores\n-h: imprime esta ayuda y sale\n"

typedef struct a{
  int jugador;
  int tiros;
} argumentos;

void *Dados(void *param);

char nombre[]="Jugador_";

int main(int argc, char * argv[])
{   
   int semilla=SEMILLA_DEFECTO, jugadores=0, tiros=TIROS_DEFECTO;
   int i,valor_ganador=-1,jugador_ganador,*aux_ganador;
   pthread_t *hilos, *aux;
   argumentos *parametros;
   
   for (i = 1; i<argc ; i++) 
   {
     if (!strcmp(argv[i],"-h"))
     {
       printf(TEXTO_AYUDA);
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
   
   
   if (hilos=(pthread_t *) malloc(sizeof(pthread_t)*jugadores))
   {
     aux=hilos;
     i=0;
     srand(semilla);
     
     while (i<jugadores) 
     {
	parametros=(argumentos *) malloc(sizeof(argumentos));
	parametros->tiros = tiros;
	parametros->jugador = i+1;
        pthread_create(aux,NULL,Dados,(void *) parametros);
	aux=aux+1;
	i++;
     }
     aux=hilos;
     i=0;
     while (i<jugadores) 
     {
	if (pthread_join(*aux,(void **) &aux_ganador)==0)
	{
	  if (*aux_ganador>valor_ganador)
	  { 
	    valor_ganador=*aux_ganador;
	    jugador_ganador=i+1;
	  }
	} else return 3;
	aux=aux+1;
	i++;
     }
   } else return 2;
   printf("El ganador es jugador %d con %d\n",jugador_ganador,valor_ganador);
return 0;}

void *Dados(void *param) {
  argumentos * arg = (argumentos *) param;
  char nombre_arch[15];
  int i=0, lanzamiento,suma=0;
  FILE *fd;
  
  sprintf(nombre_arch,"%s%d",nombre,arg->jugador);
  if (fd= fopen(nombre_arch,"w"))
  {
      fprintf(fd,"%s\n",nombre_arch);
      while (i<arg->tiros){
	lanzamiento= (rand() % 11) + 2;
	fprintf(fd,"tirada %d: %d\n",i+1,lanzamiento);
	suma=suma + lanzamiento;
	i++;
      }
  } else 
      fprintf(stderr,"No se pudo crear el archivo %s\nERROR:%s\n",nombre_arch,strerror(errno));
  pthread_exit(&suma);
  
}
