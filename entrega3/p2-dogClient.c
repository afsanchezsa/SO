#include "funciones.c"

int main(){
char mensaje[33];
char filename[33];
  int clientfd,r;
struct sockaddr_in client;
socklen_t len;
double sumando;
clientfd=socket(AF_INET,SOCK_STREAM,0);
if(clientfd==-1){
    perror("erroe en el socket");
    exit(-1);

}
client.sin_family=AF_INET;
client.sin_port=htons(PORT);
client.sin_addr.s_addr=inet_addr("127.0.0.1");

bzero(client.sin_zero,8);
len=sizeof(struct sockaddr);
r=connect(clientfd,(struct sockaddr *)&client,len);
if(r==-1){
    perror("error en el connect");
    exit(-1);
}
r=recvAll(clientfd,mensaje,sizeof(char)*33);
if(r==-1){
    perror("error en el recv");
    exit(-1);
}
printf("%s",mensaje);
int opcion;
/*if(strcmp(mensaje,"NO CONECTADO")){
  exit(-1);
} */   
do{
  printf("\nMenu\n");
  printf("1.Insertar\n");
  printf("2.Ver Registro\n");
  printf("3.Eliminar Registro\n");
  printf("4.Buscar por Nombre \n");
  printf("5.salir \n");

  printf("inserte una opcion por favor: ");
    scanf("%i",&opcion);
r=sendAll(clientfd,&opcion,sizeof(int));
  if(opcion==1){
      struct dogType *insertado=LeerEstructura();
      //toLower(insertado->nombre);
      //GuardarMascota(insertado,0);
      r=sendAll(clientfd,insertado,sizeof(struct dogType));
      free(insertado);
      r=recvAll(clientfd,mensaje,sizeof(char)*33);
      printf("%s",mensaje);
    
  }else if(opcion==2){
    r=recvAll(clientfd,&numero_mascotas,sizeof(int));
    printf("el numero de registros presentes es: %i \n",numero_mascotas);
   
    int numregistro;
    
    int respuesta;
    do{
      do{
    printf("por favor seleccione el numero de registro ");
    scanf("%i",&numregistro);
    }while(numregistro<=0 || numregistro>numero_mascotas);
    
      r=sendAll(clientfd,&numregistro,sizeof(int));
    
      r=recvAll(clientfd,&respuesta,sizeof(int));
    }while(respuesta==-1);
    printf("Desea abrir la historia clinica de la mascota? (S/N)");
    char decision;
    scanf(" %c",&decision);
    r=sendAll(clientfd,&decision,sizeof(char));
    
    if(decision=='S'|| decision=='s'){
      r=recvAll(clientfd,&respuesta,sizeof(int));
      if(respuesta==-1){
        printf("Lo sentimos el numero elegido ya no se encuentra disponible \n");
        continue;
      }
      r=recvAll(clientfd,filename,sizeof(char)*33);
      char *newFile=concat("second",filename);
      RecibirArchivo(clientfd,newFile);
      system(concat("gedit ",newFile));
      EnviarArchivo(clientfd,newFile);
      system(concat("rm ",newFile));
      free(newFile);
    }
    
      r=recvAll(clientfd,mensaje,sizeof(char)*33);
      printf("%s",mensaje);
   
   }else if(opcion==3){
      r=recvAll(clientfd,&numero_mascotas,sizeof(int));
    printf("el numero de registros presentes es: %i \n",numero_mascotas);
    
    int numregistro;
    do{
      printf("por favor seleccione el numero de registro que desea eliminar : ");
      scanf("%i",&numregistro);
    }while(numregistro<=0 || numregistro>numero_mascotas);
    
    
    r=sendAll(clientfd,&numregistro,sizeof(int));
   // EliminarMascota(numregistro-1);
   int respuesta;
   r=recvAll(clientfd,&respuesta,sizeof(int));
      if(respuesta==-1){
        printf("Lo sentimos el numero elegido ya no se encuentra disponible \n");
        continue;
      }
   r=recvAll(clientfd,mensaje,sizeof(char)*33);
      printf("%s",mensaje);
  }else if(opcion ==4){
    struct dogType * recibido=(struct dogType *)malloc(sizeof(struct dogType));
    printf("inserte el nombre por favor");
    /*char *nombre=(char *)malloc(sizeof(char)*33);
    int jp;
    for(jp=0;jp<33;jp++){
      nombre[jp]='\0';
    }*/
    char nombre[33];
    //toLower(nombre);
    if (nombre == NULL){
        perror("erroren el malloc");
        exit(-1);
    } 
    scanf("%s",nombre);
      r=sendAll(clientfd,nombre,sizeof(char)*33);
    //BuscarPorNombre(nombre,0);
    int id;
    do{
      r=recvAll(clientfd,&id,sizeof(int));
      if(id==-5){
        printf("lo sentimos no se han encontrado resultados");  
      break;
      }else if(id==-4){
        break;
      }else {
        
        r=recvAll(clientfd,recibido,sizeof(struct dogType));
        
        
        char * first=MyLower(nombre);
        char * second=MyLower(recibido->nombre);
        printf("la comparacion esta dando:%i \n y el id es :%i\n el num mas es %i",strcmp(first,second),id,numero_mascotas);
        if(id<=0 ||strcmp(first,second)!=0){continue;}
        ImprimirEstructura(recibido);
        free(first);
        free(second);
      }

    }while(1>0);
       r=recvAll(clientfd,mensaje,sizeof(char)*33);
       printf("%s",mensaje);
  }else {
    r=recvAll(clientfd,mensaje,sizeof(char)*33);
       printf("%s",mensaje);
    break;
  }
  //char ci;
  //printf("\npresione una tecla para continuar");
  //printf("\n");
  getch();
}while(1>0);
close(clientfd); 

  return 0;
}

