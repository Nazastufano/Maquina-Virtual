#include <string.h>
#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>
#include <time.h>
#include <ctype.h>
#include <conio.h>

#define ST4 5 
#define ST3 4
#define BYTES 32
#define CAPACIDADMEM 16384
#define CANTDESSEG 8

#define LAR 0
#define MAR 1
#define MBR 2
#define IP 3
#define OPC 4
#define OP1 5
#define OP2 6
#define SP 7
#define BP 8
#define EAX 10
#define ECX 12
#define EDX 13
#define EEX 14
#define AC 16
#define CC 17
#define CS 26
#define DS 27
#define ES 28
#define SS 29
#define KS 30
#define PS 31

typedef struct {
    char nombre[ST3];
    int32_t valor; 
} TReg;

typedef char MNO[ST4];

typedef void (*Instruccion)(TReg registros[BYTES], uint8_t memoria[], uint32_t descSeg[CANTDESSEG]);
typedef void (*SaltoCond)(TReg registros[BYTES], int32_t destino);
typedef void (*Operacion)(int32_t *a, int32_t *b, TReg registros[BYTES]);

void suma(int32_t *a, int32_t *b, TReg registros[BYTES]);
void resta(int32_t *a, int32_t *b, TReg registros[BYTES]);
void multi(int32_t *a, int32_t *b, TReg registros[BYTES]);
void divid(int32_t *a, int32_t *b, TReg registros[BYTES]);
void shiftLeft(int32_t *a, int32_t *b, TReg registros[BYTES]);
void shiftRightL(int32_t *a, int32_t *b, TReg registros[BYTES]);
void shiftRightSar(int32_t *a, int32_t *b, TReg registros[BYTES]);
void y(int32_t *a, int32_t *b, TReg registros[BYTES]);
void o(int32_t *a, int32_t *b, TReg registros[BYTES]);
void xO(int32_t *a, int32_t *b, TReg registros[BYTES]);
void move(int32_t *a, int32_t *b, TReg registros[BYTES]);
void swapOp(int32_t *a, int32_t *b, TReg registros[BYTES]);
void loadLow(int32_t *a, int32_t *b, TReg registros[BYTES]);
void loadHigh(int32_t *a, int32_t *b, TReg registros[BYTES]);
void rndo(int32_t *a, int32_t *b, TReg registros[BYTES]);
void comparar(int32_t *a, int32_t *b, TReg registros[BYTES]);

void jmpOp(TReg registros[BYTES], int32_t destino);
void jzOp(TReg registros[BYTES], int32_t destino);
void jnzOp(TReg registros[BYTES], int32_t destino);
void jpOp(TReg registros[BYTES], int32_t destino);
void jnpOp(TReg registros[BYTES], int32_t destino);
void jnOp(TReg registros[BYTES], int32_t destino);
void jnnOp(TReg registros[BYTES], int32_t destino);

void operacion(TReg registros[BYTES], uint8_t memoria[], uint32_t descSeg[CANTDESSEG], Operacion op);

void sys(TReg registros[BYTES], uint8_t memoria[], uint32_t descSeg[CANTDESSEG]);
void muestra(TReg registros[BYTES], uint8_t memoria[], uint32_t descSeg[CANTDESSEG], char *argv[], uint8_t *debugger, uint16_t M);
void mov(TReg registros[BYTES], uint8_t memoria[], uint32_t descSeg[CANTDESSEG]);
void add(TReg registros[BYTES], uint8_t memoria[], uint32_t descSeg[CANTDESSEG]);
void sub(TReg registros[BYTES], uint8_t memoria[], uint32_t descSeg[CANTDESSEG]);
void mul(TReg registros[BYTES], uint8_t memoria[], uint32_t descSeg[CANTDESSEG]);
void divi(TReg registros[BYTES], uint8_t memoria[], uint32_t descSeg[CANTDESSEG]);
void shl(TReg registros[BYTES], uint8_t memoria[], uint32_t descSeg[CANTDESSEG]);
void shr(TReg registros[BYTES], uint8_t memoria[], uint32_t descSeg[CANTDESSEG]);
void sar(TReg registros[BYTES], uint8_t memoria[], uint32_t descSeg[CANTDESSEG]);
void and(TReg registros[BYTES], uint8_t memoria[], uint32_t descSeg[CANTDESSEG]);
void or(TReg registros[BYTES], uint8_t memoria[], uint32_t descSeg[CANTDESSEG]);
void xor(TReg registros[BYTES], uint8_t memoria[], uint32_t descSeg[CANTDESSEG]);
void swap(TReg registros[BYTES], uint8_t memoria[], uint32_t descSeg[CANTDESSEG]);
void ldl(TReg registros[BYTES], uint8_t memoria[], uint32_t descSeg[CANTDESSEG]);
void ldh(TReg registros[BYTES], uint8_t memoria[], uint32_t descSeg[CANTDESSEG]);
void rnd(TReg registros[BYTES], uint8_t memoria[], uint32_t descSeg[CANTDESSEG]);
void cmp(TReg registros[BYTES], uint8_t memoria[], uint32_t descSeg[CANTDESSEG]);
void jmp(TReg registros[BYTES], uint8_t memoria[], uint32_t descSeg[CANTDESSEG]);
void jz(TReg registros[BYTES], uint8_t memoria[], uint32_t descSeg[CANTDESSEG]);
void jnz(TReg registros[BYTES], uint8_t memoria[], uint32_t descSeg[CANTDESSEG]);
void jp(TReg registros[BYTES], uint8_t memoria[], uint32_t descSeg[CANTDESSEG]);
void jnp(TReg registros[BYTES], uint8_t memoria[], uint32_t descSeg[CANTDESSEG]);
void jne(TReg registros[BYTES], uint8_t memoria[], uint32_t descSeg[CANTDESSEG]);
void jnn(TReg registros[BYTES], uint8_t memoria[], uint32_t descSeg[CANTDESSEG]);
void not(TReg registros[BYTES], uint8_t memoria[], uint32_t descSeg[CANTDESSEG]);
void stop(TReg registros[BYTES], uint8_t memoria[], uint32_t descSeg[CANTDESSEG]);
void push(TReg registros[BYTES], uint8_t memoria[], uint32_t descSeg[CANTDESSEG]);
void pop(TReg registros[BYTES], uint8_t memoria[], uint32_t descSeg[CANTDESSEG]);
void call(TReg registros[BYTES], uint8_t memoria[], uint32_t descSeg[CANTDESSEG]);
void ret(TReg registros[BYTES], uint8_t memoria[], uint32_t descSeg[CANTDESSEG]);


void sysUno(TReg registros[BYTES], uint8_t memoria[], uint32_t descSeg[CANTDESSEG]);
void sysDos(TReg registros[BYTES], uint8_t memoria[], uint32_t descSeg[CANTDESSEG]);
void sysTres(TReg registros[BYTES], uint8_t memoria[], uint32_t descSeg[CANTDESSEG]);
void sysCuatro(TReg registros[BYTES], uint8_t memoria[], uint32_t descSeg[CANTDESSEG]);
void sysSiete(TReg registros[BYTES], uint8_t memoria[], uint32_t descSeg[CANTDESSEG]);
void sysF(TReg registros[BYTES], uint8_t memoria[], uint32_t descSeg[CANTDESSEG], char *argv[], uint16_t M);

void cargarMnemonicos(MNO mnemonicos[32]);
void cargarCodeSeg(TReg registros[BYTES], uint8_t memoria[], uint32_t descSeg[CANTDESSEG], int argc, char *argv[], uint16_t *N, uint8_t largoMem);
void cargarRegistros(TReg registros[BYTES]);
void cargarDescSeg(uint32_t descSeg[CANTDESSEG], int N);
void disassembler(TReg registros[BYTES], uint8_t memoria[], MNO mnemonicos[BYTES], uint32_t descSeg[CANTDESSEG]);
void lecturaOperandos(TReg registros[BYTES], uint8_t memoria[]);
void inicializarReg(TReg registros[BYTES], uint32_t descSeg[CANTDESSEG]);
void mostrarOperandos(TReg registros[BYTES], uint8_t i);
void actualizarCC(TReg registros[BYTES], int32_t resultado);
int32_t cargarBinario(char *bin, TReg registros[BYTES]);
void mostrarBinario(int32_t valor);
char imprimirOPunto(char c);
void imprimirPalabra(int32_t pal, char *cad);
void controlDebugger(TReg registros[BYTES], uint8_t memoria[], uint32_t descSeg[CANTDESSEG], char *argv[], uint8_t *debugger, uint8_t tam);
void inicializarPila(TReg registros[BYTES], uint8_t memoria[], uint32_t descSeg[CANTDESSEG], char *argv[], int argc, int cantPal);
void leer(TReg registros[BYTES], uint8_t memoria[], uint32_t descSeg[CANTDESSEG], uint8_t opx);
void grabar(TReg registros[BYTES], uint8_t memoria[], uint32_t descSeg[CANTDESSEG], int32_t valor, uint8_t opx);
uint32_t obtenerByteReg(TReg registros[BYTES], uint8_t byteDeOP);
void cargarByteReg(TReg registros[BYTES], uint8_t byteDeOP, uint32_t valor);
uint32_t swapEndian32(uint32_t val);

void main(int argc, char *argv[]){
    TReg registros[BYTES];
    uint8_t i = 0, debugger = 0;
    uint16_t N, M;
    MNO mnemonicos[BYTES];
    uint32_t descSeg[CANTDESSEG];
    Instruccion tablaInstrucciones[BYTES] = {
        sys, jmp, jz, jp, jne, jnz, jnp, jnn, not, NULL,
        NULL, push, pop, call, ret, stop, mov, add, sub, mul, divi, 
        cmp, shl, shr, sar, and, or, xor, swap, ldl, ldh, rnd
    };

    //Verifico tamaño de la memoria
    if(argc > 2 && strncmp(argv[2], "M=", 2) == 0 || argc > 3 && strncmp(argv[3], "M=",2) == 0)
        M = atoi(argv[3] + 2)*1024;
    else
        M = CAPACIDADMEM;
    uint8_t memoria[M];
    
    if (M < CAPACIDADMEM*4-1 && M>=0){ //La memoria no puede ser mayor 65535 ni menor o igual a 0
        if (argc >= 2 && argc <=6){
            srand(time(NULL));
            cargarRegistros(registros);
            cargarCodeSeg(registros,memoria,descSeg,argc,argv,&N, M);
            cargarMnemonicos(mnemonicos);

            if (registros[IP].valor != 0xffffffff){
                if(strcmp("-d", argv[argc-1]) == 0 || strcmp("-d", argv[argc-2]) == 0) {
                    disassembler(registros, memoria, mnemonicos, descSeg); //muestra la traduccion
                    inicializarReg(registros, descSeg);
                }

                while (registros[IP].valor != 0xffffffff && registros[IP].valor <= N){
                    lecturaOperandos(registros, memoria);

                    if (tablaInstrucciones[registros[OPC].valor] != NULL)
                        if(tablaInstrucciones[registros[OPC].valor] != sys)
                            tablaInstrucciones[registros[OPC].valor](registros, memoria, descSeg);
                        else
                            muestra(registros, memoria, descSeg, argv, &debugger, M);
                    else { 
                        registros[IP].valor = 0xffffffff;
                        printf("Instrucción no implementada: 0x%02X\n", registros[OPC].valor);
                    }
                    controlDebugger(registros, memoria, descSeg, argv, &debugger,M);
                }
            } 
        } else
            printf("Cantidad invalida de argumentos.\n");
    } else
        printf("El valor de la memoria no puede ser mayor a %d\n", (CAPACIDADMEM*4-1));
    
}

void controlDebugger(TReg registros[BYTES], uint8_t memoria[], uint32_t descSeg[CANTDESSEG], char *argv[], uint8_t *debugger, uint8_t tam){
    char c;
    if (*debugger){
        do{
            c = _getch();
            c = toupper(c);
        } while (c != 'G' || c != 'Q' || c != '\r');

        if(c == 'G')
            *debugger = 0;
        else if(c == 'Q'){
            *debugger = 0;
            registros[IP].valor == 0xffffffff;
        } else if(c == '\r')
            sysF(registros, memoria, descSeg, argv, tam);
    }
}

void cargarCodeSeg(TReg registros[BYTES], uint8_t memoria[], uint32_t descSeg[CANTDESSEG], int argc, char *argv[], uint16_t *N, uint8_t largoMem){
    FILE * archB;
    char nomArch[20], header[6];
    uint8_t datoArch, version;
    uint16_t tam, k;
    uint32_t valor;
    int i, j, largoPal = 0, cantPal = 0, posSeg = 0;
    
    strcpy(nomArch, argv[1]);
    
    if ((archB = fopen(nomArch, "rb"))==NULL){
        registros[IP].valor = 0xffffffff;    
        printf("No fue posible abrir el archivo. Es posible que se haya ingresado mal el nombre.\n");
    } else {
        fread(&header, sizeof(char), 5, archB);
        header[5] = '\0';
        fread(&version, sizeof(uint8_t), 1, archB);

        if (strcmp(nomArch + strlen(nomArch) - 4, ".vmx") == 0){
            fread(N, sizeof(uint8_t), 1, archB);
            *N = *N << 8;
            fread(N, sizeof(uint8_t), 1, archB);
            i=0;
            if (strcmp(header, "VMX25") == 0){
                if (version == 1){
                    registros[CS].valor = 0x00000000;
                    registros[DS].valor = 0x00010000;
                    cargarDescSeg(descSeg, *N);
                    inicializarReg(registros, descSeg);
                } else if(version == 2) {
                    if (strncmp(argv[argc-1], "-p", 2) == 0){ //tiene parametros
                        tam = strlen(argv[argc-1])-3;
                        j = 3;
                        //carga de palabras
                        while (j < tam+3){
                            if (argv[argc-1][j] != ' ' && j != tam+3)
                                memoria[j-3] = argv[argc-1][j];
                            else
                                memoria[j-3] = '\0';
                            
                            j++;
                        }
                        //carga de punteros
                        for (i = 0; i < tam; i++) {
                            if (memoria[i] == '\0'){
                                valor = i-largoPal; //TODO verificar si anda bien
                                valor = swapEndian32(valor);
                                memcpy(&memoria[tam + cantPal*4], &valor, sizeof(uint32_t));
                                largoPal = 0;
                                cantPal++;
                            } else 
                                largoPal++;
                        }

                        //carga de param segment
                        i = tam + cantPal*4;
                        descSeg[posSeg] = 0;
                        descSeg[posSeg] = descSeg[posSeg] << 16 | i;
                        
                        registros[PS].valor = 0x00000000;
                        registros[CS].valor = 0x00010000;
                        posSeg++;
                    } else {
                        registros[CS].valor = 0x00000000;
                        registros[PS].valor = 0xffffffff;
                    }
                    //carga de resto de segmentos
                    for (j = 0; j < 5; j++){
                        fread(&tam, sizeof(uint8_t), 1, archB);
                        tam = tam << 8;
                        fread(&tam, sizeof(uint8_t), 1, archB);
 
                        if (tam != 0){
                            if (j == 4){ //Se deben acomodar los registros y tabla de segmento
                                for (k = posSeg; k >= registros[CS].valor >> 16; k--) //Se acomoda la tabla de segmentos
                                    descSeg[k+1] = descSeg[k];
                                
                                for (k = 0; k < 4; k++) //Si existe Constant Segment se debe sumar en uno a todas las direcciones logicas de los registros
                                    if (registros[CS + k].valor != 0xffffffff)
                                        registros[CS + k].valor = registros[CS + k].valor + 0x00010000;
                                
                                //puede ser 0 porque CS fue aumentado en uno por el simple hecho de entrar a este if
                                posSeg = (registros[CS].valor >> 16) - 1; //me paro en la anterior a CS
                                //si es 0 quiere decir que no existe PS
                            }

                            if(posSeg != 0) //Siempre que sea distinto de cero existe uno anterior
                                descSeg[posSeg] = (descSeg[posSeg-1] >> 16) + (descSeg[posSeg-1] & 0xffff);
                            else //codeSegment es el primero
                                descSeg[posSeg] = 0;
                            
                            descSeg[posSeg] = (descSeg[posSeg] << 16) | tam;
                            //CS = 26 + j
                            registros[CS + j].valor = 0x00000000 | (posSeg << 16); 
                            
                            posSeg++; //sumo uno para preparar el siguiente
                        } else
                            registros[CS + j].valor = 0xffffffff;
                    }

                    //inicializar la pila
                    inicializarPila(registros, memoria, descSeg, argv, argc, cantPal);

                    //lee offset del entry point
                    fread(&tam, sizeof(uint8_t), 1, archB);
                    tam = tam << 8;
                    fread(&tam, sizeof(uint8_t), 1, archB);

                    registros[IP].valor = (i << 16) | tam;

                    i = (descSeg[registros[CS].valor >> 16] >> 16) & 0xffff; //empieza a guardar a partir de la posicion relativa a el segmento
                }
                //carga de codeSegment
                if (version == 1 || (version == 2 && (descSeg[posSeg] >> 16) + descSeg[posSeg] & 0xffff < largoMem)){
                    while (fread(&datoArch, sizeof(uint8_t), 1, archB) == 1){
                        memoria[i] = datoArch;
                        i++;
                    }
                } else {
                    registros[IP].valor = 0xffffffff;    
                    printf("La capacidad de la memoria no es suficiente. \n");    
                }
            } else {
                registros[IP].valor = 0xffffffff;    
                printf("No esta validado el archivo.\n");
            }
        } else if (strcmp(nomArch + strlen(nomArch) - 4, ".vmi") == 0){
            if (strcmp(header, "VMI25") == 0) {
                if (version == 1){
                    fread(&tam, sizeof(uint8_t), 1, archB);
                    tam = tam << 8;
                    fread(&tam, sizeof(uint8_t), 1, archB);
                    
                    for (i = 0; i < 32*4; i++){ //cargaRegistros
                        fread(&datoArch, sizeof(uint8_t), 1,archB);
                        registros[i/32].valor = registros[i/32].valor << 8 | datoArch;
                    }
                    
                    for (i = 0; i < 8*4; i++){ //cargaTablaSegmentos
                        fread(&datoArch, sizeof(uint8_t), 1,archB);
                        descSeg[i/8] = descSeg[i/8] << 8 | datoArch;
                    }
                    
                    for (i = 0; i < tam*1024; i++){ //cargaMemoria
                        fread(&datoArch, sizeof(uint8_t), 1,archB);
                        memoria[i] = datoArch;
                    }
                }
            } else {
                registros[IP].valor = 0xffffffff;    
                printf("No esta validado el archivo.\n");
            }
        }
    
        fclose(archB);
    }
}

void inicializarPila(TReg registros[BYTES], uint8_t memoria[], uint32_t descSeg[CANTDESSEG], char *argv[], int argc, int cantPal){
    int valor, posSeg;
    if (registros[SS].valor != 0xffffffff) {
        posSeg = registros[SS].valor >> 16;
        registros[SP].valor = registros[SS].valor + (descSeg[posSeg] & 0xffff);

        if(registros[PS].valor != 0xffffffff){
            valor = strlen(argv[argc-1])-3;
            valor = swapEndian32(valor);
        } else
            valor = 0xffffffff;  

        memcpy(&memoria[(descSeg[posSeg]>>16 & 0xffff) + (descSeg[posSeg] & 0xffff)-4], &valor, sizeof(uint32_t));

        valor = swapEndian32(cantPal);
        memcpy(&memoria[(descSeg[posSeg]>>16 & 0xffff) + (descSeg[posSeg] & 0xffff)-8], &valor, sizeof(uint32_t));

        valor = 0xffffffff;
        memcpy(&memoria[(descSeg[posSeg]>>16 & 0xffff) + (descSeg[posSeg] & 0xffff)-12], &valor, sizeof(uint32_t));
        registros[SP].valor -= 12;
    }
}


uint32_t swapEndian32(uint32_t val) {
    return ((val >> 24) & 0x000000FF) |  // Byte 0 → Byte 3
           ((val >> 8)  & 0x0000FF00) |  // Byte 1 → Byte 2
           ((val << 8)  & 0x00FF0000) |  // Byte 2 → Byte 1
           ((val << 24) & 0xFF000000);   // Byte 3 → Byte 0
}


void disassembler(TReg registros[BYTES], uint8_t memoria[], MNO mnemonicos[BYTES], uint32_t descSeg[CANTDESSEG]){
    int codeSeg = registros[CS].valor >> 16;
    int i = (descSeg[codeSeg] & 0xffff0000) >> 16, j = 0, k, cant, posLetra; 
    int tam = descSeg[registros[KS].valor >> 16] & 0xffff; //tam segmento
    char cadena[tam];
    
    if (codeSeg != 0){
        k = ((descSeg[registros[KS].valor >> 16] >> 16) & 0xffff); //baseSegmento
        while (j < tam){
            printf("[%04x]: ", k+j);
            posLetra = 0;
            strcpy(cadena, "");
            while (memoria[k+j] != '\0'){
                if(posLetra + 1 < 7)
                    printf("%02x ", memoria[k + j]);
                
                cadena[posLetra] = imprimirOPunto(memoria[k + j]);
                posLetra++;
                j++;
            }
            cadena[posLetra] = '\0';
            if (posLetra > 6)
                printf(".. | \"%s\"\n", cadena);   
            else
                printf(" | \"%s\"\n", cadena);   
        }
    }
    printf(">");
    while (i < (descSeg[codeSeg] & 0xffff)){ //sacar reg
        printf("[%04x]: ", i); //mostrar i
        cant = 0;

        if (memoria[i] != 0x0f)
            registros[IP].valor = i;

        if (registros[IP].valor != 0xffffffff)
            lecturaOperandos(registros, memoria);
        
            if (registros[IP].valor != 0xffffffff){
                cant = ((registros[OP1].valor >> 24) & 0x3) + ((registros[OP2].valor >> 24) & 0x3) + 1;
                
                for (j = 0; j < cant; j++)
                    printf("%02x ", memoria[registros[IP].valor - cant + j]);
            } else {
                cant = 1;
                printf("%02x ", memoria[i]);
            }
        
        printf("| %s ", mnemonicos[registros[OPC].valor]);
            
        if (registros[OP1].valor != 0 && registros[IP].valor != 0xffffffff) {
            mostrarOperandos(registros, OP1);
            if(registros[OP2].valor != 0){
                printf(", ");
                mostrarOperandos(registros, OP2);
            }
        }
                
        printf("\n");
        i += cant;
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

void imprimirPalabra(int32_t pal, char *cad){
    int i;
    for (i = 0; i < ST4-1; i++){
        cad[i] = imprimirOPunto((pal >> 24) & 0xffff);
        pal = pal << 8;
    }
}

char imprimirOPunto(char c) {
    if (!isprint((uint8_t)c))
        c = '.';
    return c;
}

void lecturaOperandos(TReg registros[BYTES], uint8_t memoria[]){
    int32_t aux, valorAGuardar;

    int i;
    
    registros[OPC].valor = memoria[registros[IP].valor];
    registros[OPC].valor = registros[OPC].valor & 0x1f;


    if (registros[OPC].valor >= 0 && registros[OPC].valor <=8 || registros[OPC].valor >= 15 && registros[OPC].valor<= 31){
        if(registros[OPC].valor != 0x0f){ // al menos tiene un operando
            registros[OP2].valor = registros[OP1].valor = memoria[registros[IP].valor];
            registros[OP1].valor = (registros[OP1].valor >> 4) & 0x03; 
            registros[OP2].valor = (registros[OP2].valor >> 6) & 0x03; //asumo que tiene solo un operando
            
            aux = registros[OP2].valor;
            
            valorAGuardar = 0;
            for(i=1;i<=aux;i++){
                valorAGuardar = valorAGuardar << 8;
                valorAGuardar = valorAGuardar | memoria[registros[IP].valor + i];
            }
            registros[OP2].valor = registros[OP2].valor << 24 | valorAGuardar;
            
            if(registros[OP1].valor != 0x00){ //tiene dos?
                // tiene dos operando
                i = aux+1;
                aux = registros[OP1].valor + i;
                
                valorAGuardar = valorAGuardar & 0;
                for(;i<aux;i++){
                    valorAGuardar = valorAGuardar << 8;
                    valorAGuardar = valorAGuardar | memoria[registros[IP].valor + i];
                }

                registros[OP1].valor = registros[OP1].valor << 24 | valorAGuardar;
            }

            if(registros[OP1].valor == 0x00){
                registros[OP1].valor = registros[OP2].valor;
                registros[OP2].valor = 0x00;
            }

            registros[IP].valor += ((registros[OP1].valor >> 24) & 0x03) + ((registros[OP2].valor >> 24) & 0x03) + 1;
        } else {
            registros[IP].valor = 0xffffffff;
            
        }    
    }else {
        printf("Codigo de operacion invalido\n");
        registros[IP].valor = 0xffffffff;
    }
}

void cargarMnemonicos(MNO mnemonicos[32]){
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

//version 1.0
void cargarDescSeg(uint32_t descSeg[CANTDESSEG], int N){
    int i;
    for (i = 0; i < CANTDESSEG; i++){
        if (i == 0){
            descSeg[i] = 0;
            descSeg[i] = descSeg[i] << 16;
            descSeg[i] |= N;
        } else if (i == 1) {
            descSeg[i] = N;
            descSeg[i] = descSeg[i] << 16;
            descSeg[i] |= CAPACIDADMEM - N;
        } else
            descSeg[i] = 0;
    }
}

void inicializarReg(TReg registros[BYTES], uint32_t descSeg[CANTDESSEG]){
    registros[IP].valor = (descSeg[registros[CS].valor >> 16] >> 16) & 0xffff; //TODO ver bien como traer el entypoint para usar generica esta funcion para parte 1 y 2
    registros[CC].valor = 0x0000;
}

void mostrarBinario(int32_t valor){
    int i;
    printf("0b");
    for (i = 31; i >= 0; i--) 
        printf("%d", (valor >> i) & 1);    
}

int32_t cargarBinario(char *bin, TReg registros[BYTES]) {
    int32_t res = 0;
    int len = strlen(bin);
    for (int i = 0; i < len; i++) {
        if (bin[i] == '1')
            res |= 1u << (len - 1 - i); // pone el bit correspondiente
        else if (bin[i] != '0') {
            printf("Error: caracter invalido\n");
            registros[IP].valor = 0xffffffff;
        }
    }
    return res;
}


void mostrarOperandos(TReg registros[BYTES], uint8_t i){
    TReg regAux = registros[DS];
    uint8_t pos = (registros[i].valor >> 16) & 0xff;
    int16_t offset = registros[i].valor & 0xffff;
    uint8_t byteMenSig = registros[i].valor & 0xff;
    char nombre[ST3], tamCel;
    if((registros[i].valor >> 24 & 0x03) == 1){ //registro
        if(byteMenSig >> 6 == 0)
            strcpy(nombre, registros[byteMenSig & 0x1f].nombre);
        else if(byteMenSig >> 6 == 1){
            strncpy(nombre, registros[byteMenSig & 0x1f].nombre+1, 1);
            nombre[1] = '\0';
            strcat(nombre,"L");
        } else if(byteMenSig >> 6 == 2){
            strncpy(nombre, registros[byteMenSig & 0x1f].nombre+1, 1);
            nombre[1] = '\0';
            strcat(nombre,"H");
        } else {
            strncpy(nombre, registros[byteMenSig & 0x1f].nombre+1, 2);
            nombre[2] = '\0';
        }
        printf(" %s", nombre);
    } else if((registros[i].valor >> 24 & 0x03) == 2) //inmediato
        printf(" %d", offset);
        
    else if((registros[i].valor >> 24 & 0x03)== 3){
        //memoria -> [reg], [reg + offset] y [offset]

        if(pos & 0x1f  != 0)
            regAux = registros[pos & 0x1f];

        if (pos>>6 == 0)
            tamCel = 'l';
        else if(pos>>6 == 2)
            tamCel = 'w';
        else if(pos>>6 == 3)
            tamCel = 'b';
        
        printf("%c[", tamCel);
        
        if (offset == 0)
            printf("%s]", regAux.nombre);
        else
            printf("%s+%d]", regAux.nombre, offset);
    }
}

uint32_t obtenerByteReg(TReg registros[BYTES], uint8_t byteDeOP){
    uint8_t bits = byteDeOP >> 6;
    uint8_t reg = byteDeOP & 0x1f;
    uint32_t valor = registros[reg].valor;
    if (bits == 0x0)
        valor = registros[reg].valor;
    else if (bits == 0x1)
        valor = registros[reg].valor & 0xff;
    else if (bits == 0x2)
        valor = registros[reg].valor & 0xff00;
    else
        valor = registros[reg].valor & 0xffff;
    
    if (registros[reg].valor >> 31 == 1)
        valor = 0xffff0000 | valor; 
    else
        valor = 0x00000000 | valor; 

    return valor;
}

void cargarByteReg(TReg registros[BYTES], uint8_t byteDeOP, uint32_t valor){
    uint8_t bits = byteDeOP >> 6;
    uint8_t reg = byteDeOP & 0x1f;

    if (bits == 0x0)
        registros[reg].valor = valor;
    else if (bits == 0x1)
        registros[reg].valor = (registros[reg].valor & 0xffffff00) | (valor & 0xff);
    else if (bits == 0x2)
        registros[reg].valor = (registros[reg].valor & 0xffff00ff) | (valor & 0xff00);
    else
        registros[reg].valor = (registros[reg].valor & 0xffff0000) | (valor & 0xffff);
}


void operacion(TReg registros[BYTES], uint8_t memoria[], uint32_t descSeg[CANTDESSEG], Operacion op) {
    int32_t valor;
    int32_t tmp1, tmp2;
    int16_t tmp16;
    uint8_t posReg = registros[OP1].valor & 0x01f;
    if ((registros[OP1].valor & 0x0f000000) == 0x01000000) {
        
        tmp1 = obtenerByteReg(registros, (registros[OP1].valor & 0xff));
        if ((registros[OP2].valor & 0x0f000000) == 0x01000000){
            tmp2 = obtenerByteReg(registros, (registros[OP2].valor & 0xff));
            op(&tmp1, &tmp2, registros);
            cargarByteReg(registros, registros[OP2].valor & 0xff, tmp2);
        } else if ((registros[OP2].valor & 0x0f000000) == 0x02000000) {
            tmp16 = registros[OP2].valor & 0xFFFF;
            valor = (int32_t) tmp16;
            op(&tmp1, &valor, registros);            
        } else if ((registros[OP2].valor & 0x0f000000) == 0x03000000) {
            leer(registros, memoria, descSeg, 6);
            op(&tmp1, &registros[MBR].valor, registros);
        }
        cargarByteReg(registros, registros[OP1].valor & 0xff, tmp1);

        if ((registros[OPC].valor >= 0x11 && registros[OPC].valor <= 0x14) || (registros[OPC].valor >= 0x16 && registros[OPC].valor <= 0x1c))
            actualizarCC(registros, registros[posReg].valor);    
    } else if ((registros[OP1].valor & 0x0f000000) == 0x03000000) {
        if ((registros[OP2].valor & 0x0f000000) == 0x01000000) {
            leer(registros, memoria, descSeg, OP1);
            tmp2 = obtenerByteReg(registros, (registros[OP2].valor & 0xff));
            op(&registros[MBR].valor, &tmp2, registros);
            cargarByteReg(registros, registros[OP2].valor & 0xff, tmp2);
            valor = registros[MBR].valor;
            grabar(registros, memoria, descSeg, valor,OP1);
        } else if ((registros[OP2].valor & 0x0f000000) == 0x02000000) {
            leer(registros, memoria, descSeg, OP1);
            tmp16 = registros[OP2].valor & 0x0ffff;
            valor = (int32_t) tmp16;
            op(&registros[MBR].valor, &valor, registros);
            valor = registros[MBR].valor;
            grabar(registros, memoria, descSeg, valor, OP1);
        } else if ((registros[OP2].valor & 0x0f000000) == 0x03000000) {
            leer(registros, memoria, descSeg, OP1);
            valor = registros[MBR].valor;
            leer(registros, memoria, descSeg, OP2);
            op(&valor, &registros[MBR].valor, registros);

            if (op == swapOp)
                grabar(registros, memoria, descSeg, registros[MBR].valor, OP2);
            
            grabar(registros, memoria, descSeg, valor, OP1);
        }
        if ((registros[OPC].valor >= 0x11 && registros[OPC].valor <= 0x14) || (registros[OPC].valor >= 0x16 && registros[OPC].valor <= 0x1c))
            actualizarCC(registros, valor);           
    }
}

void suma(int32_t *a, int32_t *b, TReg registros[BYTES]) { 
    *a = *a + *b;
}
void resta(int32_t *a, int32_t *b, TReg registros[BYTES]) { *a = *a - *b; }
void multi(int32_t *a, int32_t *b, TReg registros[BYTES]) { *a = *a * *b; }
void divid(int32_t *a, int32_t *b, TReg registros[BYTES]) { 
    int32_t aux, aux2;
    if (*b != 0) {
        aux = *a;
        aux2 = *b;

        registros[AC].valor = aux % aux2;
        *a = aux / aux2;
    } else {
        printf("No se puede hacer una division por 0.\n");
        registros[IP].valor = 0xffffffff;
    }
}
void comparar(int32_t *a, int32_t *b, TReg registros[BYTES]) { 
    int32_t res = *a - *b;
    actualizarCC(registros, res); }
void shiftLeft(int32_t *a, int32_t *b, TReg registros[BYTES]) { *a = *a << *b; }
void shiftRightL(int32_t *a, int32_t *b, TReg registros[BYTES]) { *a = (uint32_t)(*a) >> *b; }
void shiftRightSar(int32_t *a, int32_t *b, TReg registros[BYTES]){ *a = *a >> *b; }
void y(int32_t *a, int32_t *b, TReg registros[BYTES]) { *a = *a & *b; }
void o(int32_t *a, int32_t *b, TReg registros[BYTES]) { *a = *a | *b; }
void xO(int32_t *a, int32_t *b, TReg registros[BYTES]) { *a = *a ^ *b; }
void move(int32_t *a, int32_t *b, TReg registros[BYTES]) { *a = *b; }
void swapOp(int32_t *a, int32_t *b, TReg registros[BYTES]) { 
    int32_t tmp = *a;
    *a = *b;
    *b = tmp;
}
void loadLow(int32_t *a, int32_t *b, TReg registros[BYTES]) { *a = (*a & 0xFFFF0000) | (*b & 0x0000FFFF); }
void loadHigh(int32_t *a, int32_t *b, TReg registros[BYTES]) { *a = (*a & 0x0000FFFF) | ((*b & 0x0000FFFF) << 16); }
void rndo(int32_t *a, int32_t *b, TReg registros[BYTES]) { *a = rand() % (*b + 1); }

void mov(TReg registros[BYTES], uint8_t memoria[], uint32_t descSeg[CANTDESSEG]) {
    operacion(registros, memoria, descSeg, move);
}

void add(TReg registros[BYTES], uint8_t memoria[], uint32_t descSeg[CANTDESSEG]) {
    operacion(registros, memoria, descSeg, suma);
}

void sub(TReg registros[BYTES], uint8_t memoria[], uint32_t descSeg[CANTDESSEG]){
    operacion(registros, memoria, descSeg, resta);
}

void mul(TReg registros[BYTES], uint8_t memoria[], uint32_t descSeg[CANTDESSEG]){
    operacion(registros, memoria, descSeg, multi);
}

void divi(TReg registros[BYTES], uint8_t memoria[], uint32_t descSeg[CANTDESSEG]){ //validar division por 0
    operacion(registros, memoria, descSeg, divid); 
}

void shl(TReg registros[BYTES], uint8_t memoria[], uint32_t descSeg[CANTDESSEG]){
    operacion(registros, memoria, descSeg, shiftLeft);
}

void shr(TReg registros[BYTES], uint8_t memoria[], uint32_t descSeg[CANTDESSEG]){
    operacion(registros, memoria, descSeg, shiftRightL);
}

void sar(TReg registros[BYTES], uint8_t memoria[], uint32_t descSeg[CANTDESSEG]){
    operacion(registros, memoria, descSeg, shiftRightSar);
}

void and(TReg registros[BYTES], uint8_t memoria[], uint32_t descSeg[CANTDESSEG]){
    operacion(registros, memoria, descSeg, y);
}

void or(TReg registros[BYTES], uint8_t memoria[], uint32_t descSeg[CANTDESSEG]){
    operacion(registros, memoria, descSeg, o);
}

void xor(TReg registros[BYTES], uint8_t memoria[], uint32_t descSeg[CANTDESSEG]){
    operacion(registros, memoria, descSeg, xO);
}

void swap(TReg registros[BYTES], uint8_t memoria[], uint32_t descSeg[CANTDESSEG]){
    operacion(registros, memoria, descSeg, swapOp);
}

void ldl(TReg registros[BYTES], uint8_t memoria[], uint32_t descSeg[CANTDESSEG]){
    operacion(registros, memoria, descSeg, loadLow);
}

void ldh(TReg registros[BYTES], uint8_t memoria[], uint32_t descSeg[CANTDESSEG]){
    operacion(registros, memoria, descSeg, loadHigh);
}

void rnd(TReg registros[BYTES], uint8_t memoria[], uint32_t descSeg[CANTDESSEG]){
    operacion(registros, memoria, descSeg, rndo);
}

void stop(TReg registros[BYTES], uint8_t memoria[], uint32_t descSeg[CANTDESSEG]){
    registros[IP].valor = 0xffffffff;
}

void cmp(TReg registros[BYTES], uint8_t memoria[], uint32_t descSeg[CANTDESSEG]){
    operacion(registros, memoria, descSeg, comparar);
}

void not(TReg registros[BYTES], uint8_t memoria[], uint32_t descSeg[CANTDESSEG]){
    int32_t valor;
    int32_t tipo = (registros[OP1].valor >> 24) & 0xff;

    if (tipo == 1) {          // registro
        valor = registros[registros[OP1].valor & 0xff].valor = ~registros[registros[OP1].valor & 0xff].valor;
    } else if (tipo == 2) {   // inmediato
        valor = registros[OP1].valor = registros[OP1].valor & 0xff000000 | ~registros[OP1].valor & 0xffff;
    } else if (tipo == 3) {   // memoria
        leer(registros, memoria, descSeg, OP1);
        valor = registros[MBR].valor = ~registros[MBR].valor;
        grabar(registros, memoria, descSeg, registros[MBR].valor, OP1);
    }
    actualizarCC(registros, valor);
}

void push(TReg registros[BYTES], uint8_t memoria[], uint32_t descSeg[CANTDESSEG]){
    uint8_t i;
    int32_t valor;
    int32_t tmp, dirLog, dirFis, limSeg, dirBaseSeg, tamSeg, offset;

    dirLog = registros[SP].valor;

    dirBaseSeg = descSeg[dirLog >> 16 & 0xffff] >> 16 & 0xffff;

    offset = registros[SP].valor & 0x0000ffff;

    dirFis = dirBaseSeg + offset;

    if(dirFis - 4 >= (descSeg[registros[SS].valor >> 16] >> 16)){

        registros[SP].valor -= 4;

        if ((registros[OP1].valor & 0x0f000000) == 0x01000000) 
            valor = registros[(registros[OP1].valor) & 0x1f].valor;
        else if ((registros[OP1].valor & 0x0f000000) == 0x02000000){
            valor = registros[OP1].valor & 0xffff;
        }else if ((registros[OP1].valor & 0x0f000000) == 0x03000000){
            leer(registros, memoria, descSeg, OP1);
            valor = registros[MBR].valor;
        }

        for(i=0;i<=3;i++)
            memoria[dirFis - 4 + i] = (valor >> (8*(3-i))) & 0xff;
    }else{
        printf("STACK OVERFLOW");
        registros[IP].valor = 0xffffffff;
    }
}

void pop(TReg registros[BYTES], uint8_t memoria[], uint32_t descSeg[CANTDESSEG]){
    uint8_t i;
    int32_t valor = 0;
    int32_t dirLog, dirFis, dirBaseSeg, offset;

    dirLog = registros[SP].valor;

    dirBaseSeg = descSeg[dirLog >> 16 & 0xffff] >> 16 & 0xffff;

    offset = registros[SP].valor & 0x0000ffff;

    dirFis = dirBaseSeg + offset;

    if(dirFis + 4 <= ((descSeg[registros[SS].valor >> 16] >> 16) + (descSeg[registros[SS].valor >> 16] & 0xffff))){

        registros[SP].valor += 4;

        for (i = 0; i < 4; i++)
            valor = (valor << 8) | memoria[dirFis + i];

        if ((registros[OP1].valor & 0x0f000000) == 0x01000000) {
            registros[OP1].valor = valor;            
        } else if ((registros[OP1].valor & 0x0f000000) == 0x03000000) 
            grabar(registros, memoria, descSeg, valor, OP1);
    }else{
        printf("STACK UNDERFLOW");
        registros[IP].valor = 0xffffffff;
    }
}

void call(TReg registros[BYTES], uint8_t memoria[], uint32_t descSeg[CANTDESSEG]){
    uint8_t i;
    int32_t valor;
    int32_t tmp, dirLog, dirFis, limSeg, dirBaseSeg, tamSeg, offset;

    dirLog = registros[SP].valor;

    dirBaseSeg = descSeg[dirLog >> 16 & 0xffff] >> 16 & 0xffff;

    offset = registros[SP].valor & 0x0000ffff;

    dirFis = dirBaseSeg + offset;    

    if(dirFis - 4 >= (descSeg[registros[SS].valor >> 16] >> 16)){

        registros[SP].valor -= 4;

        valor = registros[IP].valor;
        
        for(i=0;i<=3;i++)
            memoria[dirFis - 4 + i] = (valor >> (8*(3-i))) & 0xff;
        
        jmp(registros, memoria, descSeg);
    }else{
        printf("STACK OVERFLOW");
        registros[IP].valor = 0xffffffff;
    }
}

void ret(TReg registros[BYTES], uint8_t memoria[], uint32_t descSeg[CANTDESSEG]){ 
    uint8_t i;
    int32_t valor = 0;
    int32_t tmp, dirLog, dirFis, limSeg, dirBaseSeg, tamSeg, offset;

    dirLog = registros[SP].valor;

    dirBaseSeg = descSeg[dirLog >> 16 & 0xffff] >> 16 & 0xffff;

    offset = registros[SP].valor & 0x0000ffff;

    dirFis = dirBaseSeg + offset;

    if(dirFis + 4 <= ((descSeg[registros[SS].valor >> 16] >> 16) + (descSeg[registros[SS].valor >> 16] & 0xffff))){

        registros[SP].valor += 4;

        for (i = 0; i < 4; i++)
            valor = (valor << 8) | memoria[dirFis + i];
            
        jmpOp(registros, valor);
    } else {
        printf("STACK UNDERFLOW");
        registros[IP].valor = 0xffffffff;
    }
}

void sys(TReg registros[BYTES], uint8_t memoria[], uint32_t descSeg[CANTDESSEG]){
    if ((registros[OP1].valor & 0xffff) == 1)
        sysUno(registros, memoria, descSeg);
    else if ((registros[OP1].valor & 0xffff) == 2)
        sysDos(registros, memoria, descSeg);
    else if ((registros[OP1].valor & 0xffff) == 3)
        sysTres(registros, memoria, descSeg);
    else if ((registros[OP1].valor & 0xffff) == 4)
        sysCuatro(registros, memoria, descSeg);
    else if ((registros[OP1].valor & 0xffff) == 7)
        sysSiete(registros, memoria, descSeg);
}

void muestra(TReg registros[BYTES], uint8_t memoria[], uint32_t descSeg[CANTDESSEG], char *argv[], uint8_t *debugger, uint16_t M){
    if((registros[OP1].valor & 0xffff) == 0xf){
        if(strcmp(argv[2] + strlen(argv[2]) - 4, ".vmi") == 0){
            sysF(registros, memoria, descSeg, argv, M);
            *debugger = 1;
        }
    } else
        sys(registros,memoria,descSeg);
}

void sysUno(TReg registros[BYTES], uint8_t memoria[], uint32_t descSeg[CANTDESSEG]){
    int i, j;
    char binStr[33];
    int32_t valor;
    for (j = 0; j < (registros[ECX].valor & 0x0000ffff); j++){
        printf("[%04x]: ", (descSeg[registros[EDX].valor >> 16 & 0xff] >> 16 & 0xffff) + (registros[EDX].valor & 0x0000ffff) + (j*(registros[ECX].valor >> 16 & 0xffff)));
        
        if (registros[EAX].valor & 0x10){
            scanf("%32s", binStr);
            valor = cargarBinario(binStr, registros);
        } 
        if (registros[EAX].valor & 0x08)
            scanf(" %x", &valor);
        if (registros[EAX].valor & 0x04)
            scanf(" %o", &valor);
        if (registros[EAX].valor & 0x02)
            scanf(" %c", &valor);
        if (registros[EAX].valor & 0x01)
            scanf(" %d", &valor);
                            
        for (i = ((registros[ECX].valor >> 16) & 0xffff) - 1; i >= 0; i--){
            memoria[(descSeg[registros[EDX].valor >> 16 & 0xff] >> 16 & 0xffff) + (registros[EDX].valor & 0x0000ffff) + (j*(registros[ECX].valor >> 16 & 0xffff)) + i] = valor & 0xff;
            valor = valor >> 8;
        }
    }
}


void sysDos(TReg registros[BYTES], uint8_t memoria[], uint32_t descSeg[CANTDESSEG]){
    int i, j;
    char binStr[33];
    int32_t valor = 0, cant = 0;
    char cadena[ST4];
    for (j = 0; j < (registros[ECX].valor & 0x0000ffff); j++){
        for (i = cant; i < cant + (registros[ECX].valor >> 16 & 0xffff); i++)
            valor = (valor << 8) | memoria[(descSeg[registros[EDX].valor >> 16 & 0xff] >> 16 & 0xffff) + (registros[EDX].valor & 0x0000ffff) + i];

        cant += (registros[ECX].valor >> 16 & 0xffff);

        printf("[%04x]: ", (descSeg[registros[EDX].valor >> 16 & 0xff] >> 16 & 0xffff) + (registros[EDX].valor & 0x0000ffff) + j*(registros[ECX].valor >> 16 & 0xffff));
            
        
        if (registros[EAX].valor & 0x10)
            mostrarBinario(valor);
        if (registros[EAX].valor & 0x08)
            printf("0x%x ", valor);
        if (registros[EAX].valor & 0x04)
            printf("0o%o ", valor);
        if (registros[EAX].valor & 0x02){
            imprimirPalabra(valor, cadena);
            
            printf("%s ", cadena);    
        }
        if (registros[EAX].valor & 0x01)
            printf("%d ", valor);
            
        printf("\n");
    }
}

void sysTres(TReg registros[BYTES], uint8_t memoria[], uint32_t descSeg[CANTDESSEG]){
    int i = 0;
    uint16_t maxLenCad = (descSeg[registros[KS].valor >> 16] & 0xffff);
    char cadena[maxLenCad], formato[10];

    printf("[%04x]: ", (descSeg[registros[EDX].valor >> 16 & 0xff] >> 16 & 0xffff) + (registros[EDX].valor & 0x0000ffff));

    if(registros[ECX].valor & 0x0000ffff == -1){
        sprintf(formato, "%%%ds", maxLenCad-1);
        scanf(formato, cadena);

        while (i < strlen(cadena)){
            memoria[(descSeg[registros[EDX].valor >> 16 & 0xff] >> 16 & 0xffff) + (registros[EDX].valor & 0x0000ffff) + i] = cadena[i];
            i++;
        }
    } else {
        sprintf(formato, "%%%ds", registros[ECX].valor & 0x0000ffff);
        scanf(formato, cadena);

        while (i < strlen(cadena) && i<=registros[ECX].valor & 0x0000ffff){
            memoria[(descSeg[registros[EDX].valor >> 16 & 0xff] >> 16 & 0xffff) + (registros[EDX].valor & 0x0000ffff) + i] = cadena[i];
            i++;
        }
    } 
    memoria[(descSeg[registros[EDX].valor >> 16 & 0xff] >> 16 & 0xffff) + (registros[EDX].valor & 0x0000ffff) + i] = '\0';
}

void sysCuatro(TReg registros[BYTES], uint8_t memoria[], uint32_t descSeg[CANTDESSEG]){
    int i = 0;
    uint16_t maxLenCad = (descSeg[registros[KS].valor >> 16] & 0xffff);
    char cadena[maxLenCad];
    char valorMem;
    printf("[%04x]: ", (descSeg[registros[EDX].valor >> 16 & 0xff] >> 16 & 0xffff) + (registros[EDX].valor & 0x0000ffff));

    do {
        valorMem = memoria[(descSeg[registros[EDX].valor >> 16 & 0xff] >> 16 & 0xffff) + (registros[EDX].valor & 0x0000ffff) + i];
        cadena[i] = valorMem;
        i++;
    } while (valorMem != '\0' && valorMem != '\n');
    
    if(valorMem == '\n'){
        cadena[i-1] = '\0';
        printf("%s \n", cadena);
    } else
        printf("%s", cadena);
}

void sysSiete(TReg registros[BYTES], uint8_t memoria[], uint32_t descSeg[CANTDESSEG]){
    #if defined(_WIN32) || defined(_WIN64)
        system("cls");
    #else
        system("clear");
    #endif
}

void sysF(TReg registros[BYTES], uint8_t memoria[], uint32_t descSeg[CANTDESSEG], char *argv[], uint16_t tam){
    FILE *archB;
    char nomArch[20], header[6];
    uint8_t datoArch;
    int i;
    if (strcmp(argv[2] + strlen(argv[2]) - 4, ".vmi") == 0){
        strcpy(nomArch, argv[2]);
        if((archB = fopen(nomArch, "wb")) == NULL) 
            printf("No se pudo abrir el archivo.\n");
        else {
            strcpy(header,"VMI25");
            fwrite(&header, sizeof(char), 5, archB);
            datoArch = 1;
            fwrite(&datoArch, sizeof(uint8_t),1,archB);
            datoArch = (tam / 1024) >> 8;
            fwrite(&datoArch, sizeof(uint8_t),1,archB);
            datoArch = (tam / 1024) & 0xff;
            fwrite(&datoArch, sizeof(uint8_t),1,archB);

            for (i = 0; i < 32*4; i++){ //cargaRegistros
                datoArch = (registros[i/32].valor >> 8*(i % 4)) & 0xff;
                fwrite(&datoArch, sizeof(uint8_t), 1,archB);
            }
            for (i = 0; i < 8*4; i++){ //cargaTablaSegmentos
                datoArch = (descSeg[i/8] >> 8*(i % 4)) & 0xff;
                fwrite(&datoArch, sizeof(uint8_t), 1,archB);
            }
            for (i = 0; i < tam; i++){ //cargaMemoria
                datoArch = memoria[i];
                fwrite(&datoArch, sizeof(uint8_t), 1,archB);
            }    
        }
    }
}

void actualizarCC(TReg registros[BYTES], int32_t resultado) {
    registros[CC].valor = 0;              // limpio N y Z    
    if (resultado < 0) 
        registros[CC].valor |= (1u << 31);    // seteo N si es negativo

    if (resultado == 0) 
        registros[CC].valor |= (1u << 30);    // seteo Z si es cero

}

void leer(TReg registros[BYTES], uint8_t memoria[], uint32_t descSeg[CANTDESSEG], uint8_t opx) {
    uint16_t i;
    int32_t valor = 0, dirLog, dirFis, limSeg, dirBaseSeg, tamSeg, offset;
    uint16_t cant = 4 - ((registros[opx].valor >> 22) & 0x3);
    uint16_t posReg = ((registros[opx].valor >> 16) & 0x1f);
    //TODO Confirmar si es necesario asumir que si se ingresa un inmediato tranformarlo en ds
    
    if(posReg == 0)
        dirLog = registros[DS].valor;
    else
        dirLog = registros[posReg].valor;
    
    //direccion base del segmento
    dirBaseSeg = descSeg[dirLog >> 16 & 0xffff] >> 16 & 0xffff;
    
    //tamaño del segmento
    tamSeg = descSeg[dirLog >> 16 & 0xffff] & 0xffff;

    //desplazamiento
    offset = (registros[opx].valor & 0xffff) + (registros[posReg].valor & 0x0000ffff);

    //posicion fisica = direccion base + offset
    dirFis = dirBaseSeg + offset;
    
    //direccion base del segmento + tamaño del Segmento
    limSeg = dirBaseSeg + tamSeg;
        
    if (dirBaseSeg <= dirFis && limSeg >= dirFis + cant){
        registros[LAR].valor = dirLog + offset;
        registros[MAR].valor = (cant << 16 & 0xffff0000) | dirFis;
        
        for (i = 0; i < cant; i++)
            valor = (valor << 8) | memoria[dirFis + i];

        registros[MBR].valor = valor;
    }else {
        printf("La lectura en memoria intenta invadir segmento.\n");
        registros[IP].valor = 0xffffffff;
    }
}
void grabar(TReg registros[BYTES], uint8_t memoria[], uint32_t descSeg[CANTDESSEG], int32_t valor, uint8_t opx) {
    int16_t i;
    int32_t tmp, dirLog, dirFis, limSeg, dirBaseSeg, tamSeg, offset;
    uint16_t cant = 4 - ((registros[opx].valor >> 22) & 0x3);
    uint16_t posReg = ((registros[opx].valor >> 16) & 0x1f);
    //Seguir testeando con mas ejemplos
    if(posReg == 0)
        dirLog = registros[DS].valor;
    else
        dirLog = registros[posReg].valor;
    
    //direccion base del segmento
    dirBaseSeg = descSeg[dirLog >> 16 & 0xffff] >> 16 & 0xffff;
    
    //tamaño del segmento
    tamSeg = descSeg[dirLog >> 16 & 0xffff] & 0xffff;

    //desplazamiento
    offset = (registros[opx].valor & 0xffff) + (registros[posReg].valor & 0x0000ffff);

    //posicion fisica = direccion base + offset
    dirFis = dirBaseSeg + offset;
    
    //direccion base del segmento + tamaño del Segmento
    limSeg = dirBaseSeg + tamSeg;

    if (dirBaseSeg <= dirFis && limSeg >= dirFis + cant){
        registros[LAR].valor = dirLog + offset;
        registros[MAR].valor = (cant << 16 & 0xffff0000) | dirFis;
        registros[MBR].valor = valor;

        tmp = valor;
        for(i = cant - 1; i >= 0; i--) {
            memoria[dirFis + i] = tmp & 0xFF;
            tmp >>= 8;
        }
    }else {
        printf("Fallo de segmento\n");
        registros[IP].valor = 0xffffffff;
    }
}

void saltoGenerico(TReg registros[BYTES], uint8_t memoria[], uint32_t descSeg[CANTDESSEG], SaltoCond op) {
    int32_t destino = 0;
    int32_t tipo = (registros[OP1].valor >> 24) & 0xff;
    /*
    if (tipo == 1) {          // registro
        destino = registros[registros[OP1].valor & 0xff].valor;
    } else 
    */
    if (tipo == 2) {   // inmediato
        destino = registros[OP1].valor & 0xffff;
    }
    /*
    else if (tipo == 3) {   // memoria
        leer(registros, memoria, descSeg, 5);
        destino = registros[MBR].valor;
    }
    */
    if (destino < 0 || destino >= (descSeg[registros[CS].valor >> 16] & 0xffff)) {
        registros[IP].valor = 0xffffffff;
        printf("Salto fuera del segmento de codigo.\n");
    } else if (destino > CAPACIDADMEM){ //capaz pasarlo por parametro
        registros[IP].valor = 0xffffffff;
        printf("Posicion de memoria invalida.\n");
    } else 
        op(registros, destino);
}

void jmpOp(TReg registros[BYTES], int32_t destino) {
    registros[IP].valor = destino;  // salto incondicional
}

void jzOp(TReg registros[BYTES], int32_t destino) {
    if (registros[CC].valor & (1u << 30))  // Z = 1
        registros[IP].valor = destino;
}

void jnzOp(TReg registros[BYTES], int32_t destino) {
    if (!(registros[CC].valor & (1u << 30)))  // Z = 0
        registros[IP].valor = destino;
}

void jpOp(TReg registros[BYTES], int32_t destino) {
    if ((registros[CC].valor & ((1u << 31) | (1u << 30))) == 0)  // N=0 y Z=0, positivo
        registros[IP].valor = destino;
}

void jnpOp(TReg registros[BYTES], int32_t destino) {
    if (registros[CC].valor & ((1u << 31) | (1u << 30)))  // N=1 o Z=1, <=0
        registros[IP].valor = destino;
}

void jnOp(TReg registros[BYTES], int32_t destino) {
    if (registros[CC].valor & (1u << 31))  // N = 1, negativo
        registros[IP].valor = destino;
}

void jnnOp(TReg registros[BYTES], int32_t destino) {
    if (!(registros[CC].valor & (1u << 31)))  // N = 0, >=0
        registros[IP].valor = destino;
}

void jmp(TReg registros[BYTES], uint8_t memoria[], uint32_t descSeg[CANTDESSEG]) {
    saltoGenerico(registros, memoria, descSeg, jmpOp); 
}

void jz(TReg registros[BYTES], uint8_t memoria[], uint32_t descSeg[CANTDESSEG]) {
    saltoGenerico(registros, memoria, descSeg, jzOp);
}

void jnz(TReg registros[BYTES], uint8_t memoria[], uint32_t descSeg[CANTDESSEG]) {
    saltoGenerico(registros, memoria, descSeg, jnzOp);
}

void jp(TReg registros[BYTES], uint8_t memoria[], uint32_t descSeg[CANTDESSEG]) {
    saltoGenerico(registros, memoria, descSeg, jpOp);
}

void jnp(TReg registros[BYTES], uint8_t memoria[], uint32_t descSeg[CANTDESSEG]) {
    saltoGenerico(registros, memoria, descSeg, jnpOp);
}

void jne(TReg registros[BYTES], uint8_t memoria[], uint32_t descSeg[CANTDESSEG]) {
    saltoGenerico(registros, memoria, descSeg, jnOp);
}

void jnn(TReg registros[BYTES], uint8_t memoria[], uint32_t descSeg[CANTDESSEG]) {
    saltoGenerico(registros, memoria, descSeg, jnnOp);
}