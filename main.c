#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "tdas/map.h"
#include "tdas/list.h"
#include "tdas/queue.h"
#include "tdas/stack.h"

typedef struct{
    int puntaje;
    int id;
} RankingKey;

typedef struct{
    int id;
    char nombre[50];

    int victorias;
    int derrotas;
    int partidasJugadas;

    int puntaje;

    RankingKey* rankingKey;

} Jugador;

typedef struct{
    int idPartido;

    Jugador* jugador1;
    Jugador* jugador2;

} Partido;

typedef struct{

    int idPartido;

    Jugador* jugador1;
    Jugador* jugador2;

    Jugador* ganador;
    Jugador* perdedor;

    int puntosGanador;
    int puntosPerdedor;

} Resultado;

typedef struct{

    Map* jugadores;
    Map* ranking;

    List* participantes;

    Queue* partidosPendientes;

    Stack* historial;
    List* historialCompleto;

    char nombreTorneo[100];

    int torneoActivo;

    int siguientePartido;

} SistemaTorneos;

int is_equal_int(void* key1, void* key2){

    int* id1 = (int*) key1;
    int* id2 = (int*) key2;

    return (*id1 == *id2);
}

int higher_than_ranking(void* key1, void* key2){

    RankingKey* r1 = (RankingKey*) key1;
    RankingKey* r2 = (RankingKey*) key2;

    if(r1->puntaje != r2->puntaje)
        return r1->puntaje > r2->puntaje;

    return r1->id < r2->id;
}

SistemaTorneos* crearSistema()
{
    SistemaTorneos* sistema = malloc(sizeof(SistemaTorneos));

    if(sistema == NULL)
        return NULL;

    sistema->jugadores = map_create(is_equal_int);

    sistema->ranking = sorted_map_create(higher_than_ranking);

    sistema->participantes = list_create();

    sistema->partidosPendientes = queue_create(NULL);

    sistema->historial = stack_create(NULL);

    sistema->historialCompleto = list_create();

    sistema->torneoActivo = 0;

    strcpy(sistema->nombreTorneo,"");

    sistema->siguientePartido = 1;

    return sistema;
}

int leerOpcion();
void menuPrincipal(SistemaTorneos* sistema);
void menuJugadores(SistemaTorneos* sistema);
void menuTorneos(SistemaTorneos* sistema);
void menuPartidos(SistemaTorneos* sistema);
void menuEstadisticas(SistemaTorneos* sistema);
void limpiarBuffer();
int leerID();
void registrarJugador(SistemaTorneos* sistema);
void buscarJugador(SistemaTorneos* sistema);
void mostrarJugadores(SistemaTorneos* sistema);
void eliminarJugador(SistemaTorneos* sistema);
void crearTorneo(SistemaTorneos* sistema);
void inscribirJugador(SistemaTorneos* sistema);
void mostrarParticipantes(SistemaTorneos* sistema);
void generarEnfrentamientos(SistemaTorneos* sistema);
void verProximoPartido(SistemaTorneos* sistema);
void registrarResultado(SistemaTorneos* sistema);
void mostrarPartidosPendientes(SistemaTorneos* sistema);
void mostrarHistorial(SistemaTorneos* sistema);
void mostrarRanking(SistemaTorneos* sistema);
void top3Jugadores(SistemaTorneos* sistema);
void estadisticasJugador(SistemaTorneos* sistema);
void deshacerUltimaAccion(SistemaTorneos* sistema);

int leerOpcion()
{
    int opcion;

    while(1)
    {
        if(scanf("%d", &opcion) == 1)
        {
            return opcion;
        }

        puts("\nError: Debe ingresar un numero.");
        limpiarBuffer();
        printf("Seleccione una opcion: ");
    }
}

void menuPrincipal(SistemaTorneos* sistema)
{
    int opcion;
    
    do{
        puts("");
        puts("===== SISTEMA DE GESTION DE TORNEOS =====");
        puts("1. Gestion de jugadores");
        puts("2. Gestion de torneos");
        puts("3. Gestion de partidos");
        puts("4. Ranking y estadisticas");
        puts("5. Deshacer ultima accion");
        puts("0. Salir");

        printf("\nSeleccione una opcion: ");
        opcion = leerOpcion();

        switch(opcion){
            case 1:
                menuJugadores(sistema);
                break;

            case 2:
                menuTorneos(sistema);
                break;

            case 3:
                menuPartidos(sistema);
                break;

            case 4:
                menuEstadisticas(sistema);
                break;

            case 5:
                deshacerUltimaAccion(sistema);
                break;
        }
        
    }while(opcion != 0);
}

void menuJugadores(SistemaTorneos* sistema){

    int opcion;

    do{
        puts("");
        puts("===== GESTION DE JUGADORES =====");
        puts("1. Registrar jugador");
        puts("2. Buscar jugador");
        puts("3. Mostrar todos los jugadores");
        puts("4. Eliminar jugador");
        puts("0. Volver");


        printf("\nSeleccione una opcion: ");
        opcion = leerOpcion();

        switch(opcion){

            case 1:
                registrarJugador(sistema);
                break;

            case 2:
                buscarJugador(sistema);
                break;

            case 3:
                mostrarJugadores(sistema);
                break;

            case 4:
                eliminarJugador(sistema);
                break;
        }

    }while(opcion != 0);
}

void limpiarBuffer()
{
    int c;

    while((c = getchar()) != '\n' && c != EOF);
}

int leerID()
{
    int id;

    while(1)
    {
        printf("Ingrese ID: ");

        if(scanf("%d", &id) == 1 && id > 0)
        {
            return id;
        }

        puts("Error: Debe ingresar un numero entero positivo.");
        limpiarBuffer();
    }
}

void registrarJugador(SistemaTorneos* sistema)
{
    Jugador* jugador = malloc(sizeof(Jugador));
    jugador->id = leerID();

    if(map_search(sistema->jugadores, &jugador->id) != NULL){

        puts("\nYa existe un jugador con ese ID.");

        free(jugador);

        return;
    }

    printf("Ingrese nombre: ");
    if(scanf(" %49[^\n]", jugador->nombre) != 1)
    {
        puts("\nError al leer nombre.");
        free(jugador);
        return;
    }

    jugador->victorias = 0;
    jugador->derrotas = 0;
    jugador->partidasJugadas = 0;
    jugador->puntaje = 0;

    jugador->rankingKey = malloc(sizeof(RankingKey));

    jugador->rankingKey->puntaje = 0;
    jugador->rankingKey->id = jugador->id;

    map_insert(sistema->jugadores, &jugador->id, jugador);

    map_insert(sistema->ranking, jugador->rankingKey, jugador);
    puts("\nJugador registrado.");
}

void buscarJugador(SistemaTorneos* sistema)
{
    int id = leerID();

    MapPair* pair = map_search(sistema->jugadores, &id);

    if(pair == NULL){

        puts("\nJugador no encontrado.");
        return;
    }

    Jugador* jugador = pair->value;

    printf("\nID: %d\n", jugador->id);

    printf("Nombre: %s\n", jugador->nombre);

    printf("Victorias: %d\n", jugador->victorias);

    printf("Derrotas: %d\n", jugador->derrotas);

    printf("Puntaje: %d\n", jugador->puntaje);
}

void mostrarJugadores(SistemaTorneos* sistema)
{
    MapPair* pair =map_first(sistema->jugadores);

    if(pair == NULL){

        puts("\nNo hay jugadores.");
        return;
    }

    puts("\n===== JUGADORES =====");

    while(pair != NULL){

        Jugador* jugador = pair->value;

        printf("ID: %d | %s\n", jugador->id, jugador->nombre);

        pair = map_next(sistema->jugadores);
    }
}

void eliminarJugador(SistemaTorneos* sistema){

    int id = leerID();

    MapPair* pair = map_search(sistema->jugadores, &id);

    if(pair == NULL){

        puts("\nJugador no encontrado.");
        return;
    }

    Jugador* jugador = pair->value;

    /* Verificar si está inscrito */

    Jugador* participante =list_first(sistema->participantes);

    while(participante != NULL){

        if(participante->id == id){

            puts("\nNo se puede eliminar un jugador inscrito.");

            return;
        }

        participante =list_next(sistema->participantes);
    }

    /* Eliminar del ranking */

    MapPair* pairRanking = map_remove(sistema->ranking, jugador->rankingKey);

    if(pairRanking != NULL)
        free(pairRanking);

    /* Eliminar del mapa de jugadores */

    MapPair* pairJugador = map_remove(sistema->jugadores, &id);

    if(pairJugador != NULL)
        free(pairJugador);

    /* Liberar jugador */

    free(jugador->rankingKey);
    free(jugador);

    puts("\nJugador eliminado.");
}

void menuTorneos(SistemaTorneos* sistema)
{
    int opcion;

    do{
        puts("");
        puts("===== GESTION DE TORNEOS =====");
        puts("1. Crear torneo");
        puts("2. Inscribir jugador");
        puts("3. Mostrar participantes");
        puts("4. Generar enfrentamientos");
        puts("0. Volver");

        printf("\nSeleccione una opcion: ");
        opcion = leerOpcion();

        switch(opcion){

            case 1:
                crearTorneo(sistema);
                break;

            case 2:
                inscribirJugador(sistema);
                break;

            case 3:
                mostrarParticipantes(sistema);
                break;

            case 4:
                generarEnfrentamientos(sistema);
                break;
        }

    }while(opcion != 0);
}

void finalizarTorneo(SistemaTorneos* sistema)
{
    MapPair* pair = map_first(sistema->jugadores);

    while(pair != NULL)
    {
        Jugador* jugador = pair->value;

        free(jugador->rankingKey);
        free(jugador);

        pair = map_next(sistema->jugadores);
    }

    map_clean(sistema->jugadores);
    map_clean(sistema->ranking);

    list_clean(sistema->participantes);

    queue_clean(sistema->partidosPendientes);

    stack_clean(sistema->historial);

    list_clean(sistema->historialCompleto);

    sistema->siguientePartido = 1;

    // Reiniciar datos del torneo
    sistema->torneoActivo = 0;
    sistema->nombreTorneo[0] = '\0';

    puts("\nTorneo finalizado correctamente.");
}

void crearTorneo(SistemaTorneos* sistema)
{
    if(sistema->torneoActivo){

        char opcion;

        printf("\nYa existe un torneo activo.");
        printf("\n¿Desea finalizarlo y crear uno nuevo? (S/N): ");
        if(scanf(" %c", &opcion) != 1)
        {
            puts("\nError de lectura.");
            return;
        }

        if(opcion != 'S' && opcion != 's')
            return;

        finalizarTorneo(sistema);
    }

    printf("\nNombre del torneo: ");
    if(scanf(" %99[^\n]", sistema->nombreTorneo) != 1)
    {
        puts("\nError al leer nombre del torneo.");
        return;
    }

    sistema->torneoActivo = 1;
    puts("\nTorneo creado correctamente.");
}

void inscribirJugador(SistemaTorneos* sistema)
{
    if(!sistema->torneoActivo){

        puts("\nNo existe un torneo activo.");
        return;
    }
    int id = leerID();

    MapPair* pair = map_search(sistema->jugadores, &id);

    if(pair == NULL){

        puts("\nJugador no encontrado.");
        return;
    }

    Jugador* jugador = pair->value;

    Jugador* participante = list_first(sistema->participantes);

    while(participante != NULL){

        if(participante->id == id){

            puts("\nEl jugador ya esta inscrito.");
            return;
        }

        participante = list_next(sistema->participantes);
    }

    list_pushBack(sistema->participantes, jugador);

    puts("\nJugador inscrito.");
}

void mostrarParticipantes(SistemaTorneos* sistema)
{
    if(!sistema->torneoActivo){

        puts("\nNo existe un torneo activo.");
        return;
    }
    Jugador* jugador = list_first(sistema->participantes);

    if(jugador == NULL){

        puts("\nNo hay participantes.");
        return;
    }

    puts("\n===== PARTICIPANTES =====");

    while(jugador != NULL){

        printf("ID: %d | %s\n", jugador->id, jugador->nombre);

        jugador = list_next(sistema->participantes);
    }
}

void generarEnfrentamientos(SistemaTorneos* sistema)
{
    if(!sistema->torneoActivo){

        puts("\nNo existe un torneo activo.");
        return;
    }
    if(queue_front(sistema->partidosPendientes) != NULL){

        puts("\nYa existen partidos generados.");

        return;
    }

    if(list_size(sistema->participantes) < 2){

        puts("\nSe necesitan al menos 2 participantes.");

        return;
    }

    List* copia = list_create();

    Jugador* jugador = list_first(sistema->participantes);

    while(jugador != NULL){

        list_pushBack(copia, jugador);

        jugador = list_next(sistema->participantes);
    }

    while(list_size(copia) > 1){

        Jugador* jugador1 = list_first(copia);

        Jugador* jugador2 = list_next(copia);

        while(jugador2 != NULL){

            Partido* partido = malloc(sizeof(Partido));

            partido->idPartido = sistema->siguientePartido++;

            partido->jugador1 = jugador1;

            partido->jugador2 = jugador2;

            queue_insert(sistema->partidosPendientes, partido);

            jugador2 = list_next(copia);
        }

        list_popFront(copia);
    }

    list_clean(copia);
    free(copia);

    puts("\nEnfrentamientos generados correctamente.");
}

void menuPartidos(SistemaTorneos* sistema)
{
    int opcion;

    do{
        puts("");
        puts("===== GESTION DE PARTIDOS =====");
        puts("1. Ver proximo partido");
        puts("2. Registrar resultado");
        puts("3. Mostrar partidos pendientes");
        puts("4. Mostrar historial");
        puts("0. Volver");

        printf("\nSeleccione una opcion: ");
        opcion = leerOpcion();

        switch(opcion){

            case 1:
                verProximoPartido(sistema);
                break;

            case 2:
                registrarResultado(sistema);
                break;

            case 3:
                mostrarPartidosPendientes(sistema);
                break;

            case 4:
                mostrarHistorial(sistema);
                break;
        }

    }while(opcion != 0);
}

void verProximoPartido(SistemaTorneos* sistema)
{

    Partido* partido = queue_front(sistema->partidosPendientes);

    if(partido == NULL) {

        puts("\nNo hay partidos pendientes.");
        return;
    }

    puts("\n===== PROXIMO PARTIDO =====");

    printf("%s vs %s\n", partido->jugador1->nombre, partido->jugador2->nombre);
}

void actualizarRanking(SistemaTorneos* sistema, Jugador* jugador)
{
    MapPair* eliminado = map_remove(sistema->ranking, jugador->rankingKey);

    if(eliminado != NULL)
        free(eliminado);

    jugador->rankingKey->puntaje = jugador->puntaje;

    map_insert(sistema->ranking, jugador->rankingKey, jugador);
}

void registrarResultado(SistemaTorneos* sistema)
{
    Partido* partido = queue_front(sistema->partidosPendientes);

    if(partido == NULL) {

        puts("\nNo hay partidos pendientes.");
        return;
    }

    puts("\n===== REGISTRAR RESULTADO =====");

    printf("1. %s\n",partido->jugador1->nombre);

    printf("2. %s\n", partido->jugador2->nombre);

    int opcion;

    do
    {
        printf("\nGanador (1 o 2): ");

        while(scanf("%d", &opcion) != 1)
        {
            puts("Error: Debe ingresar un numero.");

            limpiarBuffer();

            printf("\nGanador (1 o 2): ");
        }

    } while(opcion != 1 && opcion != 2);

    Jugador* ganador;
    Jugador* perdedor;

    if(opcion == 1) {

        ganador = partido->jugador1;

        perdedor = partido->jugador2;
    }

    else if(opcion == 2) {

        ganador = partido->jugador2;

        perdedor = partido->jugador1;
    }

    else {

        puts("\nOpcion invalida.");
        return;
    }

    ganador->victorias++;
    ganador->partidasJugadas++;
    ganador->puntaje += 3;

    perdedor->derrotas++;
    perdedor->partidasJugadas++;

    actualizarRanking(sistema, ganador);

    actualizarRanking(sistema, perdedor);

    Resultado* resultado = malloc(sizeof(Resultado));

    resultado->idPartido = partido->idPartido;

    resultado->jugador1 = partido->jugador1;

    resultado->jugador2 = partido->jugador2;

    resultado->ganador = ganador;

    resultado->perdedor = perdedor;

    resultado->puntosGanador = 3;
    resultado->puntosPerdedor = 0;

    stack_push(sistema->historial, resultado);

    list_pushBack(sistema->historialCompleto, resultado);

    Partido* eliminado = queue_remove(sistema->partidosPendientes);

    free(eliminado);

    puts("\nResultado registrado.");
}

void mostrarPartidosPendientes(SistemaTorneos* sistema)
{
    Partido* partido = queue_front(sistema->partidosPendientes);

    if(partido == NULL){

        puts("\nNo hay partidos pendientes.");

        return;
    }

    puts("");
    puts("===== PARTIDOS PENDIENTES =====");

    while(partido != NULL){

        printf("Partido %d: %s vs %s\n", partido->idPartido, partido->jugador1->nombre, partido->jugador2->nombre);

        partido = queue_next(sistema->partidosPendientes);
    }
}

void mostrarHistorial(SistemaTorneos* sistema)
{
    Resultado* resultado =list_first(sistema->historialCompleto);

    if(resultado == NULL){

        puts("\nNo hay resultados registrados.");

        return;
    }

    puts("");
    puts("===== HISTORIAL =====");

    while(resultado != NULL){

        printf("Partido %d | %s vencio a %s\n", resultado->idPartido, resultado->ganador->nombre, resultado->perdedor->nombre);

        resultado = list_next(sistema->historialCompleto);
    }
}

void menuEstadisticas(
    SistemaTorneos* sistema
){

    int opcion;

    do{

        puts("");
        puts("===== RANKING Y ESTADISTICAS =====");

        puts("1. Mostrar ranking");
        puts("2. Top 3 jugadores");
        puts("3. Estadisticas de jugador");
        puts("0. Volver");

        printf("\nSeleccione una opcion: ");
        opcion = leerOpcion();

        switch(opcion){

            case 1:
                mostrarRanking(sistema);
                break;

            case 2:
                top3Jugadores(sistema);
                break;

            case 3:
                estadisticasJugador(sistema);
                break;
        }

    }while(opcion != 0);
}

void mostrarRanking(SistemaTorneos* sistema)
{
    MapPair* pair = map_first(sistema->ranking);

    if(pair == NULL){

        puts("\nNo hay jugadores registrados.");
        return;
    }

    puts("");
    puts("===== RANKING =====");

    int posicion = 1;

    while(pair != NULL){

        Jugador* jugador = pair->value;

        printf("%d. %s | %d puntos\n", posicion, jugador->nombre, jugador->puntaje);

        posicion++;

        pair = map_next(sistema->ranking);
    }
}

void top3Jugadores(SistemaTorneos* sistema)
{
    MapPair* pair = map_first(sistema->ranking);

    if(pair == NULL){

        puts("\nNo hay jugadores registrados.");
        return;
    }

    puts("");
    puts("===== TOP 3 JUGADORES =====");

    int posicion = 1;

    while(pair != NULL && posicion <= 3){

        Jugador* jugador =pair->value;
        printf("%d. %s (%d puntos)\n", posicion, jugador->nombre, jugador->puntaje);
        posicion++;
        pair = map_next(sistema->ranking);
    }
}

void estadisticasJugador(SistemaTorneos* sistema)
{
    int id = leerID();

    MapPair* pair = map_search(sistema->jugadores, &id);

    if(pair == NULL){

        puts("\nJugador no encontrado.");
        return;
    }

    Jugador* jugador = pair->value;

    puts("");
    puts("===== ESTADISTICAS =====");

    printf("ID: %d\n", jugador->id);
    printf("Nombre: %s\n", jugador->nombre);
    printf("Victorias: %d\n", jugador->victorias);
    printf("Derrotas: %d\n", jugador->derrotas);
    printf("Partidas Jugadas: %d\n", jugador->partidasJugadas);
    printf("Puntaje: %d\n", jugador->puntaje);
}

void deshacerUltimaAccion(SistemaTorneos* sistema)
{
    if(!sistema->torneoActivo){
        puts("\nNo existe un torneo activo.");
        return;
    }

    Resultado* ultimo = stack_top(sistema->historial);
    if(ultimo == NULL){
        puts("\nNo hay acciones para deshacer.");
        return;
    }

    printf("\nUltimo resultado registrado:\n");
    printf("%s vencio a %s\n", ultimo->ganador->nombre, ultimo->perdedor->nombre);

    printf("\n¿Desea revertir esta accion? (S/N): ");

    char opcion;
    if(scanf(" %c", &opcion) != 1)
    {
        puts("\nError de lectura.");
        return;
    }

    if(opcion != 'S' && opcion != 's')
        return;

    ultimo = stack_pop(sistema->historial);

    ultimo->ganador->victorias--;
    ultimo->ganador->partidasJugadas--;
    ultimo->ganador->puntaje -= ultimo->puntosGanador;

    ultimo->perdedor->derrotas--;
    ultimo->perdedor->partidasJugadas--;
    ultimo->perdedor->puntaje -= ultimo->puntosPerdedor;

    ultimo->ganador->rankingKey->puntaje = ultimo->ganador->puntaje;

    ultimo->perdedor->rankingKey->puntaje = ultimo->perdedor->puntaje;

    Partido* partido = malloc(sizeof(Partido));

    partido->idPartido = ultimo->idPartido;

    partido->jugador1 = ultimo->jugador1;

    partido->jugador2 = ultimo->jugador2;

    queue_insert(sistema->partidosPendientes, partido);
    list_popBack(sistema->historialCompleto);
    free(ultimo);

    puts("\nAccion revertida correctamente.");
}


void liberarSistema(SistemaTorneos* sistema)
{
    /* Liberar partidos pendientes */
    Partido* partido = queue_remove(sistema->partidosPendientes);
    while(partido != NULL){

        free(partido);
        partido = queue_remove(sistema->partidosPendientes);
    }

    /* Liberar resultados */
    Resultado* resultado = list_popFront(sistema->historialCompleto);
    while(resultado != NULL){

        free(resultado);
        resultado = list_popFront(sistema->historialCompleto);
    }

    /* Limpiar pila */
    stack_clean(sistema->historial);

    /* Liberar MapPair del ranking */
    while(map_first(sistema->ranking) != NULL){

        MapPair* primero = map_first(sistema->ranking);
        MapPair* eliminado = map_remove(sistema->ranking, primero->key);
        free(eliminado);
    }

    /* Guardar jugadores para liberarlos despues */
    MapPair* pair = map_first(sistema->jugadores);
    while(pair != NULL){

        list_pushBack(sistema->participantes, pair->value);
        pair = map_next(sistema->jugadores);
    }

    /* Liberar MapPair del mapa jugadores */
    while(map_first(sistema->jugadores) != NULL){

        MapPair* primero = map_first(sistema->jugadores);
        MapPair* eliminado = map_remove(sistema->jugadores, primero->key);
        free(eliminado);
    }

    /* Liberar jugadores */
    Jugador* jugador = list_popFront(sistema->participantes);
    while(jugador != NULL){

        free(jugador->rankingKey);
        free(jugador);
        jugador = list_popFront(sistema->participantes);
    }

    /* Liberar listas internas de los mapas */
    free(sistema->jugadores->ls);
    free(sistema->ranking->ls);

    /* Liberar mapas */
    free(sistema->jugadores);
    free(sistema->ranking);

    /* Liberar estructuras restantes */
    free(sistema->participantes);
    free(sistema->partidosPendientes);
    free(sistema->historial);
    free(sistema->historialCompleto);

    /* Liberar sistema */
    free(sistema);
}

int main(){

    SistemaTorneos* sistema = crearSistema();

    menuPrincipal(sistema);

    liberarSistema(sistema);

    return 0;
}