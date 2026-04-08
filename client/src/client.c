#include "client.h"
#include <commons/log.h>
#include <commons/config.h>

int main(void)
{
	/*---------------------------------------------------PARTE 2-------------------------------------------------------------*/

	int conexion;
	char* ip;
	char* puerto;
	char* valor;

	t_log* logger;
	t_config* config;



	/* ---------------- LOGGING ---------------- */

	 
	
    logger = iniciar_logger();
	
     log_info(logger, "Hola soy un logger");
	
	/* ---------------- ARCHIVOS DE CONFIGURACION ---------------- */


	
	config = iniciar_config();

	if(config_has_property(config, "PUERTO") == true && config_has_property(config, "IP") == true && config_has_property(config, "CLAVE") == true ){
		ip = config_get_string_value(config,"IP");
		puerto = config_get_string_value(config,"PUERTO");
		valor = config_get_string_value(config,"CLAVE");

	} else{
		abort();
	}

    log_info(logger,"%s",ip);
	log_info(logger,"%s", puerto);
	log_info(logger,"%s",valor);
	
	

	/* ---------------- LEER DE CONSOLA ---------------- */

	leer_consola(logger);



	/*---------------------------------------------------PARTE 3-------------------------------------------------------------*/

	// ADVERTENCIA: Antes de continuar, tenemos que asegurarnos que el servidor esté corriendo para poder conectarnos a él

	// Creamos una conexión hacia el servidor
	conexion = crear_conexion(ip, puerto);

	// Enviamos al servidor el valor de CLAVE como mensaje
	enviar_mensaje(valor,conexion);

	// Armamos y enviamos el paquete
	paquete(conexion);

	//terminar_programa(conexion, logger, config);
	terminar_programa(conexion,logger,config);

	/*---------------------------------------------------PARTE 5-------------------------------------------------------------*/
	// Proximamente
}

	t_log* iniciar_logger(void) {
    t_log* nuevo_logger = log_create("tp0.log", "proceso", true, LOG_LEVEL_INFO);

    if (nuevo_logger == NULL) {
        printf("Error al crear el logger\n");
        abort();
    }

    return nuevo_logger;
}

	t_config* iniciar_config(void){
    t_config* nuevo_config = config_create ("cliente.config");
    if (nuevo_config == NULL){
		printf("NO se pudo crear el config\n");
		abort();
	   }
	return nuevo_config;
	}

void leer_consola(t_log* logger)
{
	char* leido;
	leido = readline("> ");

	while (leido != NULL && strcmp(leido,"") != 0){
		log_info(logger,"%s" , leido);
		free(leido);
		leido = readline("> ");

	}

}

void paquete(int conexion)
{
	// Ahora toca lo divertido!
	char* leido;
	t_paquete* paquete;

	paquete = crear_paquete();
	leido = readline("> ");

	while (leido != NULL && strcmp(leido,"") != 0){
		agregar_a_paquete(paquete,leido, strlen(leido) +1 );
		free(leido);
		leido = readline("> ");

	}
	
	enviar_paquete(paquete,conexion);
	eliminar_paquete(paquete);
    liberar_conexion(conexion);
	// Leemos y esta vez agregamos las lineas al paquete


	// ¡No te olvides de liberar las líneas y el paquete antes de regresar!
	
}

void terminar_programa(int conexion, t_log* logger, t_config* config)
{
	liberar_conexion(conexion);
	log_destroy(logger);
	config_destroy(config);
	/* Y por ultimo, hay que liberar lo que utilizamos (conexion, log y config) 
	  con las funciones de las commons y del TP mencionadas en el enunciado */
}
