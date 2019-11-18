#include "funciones.c"
int main(){

      
  cargarTablaHash(tablahash);
  FILE * archivo_num_mascotas=fopen("tambd.dat","r");
  if (archivo_num_mascotas == NULL){
        perror("No existe el archivo");
        exit(-1);
  }
  fread(&numero_mascotas,sizeof(int),1,archivo_num_mascotas);
  fclose(archivo_num_mascotas);
  por_revisar=(struct Nodo * )malloc(sizeof(struct Nodo));  
  if (por_revisar== NULL){
        perror("error en el malloc");
        exit(-1);
  } 
      
    FILE * f=fopen("aleatorios.dat","r");
    if(f==NULL){
        perror("error al abrir archivo");
    }
    struct dogType *mascota=(struct dogType *)malloc(sizeof(struct dogType));
    int i;
     for( i =0;i<10000000;i++){
        if(i%500000==0){
            printf("%i",i);
        }  
        
        fread(mascota,sizeof(struct dogType),1,f);

    GuardarMascota(mascota,0);
//printf("se escribio: \n");  
    //ImprimirEstructura(mascota);
//printf("\n");  
    }
    CambiarTamanioBd(numero_mascotas);
  GuardarTablaHash();
  free(bd);
  free(nuevo);
  free(por_revisar);
fclose(f);
return 0;


}


