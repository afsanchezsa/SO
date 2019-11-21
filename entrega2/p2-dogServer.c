#include "funciones.c"
char * mensaje_confirmacion;
char *mensaje_accion_realizada;
char *mensaje_ocupado;
/////server
struct cliente{
  int clientfd;
  int numhilo;
};
void * funcion(void *ap){
  
struct cliente * currentClient =(struct cliente *)ap;

int opcion;
int r;
r=send(currentClient->clientfd,mensaje_confirmacion,sizeof(char)*15,0);

r=recv(currentClient->clientfd,&opcion,sizeof(int),0);

printf("elcliente escogio : %i",opcion);



}

int HILODISPONIBLE[NUMHILOS];
int NumeroHiloDisponible(){
  int a=0;

  for(a=0;a<NUMHILOS;a++){
    if(HILODISPONIBLE[a]==1){
      return a;
    }
  }
  return -1;
}


int main(){
  int opcion;
  double peso;
  bool salir=false;
      
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
  int i=0;
  for(i=0;i<NUMHILOS;i++){
    HILODISPONIBLE[i]=1;//si esta en 1 esta disponible
  }
  mensaje_accion_realizada=(char *)malloc(sizeof(char)*15);
  mensaje_accion_realizada="accion realizada\n";
mensaje_confirmacion=(char*)malloc(sizeof(char)*15);
mensaje_confirmacion="establecida\n"; 
mensaje_ocupado=(char*)malloc(sizeof(char)*15);
mensaje_ocupado="NO CONECTADO\n";
 pthread_t hilos[NUMHILOS];

      ///init
      int clientfd,r;
struct sockaddr_in server,client;
socklen_t len;
serverfd=socket(AF_INET,SOCK_STREAM,0);
if(serverfd==-1){
    perror("error en el socket");
    exit(-1);
}
server.sin_family=AF_INET;
server.sin_port=htons(PORT);
server.sin_addr.s_addr=INADDR_ANY;
bzero(server.sin_zero,8);
len=sizeof(struct sockaddr_in);

r=bind(serverfd,(struct sockaddr *)&server,len);
if(r==-1){
    perror("error al configurar el socket");
    exit(-1);
    }

r=listen(serverfd,BACKLOG);
if(r==-1){
    perror("error en el listen");
    exit(-1);
}

//do{
clientfd=accept(serverfd,(struct sockaddr *)&client,&len);
if(clientfd==-1){
    perror("error en el accept");
    exit(-1);
}
struct cliente *currentClient=(struct cliente *)malloc(sizeof(struct cliente));
currentClient->clientfd=clientfd;
if(NumeroHiloDisponible()==-1){//no se pueden aceptar mas hilos 
r=send(clientfd,mensaje_ocupado,sizeof(char)*15,0);
if(r==-1){
  perror("error en send");
}
}else{
  currentClient->numhilo=NumeroHiloDisponible();
  HILODISPONIBLE[currentClient->numhilo]=0;

pthread_create(&hilos[currentClient->numhilo],NULL,funcion,currentClient);
}

    pthread_join(hilos[0],NULL);
//}while(1>0);


/*
r=recv(clientfd,&opcion,sizeof(int),0);


  if(opcion==1){
      struct dogType *insertado=(struct dogType *)malloc(sizeof(struct dogType));
      r=recv(clientfd,insertado,sizeof(struct dogType),0);
      
      //toLower(insertado->nombre);
      GuardarMascota(insertado,0);
      free(insertado);
    r=send(clientfd,mensaje_accion_realizada,sizeof(char)*15,0);
  }else if(opcion==2){
    printf("el numero de registros presentes es: %i \n",numero_mascotas);
    printf("por favor seleccione el numero de registro ");
    int numregistro;
    scanf("%i",&numregistro);
    printf("Desea abrir la historia clinica de la mascota? (S/N)");
    char decision;
    scanf(" %c",&decision);
    if(decision=='S'|| decision=='s'){
      por_revisar=LeerdeBD(numregistro-1);
      char* seq=concat(por_revisar->mascota.nombre,por_revisar->mascota.raza);
      char *s1=concat("gedit ",seq);
	  char * archivo=concat(seq,".txt");
	  FILE * clinichistory=fopen(archivo,"r");
	  if(clinichistory==NULL){
	  clinichistory=fopen(archivo,"w");
	  fprintf(clinichistory,"NOMBRE:%s\nTIPO:%s\nEDAD:%i\nRAZA:%s\nESTATURA:%i\nPESO:%lf\nSEXO:%c\n",
	  por_revisar->mascota.nombre,
	  por_revisar->mascota.tipo,
	  por_revisar->mascota.edad,
	  por_revisar->mascota.raza,
	  por_revisar->mascota.estatura,
	  por_revisar->mascota.peso,
	  por_revisar->mascota.sexo
	  
	  );
	  
	  }
	  fclose(clinichistory);
	 // free(clinichistory); descomentar haber si funciona
      char *path=concat(s1,".txt");
      system(path);

  }}else if(opcion==3){
    printf("el numero de registros presentes es: %i \n",numero_mascotas);
    printf("por favor seleccione el numero de registro que desea eliminar : ");
    int numregistro;
    scanf("%i",&numregistro);
    EliminarMascota(numregistro-1);
  }else if(opcion ==4){
    printf("inserte el nombre por favor");
    char *nombre=(char *)malloc(sizeof(char)*33);
    //toLower(nombre);
    if (nombre == NULL){
        perror("erroren el malloc");
        exit(-1);
    } 
    scanf("%s",nombre);
    BuscarPorNombre(nombre,0);
  }else {
    
    return 0;
  }


r=close(clientfd);
r=close(serverfd);

  CambiarTamanioBd(numero_mascotas);
  GuardarTablaHash();
*/
  
  return 0;
}
