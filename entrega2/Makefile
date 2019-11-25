#con solo make ejecuta la regla del servidor
all:compile
compile:p2-dogServer p2-dogClient
p2-dogServer:p2-dogServer.c funciones.c log.c
	gcc -o p2-dogServer p2-dogServer.c -lpthread
p2-dogClient:p2-dogClient.c funciones.c
	gcc -o p2-dogClient p2-dogClient.c
	
