#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>
#include <errno.h>
#include <unistd.h>
#include <ctype.h>
#include <termios.h>
#include <assert.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <netinet/in.h>
#include <pthread.h>
//#include <curses.h>


#define tam 100151
char minuscula[33];


char * toLower(char *arr){
    int i;
   // char * minuscula=(char *)malloc(sizeof(char)*33); 
for(i=0;i<33;i++){
  minuscula[i]=0;
}    
    for( i = 0; arr[i]; i++){
  minuscula[i] = tolower(arr[i]);
}


 return minuscula;
}
char getch() {
    printf("\nPresione cualquier tecla para continuar\n");
    char buf = 0;
    struct termios old = {0};
    if (tcgetattr(0, &old) < 0)
            perror("tcsetattr()");
    old.c_lflag &= ~ICANON;
    old.c_lflag &= ~ECHO;
    old.c_cc[VMIN] = 1;
    old.c_cc[VTIME] = 0;
    if (tcsetattr(0, TCSANOW, &old) < 0)
            perror("tcsetattr ICANON");
    if (read(0, &buf, 1) < 0)
            perror ("read()");
    old.c_lflag |= ICANON;
    old.c_lflag |= ECHO;
    if (tcsetattr(0, TCSADRAIN, &old) < 0)
            perror ("tcsetattr ~ICANON");
    return (buf);
}

int tablahash[tam];
int numero_mascotas;

//declaramos la estructura de las mascotas con sus atributos
struct dogType{
  char nombre [33];
  char tipo[33];
  int edad;
  char raza[17];
  int estatura;
  double peso;
  char sexo;
};

struct Nodo{
  int siguiente;
  int anterior;
  struct dogType mascota;
};

FILE *bd;
struct Nodo *nuevo,*por_revisar;

//Metodo que imprime una tabla 
void ImprimirTabla(int *t){
  int i=0;
  for(i=0;i<tam;i++){
     printf("%i",*t);
     t++;
      
      }
}

//Imprime la estructura DogType
void ImprimirEstructura(struct dogType * mascota){
  printf( "nombre: %s \n",mascota->nombre);
  printf("tipo : %s \n",mascota->tipo);
  printf("edad: %i \n",mascota->edad);
  printf("raza : %s \n",mascota->raza);
  printf("estatura: %i \n",mascota->estatura);
  printf("peso: %lf \n",mascota->peso);
  printf("sexo: %c \n",mascota->sexo);
}

//Lee la estructura de Dogtype, desde la consola
struct  dogType * LeerEstructura(){
  struct dogType * insertado=(struct dogType * )malloc(sizeof(struct dogType));
  if (insertado == NULL){
        perror("error en el malloc");
        exit(-1);
    } 
  printf("inserte el nombre ");
  scanf( "%s",insertado->nombre);
  printf("inserte el tipo ");
  scanf("%s",insertado->tipo);
  printf("inserte la edad ");
  scanf("%i",&insertado->edad);
  printf("inserte la raza ");
  scanf("%s",insertado->raza);
  printf("inserte la estatura ");
  scanf("%i",&insertado->estatura);
  printf("inserte el peso ");
  scanf("%lf",&insertado->peso);
  printf("inserte el sexo ");
  scanf(" %c",&insertado->sexo);
  return insertado;
}

//Metodos para leer y escribir los archivos 
FILE * bdLectura(){
  return fopen("dataDogs.dat","r"); //solo leer dataDogs
}

FILE * bdEscritura(){
  return fopen("dataDogs.dat","a+"); //leer y escribir datDogs
}

FILE *bdActualizar(){
  return fopen("dataDogs.dat","r+b"); //leer y actualizar, b para abrir el archivo cmobinario
}

FILE * TablaHashEscritura(){
  return fopen("tabla.dat","w"); //solo escribir de la tabla hash
}

FILE * TablaHashLectura(){
  return fopen("tabla.dat","r"); //solo leer de la tabla hash
}

//Cargar la tabla hash,leer el archivo hasta el tamaño dela tabla hash y o va guardando en el arreglo
void cargarTablaHash(){
  int i;
  FILE *TablaHash=TablaHashLectura();
  if (TablaHash == NULL){
        perror("No existe el archivo");
        exit(-1);
    }
  int *p=(int *)malloc(sizeof(int));
  if (p==NULL){
        perror("error en malloc");
        exit(-1);
  }
  for(i=0;i<tam;i++){
    fread(p,sizeof(int),1,TablaHash);
    tablahash[i]=*p;
  }
  fclose(TablaHash);
}

//regresa un entero entre 0 y el tamaño de la tabla
 int hash(char *str){
   //toLower(str);
    unsigned long hash = 5381;
    int c;

    while ((c = *str++)){
      hash = ((hash << 5) + hash) + c; /* hash * 33 + c */
    } 
    return ( int )(hash%tam);
}

//mueve el puntero n posiciones,para hacer la busqueda 
void moverPunteroBd(FILE ** puntero,int posiciones){
  fseek(*puntero,sizeof(struct Nodo)*posiciones,SEEK_SET);//doble apuntador para que recuerde donde quedo
}

//Imprime el nodo(mascota), el siguiente y el anterior
void ImprimirNodo(struct Nodo *nodo){
  printf("anterior : %i\n",nodo->anterior);
  printf("siguiente : %i\n",nodo->siguiente);
  ImprimirEstructura(&nodo->mascota);
}

//Actualiza la mascota en la base de datos, mueve el puntero
void ActualizarMascotaenBD(int posicion,struct Nodo * nodo){
 
  bd=bdActualizar();
  moverPunteroBd(&bd,posicion);
  fwrite(nodo,sizeof(struct Nodo),1,bd);
  fclose(bd);

  
}

//Lee de la BD la posición indicada, retorna la mascota en la posicion
struct Nodo *LeerdeBD(int posicion){
  bd=bdLectura();
  moverPunteroBd(&bd,posicion);
  struct Nodo *leido=(struct Nodo *)malloc(sizeof(struct Nodo));
  if (leido==NULL){
        perror("error en malloc");
        exit(-1);
  }
  fread(leido,sizeof(struct Nodo),1,bd);
  fclose(bd);
  
  return leido;
}

//Inserta una mascota en la BD
void InsertarMascotaenBD(struct dogType * mascota,int indice_del_anterior_en_lista){
  //Actualiza el nodo
  nuevo=(struct Nodo *)malloc(sizeof(struct Nodo));//actualizar tail de la lista con su nuevo siguiente
  if (nuevo==NULL){
        perror("error en malloc");
        exit(-1);
  }
  nuevo->anterior=indice_del_anterior_en_lista;
  nuevo->siguiente=-1;
  nuevo->mascota=*mascota;
  
  
  bd=bdEscritura();//siempre escribe al final
  fwrite(nuevo,sizeof(struct Nodo),1,bd);
  fclose(bd);
  if(indice_del_anterior_en_lista>=0){
    struct Nodo *extail=LeerdeBD(indice_del_anterior_en_lista);
    extail->siguiente=numero_mascotas;
    ActualizarMascotaenBD(indice_del_anterior_en_lista,extail);
  }
}

//Devuelve el indice del último de la lista
int indiceTaildelaLista(int indice_front){
  struct Nodo * nodo=(struct Nodo *)malloc(sizeof(struct Nodo));
  if (nodo==NULL){
        perror("error en malloc");
        exit(-1);
  }

  int indicetail=indice_front;
  bd=bdLectura();
  moverPunteroBd(&bd,indicetail);
  fread(nodo,sizeof(struct Nodo),1,bd);
  
  while(nodo->siguiente!=-1){
    indicetail=nodo->siguiente; 
    moverPunteroBd(&bd,indicetail);
    fread(nodo,sizeof(struct Nodo),1,bd);
  }
  fclose(bd);
  
  return indicetail;
}

//compara si existe y si los nombres son iguales, colisiona si hay mismo hash pero diferente nombre
int  Colisiona(char *nombre_nuevo,int posicion){//retorna -1 si no hay colision y 1 si la hay
  if(posicion==-1){
    return -1;
  }
  struct Nodo *leido=LeerdeBD(posicion);
  if(strcmp(toLower(leido->mascota.nombre),toLower(nombre_nuevo))==0){  
    
    return -1;
  }
  
  return 1;
}

//retorna el cuadrado, si hay colisión suma valor al hash
int sondeo(int i){
  return (i*i);
}

//Guarda la mascota en el archivo
void GuardarMascota(struct dogType * mascota,int i){

  int indice=(hash(toLower(mascota->nombre))+sondeo(i))%tam;
  int frontLista=tablahash[indice];
  if(frontLista==-1){//no hay  datos
    tablahash[indice]=numero_mascotas;
    InsertarMascotaenBD(mascota,-1);
    numero_mascotas+=1;
  }else if(frontLista==-2){ //hubo una lista pero se borro, no se puede usar esa posición
    GuardarMascota(mascota,i+1);
  }else{
    if(Colisiona(mascota->nombre,frontLista)<0){//se llaman igual agrega al final de la lista
      int indiceUltimoLista=frontLista;
      InsertarMascotaenBD(mascota,indiceUltimoLista);
      tablahash[indice]=numero_mascotas;
      numero_mascotas+=1;

    }else{
      return GuardarMascota(mascota,i+1); //hace un nuevo osndeo
    }
  }
}

//Actualiza el tamaño de la BD
void CambiarTamanioBd(int tamanio){
  FILE* tambd=fopen("tambd.dat","w");
  if (tambd == NULL){
        perror("No existe el archivo");
        exit(-1);
    }
  fwrite(&tamanio,sizeof(int),1,tambd);
  fclose(tambd);
}

//Guarda la tabla hash
void GuardarTablaHash(){
  FILE *archivo=fopen("tabla.dat","w");
  if (archivo == NULL){
        perror("No existe el archivo");
        exit(-1);
    }
  fwrite(tablahash,sizeof(int)*tam,1,archivo);
  fclose(archivo);
}

//opcion 2, concatenar dos strings
char* concat( char *s1,  char *s2){
    char *result = malloc(strlen(s1) + strlen(s2) + 1); // +1 for the null-terminator
    // in real code you would check for errors in malloc here
    if (result == NULL){
        perror("error en el malloc");
        exit(-1);
    } 
    strcpy(result, s1);
    strcat(result, s2);
    return result;
}

//opcion 4, busqueda de dogtype por nombre
void BuscarPorNombre( char *nombre,int i){
  int index=(hash(toLower(nombre))+sondeo(i))%tam;
  int nextIndex;

  if(tablahash[index]!=-1&&tablahash[index]!=-2&&Colisiona(nombre,tablahash[index])<0){//Hay algo en esa posición
    struct Nodo * buscado=(struct Nodo *)malloc(sizeof(struct Nodo));
    if (buscado == NULL){
        perror("error en el malloc");
        exit(-1);
    } 
    bd=bdLectura();
    buscado=LeerdeBD(tablahash[index]);
    printf("/////////////////\n");
    printf("ID: %i \n",tablahash[index]+1);
    ImprimirEstructura(&buscado->mascota);
    nextIndex=buscado->anterior;

    while(nextIndex!=-1){//lee la lista 
      buscado=LeerdeBD(nextIndex);
      printf("/////////////////\n");
      printf("ID: %i\n",nextIndex+1);
      ImprimirEstructura(&buscado->mascota);
      nextIndex=buscado->anterior;
      
    }
  }else if(tablahash[index]==-2||Colisiona(nombre,tablahash[index])>0){//bsca con un sondeo diferente
    return BuscarPorNombre(nombre,i+1);
  }else{
    printf("lo sentimos no se han encontrado resultados");  
  }
  
}

//opcion 3, recorre toda la lista uno por uno comparando, retorna el indice donde coninciden en la tabla hash donde 
int indiceEnTablaHash(char * nombre){
  int indice;
  int j=-1;
  struct Nodo *leido;
  do{
    j++;
    indice=(hash(toLower(nombre))+sondeo(j))%tam;
    leido=LeerdeBD(tablahash[indice]);
  }while(strcmp(toLower(leido->mascota.nombre),toLower(nombre))!=0);
  return indice;
}

//
void DesconectarMascota(int posicion){
  int i=0;
  struct Nodo * eliminado=LeerdeBD(posicion);
  if(eliminado->siguiente!=-1&&eliminado->anterior!=-1){//esta en el medio de la lista
    struct Nodo *anterior=LeerdeBD(eliminado->anterior);
    struct Nodo *siguiente=LeerdeBD(eliminado->siguiente);
    siguiente->anterior=eliminado->anterior;
    anterior->siguiente=eliminado->siguiente;
    ActualizarMascotaenBD(eliminado->anterior,anterior);
    ActualizarMascotaenBD(eliminado->siguiente,siguiente);
    free(anterior);
    free(siguiente);
  }else if(eliminado->siguiente!=-1){//primero de la lista
    int indice_siguiente=eliminado->siguiente;
    struct Nodo * siguiente=LeerdeBD(indice_siguiente);
    siguiente->anterior=-1;
    //tablahash[indiceEnTablaHash(eliminado->mascota.nombre)]=indice_siguiente; //se actualiza la tablaen el ultimo y e l primero
    ActualizarMascotaenBD(indice_siguiente,siguiente);
    free(siguiente);
  }else if(eliminado->anterior!=-1){//ultimo de la lista
    int indice_anterior=eliminado->anterior;
    tablahash[indiceEnTablaHash(eliminado->mascota.nombre)]=indice_anterior;
    struct Nodo *anterior=LeerdeBD(indice_anterior);
    anterior->siguiente=-1;
    ActualizarMascotaenBD(indice_anterior,anterior);
    free(anterior);
  }else{//unico de la lista
    tablahash[indiceEnTablaHash(eliminado->mascota.nombre)]=-2;//se pone -2 en caso de que ya haya sido usado(problema: manejar el -2 al insertar)
  }
  
}

//archivo temporal, de escritura y lectura, lo crea si no existe
FILE* Temporal(){
  return fopen("temporal.dat","a+");
}

//eliminar de la BD la posición dada
void EliminarDeLaBD(int posicion){
  int i;
  system("touch temporal.dat"); //trae un archivo temporal para pasar todas las mascotas
  FILE* temporal=Temporal();
  if (temporal == NULL){
        perror("No existe el archivo");
        exit(-1);
  }
  struct Nodo * actual=malloc(sizeof(struct Nodo));
  if (actual == NULL){
        perror("error en el malloc");
        exit(-1);
    } 
  for(i=0;i<numero_mascotas;i++){
    if(i!=posicion){//lee todos menos lo que esta en esa posición, saltarlo
     actual=LeerdeBD(i);
     if(actual->anterior>posicion){
       actual->anterior=(actual->anterior-1);//corre todas las posiciones
     }
     if(actual->siguiente>posicion){
       actual->siguiente=(actual->siguiente-1);
     }
     if(i>posicion&&actual->siguiente==-1){
       tablahash[indiceEnTablaHash(actual->mascota.nombre)]=i-1; //es el ultimo, actualiza la cabeza de lista
     }
     fwrite(actual,sizeof(struct Nodo),1,temporal);
     free(actual);
    }
    
  }
  fclose(temporal);
  
  system("mv temporal.dat dataDogs.dat");
}

//Elimina la mascota del archivo
void EliminarMascota(int posicion){
  DesconectarMascota(posicion);
  EliminarDeLaBD(posicion);
  numero_mascotas--;
}


