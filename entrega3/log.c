
#include <time.h>
#include <stdio.h>

struct Log{

    char *ip;
    int opcion;
    int registro;
    char * cadena_buscada;

};


char *fecha()
{
  time_t t;
  struct tm *tm;
  char *fechayhora=(char *) malloc(sizeof(char)*100);

  t=time(NULL);
  tm=localtime(&t);
  strftime(fechayhora, 100, "%Y/%m/%d %H:%M:%S", tm);
  return  fechayhora;
}
void writelog(struct Log * log){
FILE * file=fopen("serverDogs.log","a+");

if(log->opcion==1){
fprintf(file,"%s %s %s %i %s",fecha(),log->ip,"insercion",log->registro," \n");

}else if(log->opcion==2){
    fprintf(file,"%s %s %s %i %s",fecha(),log->ip,"lectura",log->registro," \n");

}else if(log->opcion==3){
    fprintf(file,"%s %s %s %i %s",fecha(),log->ip,"borrado",log->registro," \n");
}else if(log->opcion==4){
    fprintf(file,"%s %s %s %s %s",fecha(),log->ip,"busqueda",log->cadena_buscada," \n");

}

fclose(file);

free(log);
}