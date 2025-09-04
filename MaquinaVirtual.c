#include <string.h>
#include <stdio.h>

#define ST4 5 // poner en TDA?
#define BYTES 32
#define CAPACIDADMEM 2048

typedef struct {
    char registro[ST4];
}TST4;  // poner en TDA?

void leerArch(int argc, char *argv[]);
void disassembler();


void main(int argc, char *argv[]){
    TST4 registros[BYTES];  // poner en TDA?
    char memoria[CAPACIDADMEM]; // poner en TDA?
    
    if (argc >= 2 && argc <=3){
        leerArch(argc, argv[1]);

        if(argc == 3)
            if (strcmp("-d", argv[3])){
                // muestra la traduccion
                //disassembler(memoria); 
                scanf("");
            } else
                printf("Argumento invalido. %s no existe.\n", argv[3]);
    } else
        printf("Cantidad invalida de argumentos.\n");
}

void leerArch(int argc, char *argv[]){
    FILE * archB;
    char nomArch[20], header, datoArch;
    
    strcpy(nomArch, argv[1]);

    if ((archB = fopen(nomArch, "rb"))==NULL)
        printf("No fue posible abrir el archivo. Es posible que se haya ingresado mal el nombre.\n");
    else {
        fread(&header, sizeof(char [6]), 5, archB);
        if (strcmp(header, "VMX25") == 0){
            while (fread(&datoArch, sizeof(char), 1, archB) == 1){
                
                
            }
        }
    
        fclose(archB);
    }
}

void disassembler(){
    
    
}

void validarArgs(int argc, char *argv[]){

}