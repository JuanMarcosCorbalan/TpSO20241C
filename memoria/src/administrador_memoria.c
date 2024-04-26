/*
 * administrador_memoria.c
 *
 *  Created on: Apr 5, 2024
 *      Author: utnso
 */


#include "../Headers/administrador_memoria.h"


void iniciar_memoria_de_usuario(){

    memoria_de_usuario = malloc(app_config->tam_memoria);
    memoria_procesos = list_create();
    int cantidad_de_frames = app_config->tam_memoria/app_config->tam_pagina;
    char *espacio_bitarray = malloc(cantidad_de_frames/CHAR_BIT);
    tabla_de_frames = bitarray_create_with_mode(espacio_bitarray,app_config->tam_memoria,LSB_FIRST);

}

void cargar_memoria_de_proceso(uint32_t pid){

    t_memoria_proceso *proceso = malloc(sizeof(t_memoria_proceso));

    proceso->pid=pid;
    proceso->tamanio_asignado = 0;
    proceso->cantidad_de_paginas = 0;
    proceso->tabla_de_paginas = NULL;

    list_add(memoria_procesos,proceso);
    //Loggear
}

void asignar_frames(t_memoria_proceso* proceso,int cantidad){

    //Buscar frames vacíos
    int cantidad_restante = cantidad;
    int* frames_vacios = malloc(cantidad*sizeof(int));

    for(int i=0;i<tabla_de_frames->size;i++){
        if(!bitarray_test_bit(tabla_de_frames,i)){
            frames_vacios[cantidad-(cantidad_restante--)] = i;
            if(cantidad_restante==0)
                break;
        }
    }

    if(proceso->cantidad_de_paginas==0)             //Si el proceso no tenía memoria:
        proceso->tabla_de_paginas = frames_vacios;
    else{
        //Sino, le agrego a su lista de páginas las que necesita
        int nueva_cantidad = proceso->cantidad_de_paginas + cantidad;
        int *nueva_tabla_de_paginas = realloc(proceso->tabla_de_paginas,nueva_cantidad*sizeof(int));
        memcpy(nueva_tabla_de_paginas+(proceso->cantidad_de_paginas),
            frames_vacios,
            cantidad*sizeof(int));
        free(frames_vacios);
        proceso->tabla_de_paginas = nueva_tabla_de_paginas;
    }

    for(int i=0;i<cantidad; i++){
        bitarray_set_bit(tabla_de_frames, frames_vacios[i]); 
    }

    //Actualizo los datos del proceso
    proceso->cantidad_de_paginas+=cantidad;
}

void liberar_frames(t_memoria_proceso *proceso, int cantidad_frames){
    for(int i=0;i<cantidad_frames;i++){
        int frame =proceso->tabla_de_paginas[proceso->cantidad_de_paginas-i];
        bitarray_clean_bit(tabla_de_frames,frame);
    }
    proceso->cantidad_de_paginas -= cantidad_frames;
}

void cambiar_tamanio(t_memoria_proceso *proceso, int nuevo_tamanio){
    
    //Calculo la cantidad de páginas necesarias
    int paginas_necesarias = nuevo_tamanio/app_config->tam_pagina;

    //Si no me alcanzan las páginas para el tamanio total le agrego uno
    if((nuevo_tamanio%app_config->tam_pagina)>0)
        paginas_necesarias++;
    
    if(paginas_necesarias>proceso->cantidad_de_paginas){
        //Si necesito más páginas
        asignar_frames(proceso,paginas_necesarias-(proceso->cantidad_de_paginas)); //Le asigno nuevos frames
    }else if (paginas_necesarias<proceso->cantidad_de_paginas){
        //Si en cambio me sobran páginas
        liberar_frames(proceso,proceso->cantidad_de_paginas-paginas_necesarias);
    }

    //Actualizo los datos del proceso
    proceso->tamanio_asignado = nuevo_tamanio;

}

void liberar_memoria(t_memoria_proceso *proceso, int bytes){

    for(int i=0;i<proceso->cantidad_de_paginas;i++){
        bitarray_clean_bit(tabla_de_frames,proceso->tabla_de_paginas[i]);
    }
    proceso->tamanio_asignado=0;
    proceso->cantidad_de_paginas = 0;
    free(proceso->tabla_de_paginas);
    proceso->tabla_de_paginas=NULL;
}

t_memoria_proceso *buscar_proceso(int PID){
    
    bool proceso_id(void *value){
        t_memoria_proceso *proceso = (t_memoria_proceso*)value;

        if(proceso->pid==PID)
            return true;
        else
            return false;

    };

    return list_find(memoria_procesos,proceso_id);
    
}

int escribir_en_memoria(t_memoria_proceso *proceso,int posicion, void *contenido, int tamanio){

    int tamanio_total = posicion + tamanio;
    
    //Primero me aseguro que el proceso tenga suficiente espacio asignado
    if(tamanio_total>proceso->tamanio_asignado)
        return -1;

    int primera_pagina = posicion/app_config->tam_pagina;
    int ultima_pagina = tamanio_total/app_config->tam_pagina;
    int offset = posicion%app_config->tam_pagina;

    //Por cada ṕagina me fijo:
    for(int i=primera_pagina;i<=ultima_pagina;i++){
        int principio = i==primera_pagina?offset:0;         //Si es la primara pág le agrego el offset
        int tam = i==ultima_pagina?(tamanio_total%app_config->tam_pagina):app_config->tam_pagina;   //Si es la última pag lo corto antes

        memcpy(
            memoria_de_usuario + proceso->tabla_de_paginas[i]*app_config->tam_pagina + principio,
            contenido,
            tam
        );
    }
}

void *leer_en_memoria(t_memoria_proceso *proceso,int posicion,int tamanio){

    //TODO Faltan verificaciones de que el proceso tiene permiso para leer la memoria, o memoria suficiente
    void *contenido = malloc(tamanio);

    int tamanio_restante = tamanio;
    int cantidad_leida = 0;
    int pagina = posicion/app_config->tam_pagina;
    int offset = posicion%app_config->tam_pagina;

    while(tamanio_restante>0){
        
        int fin_de_pagina = tamanio_restante>app_config->tam_pagina?app_config->tam_pagina:tamanio_restante;
        int frame = proceso->tabla_de_paginas[pagina];

        memcpy(
            contenido+cantidad_leida,
            memoria_de_usuario + frame*app_config->tam_pagina + offset,
            fin_de_pagina
            );
        
        cantidad_leida += fin_de_pagina;
        tamanio_restante -=fin_de_pagina;
        pagina++;
    }

    return contenido;

}