#include "funciones.c"

/////server
void * funcion(void *ap){
int *clientdf=(int *)ap;

int opcion;
int r;
r=send(*clientdf,"conexion establecida",20,0);

do{
 r=recv(serverfd,&opcion,sizeof(int),0);
    printf("el cliente escogio %i",opcion);
break;
  if(opcion==1){
      
    
  }else if(opcion==2){
}else if(opcion==3){
  
  }else if(opcion ==4){
    
  }else {
    
    break;
  }
 

}while(1>0);




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
  
  
  char *mensaje_accion_realizada=(char *)malloc(sizeof(char)*15);
  mensaje_accion_realizada="accion realizada\n";
char *mensaje_confirmacion=(char*)malloc(sizeof(char)*15);
mensaje_confirmacion="establecida\n"; 
  
 pthread_t tfd[NUMHILOS];

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

clientfd=accept(serverfd,(struct sockaddr *)&client,&len);
if(clientfd==-1){
    perror("error en el accept");
    exit(-1);
}


r=send(clientfd,mensaje_confirmacion,sizeof(char)*15,0);
if(r==-1){
  perror("error en send");
}

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

  
  return 0;
}

