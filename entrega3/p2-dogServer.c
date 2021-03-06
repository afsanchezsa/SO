#include "funciones.c"
#include "log.c"

char * mensaje_confirmacion;
char *mensaje_accion_realizada;
char *mensaje_ocupado;
char *mensaje_despedida;
/////server
struct cliente{
  int clientfd;
  int numhilo;
  char * ip;
};
int HILODISPONIBLE[NUMHILOS];
int HISTORIA_ABIERTA[NUMHILOS];///-1 si no tiene abierto ningun archivo o el numero del id del archivo abierto
int numclientesactual;
void * funcion(void *ap){
  
struct cliente * currentClient =(struct cliente *)ap;

int opcion;
int r;
r=sendAll(currentClient->clientfd,mensaje_confirmacion,sizeof(char)*33);

do{
r=recvAll(currentClient->clientfd,&opcion,sizeof(int));
struct Log * log=(struct Log *)malloc(sizeof(struct Log));
if(opcion==1){
      struct dogType *insertado=(struct dogType *)malloc(sizeof(struct dogType));
      
      r=recvAll(currentClient->clientfd,insertado,sizeof(struct dogType));
     // ------------------------------> bloquear
     Bloquear();
      GuardarMascota(insertado,0);
      Desbloquear();
      //-------->desbloquear
    free(insertado);
    //warning("esto es una alerta");
    
    log->opcion=opcion;
    Bloquear();
    log->registro=numero_mascotas;
    Desbloquear();
    log->ip=currentClient->ip;
    ////------->bloquear log
    BloquearLog();
    writelog(log);
    DesbloquearLog();
    /////------->desbloquear log
    r=sendAll(currentClient->clientfd,mensaje_accion_realizada,sizeof(char)*33);
  }else if(opcion==2){
   // printf("el numero de registros presentes es: %i \n",numero_mascotas);
    ///----->bloquear
    Bloquear();
    r=sendAll(currentClient->clientfd,&numero_mascotas,sizeof(int));
    Desbloquear();
    /////------>debloquear
    int numregistro=-1;
    
    while(numregistro==-1){
r=recvAll(currentClient->clientfd,&numregistro,sizeof(int));
    Bloquear();
    numregistro=numregistro>0&&numregistro<=numero_mascotas?numregistro:-1;
    Desbloquear();
    r=sendAll(currentClient->clientfd,&numregistro,sizeof(int));
    }
    
    struct Nodo * por_abrir;
    
    char decision;
    r=recvAll(currentClient->clientfd,&decision,sizeof(char));
    if(decision=='S'|| decision=='s'){
      ////------->bloquear
      int menosuno=-1;
      int uno=1;
      Bloquear();
      if(numregistro<=0 || numregistro>numero_mascotas){
        
        r=sendAll(currentClient->clientfd,&menosuno,sizeof(int));
        Desbloquear();
        continue;
      }else{
        r=sendAll(currentClient->clientfd,&uno,sizeof(int));
      }
      por_abrir=LeerdeBD(numregistro-1);
      
      Desbloquear();
      //////---->desbloquear
      
      
      char* seq=concat(por_abrir->mascota.nombre,por_abrir->mascota.raza);
      char *s1=concat("gedit ",seq);
	  char * archivo=concat(seq,".txt");
    //-------->bloquear historia
int p;
bool disponible;
    do{
///////------->bloquing para lectura unicamente con sleep() para que otros hilos puedan leer si esta abierta otra historia (no la del mismo registro)
disponible=true;
BloquearHistorias();
for(p=0;p<NUMHILOS;p++){
disponible=disponible&&HISTORIA_ABIERTA[p]!=numregistro;
}
DesbloquearHistorias();
//////------>desbloquing 
    }while(!disponible);
HISTORIA_ABIERTA[currentClient->numhilo]=numregistro;
      
	  FILE * clinichistory=fopen(archivo,"r");
	  if(clinichistory==NULL){
	  clinichistory=fopen(archivo,"w");
	  fprintf(clinichistory,"NOMBRE:%s\nTIPO:%s\nEDAD:%i\nRAZA:%s\nESTATURA:%i\nPESO:%lf\nSEXO:%c\n",
	  por_abrir->mascota.nombre,
	  por_abrir->mascota.tipo,
	  por_abrir->mascota.edad,
	  por_abrir->mascota.raza,
	  por_abrir->mascota.estatura,
	  por_abrir->mascota.peso,
	  por_abrir->mascota.sexo
	  
	  );
	  
	  }
if(por_abrir!=NULL){free(por_abrir);}

	  fclose(clinichistory);
    
    r=sendAll(currentClient->clientfd,archivo,sizeof(char)*33);//envia nombre del archivo
    
    EnviarArchivo(currentClient->clientfd,archivo);
	 // free(clinichistory); descomentar haber si funciona
     RecibirArchivo(currentClient->clientfd,archivo);
    ///------->>> desbloquear historia
     HISTORIA_ABIERTA[currentClient->numhilo]=-1;
     // char *path=concat(s1,".txt");
      //system(path);
      log->opcion=opcion;
      log->registro=numregistro;
      log->ip=currentClient->ip;
      ////----->bloquear log
      BloquearLog();
      writelog(log);
      DesbloquearLog();
      ///---->desbloquear log
  }
  r=sendAll(currentClient->clientfd,mensaje_accion_realizada,sizeof(char)*33);
  
  }else if(opcion==3){
    //printf("el numero de registros presentes es: %i \n",numero_mascotas);
    //////----->bloquear
    Bloquear();
    r=sendAll(currentClient->clientfd,&numero_mascotas,sizeof(int));
    Desbloquear();
    //////-------->desbloquear
    int numregistro;
    r=recvAll(currentClient->clientfd,&numregistro,sizeof(int));
    int menosuno=-1;
    int uno=1;
    ////------------>bloquear

    Bloquear();
      if(numregistro<=0 || numregistro>numero_mascotas){
        
        r=sendAll(currentClient->clientfd,&menosuno,sizeof(int));
        Desbloquear();
        continue;
      }else{
        r=sendAll(currentClient->clientfd,&uno,sizeof(int));
      
    EliminarMascota(numregistro-1);
    Desbloquear();
    ////------>desbloquear
    log->opcion=opcion;
      log->registro=numregistro;
      log->ip=currentClient->ip;
      //---->bloquearLog
      BloquearLog();
      writelog(log);
      DesbloquearLog();
      //----->desbloquear log
    r=sendAll(currentClient->clientfd,mensaje_accion_realizada,sizeof(char)*33);
  
      }
    
  }else if(opcion ==4){
   // printf("inserte el nombre por favor");
    /*char *nombre=(char *)malloc(sizeof(char)*33);
    int jp;
    for(jp=0;jp<33;jp++){
      nombre[jp]=0;
    }*/
    char nombre[33];
    //toLower(nombre);
    if (nombre == NULL){
        perror("erroren el malloc");
        exit(-1);
    } 
    r=recvAll(currentClient->clientfd,nombre,sizeof(char)*33);
    //----->bloquear
    Bloquear();
    BuscarPorNombreyEnviaraCliente(nombre,0,currentClient->clientfd);
    Desbloquear();
    ////-------->desbloquear
     log->opcion=opcion;
      log->cadena_buscada=nombre;
      log->ip=currentClient->ip;
      /////---->bloquear log
      BloquearLog();
      writelog(log);
      DesbloquearLog();
      /////------>desbloquear log
     r=sendAll(currentClient->clientfd,mensaje_accion_realizada,sizeof(char)*33);
  }else {
     r=sendAll(currentClient->clientfd,mensaje_despedida,sizeof(char)*33);
      ////----->bloquear
      Bloquear();
      CambiarTamanioBd(numero_mascotas);
    GuardarTablaHash();
    Desbloquear();
    
    /////---->>desbloquear
    break;
  }


}while(1>0);
//////////////////////!!!!!!!!!!!!!!!!!!! posible seccion critica
close(currentClient->clientfd);
HILODISPONIBLE[currentClient->numhilo]=1;
numclientesactual--;
if(numclientesactual==0){
DestroySync();
printf("FINALIZADO");
exit(0);
}

}


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
  numclientesactual=0;
  int opcion;
  double peso;
  bool salir=false;
 
 InitSync();     
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
    HISTORIA_ABIERTA[i]=-1;
  }
  mensaje_accion_realizada=(char *)malloc(sizeof(char)*33);
  mensaje_accion_realizada="accion realizada\n";
mensaje_confirmacion=(char*)malloc(sizeof(char)*33);
mensaje_confirmacion="establecida\n"; 
mensaje_ocupado=(char*)malloc(sizeof(char)*33);
mensaje_ocupado="NO CONECTADO\n";
mensaje_despedida=(char*)malloc(sizeof(char)*33);
mensaje_despedida="ADIOS\n";
 pthread_t hilos[NUMHILOS];

      ///init
      int clientfd,r;
struct sockaddr_in server,client;
socklen_t len;
serverfd=socket(AF_INET,SOCK_STREAM,0);
int conf = 1;
setsockopt(serverfd, SOL_SOCKET, SO_REUSEADDR, &conf, sizeof(conf));//configuracion para liberar la direccion al terminar el programa
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

do{
  
  clientfd=accept(serverfd,(struct sockaddr *)&client,&len);
if(clientfd==-1){
    perror("error en el accept");
    exit(-1);
}
numclientesactual++;
struct cliente *currentClient=(struct cliente *)malloc(sizeof(struct cliente));
currentClient->clientfd=clientfd;
if(NumeroHiloDisponible()==-1){//no se pueden aceptar mas hilos 
r=sendAll(clientfd,mensaje_ocupado,sizeof(char)*15);
if(r==-1){
  perror("error en send");
}
}else{
  currentClient->numhilo=NumeroHiloDisponible();
  HILODISPONIBLE[currentClient->numhilo]=0;
//printf("%i",currentClient->numhilo);
currentClient->ip=inet_ntoa(client.sin_addr);
//printf("%s",currentClient->ip);
pthread_create(&hilos[currentClient->numhilo],NULL,funcion,currentClient);
}
/*  int finhilo;
bool hay_hilos_presentes=true;
while(hay_hilos_presentes){
  
for(finhilo=0;finhilo<NUMHILOS;finhilo++){
hay_hilos_presentes=HILODISPONIBLE[finhilo]==1&&hay_hilos_presentes;

}

} */


}while(numclientesactual>0);


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
  r=close(serverfd);
  CambiarTamanioBd(numero_mascotas);
  GuardarTablaHash();
  return 0;
}

