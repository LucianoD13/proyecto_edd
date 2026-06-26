## Descripción
El Sistema de Gestión de Torneos es una aplicación desarrollada en lenguaje C que permite administrar
torneos mediante una interfaz por consola. El sistema facilita el registro de jugadores, la creación de un torneo, 
la organización de enfrentamientos, el registro de resultados y la visualización del ranking actualizado de los participantes, 
entre otros.

## Compilar y ejecutar(utilizar replit)
````
gcc *.c tdas/*.c -o main
````

Y luego ejecutar:
````
./main
````

## Funcionalidades

  ## Funcionando correctamente:
  -Función leerOpcion
  
  -Función menuPrincipal
  
  -Función menuJugadores
  
  -Función menuTorneos
  
  -Función menuPartidos
  
  -Función menuEstadisticas
  
  -Función limpiarBuffer
  
  -Función leerID
  
  -Función registrarJugador
  
  -Función buscarJugador
  
  -Función mostrarJugadores
  
  -Función eliminarJugador

  -Función finalizarTorneo
  
  -Función crearTorneo
  
  -Función inscribirJugador
  
  -Función mostrarParticipantes
  
  -Función generarEnfrentamientos
  
  -Función verProximoPartido

  -Función actualizarRanking
  
  -Función registrarResultado
  
  -Función mostrarPartidosPendientes
  
  -Función mostrarHistorial
  
  -Función mostrarRanking
  
  -Función top3Jugadores
  
  -Función estadisticasJugador
  
  -Función deshacerUltimaAccion

  -Función liberarSistema
  
  ## Problemas conocidos:
  -Actualmente los datos del sistema solo permanecen durante la ejecución del programa. 
  Al finalizar, toda la información almacenada en memoria se pierde.

  -Una vez finalizado un torneo, toda la información asociada a él se elimina para permitir la creación de un nuevo torneo.

  ## A mejorar:
  -Permitir guardar y cargar la información de jugadores, 
  torneos y resultados desde archivos para conservar los datos entre ejecuciones.

  -Gestionar varios torneos simultáneamente en lugar de permitir solo uno activo a la vez.

  -Agregar modalidades como eliminación directa, doble eliminación o fase de grupos, además del sistema actual.

  -Mostrar información como porcentaje de victorias, historial individual de enfrentamientos y cantidad de torneos ganados.

## Ejemplos de uso:

  -Gestionar Jugadores
  
  -Gestión de torneo
  
  -Puede probar finalizar el torneo actual y empezar otro, para esto debe crear un torneo y usar esta misma opción 
  posteriormente. Considere que debera hacer la gestión de jugadores otra vez
  
  -Gestión de partidos

  -Revisión de estadisticas.

  -Deshacer resultado, puede deshacer el último resultado registrado y notar que esto se vera reflejado en el menú de 
  estadísticas, considere que para volver a registrar un resultado debe volver a la gestión de partidos, ya que este
  vuelve a quedar como pendiente.

## Contribuciones:

  # Luciano Castañeda:
  -Función leerOpcion
  
  -Función menuPrincipal

  -Función menuEstadisticas
  
  -Función limpiarBuffer
  
  -Función leerID

  -Función finalizarTorneo

  -Función actualizarRanking

  -Función mostrarRanking
  
  -Función top3Jugadores
  
  -Función estadisticasJugador
  
  -Función deshacerUltimaAccion

  -Función liberarSistema

  -Depuración y revisión del programa

  -Redacción del README
  
  # Javier Osorio:
  -Función menuJugadores

  -Función registrarJugador
  
  -Función buscarJugador
  
  -Función mostrarJugadores
  
  -Función eliminarJugador

  # Leonardo Jorquera:
  -Función menuTorneos

  -Función crearTorneo
  
  -Función inscribirJugador
  
  -Función mostrarParticipantes
  
  -Función generarEnfrentamientos

  # Maria Marquez:
  -Función menuPartidos

  -Función verProximoPartido
  
  -Función registrarResultado
  
  -Función mostrarPartidosPendientes
  
  -Función mostrarHistorial
