#include "funciones.c"

int main(){
char mensaje[33];
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
printf("\nMenu\n");
  printf("1.Insertar\n");
  printf("2.Ver Registro\n");
  printf("3.Eliminar Registro\n");
  printf("4.Buscar por Nombre \n");
  printf("5.salir \n");

  printf("inserte una opcion por favor: ");
    scanf("%i",&opcion);
r=send(clientfd,&opcion,sizeof(int),0);

close(clientfd); 
  return 0;
}

