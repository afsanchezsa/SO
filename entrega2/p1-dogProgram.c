#include "funciones.c"
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
      
      
do{
  printf("\nMenu\n");
  printf("1.Insertar\n");
  printf("2.Ver Registro\n");
  printf("3.Eliminar Registro\n");
  printf("4.Buscar por Nombre \n");
  printf("5.salir \n");

  printf("inserte una opcion por favor: ");
    scanf("%i",&opcion);

  if(opcion==1){
      struct dogType *insertado=LeerEstructura();
      //toLower(insertado->nombre);
      GuardarMascota(insertado,0);
    
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
    
    break;
  }
  //char ci;
  //printf("\npresione una tecla para continuar");
  //printf("\n");
  getch();
}while(!salir);

  CambiarTamanioBd(numero_mascotas);
  GuardarTablaHash();

  
  return 0;
}

