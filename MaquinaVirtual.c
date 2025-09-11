#include <string.h>
#include <stdio.h>
#include <stdint.h>

#define ST4 5 // poner en TDA?
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
#define CS 26
#define DS 27

typedef struct {
    char nombre[ST3];
    int32_t valor; 
} TReg;

typedef char MNO[ST4];

typedef int32_t (*Operacion)(int32_t a, int32_t b);
int32_t suma(int32_t a, int32_t b) { return a + b; }
int32_t resta(int32_t a, int32_t b) { return a - b; }
int32_t multi(int32_t a, int32_t b) { return a * b; }
int32_t divi(int32_t a, int32_t b) { return a / b; }
int32_t shiftLeft(int32_t a, int32_t b) { return a << b; }
int32_t shiftRightL(int32_t a, int32_t b) { return (uint32_t)a >> b; }
int32_t shiftRightSar(int32_t a, int32_t b) { return a >> b; }
int32_t y(int32_t a, int32_t b) { return a & b; }
int32_t o(int32_t a, int32_t b) { return a | b; }
int32_t xO(int32_t a, int32_t b) { return a ^ b; }

int32_t not(int32_t a) { return ~a; }

void operacion(TReg registros[BYTES], uint8_t memoria[CAPACIDADMEM], uint32_t descSeg[CANTDESSEG], Operacion op);

void add(TReg registros[BYTES], uint8_t memoria[CAPACIDADMEM], uint32_t descSeg[CANTDESSEG]);
void sub(TReg registros[BYTES], uint8_t memoria[CAPACIDADMEM], uint32_t descSeg[CANTDESSEG]);
void mul(TReg registros[BYTES], uint8_t memoria[CAPACIDADMEM], uint32_t descSeg[CANTDESSEG]);
void div(TReg registros[BYTES], uint8_t memoria[CAPACIDADMEM], uint32_t descSeg[CANTDESSEG]);
void shl(TReg registros[BYTES], uint8_t memoria[CAPACIDADMEM], uint32_t descSeg[CANTDESSEG]);
void shr(TReg registros[BYTES], uint8_t memoria[CAPACIDADMEM], uint32_t descSeg[CANTDESSEG]);
void sar(TReg registros[BYTES], uint8_t memoria[CAPACIDADMEM], uint32_t descSeg[CANTDESSEG]);
void and(TReg registros[BYTES], uint8_t memoria[CAPACIDADMEM], uint32_t descSeg[CANTDESSEG]);
void or(TReg registros[BYTES], uint8_t memoria[CAPACIDADMEM], uint32_t descSeg[CANTDESSEG]);
void xor(TReg registros[BYTES], uint8_t memoria[CAPACIDADMEM], uint32_t descSeg[CANTDESSEG]);

void cargarMnemonicos(MNO mnemonicos[32]);
void cargarCodeSeg(int argc, char *argv[], uint8_t *N, uint8_t memoria[CAPACIDADMEM]);
void disassembler(TReg registros[BYTES], uint8_t memoria[CAPACIDADMEM], MNO mnemonicos[BYTES], int N);
void cargarRegistros(TReg registros[BYTES]);
void lecturaOperandos(TReg registros[BYTES], uint8_t memoria[CAPACIDADMEM]);
void cargarDescSeg(uint32_t descSeg[CANTDESSEG], int N);
void inicializarReg(TReg registros[BYTES]);
void mostrarOperandos(TReg registros[BYTES], uint8_t i);

void leer(TReg registros[BYTES], uint8_t memoria[CAPACIDADMEM], uint32_t descSeg[CANTDESSEG], uint8_t opx);
void grabar(TReg registros[BYTES], uint8_t memoria[CAPACIDADMEM], uint32_t descSeg[CANTDESSEG], uint32_t valor);


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
        cargarMnemonicos(mnemonicos);

        inicializarReg(registros);
        if(argc == 3)
            if (strcmp("-d", argv[2]) == 0){
                disassembler(registros, memoria, mnemonicos, N); //muestra la traduccion
                inicializarReg(registros);
                scanf("%c");
            } else
                printf("Argumento invalido. %s no existe.\n", argv[2]);
        
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
    int i = 0, j, cant;

    while (i <= N && registros[IP].valor != 0xffffffff){
        printf("[%04x]: ", registros[IP].valor);
        
        lecturaOperandos(registros, memoria);
        
        if (registros[IP].valor != 0xffffffff){
            cant = ((registros[OP1].valor >> 24) & 0x3) + ((registros[OP2].valor >> 24) & 0x3) + 1;
            for (j = 0; j < cant; j++)
                printf("%02x ", memoria[registros[IP].valor - cant + j]);
        } else
            printf("%02x ", memoria[i]);
        
        printf("| %s ", mnemonicos[registros[OPC].valor]);
            
        if (registros[OP1].valor != 0 && registros[IP].valor != 0xffffffff) { //puede ser inmediato, memoria, registro o ninguno
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

void lecturaOperandos(TReg registros[BYTES], uint8_t memoria[CAPACIDADMEM]){
    uint32_t aux, valorAGuardar;

    int i;

    registros[OPC].valor = memoria[registros[IP].valor];
    registros[OPC].valor = registros[OPC].valor & 0x1f;

    if (registros[OPC].valor>= 0 && registros[OPC].valor <=8 || registros[OPC].valor >= 15 && registros[OPC].valor<= 31){
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

            if(registros[OP1].valor == 0x00){ //Si trae problemas, es esto
                registros[OP1].valor = registros[OP2].valor;
                registros[OP2].valor = 0x00;
            }

            registros[IP].valor += ((registros[OP1].valor >> 24) & 0x03) + ((registros[OP2].valor >> 24) & 0x03) + 1; // si no es JMP o primos       
        } else {
            registros[IP].valor = 0xffffffff;
        }    
    }else
        printf("Codigo de operacion invalido\n");
    
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
    registros[CS].valor = 0x0000;
    registros[DS].valor = 0x01000000;
    registros[IP].valor = 0x0000;
}

void mostrarOperandos(TReg registros[BYTES], uint8_t i){
    TReg regAux = registros[DS];
    uint8_t pos = (registros[i].valor << 8) >> 24;
    int8_t offset = (registros[i].valor << 16) >> 16;
    uint8_t byte1 = (registros[i].valor << 24) >> 24;

    if((registros[i].valor >> 24) & 0x3 == 1) //registro
        printf(" %s", registros[byte1].nombre);

    else if((registros[i].valor >> 24) & 0x3 == 2) //inmediato
        printf(" %d", offset);
        
    else if((registros[i].valor >> 24) & 0x3 == 3){
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
    uint32_t valor;
    
    if((registros[OP1].valor & 0x0f000000) == 0x01000000){
        if((registros[OP2].valor & 0x0f000000) == 0x01000000)
            registros[registros[OP1].valor & 0x0ff].valor = op(registros[registros[OP1].valor & 0x0ff].valor, registros[registros[OP2].valor & 0x0ff].valor);
        else if((registros[OP2].valor & 0x0f000000) == 0x02000000)
            registros[registros[OP1].valor & 0x0ff].valor = op(registros[registros[OP1].valor & 0x0ff].valor, registros[OP2].valor & 0x0ffff);
        else if((registros[OP2].valor & 0x0f000000) == 0x03000000){
            leer(registros,memoria, descSeg, 6);
            registros[registros[OP1].valor & 0x0ff].valor = op(registros[registros[OP1].valor & 0x0ff].valor, registros[MBR].valor);
        }
    } else if ((registros[OP1].valor & 0x0f000000) == 0x02000000){
        if((registros[OP2].valor & 0x0f000000) == 0x01000000)
            valor = op(registros[OP1].valor & 0x0ffff, registros[registros[OP2].valor & 0x0ff].valor);
        else if ((registros[OP2].valor & 0x0f000000) == 0x02000000)
            valor = op(registros[OP1].valor & 0x0ffff, registros[OP2].valor & 0x0ffff);
        else if((registros[OP2].valor & 0x0f000000) == 0x03000000){
            leer(registros,memoria, descSeg, 6);
            valor = op(registros[OP1].valor & 0x0ffff, registros[MBR].valor);
        }
    } else if((registros[OP1].valor & 0x0f000000) == 0x03000000){
        if((registros[OP2].valor & 0x0f000000) == 0x01000000){
            leer(registros,memoria, descSeg, 5);
            valor = op(registros[MBR].valor, registros[registros[OP2].valor & 0x0ff].valor);
            grabar(registros, memoria, descSeg, valor);
        
        } else if((registros[OP2].valor & 0x0f000000) == 0x02000000){
            leer(registros,memoria, descSeg, 5); 
            valor = op(registros[MBR].valor, registros[OP2].valor & 0x0ffff);
            grabar(registros, memoria, descSeg, valor);
        
        } else if((registros[OP2].valor & 0x0f000000) == 0x03000000){
            leer(registros,memoria, descSeg, 5);
            valor = registros[MBR].valor;
            leer(registros,memoria, descSeg, 6);
            valor = op(valor, registros[MBR].valor);
            grabar(registros, memoria, descSeg, valor);
        }
    }
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

void div(TReg registros[BYTES], uint8_t memoria[CAPACIDADMEM], uint32_t descSeg[CANTDESSEG]){ //validar division por 0
    int tipoOp = (registros[OP1].valor >> 24) & 0x3;
    
    leer(registros, memoria, descSeg, 5);
    
    if((registros[OP2].valor & 0x0ffff) != 0 && tipoOp == 1 || registros[registros[OP2].valor & 0x0ff].valor != 0 && tipoOp == 2 || registros[MBR].valor != 0 && tipoOp == 3)
        operacion(registros, memoria, descSeg, divi); 
    else {
        printf("No se puede hacer una division por 0.\n");
        registros[IP].valor = 0xffffffff;
    }
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

void jmp(){
    /*
    
    if (registros[OPC].valor >= 0x01 && registros[OPC].valor <= 0x07){ //avance normal o jump
            if ((registros[OP1].valor & 0x0000FFFF) >= registros[DS].valor){
                *condicion = 0;
                printf("Invade segmento de datos.\n");
            } else if ((registros[OP1].valor & 0x0000FFFF) > CAPACIDADMEM){
                *condicion = 0;
                printf("Posicion de memoria invalida.\n");
            } else
                registros[IP].valor = registros[OP1].valor & 0x0000FFFF; //
        } else 
         
        */
}

void grabar(TReg registros[BYTES], uint8_t memoria[CAPACIDADMEM], uint32_t descSeg[CANTDESSEG], uint32_t valor){ // donde indica el operando 1
    uint16_t i, cant = 0x0004;

    registros[LAR].valor = (descSeg[1] & 0xffff0000) | (registros[OP1].valor & 0x0ffff); // carga lar
    registros[MAR].valor = (registros[OP1].valor << 16) | (registros[(registros[OP1].valor >> 16) & 0x0ff].valor + (registros[OP1].valor & 0x0ffff)); // carga mar
    registros[MBR].valor = valor; // carga mbr

    i = (registros[MAR].valor & 0x0ffff) + cant - 1;
    for(;i>=(registros[MAR].valor & 0x0ffff);i--){
        memoria[i] = (valor & 0x0ff);
        valor = valor >> 8; 
    } //carga memoria
}

void leer(TReg registros[BYTES], uint8_t memoria[CAPACIDADMEM], uint32_t descSeg[CANTDESSEG], uint8_t opx){ // op1(5) y op2(6)
    uint16_t i, cant = 0x0004;
    uint32_t valor;
    
    registros[LAR].valor = (descSeg[1] & 0xffff0000) | (registros[opx].valor & 0x0ffff); // carga lar
    registros[MAR].valor = (registros[opx].valor << 16) | (registros[(registros[opx].valor >> 16) & 0x0ff].valor + (registros[opx].valor & 0x0ffff)); // carga mar

    i = (registros[MAR].valor & 0x0ffff);
    for(;i<=((registros[MAR].valor & 0x0ffff) + cant - 1);i++){
        valor = memoria[i];
        valor = valor << 8; 
    } //carga memoria

    registros[MBR].valor = valor; // carga mbr
}
