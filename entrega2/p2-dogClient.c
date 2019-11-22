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
r=recv(clientfd,mensaje,sizeof(char)*33,0);
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
r=send(clientfd,&opcion,sizeof(int),0);
  if(opcion==1){
      struct dogType *insertado=LeerEstructura();
      //toLower(insertado->nombre);
      //GuardarMascota(insertado,0);
      r=send(clientfd,insertado,sizeof(struct dogType),0);
      free(insertado);
      r=recv(clientfd,mensaje,sizeof(char)*33,0);
      printf("%s",mensaje);
    
  }else if(opcion==2){
    r=recv(clientfd,&numero_mascotas,sizeof(int),0);
    printf("el numero de registros presentes es: %i \n",numero_mascotas);
    printf("por favor seleccione el numero de registro ");
    int numregistro;
    scanf("%i",&numregistro);
    r=send(clientfd,&numregistro,sizeof(int),0);
    printf("Desea abrir la historia clinica de la mascota? (S/N)");
    char decision;
    scanf(" %c",&decision);
    r=send(clientfd,&decision,sizeof(char),0);
    
    if(decision=='S'|| decision=='s'){
      r=recv(clientfd,filename,sizeof(char)*33,0);
      RecibirArchivo(clientfd,filename);
      system(concat("gedit ",filename));
    EnviarArchivo(clientfd,filename);
   
    }
    
      r=recv(clientfd,mensaje,sizeof(char)*33,0);
      printf("%s",mensaje);
   
   }else if(opcion==3){
      r=recv(clientfd,&numero_mascotas,sizeof(int),0);
    printf("el numero de registros presentes es: %i \n",numero_mascotas);
    printf("por favor seleccione el numero de registro que desea eliminar : ");
    int numregistro;
    scanf("%i",&numregistro);
    r=send(clientfd,&numregistro,sizeof(int),0);
   // EliminarMascota(numregistro-1);
   r=recv(clientfd,mensaje,sizeof(char)*33,0);
      printf("%s",mensaje);
  }else if(opcion ==4){
    struct dogType * recibido=(struct dogType *)malloc(sizeof(struct dogType));
    printf("inserte el nombre por favor");
    char *nombre=(char *)malloc(sizeof(char)*33);
    //toLower(nombre);
    if (nombre == NULL){
        perror("erroren el malloc");
        exit(-1);
    } 
    scanf("%s",nombre);
      r=send(clientfd,nombre,sizeof(char)*33,0);
    //BuscarPorNombre(nombre,0);
    int id;
    do{
      r=recv(clientfd,&id,sizeof(int),0);
      if(id==-5){
        printf("lo sentimos no se han encontrado resultados");  
      break;
      }else if(id==-4){
        break;
      }else{
        r=recv(clientfd,recibido,sizeof(struct dogType),0);
        ImprimirEstructura(recibido);
      }

    }while(1>0);
       r=recv(clientfd,mensaje,sizeof(char)*33,0);
       printf("%s",mensaje);
  }else {
    r=recv(clientfd,mensaje,sizeof(char)*33,0);
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

