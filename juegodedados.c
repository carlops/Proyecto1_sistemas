/* 
*/

#include <stdio.h>
#include <sys/types.h>
#include <unistd.h>
#include <stdlib.h> 
#include <string.h>
#include <errno.h>
#include <ctype.h>

#define MAX_TIROS 20
#define SEMILLA_DEFECTO 1
#define TIROS_DEFECTO 2

int Verificar_opciones(int n,int argc,char *numero,int * var);
void ImprimirAyuda(void);

int main(int argc, char * argv[])
{   
   int semilla=SEMILLA_DEFECTO, jugadores=0, tiros=TIROS_DEFECTO, i;
   int status;
   pid_t childpid;
   
   for (i = 1; i<argc ; i++) 
   {
     if (strlen(argv[i])==2 && argv[i][0]=='-') {
       switch (argv[i][1]) {
	 case 'h':
	   ImprimirAyuda();
	   return 0;
	 case 'j':
	   i++;
	   if (Verificar_opciones(i,argc,argv[i],&jugadores)!=0){
	     printf("mal pase de parametros\n");
	     return 1;
	   }
	   break;
	 case 'n':
	   i++;
	   if (Verificar_opciones(i,argc,argv[i],&tiros)!=0) {
	       printf("mal pase de parametros\n");
	       return 1;
	   } else { 
	     if (tiros>20) {
	       printf("n debe ser <=20\n");
	       return 1;
	     }
	   }
	   break;
	 case 's':
	   i++;
	   if (Verificar_opciones(i,argc,argv[i],&semilla)!=0){
	     printf("mal pase de parametros\n");
	     return 1;
	   }
	   break;
	 default: 
	   printf("opcion invalida\n");
	   return 1;
       } 
     } else {
       printf("opcion invalida\n");
       return 1;
     }
   }
   
   printf("j=%d, seed=%d,tiros=%d\n",jugadores,semilla,tiros);
   
}

int Verificar_opciones(int n,int argc,char *numero,int * var) {
char * fin_num;
  if (argc==n)
    return 1;
  else {
    if ((*var=strtol(numero,&fin_num,10))==0) return 1;
    if (*fin_num!='\0') return 1;
    if (*var<0) {
      printf("El numero debe ser mayor que 0\n");
      return 1;
    }
  }
  return 0;
}

void ImprimirAyuda(void){
  fputs("juegodedados_h: \
  imprime una secuencia de numeros aleatorios\n\
  SINTAXIS:\njuegodedados_p [-n i] [-s x] [-h] \n\
  -n i: tamano de la secuencia (por defecto, 10)\n\
  -s x: semilla (por defecto, 1)\n\
  -j y: numero de jugadores. Esta opcion es obligatoria\n\
  -h: imprime esta ayuda y sale\n",stdout);
}



