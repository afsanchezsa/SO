#include "funciones.c"
#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>
#include <errno.h>
#include <unistd.h>
#include <ctype.h>
#include <termios.h>
#include <assert.h>
#include <time.h>

//#include <curses.h>

int main(){
    
    srand(time(NULL));
    int opcion;
  double peso;
  bool salir=false;
      
 
      
    FILE* nombres=fopen("nombres.txt","r");
    char nom[33];
    char tipos[6][33]={{"Exotico"},{"Domestico"},{"Mamifero"},{"Acuatico"},{"Cuadrupedo"},{"Salvaje"}};
    char raza[5][33]={{"Canina"},{"Equina"},{"Felina"},{"Otra"},{"Rumiante"}};
    struct dogType *mascota=(struct dogType *)malloc(sizeof(struct dogType));
    int insertadas=0;
    //int aleatorio=3;//(int)(((double)rand()/RAND_MAX)*2)+2;
    int contador=0;
    int j=0;
    FILE *random=fopen("aleatorios.dat","a+");
    int numero_mascotas=0;
    while(numero_mascotas<10000000){
        if(numero_mascotas%500000==0){
printf("%i \n",numero_mascotas);
        }
    int q,t;
         for(q=0;q<33;q++){
            nom[q]='\0';
    }
fscanf(nombres,"%s",nom);
    for(t=0;t<10000;t++){
       
        
       

      
strcpy(mascota->nombre,nom);
        
        //printf("%s %i \n",nom,hash(nom));
           strcpy(mascota->raza,raza[(int)(((double)rand()/RAND_MAX)*4)]);
        strcpy(mascota->tipo,tipos[(int)(((double)rand()/RAND_MAX)*5)]);
       mascota->edad=(int)(((double)rand()/RAND_MAX)*17);
       mascota->estatura=(int)(((double)rand()/RAND_MAX)*180);
       mascota->peso=(int)(((double)rand()/RAND_MAX)*50);
       mascota->sexo=(int)(((double)rand()/RAND_MAX)*2)>1?'M':'H';
       //ImprimirEstructura(mascota);
      fwrite(mascota,sizeof(struct dogType),1,random);
       numero_mascotas++;        
    
    }    
j++;        
    }
    fclose(random);
    fclose(nombres);
    free(random);
    free(nombres);
    free(mascota);
    //CambiarTamanioBd(numero_mascotas);
    //GuardarTablaHash();
return 0;}
