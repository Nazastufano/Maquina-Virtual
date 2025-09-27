#include <string.h>
#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>
#include <time.h>
#include <ctype.h>

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
#define EAX 10
#define ECX 12
#define EDX 13
#define EEX 14
#define AC 16
#define CC 17
#define CS 26
#define DS 27

typedef struct {
    char nombre[ST3];
    int32_t valor; 
} TReg;

typedef char * CADENA;

typedef char MNO[ST4];

typedef void (*Instruccion)(TReg registros[BYTES], uint8_t memoria[CAPACIDADMEM], uint32_t descSeg[CANTDESSEG]);
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

void operacion(TReg registros[BYTES], uint8_t memoria[CAPACIDADMEM], uint32_t descSeg[CANTDESSEG], Operacion op);

void sys(TReg registros[BYTES], uint8_t memoria[CAPACIDADMEM], uint32_t descSeg[CANTDESSEG]);
void mov(TReg registros[BYTES], uint8_t memoria[CAPACIDADMEM], uint32_t descSeg[CANTDESSEG]);
void add(TReg registros[BYTES], uint8_t memoria[CAPACIDADMEM], uint32_t descSeg[CANTDESSEG]);
void sub(TReg registros[BYTES], uint8_t memoria[CAPACIDADMEM], uint32_t descSeg[CANTDESSEG]);
void mul(TReg registros[BYTES], uint8_t memoria[CAPACIDADMEM], uint32_t descSeg[CANTDESSEG]);
void divi(TReg registros[BYTES], uint8_t memoria[CAPACIDADMEM], uint32_t descSeg[CANTDESSEG]);
void shl(TReg registros[BYTES], uint8_t memoria[CAPACIDADMEM], uint32_t descSeg[CANTDESSEG]);
void shr(TReg registros[BYTES], uint8_t memoria[CAPACIDADMEM], uint32_t descSeg[CANTDESSEG]);
void sar(TReg registros[BYTES], uint8_t memoria[CAPACIDADMEM], uint32_t descSeg[CANTDESSEG]);
void and(TReg registros[BYTES], uint8_t memoria[CAPACIDADMEM], uint32_t descSeg[CANTDESSEG]);
void or(TReg registros[BYTES], uint8_t memoria[CAPACIDADMEM], uint32_t descSeg[CANTDESSEG]);
void xor(TReg registros[BYTES], uint8_t memoria[CAPACIDADMEM], uint32_t descSeg[CANTDESSEG]);
void swap(TReg registros[BYTES], uint8_t memoria[CAPACIDADMEM], uint32_t descSeg[CANTDESSEG]);
void ldl(TReg registros[BYTES], uint8_t memoria[CAPACIDADMEM], uint32_t descSeg[CANTDESSEG]);
void ldh(TReg registros[BYTES], uint8_t memoria[CAPACIDADMEM], uint32_t descSeg[CANTDESSEG]);
void rnd(TReg registros[BYTES], uint8_t memoria[CAPACIDADMEM], uint32_t descSeg[CANTDESSEG]);
void cmp(TReg registros[BYTES], uint8_t memoria[CAPACIDADMEM], uint32_t descSeg[CANTDESSEG]);
void jmp(TReg registros[BYTES], uint8_t memoria[CAPACIDADMEM], uint32_t descSeg[CANTDESSEG]);
void jz(TReg registros[BYTES], uint8_t memoria[CAPACIDADMEM], uint32_t descSeg[CANTDESSEG]);
void jnz(TReg registros[BYTES], uint8_t memoria[CAPACIDADMEM], uint32_t descSeg[CANTDESSEG]);
void jp(TReg registros[BYTES], uint8_t memoria[CAPACIDADMEM], uint32_t descSeg[CANTDESSEG]);
void jnp(TReg registros[BYTES], uint8_t memoria[CAPACIDADMEM], uint32_t descSeg[CANTDESSEG]);
void jne(TReg registros[BYTES], uint8_t memoria[CAPACIDADMEM], uint32_t descSeg[CANTDESSEG]);
void jnn(TReg registros[BYTES], uint8_t memoria[CAPACIDADMEM], uint32_t descSeg[CANTDESSEG]);
void not(TReg registros[BYTES], uint8_t memoria[CAPACIDADMEM], uint32_t descSeg[CANTDESSEG]);
void stop(TReg registros[BYTES], uint8_t memoria[CAPACIDADMEM], uint32_t descSeg[CANTDESSEG]);

void cargarMnemonicos(MNO mnemonicos[32]);
void cargarCodeSeg(int argc, char *argv[], uint8_t *N, uint8_t memoria[CAPACIDADMEM], TReg registros[BYTES]);
void cargarRegistros(TReg registros[BYTES]);
void cargarDescSeg(uint32_t descSeg[CANTDESSEG], int N);
void disassembler(TReg registros[BYTES], uint8_t memoria[CAPACIDADMEM], MNO mnemonicos[BYTES], int N);
void lecturaOperandos(TReg registros[BYTES], uint8_t memoria[CAPACIDADMEM]);
void inicializarReg(TReg registros[BYTES]);
void mostrarOperandos(TReg registros[BYTES], uint8_t i);
void actualizarCC(TReg registros[BYTES], int32_t resultado);
int32_t cargarBinario(char *bin, TReg registros[BYTES]);
void mostrarBinario(int32_t valor);
char imprimirOPunto(char c);
void imprimirPalabra(int32_t pal, char *cad);

void leer(TReg registros[BYTES], uint8_t memoria[CAPACIDADMEM], uint32_t descSeg[CANTDESSEG], uint8_t opx);
void grabar(TReg registros[BYTES], uint8_t memoria[CAPACIDADMEM], uint32_t descSeg[CANTDESSEG], int32_t valor, uint8_t opx);

void main(int argc, char *argv[]){
    TReg registros[BYTES];  // poner en TDA?
    uint8_t memoria[CAPACIDADMEM]; // poner en TDA?

    uint8_t N, i = 0;
    MNO mnemonicos[BYTES];
    uint32_t descSeg[CANTDESSEG];
    Instruccion tablaInstrucciones[BYTES] = {
        sys, jmp, jz, jp, jne, jnz, jnp, jnn, not, NULL,
        NULL, NULL, NULL, NULL, NULL, stop, mov, add, sub, mul, divi, 
        cmp, shl, shr, sar, and, or, xor, swap, ldl, ldh, rnd
    };

    if (argc >= 2 && argc <=3){
        srand(time(NULL));
        cargarRegistros(registros);
        inicializarReg(registros);
        cargarCodeSeg(argc, argv, &N, memoria, registros);
        cargarDescSeg(descSeg, N);
        cargarMnemonicos(mnemonicos);
    
        if (registros[IP].valor != 0xffffffff){
            if(argc == 3)
                if (strcmp("-d", argv[2]) == 0){
                    disassembler(registros, memoria, mnemonicos, N); //muestra la traduccion
                    inicializarReg(registros);
                } else
                    printf("Argumento invalido. %s no existe.\n", argv[2]);

            while (registros[IP].valor != 0xffffffff && registros[IP].valor <= N){
                lecturaOperandos(registros, memoria);

                if (tablaInstrucciones[registros[OPC].valor] != NULL)
                    tablaInstrucciones[registros[OPC].valor](registros, memoria, descSeg);
                else { 
                    registros[IP].valor = 0xffffffff;
                    printf("Instrucción no implementada: 0x%02X\n", registros[OPC].valor);
                }
                
            }
        } 
    } else
        printf("Cantidad invalida de argumentos.\n");
}

void cargarCodeSeg(int argc, char *argv[], uint8_t *N, uint8_t memoria[CAPACIDADMEM], TReg registros[BYTES]){
    FILE * archB;
    char nomArch[20], header[6];
    uint8_t datoArch, version;
    int i;
    
    strcpy(nomArch, argv[1]);
    
    if ((archB = fopen(nomArch, "rb"))==NULL){
        registros[IP].valor = 0xffffffff;    
        printf("No fue posible abrir el archivo. Es posible que se haya ingresado mal el nombre.\n");
    } else {
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
            registros[IP].valor = 0xffffffff;    
            printf("No esta validado el archivo.\n");
        }
    
        fclose(archB);
    }
}

void disassembler(TReg registros[BYTES], uint8_t memoria[CAPACIDADMEM], MNO mnemonicos[BYTES], int N){
    int i = 0, j, cant;

    while (i < N){ //sacar reg
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
            mostrarOperandos(registros, 5);
            if(registros[OP2].valor != 0){
                printf(", ");
                mostrarOperandos(registros, 6);
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
    if (isprint((uint8_t)c))
        return c;
    else
        return '.';
}

void lecturaOperandos(TReg registros[BYTES], uint8_t memoria[CAPACIDADMEM]){
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


void cargarDescSeg(uint32_t descSeg[CANTDESSEG], int N){
    int i;
    for (i = 0; i < CANTDESSEG; i++){
        if (i == 0){
            descSeg[i] = 0;
            descSeg[i] = descSeg[i] << 16;
            descSeg[i] |= N-1;
        } else if (i == 1) {
            descSeg[i] = N;
            descSeg[i] = descSeg[i] << 16;
            descSeg[i] |= CAPACIDADMEM - N;
        } else
            descSeg[i] = 0;
    }
}

void inicializarReg(TReg registros[BYTES]){
    registros[CS].valor = 0x0000;
    registros[DS].valor = 0x00010000;
    registros[IP].valor = 0x0000;
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
        if (bin[i] == '1') {
            res |= 1u << (len - 1 - i); // pone el bit correspondiente
        } else if (bin[i] != '0') {
            printf("Error: caracter invalido\n");
            registros[IP].valor = 0xffffffff;
        }
    }
    return res;
}


void mostrarOperandos(TReg registros[BYTES], uint8_t i){
    TReg regAux = registros[DS];
    uint8_t pos = (registros[i].valor << 8) >> 24;
    int16_t offset = (registros[i].valor << 16) >> 16;
    uint8_t byte1 = (registros[i].valor << 24) >> 24;

    if((registros[i].valor >> 24 & 0x03) == 1) //registro
        printf(" %s", registros[byte1].nombre);

    else if((registros[i].valor >> 24 & 0x03) == 2) //inmediato
        printf(" %d", offset);
        
    else if((registros[i].valor >> 24 & 0x03)== 3){
        //memoria -> [reg], [reg + offset] y [offset]

        if(pos != 0)
            regAux = registros[pos];
        
        if (offset == 0)
            printf(" [%s]", regAux.nombre);
        else
            printf(" [%s+%d]", regAux.nombre, offset);
    }
}

void operacion(TReg registros[BYTES], uint8_t memoria[CAPACIDADMEM], uint32_t descSeg[CANTDESSEG], Operacion op) {
    int32_t valor;
    int32_t tmp;
    int16_t tmp16;
    if ((registros[OP1].valor & 0x0f000000) == 0x01000000) {
        if ((registros[OP2].valor & 0x0f000000) == 0x01000000)
            op(&registros[registros[OP1].valor & 0x0ff].valor, &registros[registros[OP2].valor & 0x0ff].valor, registros);
        else if ((registros[OP2].valor & 0x0f000000) == 0x02000000) {
            tmp16 = registros[OP2].valor & 0xFFFF;
            valor = (int32_t) tmp16;
            op(&registros[registros[OP1].valor & 0x0ff].valor, &valor, registros);            
        } else if ((registros[OP2].valor & 0x0f000000) == 0x03000000) {
            leer(registros, memoria, descSeg, 6);
            op(&registros[registros[OP1].valor & 0x0ff].valor, &registros[MBR].valor, registros);
        }
        if ((registros[OPC].valor >= 0x11 && registros[OPC].valor <= 0x14) || (registros[OPC].valor >= 0x16 && registros[OPC].valor <= 0x1c))
            actualizarCC(registros, registros[registros[OP1].valor & 0x0ff].valor);    
    } else if ((registros[OP1].valor & 0x0f000000) == 0x03000000) {
        if ((registros[OP2].valor & 0x0f000000) == 0x01000000) {
            leer(registros, memoria, descSeg, OP1);
            op(&registros[MBR].valor, &registros[registros[OP2].valor & 0x0ff].valor, registros);
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

void mov(TReg registros[BYTES], uint8_t memoria[CAPACIDADMEM], uint32_t descSeg[CANTDESSEG]) {
    operacion(registros, memoria, descSeg, move);
}

void add(TReg registros[BYTES], uint8_t memoria[CAPACIDADMEM], uint32_t descSeg[CANTDESSEG]) {
    operacion(registros, memoria, descSeg, suma);
}

void sub(TReg registros[BYTES], uint8_t memoria[CAPACIDADMEM], uint32_t descSeg[CANTDESSEG]){
    operacion(registros, memoria, descSeg, resta);
}

void mul(TReg registros[BYTES], uint8_t memoria[CAPACIDADMEM], uint32_t descSeg[CANTDESSEG]){
    operacion(registros, memoria, descSeg, multi);
}

void divi(TReg registros[BYTES], uint8_t memoria[CAPACIDADMEM], uint32_t descSeg[CANTDESSEG]){ //validar division por 0
    operacion(registros, memoria, descSeg, divid); 
}

void shl(TReg registros[BYTES], uint8_t memoria[CAPACIDADMEM], uint32_t descSeg[CANTDESSEG]){
    operacion(registros, memoria, descSeg, shiftLeft);
}

void shr(TReg registros[BYTES], uint8_t memoria[CAPACIDADMEM], uint32_t descSeg[CANTDESSEG]){
    operacion(registros, memoria, descSeg, shiftRightL);
}

void sar(TReg registros[BYTES], uint8_t memoria[CAPACIDADMEM], uint32_t descSeg[CANTDESSEG]){
    operacion(registros, memoria, descSeg, shiftRightSar);
}

void and(TReg registros[BYTES], uint8_t memoria[CAPACIDADMEM], uint32_t descSeg[CANTDESSEG]){
    operacion(registros, memoria, descSeg, y);
}

void or(TReg registros[BYTES], uint8_t memoria[CAPACIDADMEM], uint32_t descSeg[CANTDESSEG]){
    operacion(registros, memoria, descSeg, o);
}

void xor(TReg registros[BYTES], uint8_t memoria[CAPACIDADMEM], uint32_t descSeg[CANTDESSEG]){
    operacion(registros, memoria, descSeg, xO);
}

void swap(TReg registros[BYTES], uint8_t memoria[CAPACIDADMEM], uint32_t descSeg[CANTDESSEG]){
    operacion(registros, memoria, descSeg, swapOp);
}

void ldl(TReg registros[BYTES], uint8_t memoria[CAPACIDADMEM], uint32_t descSeg[CANTDESSEG]){
    operacion(registros, memoria, descSeg, loadLow);
}

void ldh(TReg registros[BYTES], uint8_t memoria[CAPACIDADMEM], uint32_t descSeg[CANTDESSEG]){
    operacion(registros, memoria, descSeg, loadHigh);
}

void rnd(TReg registros[BYTES], uint8_t memoria[CAPACIDADMEM], uint32_t descSeg[CANTDESSEG]){
    operacion(registros, memoria, descSeg, rndo);
}

void stop(TReg registros[BYTES], uint8_t memoria[CAPACIDADMEM], uint32_t descSeg[CANTDESSEG]){
    registros[IP].valor = 0xffffffff;
}

void cmp(TReg registros[BYTES], uint8_t memoria[CAPACIDADMEM], uint32_t descSeg[CANTDESSEG]){
    operacion(registros, memoria, descSeg, comparar);
}

void not(TReg registros[BYTES], uint8_t memoria[CAPACIDADMEM], uint32_t descSeg[CANTDESSEG]){
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

void sys(TReg registros[BYTES], uint8_t memoria[CAPACIDADMEM], uint32_t descSeg[CANTDESSEG]){
    int i, j, k;
    int32_t valor = 0, cant = 0;
    char *cadena;
    char binStr[33];
    
    if ((registros[OP1].valor & 0xffff) == 1){
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
            
            for (i = ((registros[ECX].valor >> 16) & 0xffff) - 1; i >= 0 & 0xffff; i--){
                memoria[(descSeg[registros[EDX].valor >> 16 & 0xff] >> 16 & 0xffff) + (registros[EDX].valor & 0x0000ffff) + (j*(registros[ECX].valor >> 16 & 0xffff)) + i] = valor & 0xff;
                valor = valor >> 8;
            }
        }
    } else if ((registros[OP1].valor & 0xffff) == 2){
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
    
}

void actualizarCC(TReg registros[BYTES], int32_t resultado) {
    registros[CC].valor = 0;              // limpio N y Z    
    if (resultado < 0) 
        registros[CC].valor |= (1u << 31);    // seteo N si es negativo

    if (resultado == 0) 
        registros[CC].valor |= (1u << 30);    // seteo Z si es cero

}

void leer(TReg registros[BYTES], uint8_t memoria[CAPACIDADMEM], uint32_t descSeg[CANTDESSEG], uint8_t opx) {
    uint16_t i;
    int32_t valor = 0, dirLog, dirFis, limSeg, dirBaseSeg, tamSeg, offset;
    uint16_t cant = 4;

    //TODO Confirmar si es necesario asumir que si se ingresa un inmediato tranformarlo en ds
    if((registros[opx].valor >> 16 & 0xff) == 0)
        dirLog = registros[DS].valor;
    else
        dirLog = registros[registros[opx].valor >> 16 & 0xff].valor;
    
    //direccion base del segmento
    dirBaseSeg = descSeg[dirLog >> 16 & 0xffff] >> 16 & 0xffff;
    
    //tamaño del segmento
    tamSeg = descSeg[dirLog >> 16 & 0xffff] & 0xffff;

    //desplazamiento
    offset = (registros[opx].valor & 0xffff) + (registros[registros[opx].valor >> 16 & 0xff].valor & 0x0000ffff);

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
        printf("Se invade segmento\n");
        registros[IP].valor = 0xffffffff;
    }
}
void grabar(TReg registros[BYTES], uint8_t memoria[CAPACIDADMEM], uint32_t descSeg[CANTDESSEG], int32_t valor, uint8_t opx) {
    int16_t i, cant = 4;
    int32_t tmp, dirLog, dirFis, limSeg, dirBaseSeg, tamSeg, offset;
    
    //Seguir testeando con mas ejemplos
    if(((registros[opx].valor >> 16) & 0xffff) == 0)
        dirLog = registros[DS].valor;
    else
        dirLog = registros[(registros[opx].valor >> 16) & 0xff].valor;

    //direccion base del segmento
    dirBaseSeg = descSeg[dirLog >> 16 & 0xffff] >> 16 & 0xffff;
    
    //tamaño del segmento
    tamSeg = descSeg[dirLog >> 16 & 0xffff] & 0xffff;

    //desplazamiento
    offset = (registros[opx].valor & 0xffff) + (registros[registros[opx].valor >> 16 & 0xff].valor & 0x0000ffff);

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

void saltoGenerico(TReg registros[BYTES], uint8_t memoria[CAPACIDADMEM], uint32_t descSeg[CANTDESSEG], SaltoCond op) {
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
    if (destino < 0 || destino >= (descSeg[registros[CS].valor] & 0xffff)) {
        registros[IP].valor = 0xffffffff;
        printf("Salto fuera del segmento de codigo.\n");
    } else if (destino > CAPACIDADMEM){
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

void jmp(TReg registros[BYTES], uint8_t memoria[CAPACIDADMEM], uint32_t descSeg[CANTDESSEG]) {
    saltoGenerico(registros, memoria, descSeg, jmpOp); 
}

void jz(TReg registros[BYTES], uint8_t memoria[CAPACIDADMEM], uint32_t descSeg[CANTDESSEG]) {
    saltoGenerico(registros, memoria, descSeg, jzOp);
}

void jnz(TReg registros[BYTES], uint8_t memoria[CAPACIDADMEM], uint32_t descSeg[CANTDESSEG]) {
    saltoGenerico(registros, memoria, descSeg, jnzOp);
}

void jp(TReg registros[BYTES], uint8_t memoria[CAPACIDADMEM], uint32_t descSeg[CANTDESSEG]) {
    saltoGenerico(registros, memoria, descSeg, jpOp);
}

void jnp(TReg registros[BYTES], uint8_t memoria[CAPACIDADMEM], uint32_t descSeg[CANTDESSEG]) {
    saltoGenerico(registros, memoria, descSeg, jnpOp);
}

void jne(TReg registros[BYTES], uint8_t memoria[CAPACIDADMEM], uint32_t descSeg[CANTDESSEG]) {
    saltoGenerico(registros, memoria, descSeg, jnOp);
}

void jnn(TReg registros[BYTES], uint8_t memoria[CAPACIDADMEM], uint32_t descSeg[CANTDESSEG]) {
    saltoGenerico(registros, memoria, descSeg, jnnOp);
}