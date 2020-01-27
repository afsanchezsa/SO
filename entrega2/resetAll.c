#include <stdio.h>
#include <stdlib.h>
#define tam 100151

void IniciarTablaHash(){//solo se usa si el archivo no existe 
FILE *tablahash=fopen("tabla.dat","w");
int menousuno=-1; 
int i;
for(i=0;i<tam;i++){
fwrite(&menousuno,sizeof(menousuno),1,tablahash);
}
fclose(tablahash);
}
int main(){
IniciarTablaHash();
FILE * file=fopen("tambd.dat","w");
int cero=0;
fwrite(&cero,sizeof(int),1,file);
fclose(file);
system("rm dataDogs.dat");
return 0;

}
