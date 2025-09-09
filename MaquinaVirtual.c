#include <string.h>
#include <stdio.h>
#include <stdint.h>

#define ST4 5 // poner en TDA?
#define ST3 4
#define BYTES 32
#define CAPACIDADMEM 16384
#define CANTDESSEG 8
typedef struct {
    char nombre[ST3];
    uint32_t valor; 
} TReg;

typedef char MNO[ST4];

void cargarMnemonicos(MNO mnemonicos[32]);
void cargarCodeSeg(int argc, char *argv[], uint8_t *N, uint8_t memoria[CAPACIDADMEM]);
void disassembler(TReg registros[BYTES], uint8_t memoria[CAPACIDADMEM], MNO mnemonicos[BYTES], int N);
void cargarRegistros(TReg registros[BYTES]);
void lecturaOperandos(TReg registros[BYTES], uint8_t memoria[CAPACIDADMEM], int *condicion);
void cargarDescSeg(uint32_t descSeg[CANTDESSEG], int N);
void inicializarReg(TReg registros[BYTES]);


void main(int argc, char *argv[]){
    TReg registros[BYTES];  // poner en TDA?
    uint8_t memoria[CAPACIDADMEM]; // poner en TDA?
    uint8_t N;
    MNO mnemonicos[BYTES];
    uint32_t descSeg[CANTDESSEG];

    if (argc >= 2 && argc <=3){
        cargarCodeSeg(argc, argv, &N, memoria);
        cargarRegistros(registros);
        cargarDescSeg(descSeg, N);
        cargarNmonicos(mnemonicos);

        inicializarReg(registros);

        if(argc == 3)
            if (strcmp("-d", argv[3])){
                //muestra la traduccion
                
                
                //disassembler(memoria, registros, N); 
                scanf("");
            } else
                printf("Argumento invalido. %s no existe.\n", argv[3]);
        
        //ejecutarInstrucciones(memoria,N);
        /*
        EjecutarPrograma
            CargarOPC
            EjecutarInstruccion
        */

    } else
        printf("Cantidad invalida de argumentos.\n");
}

void cargarCodeSeg(int argc, char *argv[], uint8_t *N, uint8_t memoria[CAPACIDADMEM]){
    FILE * archB;
    char nomArch[20], header[6];
    uint8_t datoArch, version;
    int i;
    
    strcpy(nomArch, argv[1]);

    if ((archB = fopen(nomArch, "rb"))==NULL)
        printf("No fue posible abrir el archivo. Es posible que se haya ingresado mal el nombre.\n");
    else {
        fread(&header, sizeof(char [5]), 1, archB);
        header[6] = '\0';
        
        fread(&version, sizeof(uint8_t), 1, archB);
        
        fread(N, sizeof(uint8_t), 1, archB);
        *N = *N << 8;
        fread(N, sizeof(uint8_t), 1, archB);
        
        if (strcmp(header, "VMX25") == 0 && version == 1){
            i=0;
            while (fread(&datoArch, sizeof(uint8_t), 1, archB) == 1){
                memoria[i] = datoArch;
                i++;
            }
        } else {
            printf("No esta validado el archivo.\n");
        }
    
        fclose(archB);
    }
}

void disassembler(TReg registros[BYTES], uint8_t memoria[CAPACIDADMEM], MNO mnemonicos[BYTES], int N){
    int condicion = 1, i = 0, cant;
    char *instruccion;
    while (i<N && condicion){
        printf("[%04x]: ", i);
        lecturaOperandos(registros, memoria, &condicion);
        
        /*
        cant = (registros[5].valor >> 24) + (registros[6].valor >> 24) + 1;
        for (j = 0; j < cant; j++)
            printf("%02x ", memoria[i + j]);
        
        printf("| %s ", mnemonicos[registros[4].valor]);
        
        if (registros[5] != 0) //puede ser inmediato, memoria, registro o ninguno
            if(registros[6] != 0)
                printf("%s, %s", ); 
            else
                printf("%s ", );
        
        printf("\n");
        i += cant;
        */
    }
}

void cargarRegistros(TReg registros[BYTES]){
    FILE *arch;
    int i = 0;
    char nom[ST3];

    if ((arch = fopen("assets/archivos/nombresReg.TXT", "r")) == NULL)
        printf("No se pudo abrir el archivo");
    else{
        while (fscanf(arch, "%s", nom) == 1){
            strcpy(registros[i].nombre, nom);
            registros[i].valor = 0;
            i++;
        }
        fclose(arch);
    }
}

void lecturaOperandos(TReg registros[BYTES], uint8_t memoria[CAPACIDADMEM], int *condicion){
    uint32_t aux;
    int i;

    registros[4].valor = memoria[registros[3].valor];
    registros[4].valor = registros[4].valor & 0x1f;

    if(registros[4].valor != 0x0f){ // al menos tiene un operando
        registros[6].valor = registros[5].valor = memoria[registros[3].valor];
        registros[5].valor = (registros[5].valor >> 4) & 0x03; 
        registros[6].valor = (registros[6].valor  >> 6) & 0x03; //asumo que tiene solo un operando
        
        if(registros[5].valor == 0x00){
           registros[5].valor = registros[6].valor;
           registros[6].valor = 0x00;
        }
        
        aux=registros[5].valor;
        
        for(i=1;i<=aux;i++){
            registros[5].valor = registros[5].valor << 8;
            registros[5].valor = registros[5].valor | memoria[registros[3].valor + i];
        }
        
        if(registros[6].valor != 0x00){ //tiene dos?
            // tiene dos operando
            i = aux;
            aux=registros[6].valor + i;
        
            for(;i<=aux;i++){
                registros[6].valor = registros[6].valor << 8;
                registros[6].valor = registros[6].valor | memoria[registros[3].valor + i];
            }
        }

        if (registros[4].valor >= 0x01 && registros[4].valor <= 0x07){ //avance normal o jump
            if (registros[4].valor & 0x00FFFFFF < registros[27].valor){
                *condicion = 0;
                printf("Invade segmento de codigo.\n");
            } else if (registros[5].valor & 0x00FFFFFF >= CAPACIDADMEM){
                *condicion = 0;
                printf("Posicion de memoria invalida.\n");
            } else
                registros[3].valor = registros[5].valor & 0x00FFFFFF;
        } else 
            registros[3].valor += ((registros[5].valor >> 6) & 0x03)+ ((registros[6].valor >> 6) & 0x03) + 1; // si no es JMP o primos
    }else{
        registros[3].valor = 0xffffffff;
        *condicion = 0;
    }
}

void cargarNmonicos(MNO mnemonicos[32]){
    FILE * arch;
    int i = 0;

    if ((arch = fopen("assets/archivos/instrucciones.TXT","rt"))==NULL)
        printf("No es posible abrir el archivo de instrucciones. Es posible que se haya ingresado mal la direccion.\n");
    else{
        while(fscanf(arch," %s", mnemonicos[i]) == 1)
            i++;
        fclose(arch);
    }
}


void cargarDescSeg(uint32_t descSeg[CANTDESSEG], int N){
    int i;
    for (i = 0; i < CANTDESSEG; i++){
        if (i == 0){
            descSeg[i] = 0;
            descSeg[i] = descSeg[i] << 16;
            descSeg[i] += N-1;
        } else if (i == 1) {
            descSeg[i] = N;
            descSeg[i] = descSeg[i] << 16;
            descSeg[i] += CAPACIDADMEM - N;
        } else
            descSeg[i] = 0;
    }
}

void inicializarReg(TReg registros[BYTES]){
    registros[26].valor = 0x0000;
    registros[27].valor = 0x0100;
    registros[3].valor = 0x0000;
}

void div(); //validar division por 0

void sum();