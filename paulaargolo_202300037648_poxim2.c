#include <stdbool.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>


void verifyZero(uint32_t reg, uint32_t teste, uint64_t temp) {
  if (reg == 0) {
    if (teste == 1) {
    
    } else {
      temp = temp & 0xFFFFFFFF00000000;  
    }
  } 
}

//verificar um bit  -- value: valor a verificar -- bitPosition: A posição do bit (0 a 31).
// 1 se o bit estiver definido -- 0 se o bit estiver desligado -- -1 se bitPosition for inválido.
int checkBit32(uint32_t value, int bitPosition) {
    // Cria uma máscara para isolar o bit
    uint32_t mask = 1u << bitPosition;
    // Aplica a máscara e verifica se o bit é 0 ou 1
    return (value & mask) != 0;
}


int checkBit64(uint64_t value2, int bitPosition2) {
    uint64_t mask2 = 1ull << bitPosition2;
    return (value2 & mask2) != 0;
}


uint32_t ExtendedBit21To32(uint32_t hex) {
  if (checkBit32(hex, 20)) {
    return hex | 0xFFF00000;
  } else {
    return hex;
  }
}

uint32_t ExtendedBit15To32(uint32_t hex) {
  if (checkBit32(hex, 15)) {
    return hex | 0xFFFF0000;
  } else {
    return hex;
  }
}

uint32_t ExtendedBit25To32(uint32_t hex) {
  if (checkBit32(hex, 25)) {
    return hex | 0xFC000000;
  } else {
    return hex;
  }
}


char *getRegisterSmaller(uint32_t reg) {
  char *result;
  if (reg == 28) {
    result = strdup("ir");
  } else if (reg == 26) {
    result = strdup("cr");
  } else if (reg == 27) {
    result = strdup("ipc");
  } else if (reg == 29) {
    result = strdup("pc");
  } else if (reg == 30) {
    result = strdup("sp");
  } else if (reg == 31) {
    result = strdup("sr");
  } else {
    result = (char *)malloc(2 + snprintf(NULL, 0, "%u", reg));
    sprintf(result, "r%u", reg);
  }
  return result;
}


char *getRegisterBigger(uint32_t reg) {
  char *result;
  if (reg == 28) {
    result = strdup("IR");
  } else if (reg == 26) {
    result = strdup("CR");
  } else if (reg == 27) {
    result = strdup("IPC");
  }else if (reg == 29) {
    result = strdup("PC");
  } else if (reg == 30) {
    result = strdup("SP");
  } else if (reg == 31) {
    result = strdup("SR");
  } else {
    result = (char *)malloc(2 + snprintf(NULL, 0, "%u", reg));
    sprintf(result, "R%u", reg);
  }
  return result;
}


uint64_t extendTo64(uint32_t num) {
    uint64_t extendedNum;
    if (num < 0)
        extendedNum = ((uint64_t)num) | ((uint64_t)0xFFFFFFFF << 32);
    else
        extendedNum = (uint64_t)num;

    return extendedNum;
}


uint32_t setRegistrador(uint32_t r, uint32_t valor) {
  if (r == 0) {
    return 0;
  } else {
    return valor;
  }
}

// Função para obter o expoente de um ponto flutuante
uint32_t calcularExpoente(float numero) {
  unsigned int *ponteiro = (unsigned int*) &numero;     //cast do endereço do número de ponto flutuante para um ponteiro
  int expoente = 0;
  expoente = *ponteiro & 0x7F800000; //máscara para isolar os bits correspondentes ao expoente
  expoente = expoente >> 23;
  return expoente;
}


// Função para calcular o número de ciclos necessários para executar uma operação em ponto flutuante
uint32_t calcularNumeroDeCiclos(float X, float Y) {
  uint32_t expoenteX = calcularExpoente(X);
  uint32_t expoenteY = calcularExpoente(Y);
  uint32_t numeroDeCiclos = abs(expoenteX - expoenteY) + 1;  //	|exp(X ) − exp(Y )| + 1
  return numeroDeCiclos;
}


//ZN 
bool setZN(uint32_t R) {
  return checkBit32(R, 6);
}

// ZD 5
bool setZD(uint32_t R) {
  return checkBit32(R, 5);
}

// SN 4
bool setSN(uint32_t R) {
  return checkBit32(R, 4);
}

// OV 3
bool setOV(uint32_t R) {
  return checkBit32(R, 3);
}

// IV 2
bool setIV(uint32_t R) {
  return checkBit32(R, 2);
  
}

//IE 1
bool setIE(uint32_t R) {
    return checkBit32(R, 1);
}

// CY 0
bool setCY(uint32_t R) {
  return checkBit32(R, 0);
}


//ZN 6
uint32_t bitZN(uint32_t SR, bool condicao) {
  if (condicao) {
      SR = SR | 0b100000;
  } else {
      SR = SR & ~0b100000;
  }
  return SR;
}


// ZD 5
uint32_t bitZD(uint32_t SR, bool condicao) {
  if (condicao) {
      SR = SR | 0b100000;
  } else {
      SR = SR & ~0b100000;
  }
  return SR;
}

// SN 4
uint32_t bitSN(uint32_t SR, bool condicao) {
  if (condicao) {
      SR = SR | 0b10000;
  } else {
      SR = SR & ~0b10000;
  }
  return SR;
}

// OV 3
uint32_t bitOV(uint32_t SR, bool condicao) {
  if (condicao) {
      SR = SR | 0b1000;
  } else {
      SR = SR & ~0b1000;
  }
  return SR;
}

// IV 2
uint32_t bitIV(uint32_t SR, bool condicao) {
  if (condicao) {
      SR = SR | 0b100;
  } else {
      SR = SR & ~0b100;
  }
  return SR;
}

//IE 1
uint32_t bitIE(uint32_t SR, bool condicao) {
  if (condicao) {
      SR = SR | 0b10;
  } else {
      SR = SR & ~0b10;
  }
  return SR;
}

// CY 0
uint32_t bitCY(uint32_t SR, bool condicao) {
  if (condicao) {
      SR = SR | 0b1;
  } else {
      SR = SR & ~0b1;
  }
  return SR;
}


void pilhaISR(uint32_t *MEM, uint32_t CR, uint32_t IPC, uint32_t PC, uint32_t SP, uint32_t SR,
              uint32_t *novo_PC, uint32_t *novo_SP, uint32_t *novo_CR, uint32_t *novo_IPC) {
    MEM[SP >> 2] = PC + 4;
    *novo_SP = SP - 4;
    MEM[*novo_SP >> 2] = CR;
    *novo_SP = *novo_SP - 4;
    MEM[*novo_SP >> 2] = IPC;
    *novo_SP = *novo_SP - 4;

    *novo_PC = PC + 4;
    *novo_CR = CR;
    *novo_IPC = IPC;
}


// Função para configurar o estado do registrador FPU
void statusFpu(uint8_t fpuSTandOP, bool OneOrZero) {
    const uint8_t PRONTO = 0; // Representa o estado "PRONTO"
    const uint8_t ERRO = 0b100000; // Representa o estado "ERRO"
    if (OneOrZero) {
      fpuSTandOP |= ERRO; // Faz a operação OR bit a bit com o estado "ERRO"
    } else {
      fpuSTandOP &= ~ERRO; // Faz a operação AND bit a bit com o complemento do estado "ERRO"
    }
}


int main(int argc, char *argv[]) { 

  FILE *input = fopen(argv[1], "r");
  FILE *output = fopen(argv[2], "w");

  uint32_t R[32] = { 0 };

  //WATCHDOG 
  uint32_t watchdog = 0, counter = 0;
  bool watchdog_pending = false;


  //TERMINAL
  int caps = 16;
  int terminal;
  char* teste = (char*)(calloc(1, caps));
  int count = 0;

  
  //FPU
  float fpuX = 0; // 0x80808880
	float fpuY = 0; // 0x80808884
	float fpuZ = 0; // 0x80808888
	uint8_t fpuStatusAndOperation = 0; // 0x8080888F
	uint8_t fpuOperation = 0;
	uint8_t numberOfCycles = 0;
	bool shiftIEEE = false;
	bool shiftIEEEOfX = false;
	bool shiftIEEEOfY = false;
	uint8_t fpuLastOperation = 0;
	uint32_t fpuRounded = 0;

  uint8_t *MEM8 = (uint8_t*)(calloc(32, 1024));
  uint32_t *MEM32 = (uint32_t*)(calloc(32, 1024));

  printf("[START OF SIMULATION]\n");
  fprintf(output, "[START OF SIMULATION]\n");


  char row[300];
  uint32_t c = 0;
  while (fgets(row, sizeof(row), input) != NULL) {
    MEM32[c] = strtoul(row, NULL, 16);
    c++;
  }


  uint8_t executa = 1;
  while (executa) {
    char instrucao[30] = {0};

    uint8_t z = 0, x = 0, y = 0, v = 0, w = 0;
    uint32_t pc = 0, xyl = 0, sp = 0, tmpSubi = 0, i = 0, ipc = 0, cr = 0, temp = 0, addr = 0, shift = 0, hardwareValue = 0, novo_PC, novo_SP, novo_CR, novo_IPC, SR = 0;
    uint64_t tmpSla_1 = 0, tmpSll_1 = 0,tmpSra_1 = 0, tmpSrl_1 = 0, cmp1 = 0, cmpi1 = 0, tmpMul_1 = 0, tmpMuls_1 = 0, tmpAdd_1 = 0;
    bool condicao = true;

    R[28] = ((MEM8[R[29] + 0] << 24) | (MEM8[R[29] + 1] << 16) | (MEM8[R[29] + 2] << 8) | (MEM8[R[29] + 3] << 0)) | MEM32[R[29] >> 2];

    uint8_t opcode = (R[28] & (0b111111 << 26)) >> 26;
    uint8_t subcode = (R[28] & (0b111 << 8)) >> 8;


    //watchdog

      //10000000000000000000000000000000 AND ADDRESS OF WACTHDOG TRUE
    if (0x80000000 & watchdog) {
       if(counter == 0) {
         watchdog_pending = true;
        //and IE (bit 1  in status register SR) 
        if (R[31] & 0b10) {
          watchdog = 0;
          printf("[HARDWARE INTERRUPTION 1]\n");
          fprintf(output, "[HARDWARE INTERRUPTION 1]\n");
          MEM32[R[30] >> 2] = R[29] + 4, R[30] = R[30] - 4;
          MEM32[R[30] >> 2] = R[26], R[30] = R[30] - 4;
          MEM32[R[30] >> 2] = R[27], R[30] = R[30] - 4;
          R[29] = 0x00000010;
          R[26] = 0xE1AC04DA; //ARMAZENA O CÓDIGO IDENTIFICADOR DAS INTERRUPÇÕES
          R[27] = R[29]; //ARMAZENA O ENDEREÇO DA INSTRUÇÃO ONDE A INTERRUPÇÃO FOI GERADA
          watchdog_pending = false;
          continue;
        }
      } else {
        counter -= 1;
        watchdog -= 1;
      }
    }

     
    switch (opcode) 
    {
      
      // mov (tipe U)
      case 0b000000:
        pc = R[29];
        sp = R[30];
        z = (R[28] & (0b11111 << 21)) >> 21;
        xyl = R[28] & 0x1FFFFF;
        R[z] = xyl;

        //0x????????:	mov rz,u                 	Rz=0x????????
        sprintf(instrucao, "mov %s,%i", getRegisterSmaller(z), xyl);
        fprintf(output, "0x%08X:\t%-25s\t%s=0x%08X\n", pc, instrucao, getRegisterBigger(z), R[z]);
        printf("0x%08X:\t%-25s\t%s=0x%08X\n", pc, instrucao, getRegisterBigger(z), R[z]);
        break;


      // movs (tipe U)
      case 0b000001:
        pc = R[29];
        z = (R[28] & (0b11111 << 21)) >> 21;
        x = (R[28] & (0b11111 << 16)) >> 21;
        y = (R[28] & (0b11111 << 11)) >> 11;
        xyl = R[28] & 0x1FFFFF;

        R[z] = ExtendedBit21To32(xyl);

      //0x????????:	movs rz,s                	Rz=0x????????
        sprintf(instrucao, "movs %s,%i", getRegisterSmaller(z), R[z]);
        fprintf(output, "0x%08X:\t%-25s\t%s=0x%08X\n", pc, instrucao, getRegisterBigger(z), R[z]);
        printf("0x%08X:\t%-25s\t%s=0x%08X\n", pc, instrucao, getRegisterBigger(z), R[z]);
        break;
        

      // add (tipe U)
      case 0b000010:

        pc = R[29];
        z = (R[28] & (0b11111 << 21)) >> 21;
        x = (R[28] & (0b11111 << 16)) >> 16;
        y = (R[28] & (0b11111 << 11)) >> 11;

        //tmpAdd_1 = R[x] + R[y];

        tmpAdd_1 = (uint64_t)(R[x]) + (uint64_t)(R[y]);
        (uint64_t)(R[z]);
        R[z] = tmpAdd_1;


        //zn rz = 0
        if (R[z] != 0) {
          R[31] = R[31] & ~0b1000000;
        } else {
          R[31] = R[31] | 0b1000000;
        }

        //sn rz31 = 1
        if (checkBit64(R[z], 31)) {
          R[31] = R[31] | 0b10000;
        } else {
          R[31] = R[31] & ~0b10000;
        }

        //ov Rx31 = Ry31 ^ Rz31 != Rx31
        if ((checkBit32(R[x], 31) == checkBit32(R[y], 31)) && (checkBit32(R[z], 31) != checkBit32(R[x], 31))) {
          R[31] = R[31] | 0b1000;
        } else {
            R[31] = R[31] & ~0b1000;
        }

        //cy rz32 = 1
        if ((checkBit64(tmpAdd_1, 32)) != 0) {
          R[31] = R[31] | 0b1;
        } else {
          R[31] = R[31] & ~0b1;
        }


      // 0x????????:	add rz,rx,ir Rz=Rx+IR=0x????????,SR=0x????????
        sprintf(instrucao, "add %s,%s,%s", getRegisterSmaller(z), getRegisterSmaller(x), getRegisterSmaller(y));
        fprintf(output, "0x%08X:\t%-25s\t%s=%s+%s=0x%08X,SR=0x%08X\n", pc, instrucao, getRegisterBigger(z), getRegisterBigger(x), getRegisterBigger(y), R[z], R[31]);
        printf("0x%08X:\t%-25s\t%s=%s+%s=0x%08X,SR=0x%08X\n", pc, instrucao, getRegisterBigger(z), getRegisterBigger(x), getRegisterBigger(y), R[z], R[31]);
        break;


      // sla
      case 0b000100:

        switch (subcode) 
        {
          //sla
          case 0b011:

            pc = R[29]; 
            xyl = R[28] & 0b11111;
            z = (R[28] & (0b11111 << 21)) >> 21;
            x = (R[28] & (0b11111 << 16)) >> 16;
            y = (R[28] & (0b11111 << 11)) >> 11;

            // R[z]:R[x]=(R[z]:R[y])*(2(l + 1))
            tmpSla_1 = (((uint64_t)R[z] << 32) | (uint64_t)R[y]);
            tmpSla_1 = tmpSla_1 << (xyl + 1);

            verifyZero(z, 1, tmpSla_1);
            verifyZero(x, 1, tmpSla_1);

            R[z] = (uint32_t)setRegistrador(z, (tmpSla_1 >> 32) & 0xFFFFFFFF);
            R[x] = (uint32_t)setRegistrador(x, (tmpSla_1) & 0xFFFFFFFF);

            tmpSla_1 = R[x] | R[z];

            //zn rlrz = 0
            if ((tmpSla_1) != 0) {
              R[31] = R[31] & ~0b1000000;
            } else {
              R[31] = R[31] | 0b1000000; 
            }

            //ov rz !=0
            if (R[z] != 0) {
              R[31] = R[31] | 0b1000;
            } else {
              R[31] = R[31] & ~0b1000;
              }

            
            // 0x????????:	sla rz,rx,ry,u Rz:Rx=Rz:Ry<<u=0x????????????????,SR=0x????????
            sprintf(instrucao, "sla %s,%s,%s,%i", getRegisterSmaller(z), getRegisterSmaller(x), getRegisterSmaller(y), xyl);
            fprintf(output, "0x%08X:\t%-25s\t%s:%s=%s:%s<<%u=0x%016lX,SR=0x%08X\n", pc, instrucao, getRegisterBigger(z), getRegisterBigger(x), getRegisterBigger(z), getRegisterBigger(y), xyl+1, tmpSla_1, R[31]);
            printf("0x%08X:\t%-25s\t%s:%s=%s:%s<<%u=0x%016lX,SR=0x%08X\n", pc, instrucao, getRegisterBigger(z), getRegisterBigger(x), getRegisterBigger(z), getRegisterBigger(y), xyl+1,tmpSla_1, R[31]);
            break;  


            //mul
          case 0b000:

            pc = R[29];
            xyl = R[28] & 0b11111;
            z = (R[28] & (0b11111 << 21)) >> 21;
            x = (R[28] & (0b11111 << 16)) >> 16;
            y = (R[28] & (0b11111 << 11)) >> 11;   

           
            // R[l] : R[z] = R[x] * R[y]
            tmpMul_1 = (((uint64_t)R[xyl] << 32) | (uint64_t)R[z]);
            tmpMul_1 = (uint64_t)(R[x] * R[y]);

            verifyZero(xyl, 1, tmpMul_1);
            verifyZero(z, 1, tmpMul_1);

             // Extracting the 32 most significant bits
            R[xyl] = (uint32_t)setRegistrador(xyl, (tmpMul_1 >> 32) & 0xFFFFFFFF);
            // Extracting the least significant bits
            R[z] = (uint32_t)setRegistrador(z, (tmpMul_1) & 0xFFFFFFFF);

            
            //zn rlrz = 0
            if ((tmpMul_1) != 0) {
              R[31] = R[31] & ~0b1000000;
            } else {
              R[31] = R[31] | 0b1000000; 
            }

            //cy rl != 0
            if ((R[xyl]) != 0) {
              R[31] = R[31] | 0b1;
            } else {
              R[31] = R[31] & ~0b1; 
            }

                            
            // 0x????????:	mul rl,rz,rx,ry Rl:Rz=Rx*Ry=0x????????????????,SR=0x????????
            sprintf(instrucao, "mul %s,%s,%s,%s", getRegisterSmaller(xyl), getRegisterSmaller(z), getRegisterSmaller(x), getRegisterSmaller(y));
            fprintf(output, "0x%08X:\t%-25s\t%s:%s=%s*%s=0x%016lX,SR=0x%08X\n", pc, instrucao, getRegisterBigger(xyl), getRegisterBigger(z), getRegisterBigger(x), getRegisterBigger(y), tmpMul_1, R[31]);
            printf("0x%08X:\t%-25s\t%s:%s=%s*%s=0x%016lX,SR=0x%08X\n", pc, instrucao, getRegisterBigger(xyl), getRegisterBigger(z), getRegisterBigger(x), getRegisterBigger(y), tmpMul_1, R[31]); 

            break;


          // sll
          case 0b001:

            pc = R[29];
            xyl = R[28] & 0b11111;
            z = (R[28] & (0b11111 << 21)) >> 21;
            x = (R[28] & (0b11111 << 16)) >> 16;
            y = (R[28] & (0b11111 << 11)) >> 11;

            // R[z]:R[x]=(R[z]:R[y])*(2(l + 1))
            tmpSll_1 = (((uint64_t)R[z] << 32) | (uint64_t)R[y]);
            tmpSll_1 = tmpSll_1 << (xyl + 1);

            verifyZero(z, 1, tmpSll_1);
            verifyZero(x, 1, tmpSll_1);

            // Extracting the 32 most significant bits
            R[z] = (uint32_t)setRegistrador(z, (tmpSll_1 >> 32) & 0xFFFFFFFF);
            // Extracting the least significant bits 
            R[x] = (uint32_t)setRegistrador(x, (tmpSll_1) & 0xFFFFFFFF);
            
            tmpSll_1 = R[x] | R[z];
            
            //zn rzrx = 0
            if ((tmpSll_1) != 0) {
              R[31] = R[31] & ~0b1000000;
            } else {
              R[31] = R[31] | 0b1000000; 
            }

           //cy rz != 0
            if (R[z] != 0) {
              R[31] = R[31] | 0b1;
            } else {
              R[31] = R[31] & ~0b1;
            }

            // 0x????????:	sll rz,rx,ry,u  // Rz:Rx=Rz:Ry<<u=0x????????????????,SR=0x????????
            sprintf(instrucao, "sll %s,%s,%s,%u", getRegisterSmaller(z), getRegisterSmaller(x), getRegisterSmaller(y), xyl);
            fprintf(output, "0x%08X:\t%-25s\t%s:%s=%s:%s<<%u=0x%016lX,SR=0x%08X\n", pc, instrucao, getRegisterBigger(z), getRegisterBigger(x), getRegisterBigger(z), getRegisterBigger(y), xyl + 1, tmpSll_1, R[31]);
            printf("0x%08X:\t%-25s\t%s:%s=%s:%s<<%u=0x%016lX,SR=0x%08X\n", pc, instrucao, getRegisterBigger(z), getRegisterBigger(x), getRegisterBigger(z), getRegisterBigger(y), xyl + 1, tmpSll_1, R[31]);
            break;


            // muls
          case 0b010:

            pc = R[29];
            xyl = R[28] & 0b11111;
            z = (R[28] & (0b11111 << 21)) >> 21;
            x = (R[28] & (0b11111 << 16)) >> 16;
            y = (R[28] & (0b11111 << 11)) >> 11;

            // R[l] : R[z] = R[x] * R[y]
            tmpMuls_1 = (((uint64_t)R[xyl] << 32) | (uint64_t)R[z]);
            tmpMuls_1 = (uint64_t)(R[x] * R[y]);

            verifyZero(xyl, 1, tmpMuls_1);
            verifyZero(z, 1, tmpMuls_1);

            R[xyl] = (uint32_t)setRegistrador(xyl, (tmpMuls_1 >> 32) & 0xFFFFFFFF);

            R[z] = (uint32_t)setRegistrador(z, (tmpMuls_1) & 0xFFFFFFFF);
            

            //zn rlrz = 0
            if ((tmpMuls_1) != 0) {
              R[31] = R[31] & ~0b1000000;
            } else {
              R[31] = R[31] | 0b1000000; 
            }

            //ov rl != 0 0b1000;
            if (R[xyl] != 0) {
              R[31] = R[31] | 0b1000;
            } else {
              R[31] = R[31] & ~0b1000;
            }


           // 0x????????:	muls rl,rz,rx,ry  Rl:Rz=Rx*Ry=0x????????????????,SR=0x????????
            sprintf(instrucao, "muls %s,%s,%s,%s", getRegisterSmaller(xyl), getRegisterSmaller(z), getRegisterSmaller(x), getRegisterSmaller(y));
            fprintf(output, "0x%08X:\t%-25s\t%s:%s=%s*%s=0x%016lX,SR=0x%08X\n", pc, instrucao, getRegisterBigger(xyl), getRegisterBigger(z), getRegisterBigger(x), getRegisterBigger(y), tmpMuls_1, R[31]);
            printf("0x%08X:\t%-25s\t%s:%s=%s*%s=0x%016lX,SR=0x%08X\n", pc, instrucao, getRegisterBigger(xyl), getRegisterBigger(z), getRegisterBigger(x), getRegisterBigger(y), tmpMuls_1, R[31]);
            break;
        

          // div
          case 0b100:

            cr = R[26]; 
            ipc = R[27];
            pc = R[29];
            xyl = R[28] & 0b11111;
            z = (R[28] & (0b11111 << 21)) >> 21;
            x = (R[28] & (0b11111 << 16)) >> 16;
            y = (R[28] & (0b11111 << 11)) >> 11;

            // ZD ry = 0
            if (y == 0) {
              R[31] = R[31] | 0b100000;
              if (bitIE(R[31], true)) {
                  pilhaISR(MEM32, R[26], R[27], R[29], R[30], R[31], &novo_PC, &novo_SP, &novo_CR, &novo_IPC);
                  R[26] = 0;
                  R[27] = R[29];
                  R[29] = 0x00000008;

                  // ZN rz = 0
                  if (R[z] == 0) {
                    (bitZN(R[31], true));
                  }

                  // CY rl != 0
                  if (R[xyl] != 0) {
                    (bitCY(R[31], true));
                  }

                  //0x????????:	div rl,rz,rx,ry          	Rl=Rx%Ry=0x????????,Rz=Rx/Ry=0x????????,SR=0x????????
                  sprintf(instrucao, "div %s,%s,%s,%s", getRegisterSmaller(xyl), getRegisterSmaller(z), getRegisterSmaller(x), getRegisterSmaller(y));
                  fprintf(output, "0x%08X:\t%-25s\t%s=%s%%%s=0x%08X,%s=%s/%s=0x%08X,SR=0x%08X\n", pc, instrucao, getRegisterBigger(xyl), getRegisterBigger(x), getRegisterBigger(y), R[xyl], getRegisterBigger(z), getRegisterBigger(x), getRegisterBigger(y), R[z], R[31]);
                  printf("0x%08X:\t%-25s\t%s=%s%%%s=0x%08X,%s=%s/%s=0x%08X,SR=0x%08X\n", pc, instrucao, getRegisterBigger(xyl), getRegisterBigger(x), getRegisterBigger(y), R[xyl], getRegisterBigger(z), getRegisterBigger(x), getRegisterBigger(y), R[z], R[31]);
                  printf("[SOFTWARE INTERRUPTION]\n");
                  fprintf(output, "[SOFTWARE INTERRUPTION]\n");
                  R[29] = R[29] - 4;
                  break;
              }

            } else {

              R[xyl] = ((int)R[x] % R[y]); 
              R[z] = ((int)(R[x] / R[y]));

              // ZN rz = 0
              if (R[z] != 0) {
                R[31] = R[31] & ~0b1000000;
              } else {
                R[31] = R[31] | 0b1000000;
              }

              // ZD ry = 0
              if (R[y] != 0) {
                R[31] = R[31] & ~0b100000;
              } else {
                R[31] = R[31] | 0b100000;
              }

              // CY rl != 0
              if (R[xyl] != 0) {
                R[31] = R[31] | 0b1;
              } else {
                R[31] = R[31] & ~0b1;
              }


              sprintf(instrucao, "div %s,%s,%s,%s", getRegisterSmaller(xyl), getRegisterSmaller(z), getRegisterSmaller(x), getRegisterSmaller(y));
              fprintf(output, "0x%08X:\t%-25s\t%s=%s%%%s=0x%08X,%s=%s/%s=0x%08X,SR=0x%08X\n", pc, instrucao, getRegisterBigger(xyl), getRegisterBigger(x), getRegisterBigger(y), R[xyl], getRegisterBigger(z), getRegisterBigger(x), getRegisterBigger(y), R[z], R[31]);
              printf("0x%08X:\t%-25s\t%s=%s%%%s=0x%08X,%s=%s/%s=0x%08X,SR=0x%08X\n", pc, instrucao, getRegisterBigger(xyl), getRegisterBigger(x), getRegisterBigger(y), R[xyl], getRegisterBigger(z), getRegisterBigger(x), getRegisterBigger(y), R[z], R[31]);
              break;
            }


            // srl
          case 0b101:
            pc = R[29];
            xyl = R[28] & 0b11111;
            z = (R[28] & (0b11111 << 21)) >> 21;
            x = (R[28] & (0b11111 << 16)) >> 16;
            y = (R[28] & (0b11111 << 11)) >> 11;

            // R[z]:R[x]=(R[z]:R[y])*(2(l + 1))
            tmpSrl_1 = (((uint64_t)R[z] << 32) | (uint64_t)R[y]);
            tmpSrl_1 = tmpSrl_1 << (xyl + 1);

            verifyZero(z, 1, tmpSrl_1);
            verifyZero(x, 1, tmpSrl_1);

            // Extracting the 32 most significant bits
            R[z] = (uint32_t)setRegistrador(z, (tmpSrl_1 >> 32) & 0xFFFFFFFF);
            // Extracting the least significant bits 
            R[x] = (uint32_t)setRegistrador(x, (tmpSrl_1) & 0xFFFFFFFF);

            tmpSrl_1 = R[x] | R[z];

            //zn rz : rx = 0
            if ((tmpSrl_1) != 0) {
              R[31] = R[31] & ~0b1000000;
            } else {
              R[31] = R[31] | 0b1000000;
            }

            //cy rz != 0 
            if (R[z] != 0) {
              R[31] = R[31] | 0b1;
            } else {
              R[31] = R[31] & ~0b1;
            }

            // 0x????????:	srl rz,rx,ry,u // Rz:Rx=Rz:Ry>>u=0x????????????????,SR=0x????????
            sprintf(instrucao, "srl %s,%s,%s,%u", getRegisterSmaller(z), getRegisterSmaller(x), getRegisterSmaller(y), xyl);
            fprintf(output, "0x%08X:\t%-25s\t%s:%s=%s:%s>>%u=0x%016lX,SR=0x%08X\n", pc, instrucao, getRegisterBigger(z), getRegisterBigger(x), getRegisterBigger(z), getRegisterBigger(y), xyl + 1, tmpSrl_1, R[31]);
            printf("0x%08X:\t%-25s\t%s:%s=%s:%s>>%u=0x%016lX,SR=0x%08X\n", pc, instrucao, getRegisterBigger(z), getRegisterBigger(x), getRegisterBigger(z), getRegisterBigger(y), xyl + 1, tmpSrl_1, R[31]);
            break;  


          // divs
          case 0b110:
            pc = R[29];
            xyl = R[28] & 0b11111;
            z = (R[28] & (0b11111 << 21)) >> 21;
            x = (R[28] & (0b11111 << 16)) >> 16;
            y = (R[28] & (0b11111 << 11)) >> 11;

        
            // ZD ry = 0
            if (y == 0) {
              R[31] = R[31] | 0b100000;
              if (bitIE(R[31], true)) {
                pilhaISR(MEM32, R[26], R[27], R[29], R[30], R[31], &novo_PC, &novo_SP, &novo_CR, &novo_IPC);
                R[26] = 0;
                R[27] = R[29];
                R[29] = 0x00000008;

                // ZN rz = 0
                if (R[z] == 0) {
                  (bitZN(R[31], true));
                }

                // OV rl != 0
                if (R[xyl] != 0) {
                  (bitOV(R[31], true));
                }

                // 0x????????:	divs rl,rz,rx,ry // Rl=Rx%Ry=0x????????,Rz=Rx/Ry=0x????????,SR=0x????????
                sprintf(instrucao, "divs %s,%s,%s,%s", getRegisterSmaller(xyl), getRegisterSmaller(z), getRegisterSmaller(x), getRegisterSmaller(y));
                fprintf(output, "0x%08X:\t%-25s\t%s=%s%%%s=0x%08X,%s=%s/%s=0x%08X,SR=0x%08X\n", pc, instrucao, getRegisterBigger(xyl), getRegisterBigger(x), getRegisterBigger(y), R[xyl], getRegisterBigger(z), getRegisterBigger(x), getRegisterBigger(y), R[z], R[31]);
                printf("0x%08X:\t%-25s\t%s=%s%%%s=0x%08X,%s=%s/%s=0x%08X,SR=0x%08X\n", pc, instrucao, getRegisterBigger(xyl), getRegisterBigger(x), getRegisterBigger(y), R[xyl], getRegisterBigger(z), getRegisterBigger(x), getRegisterBigger(y), R[z], R[31]);
                printf("[SOFTWARE INTERRUPTION]\n");
                fprintf(output, "[SOFTWARE INTERRUPTION]\n");
                R[29] = R[29] - 4;
                break;
              }  
            } else {

              R[xyl] = ((int)R[x] % R[y]); 
              R[z] = ((int)(R[x] / R[y]));

              // ZN rz = 0
              if (R[z] != 0) {
                R[31] = R[31] & ~0b1000000;
              } else {
                R[31] = R[31] | 0b1000000;
              }

              // ZD ry = 0
              if (R[y] != 0) {
                R[31] = R[31] & ~0b100000;
              } else {
                R[31] = R[31] | 0b100000;
              }

              // OV rl != 0
              if (R[xyl] != 0) {
                R[31] = R[31] | 0b1000;
              } else {
                R[31] = R[31] & ~0b1000;
              }


              sprintf(instrucao, "divs %s,%s,%s,%s", getRegisterSmaller(xyl), getRegisterSmaller(z), getRegisterSmaller(x), getRegisterSmaller(y));
              fprintf(output, "0x%08X:\t%-25s\t%s=%s%%%s=0x%08X,%s=%s/%s=0x%08X,SR=0x%08X\n", pc, instrucao, getRegisterBigger(xyl), getRegisterBigger(x), getRegisterBigger(y), R[xyl], getRegisterBigger(z), getRegisterBigger(x), getRegisterBigger(y), R[z], R[31]);
              printf("0x%08X:\t%-25s\t%s=%s%%%s=0x%08X,%s=%s/%s=0x%08X,SR=0x%08X\n", pc, instrucao, getRegisterBigger(xyl), getRegisterBigger(x), getRegisterBigger(y), R[xyl], getRegisterBigger(z), getRegisterBigger(x), getRegisterBigger(y), R[z], R[31]);
              break;
            }



          // sra
          case 0b111:

            pc = R[29];
            i = R[28] & 0b11111;
            z = (R[28] & (0b11111 << 21)) >> 21;
            x = (R[28] & (0b11111 << 16)) >> 16;
            y = (R[28] & (0b11111 << 11)) >> 11;

            // R[z]:R[x]=(R[z]:R[y])*(2(l + 1))
            tmpSra_1 = (extendTo64(R[z]) | extendTo64(R[y]));
            tmpSra_1 = (tmpSra_1 >> (i + 1));

            R[x] = (uint32_t)setRegistrador(x, (tmpSra_1) & 0xFFFFFFFF);
            R[z] = (uint32_t)setRegistrador(z, (tmpSra_1 >> 32) & 0xFFFFFFFF);
             
            tmpSra_1 = R[x] | R[z];

            //zn rz : rx = 0
            if ((tmpSra_1) != 0) {
              R[31] = R[31] & ~0b1000000;
            } else {
              R[31] = R[31] | 0b1000000;
            }

            //ov rz != 0 
            if (R[z] != 0) {
              R[31] = R[31] | 0b1000;
            } else {
              R[31] = R[31] & ~0b1000;
            }
            
           
            // 0x????????:	sra rz,rx,ry,u // Rz:Ry=Rz:Rx>>u=0x????????????????,SR=0x????????
            sprintf(instrucao, "sra %s,%s,%s,%u", getRegisterSmaller(z), getRegisterSmaller(x), getRegisterSmaller(y), i);
            fprintf(output, "0x%08X:\t%-25s\t%s:%s=%s:%s>>%u=0x%016lX,SR=0x%08X\n", pc, instrucao, getRegisterBigger(z), getRegisterBigger(y), getRegisterBigger(z), getRegisterBigger(x), i + 1, tmpSra_1, R[31]);
            printf("0x%08X:\t%-25s\t%s:%s=%s:%s>>%u=0x%016lX,SR=0x%08X\n", pc, instrucao, getRegisterBigger(z), getRegisterBigger(y), getRegisterBigger(z), getRegisterBigger(x), i + 1, tmpSra_1, R[31]);
            break;  


          default:

            pc = R[29]; 
            fprintf(output, "[INVALID INSTRUCTION @ 0x%08X]\n", pc);
            printf("[INVALID INSTRUCTION @ 0x%08X]\n", pc);
            printf("[SOFTWARE INTERRUPTION]\n");
            fprintf(output, "[SOFTWARE INTERRUPTION]\n");

            pilhaISR(MEM32, R[26], R[27], R[29], R[30], R[31], &novo_PC, &novo_SP, &novo_CR, &novo_IPC);
            (bitIV(R[31], true));
            R[26] = (R[28] & (0b111111 << 26)) >> 26; 
            R[27] = R[29];
            R[29] = 0x00000004;
            R[29] = R[29] - 4;
            break;

        }
        break;  


      // sub (tipe U)
      case 0b000011:

        pc = R[29];
        z = (R[28] & (0b11111 << 21)) >> 21;
        x = (R[28] & (0b11111 << 16)) >> 16;
        y = (R[28] & (0b11111 << 11)) >> 11;

        R[z] = (R[x] - R[y]);

        //zn rz = 0
        if (R[z] != 0) {
          R[31] = R[31] & ~0b1000000;
        } else {
          R[31] = R[31] | 0b1000000;
        }

        //sn rz31 = 1
        if ((checkBit64(R[z], 31)) != 0) {
          R[31] = R[31] | 0b10000;
        } else {
          R[31] = R[31] & ~0b10000;
        }

        //ov
        if ((checkBit64(R[x], 31) != checkBit64(R[y], 31)) && (checkBit64(R[z], 31) != checkBit64(R[x], 31))) {
          R[31] = R[31] | 0b1000;
        } else {
            R[31] = R[31] & ~0b1000;
        }

        //cy rz32 = 1
        if ((checkBit64(R[z], 32)) != 0) {
          R[31] = R[31] | 0b1;
        } else {
          R[31] = R[31] & ~0b1;
        }

       
      // 0x????????: sub rz,pc,ry  Rz=PC-Ry=0x????????,SR=0x????????
        sprintf(instrucao, "sub %s,%s,%s", getRegisterSmaller(z), getRegisterSmaller(x), getRegisterSmaller(y));
        fprintf(output, "0x%08X:\t%-25s\t%s=%s-%s=0x%08X,SR=0x%08X\n", pc, instrucao, getRegisterBigger(z), getRegisterBigger(x), getRegisterBigger(y), R[z], R[31]);
        printf("0x%08X:\t%-25s\t%s=%s-%s=0x%08X,SR=0x%08X\n", pc, instrucao, getRegisterBigger(z), getRegisterBigger(x), getRegisterBigger(y), R[z], R[31]);


        break;



    // cmp
      case 0b000101:

        pc = R[29];
        x = (R[28] & (0b11111 << 16)) >> 16;
        y = (R[28] & (0b11111 << 11)) >> 11;
        cmp1 = R[x] - R[y];

        //zn cmp = 0
        if ((cmp1) != 0) {
          R[31] = R[31] & ~0b1000000;
        } else {
          R[31] = R[31] | 0b1000000;
        }

        //sn cmp31 = 1
        if ((checkBit64(cmp1, 31)) != 0) {
          R[31] = R[31] | 0b10000;
        } else {
          R[31] = R[31] & ~0b10000;
        }

        //ov rx31 != ry31 ^ cmp31 != rx31
        if ((checkBit64(R[x], 31) != checkBit64(R[y], 31)) && (checkBit64(cmp1, 31) != checkBit64(R[x], 31))) {
          R[31] = R[31] | 0b1000;
        } else {
            R[31] = R[31] & ~0b1000;
        }

        //cy cmp32 = 1
        if ((checkBit64(cmp1, 32)) != 0) {
          R[31] = R[31] | 0b1;
        } else {
          R[31] = R[31] & ~0b1;
        }

      // 0x????????:	cmp rx,sr                	SR=0x????????
        sprintf(instrucao, "cmp %s,%s", getRegisterSmaller(x), getRegisterSmaller(y));
        fprintf(output, "0x%08X:\t%-25s\tSR=0x%08X\n", pc, instrucao, R[31]);
        printf("0x%08X:\t%-25s\tSR=0x%08X\n", pc, instrucao, R[31]);
        break;


    // and
      case 0b000110:

        pc = R[29];
        z = (R[28] & (0b11111 << 21)) >> 21;
        x = (R[28] & (0b11111 << 16)) >> 16;
        y = (R[28] & (0b11111 << 11)) >> 11;
        R[z] = R[x] & R[y];

        //zn rz = 0
        if (R[z] != 0) {
          R[31] = R[31] & ~0b1000000;
        } else {
          R[31] = R[31] | 0b1000000; 
        }

        //sn rz31 = 1
        if (checkBit64(R[z], 31) != 0) {
          R[31] = R[31] | 0b10000;
        } else {
          R[31] = R[31] & ~0b10000;
        }


      // 0x????????:	and rz,rx,ry Rz=Rx&Ry=0x????????,SR=0x????????
        sprintf(instrucao, "and %s,%s,%s", getRegisterSmaller(z), getRegisterSmaller(x), getRegisterSmaller(y));
        fprintf(output, "0x%08X:\t%-25s\t%s=%s&%s=0x%08X,SR=0x%08X\n", pc, instrucao, getRegisterBigger(z), getRegisterBigger(x), getRegisterBigger(y), R[z], R[31]);
        printf("0x%08X:\t%-25s\t%s=%s&%s=0x%08X,SR=0x%08X\n", pc, instrucao, getRegisterBigger(z), getRegisterBigger(x), getRegisterBigger(y), R[z], R[31]);
        break;

    // or
      case 0b000111:

        pc = R[29];
        z = (R[28] & (0b11111 << 21)) >> 21;
        x = (R[28] & (0b11111 << 16)) >> 16;
        y = (R[28] & (0b11111 << 11)) >> 11;
        R[z] = R[x] | R[y];

        
        //zn rz = 0
        if (R[z] != 0) {
          R[31] = R[31] & ~0b1000000;
        } else {
          R[31] = R[31] | 0b1000000; 
        }

        //sn rz31 = 1
        if (checkBit64(R[z], 31) != 0) {
          R[31] = R[31] | 0b10000;
        } else {
          R[31] = R[31] & ~0b10000;
        }


      // 0x????????:	or rz,rx,ry Rz=Rx|Ry=0x????????,SR=0x????????
        sprintf(instrucao, "or %s,%s,%s", getRegisterSmaller(z), getRegisterSmaller(x), getRegisterSmaller(y));
        fprintf(output, "0x%08X:\t%-25s\t%s=%s|%s=0x%08X,SR=0x%08X\n", pc, instrucao, getRegisterBigger(z), getRegisterBigger(x), getRegisterBigger(y), R[z], R[31]);
        printf("0x%08X:\t%-25s\t%s=%s|%s=0x%08X,SR=0x%08X\n", pc, instrucao, getRegisterBigger(z), getRegisterBigger(x), getRegisterBigger(y), R[z], R[31]);
        break;

    // not
      case 0b001000:

        pc = R[29];
        z = (R[28] & (0b11111 << 21)) >> 21;
        x = (R[28] & (0b11111 << 16)) >> 16;
        y = (R[28] & (0b11111 << 11)) >> 11;
        R[z] = ~R[x];

        //zn rz = 0
        if (R[z] != 0) {
          R[31] = R[31] & ~0b1000000;
        } else {
          R[31] = R[31] | 0b1000000; 
        }

        //sn rz31 = 1
        if (checkBit64(R[z], 31) != 0) {
          R[31] = R[31] | 0b10000;
        } else {
          R[31] = R[31] & ~0b10000;
        }


      // 0x????????:	not rz,rx Rz=~Ry=0x????????,SR=0x????????
        sprintf(instrucao, "not %s,%s", getRegisterSmaller(z), getRegisterSmaller(x));
        fprintf(output, "0x%08X:\t%-25s\t%s=~%s=0x%08X,SR=0x%08X\n", pc, instrucao, getRegisterBigger(z), getRegisterBigger(x), R[z], R[31]);
        printf("0x%08X:\t%-25s\t%s=~%s=0x%08X,SR=0x%08X\n", pc, instrucao, getRegisterBigger(z), getRegisterBigger(x), R[z], R[31]);
        break;


     // xor
      case 0b001001:

        pc = R[29];
        z = (R[28] & (0b11111 << 21)) >> 21;
        x = (R[28] & (0b11111 << 16)) >> 16;
        y = (R[28] & (0b11111 << 11)) >> 11;

        R[z] = R[x] && R[y];

        //zn rz = 0
        if (R[z] != 0) {
          R[31] = R[31] & ~0b1000000;
        } else {
          R[31] = R[31] | 0b1000000; 
        }

        //sn rz31 = 1
        if (checkBit64(R[z], 31) != 0) {
          R[31] = R[31] | 0b10000;
        } else {
          R[31] = R[31] & ~0b10000;
        }


      // 0x????????:	xor rz,rx,ry             	Rz=Rx^Ry=0x????????,SR=0x????????
        sprintf(instrucao, "xor %s,%s,%s", getRegisterSmaller(z), getRegisterSmaller(x), getRegisterSmaller(y));
        fprintf(output, "0x%08X:\t%-25s\t%s=%s^%s=0x%08X,SR=0x%08X\n", pc, instrucao, getRegisterBigger(z), getRegisterBigger(x), getRegisterBigger(y), R[z], R[31]);
        printf("0x%08X:\t%-25s\t%s=%s^%s=0x%08X,SR=0x%08X\n", pc, instrucao, getRegisterBigger(z), getRegisterBigger(x), getRegisterBigger(y), R[z], R[31]);
        break;


     //addi
      case 0b010010:

        pc = R[29];
        z = (R[28] & (0b11111 << 21)) >> 21;
        x = (R[28] & (0b11111 << 16)) >> 16;
        i = R[28] & 0xFFFF; 

        R[z] = (R[x] + ExtendedBit15To32(i));

        //zn rz = 0
        if (R[z] != 0) {
          R[31] = R[31] & ~0b1000000;
        } else {
          R[31] = R[31] | 0b1000000;
        }

        //sn rz31 = 1
        if ((checkBit64(R[z], 31)) != 0) {
          R[31] = R[31] | 0b10000;
        } else {
          R[31] = R[31] & ~0b10000;
        }

        //ov rx31 = i15 ^ rz31 != rx31
        if ((checkBit64(R[x], 31) == checkBit64(i, 15)) && (checkBit64(R[z], 31) != checkBit64(R[x], 31))) {
          R[31] = R[31] | 0b1000;
        } else {
            R[31] = R[31] & ~0b1000;
        }

        //cy rz32 = 1
        if ((checkBit64(R[z], 32)) != 0) {
          R[31] = R[31] | 0b1;
        } else {
          R[31] = R[31] & ~0b1;
        }


      //0x????????:	addi rz,rx,s             	Rz=Rx+0x????????=0x????????,SR=0x????????
        sprintf(instrucao, "addi %s,%s,%i", getRegisterSmaller(z), getRegisterSmaller(x), ExtendedBit15To32(i));
        fprintf(output, "0x%08X:\t%-25s\t%s=%s+0x%08X=0x%08X,SR=0x%08X\n", R[29], instrucao, getRegisterBigger(z), getRegisterBigger(x), ExtendedBit15To32(i), R[z], R[31]);
        printf("0x%08X:\t%-25s\t%s=%s+0x%08X=0x%08X,SR=0x%08X\n", R[29], instrucao, getRegisterBigger(z), getRegisterBigger(x), ExtendedBit15To32(i), R[z], R[31]);
        break;


     //subi
      case 0b010011: 

        pc = R[29];
        z = (R[28] & (0b11111 << 21)) >> 21;
        x = (R[28] & (0b11111 << 16)) >> 16;
        i = R[28] & 0xFFFF;

       
        R[z] = (R[x] - ExtendedBit15To32(i));
        
        //zn rz = 0
        if (R[z] != 0) {
          R[31] = R[31] & ~0b1000000;
        } else {
          R[31] = R[31] | 0b1000000;
        }

        //sn rz31 = 1
        if ((checkBit64(R[z], 31)) != 0) {
          R[31] = R[31] | 0b10000;
        } else {
          R[31] = R[31] & ~0b10000;
        }

        //ov rx31 != i15 ^ rz31 != rx31
        if ((checkBit64(R[x], 31) != checkBit64(i, 15)) && (checkBit64(R[z], 31) != checkBit64(R[x], 31))) {
          R[31] = R[31] | 0b1000;
        } else {
            R[31] = R[31] & ~0b1000;
        }

        //cy rz32 = 1
        if ((checkBit64(R[z], 32)) != 0) {
          R[31] = R[31] | 0b1;
        } else {
          R[31] = R[31] & ~0b1;
        }


      //0x????????:	subi rz,rx,s             	Rz=Rx-0x????????=0x????????,SR=0x????????  
        sprintf(instrucao, "subi %s,%s,%i", getRegisterSmaller(z), getRegisterSmaller(x), ExtendedBit15To32(i));
        fprintf(output, "0x%08X:\t%-25s\t%s=%s-0x%08X=0x%08X,SR=0x%08X\n", R[29], instrucao, getRegisterBigger(z), getRegisterBigger(x), ExtendedBit15To32(i), R[z], R[31]);
        printf("0x%08X:\t%-25s\t%s=%s-0x%08X=0x%08X,SR=0x%08X\n", R[29], instrucao, getRegisterBigger(z), getRegisterBigger(x), ExtendedBit15To32(i), R[z], R[31]);
        break;


     //muli 
      case 0b010100:

        pc = R[29];
        z = (R[28] & (0b11111 << 21)) >> 21;
        x = (R[28] & (0b11111 << 16)) >> 16; 
        i = R[28] & 0xFFFF;
        R[z] = ((R[x]) * (ExtendedBit15To32(i)));
        
        //zn rz = 0
        if (R[z] != 0) {
          R[31] = R[31] & ~0b1000000;
        } else {
          R[31] = R[31] | 0b1000000;
        }

        //ov 
        if ((R[z] > 0XFFFFFFFF) != 0) {
          R[31] = R[31] | 0b1000;
        } else {
          R[31] = R[31] & ~0b1000;
        }

      //0x????????:	muli rz,rx,s             	Rz=Rx*0x????????=0x????????,SR=0x????????  
        sprintf(instrucao, "muli %s,%s,%i", getRegisterSmaller(z), getRegisterSmaller(x), ExtendedBit15To32(i));
        fprintf(output, "0x%08X:\t%-25s\t%s=%s*0x%08X=0x%08X,SR=0x%08X\n", R[29], instrucao, getRegisterBigger(z), getRegisterBigger(x), ExtendedBit15To32(i), R[z], R[31]);
        printf("0x%08X:\t%-25s\t%s=%s*0x%08X=0x%08X,SR=0x%08X\n", R[29], instrucao, getRegisterBigger(z), getRegisterBigger(x), ExtendedBit15To32(i), R[z], R[31]);
        break;


     //divi  
      case 0b010101:

        pc = R[29];
        z = (R[28] & (0b11111 << 21)) >> 21;
        x = (R[28] & (0b11111 << 16)) >> 16; 
        i = R[28] & 0xFFFF;

        // ZD i = 0
        if (i == 0) {
          R[31] = R[31] | 0b100000;
          if (bitIE(R[31], true)) {
            pilhaISR(MEM32, R[26], R[27], R[29], R[30], R[31], &novo_PC, &novo_SP, &novo_CR, &novo_IPC);
            R[26] = 0;
            R[27] = R[29];
            R[29] = 0x00000008;

            // ZN rz = 0
            if (R[z] != 0) {
              R[31] = R[31] & ~0b1000000;
            } else {
              R[31] = R[31] | 0b1000000;
            }

            // OV rl != 0
            R[31] = R[31] & ~0b1000;
            

            //0x????????:	divi rz,rx,s             	Rz=Rx/0x????????=0x????????,SR=0x????????
            sprintf(instrucao, "divi %s,%s,%i", getRegisterSmaller(z), getRegisterSmaller(x), ExtendedBit15To32(i));
            fprintf(output, "0x%08X:\t%-25s\t%s=%s/0x%08X=0x%08X,SR=0x%08X\n", R[29], instrucao, getRegisterBigger(z), getRegisterBigger(x), ExtendedBit15To32(i), R[z], R[31]);
            printf("0x%08X:\t%-25s\t%s=%s/0x%08X=0x%08X,SR=0x%08X\n", R[29], instrucao, getRegisterBigger(z), getRegisterBigger(x), ExtendedBit15To32(i), R[z], R[31]);
            printf("[SOFTWARE INTERRUPTION]\n");
            fprintf(output, "[SOFTWARE INTERRUPTION]\n");
            R[29] = R[29] - 4;
            break;

          }  
        } else {

          R[z] = (R[x] / ExtendedBit15To32(i));

          // ZN rz = 0
          if (R[z] != 0) {
            R[31] = R[31] & ~0b1000000;
          } else {
            R[31] = R[31] | 0b1000000;
          }

          // ZD i = 0
          if (ExtendedBit15To32(i) != 0) {
            R[31] = R[31] & ~0b100000;
          } else {
            R[31] = R[31] | 0b100000;
          }

          // OV rl != 0
          R[31] = R[31] & ~0b1000;


          //0x????????:	divi rz,rx,s             	Rz=Rx/0x????????=0x????????,SR=0x????????
          sprintf(instrucao, "divi %s,%s,%i", getRegisterSmaller(z), getRegisterSmaller(x), ExtendedBit15To32(i));
          fprintf(output, "0x%08X:\t%-25s\t%s=%s/0x%08X=0x%08X,SR=0x%08X\n", R[29], instrucao, getRegisterBigger(z), getRegisterBigger(x), ExtendedBit15To32(i), R[z], R[31]);
          printf("0x%08X:\t%-25s\t%s=%s/0x%08X=0x%08X,SR=0x%08X\n", R[29], instrucao, getRegisterBigger(z), getRegisterBigger(x), ExtendedBit15To32(i), R[z], R[31]);
          break;
        }


     //modi
      case 0b010110:

        pc = R[29];
        z = (R[28] & (0b11111 << 21)) >> 21;
        x = (R[28] & (0b11111 << 16)) >> 16; 
        i = R[28] & 0xFFFF;
       
        R[z] = (R[x] % ExtendedBit15To32(i));

      // ZN rz = 0
        if (R[z] != 0) {
          R[31] = R[31] & ~0b1000000;
        } else {
            R[31] = R[31] | 0b1000000;
        }

        // ZD i = 0

        if (ExtendedBit15To32(i) != 0) {
              R[31] = R[31] & ~0b100000;
            } else if (ExtendedBit15To32(i) == 0 || R[31] & 0b10) {
              R[31] = R[31] | 0b100000;
              R[29] = 0x00000008;
              R[26] = 0;
              R[27] = R[29];
            } else {
              R[31] = R[31] | 0b100000;
            }

        if (ExtendedBit15To32(i) != 0) {
          R[31] = R[31] & ~0b100000;
        } else {
            R[31] = R[31] | 0b100000;
        }

        // ov rl != 0
        R[31] = R[31] & ~0b1000;


      //0x????????:	modi rz,rx,s             	Rz=Rx%0x????????=0x????????,SR=0x????????
        sprintf(instrucao, "modi %s,%s,%i", getRegisterSmaller(z), getRegisterSmaller(x), ExtendedBit15To32(i));
        fprintf(output, "0x%08X:\t%-25s\t%s=%s%%0x%08X=0x%08X,SR=0x%08X\n", R[29], instrucao, getRegisterBigger(z), getRegisterBigger(x), ExtendedBit15To32(i), R[z], R[31]);
        printf("0x%08X:\t%-25s\t%s=%s%%0x%08X=0x%08X,SR=0x%08X\n", R[29], instrucao, getRegisterBigger(z), getRegisterBigger(x), ExtendedBit15To32(i), R[z], R[31]);
        break;


     //cmpi
      case 0b010111:

        pc = R[29];
        x = (R[28] & (0b11111 << 16)) >> 16; 
        i = R[28] & 0xFFFF;

        cmpi1 = ((uint64_t)R[x] - (uint64_t)(ExtendedBit15To32(i)));

        //zn cmp = 0
        if ((cmpi1) != 0) {
          R[31] = R[31] & ~0b1000000;
        } else {
          R[31] = R[31] | 0b1000000;
        }

        //sn cmp31 = 1
        if ((checkBit64(cmpi1, 31)) != 0) {
          R[31] = R[31] | 0b10000;
        } else {
          R[31] = R[31] & ~0b10000;
        }
        
        //ov rx31 != i31 ^ cmp31 != rx31
        if ((checkBit64(R[x], 31) != checkBit64(i, 15)) && (checkBit64(cmpi1, 31) != checkBit64(R[x], 31))) {
          R[31] = R[31] | 0b1000;
        } else {
            R[31] = R[31] & ~0b1000;
        }
      
        //cy cmpi32 = 1
        if ((checkBit64(cmpi1, 32)) != 0) {
          R[31] = R[31] | 0b1;
        } else {
          R[31] = R[31] & ~0b1;
        }
      

      //0x????????:	cmpi rx,s                	SR=0x???????? 
        sprintf(instrucao, "cmpi %s,%i", getRegisterSmaller(x), ExtendedBit15To32(i));
        fprintf(output, "0x%08X:\t%-25s\tSR=0x%08X\n", pc, instrucao, R[31]);
        printf("0x%08X:\t%-25s\tSR=0x%08X\n", pc, instrucao, R[31]);
        break; 


    // l8
      case 0b011000:

        pc = R[29];
        z = (R[28] & (0b11111 << 21)) >> 21;
        x = (R[28] & (0b11111 << 16)) >> 16;
        i = R[28] & 0xFFFF;

        addr = R[x] + i;
        shift = (8 * (3 - (addr % 4)));  


        /*default:
          R[z] = ((MEM32[addr >> 2]) & (0xFF << (shift))) >> shift; 
          break; */

        sprintf(instrucao, "l8 %s,[%s%s%i]", getRegisterSmaller(z), getRegisterSmaller(x), (i >= 0) ? ("+") : (""), i);
        fprintf(output, "0x%08X:\t%-25s\t%s=MEM[0x%08X]=0x%02X\n", pc, instrucao, getRegisterBigger(z), addr, R[z]);
        printf("0x%08X:\t%-25s\t%s=MEM[0x%08X]=0x%02X\n", pc, instrucao, getRegisterBigger(z), addr, R[z]);
        break;


      //l16
      case 0b011001:

        z = (R[28] & (0b11111 << 21)) >> 21;
        x = (R[28] & (0b11111 << 16)) >> 16;
        i = R[28] & 0xFFFF;

        addr = R[x] + i;
        shift = (16 * (1 - (addr % 2))); 
        R[z] = ((MEM32[addr >> 1]) & (0xFFFF << (shift))) >> shift; 


      //0x????????:	l16 rz,[rx+-s]           	Rz=MEM[0x????????]=0x????
        sprintf(instrucao, "l16 r%u,[r%u%s%i]", z, x, (i >= 0) ? ("+") : (""), i);
        fprintf(output, "0x%08X:\t%-25s\tR%u=MEM[0x%08X]=0x%04X\n", R[29], instrucao, z, (R[x] + i) << 1, R[z]);
        printf("0x%08X:\t%-25s\tR%u=MEM[0x%08X]=0x%04X\n", R[29], instrucao, z, (R[x] + i) << 1, R[z]);
        break;


      // l32
      case 0b011010:
        
        z = (R[28] & (0b11111 << 21)) >> 21;
        x = (R[28] & (0b11111 << 16)) >> 16;
        i = R[28] & 0xFFFF;
        
        //R[z] = MEM32[R[x] + ExtendedBit15To32(i)]; 
        if  (R[x] + ExtendedBit15To32(i) == 0x20202020) {
          R[z] = watchdog;
        } else {
          R[z] = MEM32[R[x] + ExtendedBit15To32(i)];
        }


        sprintf(instrucao, "l32 r%u,[r%u%s%i]", z, x, (i >= 0) ? ("+") : (""), i);
        fprintf(output, "0x%08X:\t%-25s\tR%u=MEM[0x%08X]=0x%08X\n", R[29], instrucao, z, (R[x] + i) << 2, R[z]);
        printf("0x%08X:\t%-25s\tR%u=MEM[0x%08X]=0x%08X\n", R[29], instrucao, z, (R[x] + i) << 2, R[z]);
        break;


      // s8
      case 0b011011:

        z = (R[28] & (0b11111 << 21)) >> 21;
        x = (R[28] & (0b11111 << 16)) >> 16;
        i = R[28] & 0xFFFF;
        pc = R[29];

        addr = R[x] + i;
        shift = (8 * (3 - (addr % 4))); 


		switch (addr) {
          // FPU
          case 0x80808880:
            fpuX = R[z];
            break;

          case 0x80808884:
            fpuY = R[z];
            break;

          case 0x80808888:
            fpuZ = R[z];
            break;

          case 0x8080888F:

            fpuStatusAndOperation = R[z];
            fpuOperation = fpuStatusAndOperation & 0b11111;
            fpuLastOperation = fpuOperation;

            if (fpuOperation >= 0b1 && fpuOperation <= 0b100) {      //estiver entre 1 e 4, então o número de ciclos é calculado chamando a função calcular n° ciclos
              numberOfCycles = calcularNumeroDeCiclos(fpuX, fpuY);
            } else {
              numberOfCycles = 1;
            }
            break;

          case 0x88888888:
            //Terminal

            terminal = R[z];
            if (count == caps) {
                caps = caps << 1;
                teste = (char*)(realloc(teste, caps));
            }
            teste[count] = terminal;
            count++;
			break;


		  default:

            shift = (8 * (3 - (addr % 4))); 
            break;
		}


        //0x????????:	s8 [rx+-s],rz            	MEM[0x????????]=Rz=0x??
        sprintf(instrucao, "s8 [r%u%s%i],%s", x, (i >= 0) ? ("+") : (""), i, getRegisterSmaller(z));
        fprintf(output, "0x%08X:\t%-25s\tMEM[0x%08X]=%s=0x%02X\n", pc, instrucao, R[x] + i, getRegisterBigger(z), R[z]);
        printf("0x%08X:\t%-25s\tMEM[0x%08X]=%s=0x%02X\n", pc, instrucao, R[x] + i, getRegisterBigger(z), R[z]);
        break;


      //s16
      case 0b011100:

        z = (R[28] & (0b11111 << 21)) >> 21;
        x = (R[28] & (0b11111 << 16)) >> 16;
        i = R[28] & 0xFFFF;
        pc = R[29];

        addr = R[x] + i;
        shift = (16 * (1 - (addr % 2))); 

        R[z] = (((0xFFFF << (shift))) >> shift) & (MEM32[addr >> 1]); 


      //0x????????:	s16 [rx+-s],rz           	//MEM[0x????????]=Rz=0x????
        sprintf(instrucao, "s16 [%s%s%i],%s", getRegisterSmaller(x), (i >= 0) ? ("+") : (""), i, getRegisterSmaller(z));
        fprintf(output, "0x%08X:\t%-25s\tMEM[0x%08X]=%s=0x%04X\n", pc, instrucao, (R[x] + i) << 1, getRegisterBigger(z), R[z]);
        printf("0x%08X:\t%-25s\tMEM[0x%08X]=%s=0x%04X\n", pc, instrucao, (R[x] + i) << 1, getRegisterBigger(z), R[z]);
        break;


      //s32
      case 0b011101:

        z = (R[28] & (0b11111 << 21)) >> 21;
        x = (R[28] & (0b11111 << 16)) >> 16; 
        i = R[28] & 0xFFFF;  
        pc = R[29];
        

        addr = ((R[x] + ExtendedBit15To32(i)) << 2);

        switch (addr) {
        // FPU
        case 0x80808880:
          fpuX = R[z];
          break;

        case 0x80808884:
          fpuY = R[z];
          break;

        case 0x80808888:
          fpuZ = R[z];
          break;

        case 0x8080888C:
          fpuStatusAndOperation = R[z];
          fpuLastOperation = fpuStatusAndOperation & 0b11111;
          fpuLastOperation = fpuOperation;
          if (fpuOperation >= 0b1 && fpuOperation <= 0b00100) {
            numberOfCycles = calcularNumeroDeCiclos(fpuX, fpuY);
          } else {
            // Invalid instruction
            numberOfCycles = 1;
          }

          break;

          // Watchdog
        case 0x80808080:
          watchdog = R[z];
          counter = (0x7FFFFFFF & watchdog);
          break;
          
        default:
          // Regular memory
          MEM32[addr >> 2] = R[z];
          break;
        }
               

        //0x????????:	s32 [rx+-s],rz           	MEM[0x????????]=Rz=0x???????? 
        sprintf(instrucao, "s32 [r%u%s%i],%s", x, (i >= 0) ? ("+") : (""), i, getRegisterSmaller(z));
        fprintf(output, "0x%08X:\t%-25s\tMEM[0x%08X]=%s=0x%08X\n", pc, instrucao, (R[x] + i) << 2, getRegisterBigger(z), R[z]);
        printf("0x%08X:\t%-25s\tMEM[0x%08X]=%s=0x%08X\n", pc, instrucao, (R[x] + i) << 2, getRegisterBigger(z), R[z]);
        break;


      //bae
      case 0b101010:

        pc = R[29];
        i = R[28] & 0x3FFFFFF;
        temp = ExtendedBit25To32(i);

        // cy = 0
        if (setCY(R[31]) == 0) {
          R[29] = R[29] + (temp << 2);
          //R[29] -= 4;
        } 

        sprintf(instrucao, "bae %i", temp);
        fprintf(output, "0x%08X:\t%-25s\tPC=0x%08X\n", pc, instrucao, R[29] + 4);
        printf("0x%08X:\t%-25s\tPC=0x%08X\n", pc, instrucao, R[29] + 4);
        break;


      //bat
      case 0b101011:  

        pc = R[29];
        i = R[28] & 0x3FFFFFF;
        temp = ExtendedBit25To32(i);

        // zn = 0 ^ cy = 0
        if (((setZN(R[31])) == 0) && (setCY(R[31])) == 0) {
          R[29] = R[29] + (temp << 2);
          //R[29] -= 4;
        } 

        sprintf(instrucao, "bat %i", temp);
        fprintf(output, "0x%08X:\t%-25s\tPC=0x%08X\n", pc, instrucao, R[29] + 4);
        printf("0x%08X:\t%-25s\tPC=0x%08X\n", pc, instrucao, R[29] + 4);
        break;


      //bbe
      case 0b101100:

        pc = R[29];
        i = R[28] & 0x3FFFFFF;
        temp = ExtendedBit25To32(i);

        //zn = 1 v cy = 1
        if (((setZN(R[31])) != 0) || ((setCY(R[31])) != 0)) {
          R[29] = R[29] + (temp << 2);
          //R[29] -= 4;
        } 

        sprintf(instrucao, "bbe %i", temp);
        fprintf(output, "0x%08X:\t%-25s\tPC=0x%08X\n", pc, instrucao, R[29] + 4);
        printf("0x%08X:\t%-25s\tPC=0x%08X\n", pc, instrucao, R[29] + 4);
        break;


      //bbt
      case 0b101101:

        pc = R[29];
        i = R[28] & 0x3FFFFFF;
        temp = ExtendedBit25To32(i);

        // cy = 1
        if ((setCY(R[31])) != 0) {
          R[29] = R[29] + (temp << 2);
          //R[29] -= 4;
        } 

        sprintf(instrucao, "bbt %i", temp);
        fprintf(output, "0x%08X:\t%-25s\tPC=0x%08X\n", pc, instrucao, R[29] + 4);
        printf("0x%08X:\t%-25s\tPC=0x%08X\n", pc, instrucao, R[29] + 4);
        break;  


      //beq
      case 0b101110:

        pc = R[29];
        i = R[28] & 0x3FFFFFF;
        temp = ExtendedBit25To32(i);


        // zn = 1
        if ((setZN(R[31])) != 0) {
          R[29] = R[29] + (temp << 2);
          //R[29] -= 4;
        } 

        sprintf(instrucao, "beq %i", temp);
        fprintf(output, "0x%08X:\t%-25s\tPC=0x%08X\n", pc, instrucao, R[29] + 4);
        printf("0x%08X:\t%-25s\tPC=0x%08X\n", pc, instrucao, R[29] + 4);
        break;  


      //bge
      case 0b101111:

        pc = R[29];
        i = R[28] & 0x3FFFFFF;
        temp = ExtendedBit25To32(i);

        // sn = ov
        if ((setSN(R[31])) == (setOV(R[31]))) {
          R[29] = R[29] + (temp << 2);
          //R[29] -= 4;
        } 

        sprintf(instrucao, "bge %i", temp);
        fprintf(output, "0x%08X:\t%-25s\tPC=0x%08X\n", pc, instrucao, R[29] + 4);
        printf("0x%08X:\t%-25s\tPC=0x%08X\n", pc, instrucao, R[29] + 4);
        break;  


      //bgt
      case 0b110000:

        pc = R[29];
        i = R[28] & 0x3FFFFFF;
        temp = ExtendedBit25To32(i);

        // ZN = 0 ^ SN = OV
        if (((setZN(R[31])) == 0) && ((setSN(R[31])) == (setOV(R[31])))) {
          R[29] = R[29] + (temp << 2);
          //R[29] -= 4;
        } 

        sprintf(instrucao, "bgt %i", temp);
        fprintf(output, "0x%08X:\t%-25s\tPC=0x%08X\n", pc, instrucao, R[29] + 4);
        printf("0x%08X:\t%-25s\tPC=0x%08X\n", pc, instrucao, R[29] + 4);
        break;  


      //biv
      case 0b110001:

        pc = R[29];
        i = R[28] & 0x3FFFFFF;
        temp = ExtendedBit25To32(i);

        // IV = 1   
        if (setIV(R[31]) != 0) {
          R[29] = R[29] + (temp << 2);
        }

        sprintf(instrucao, "biv %i", temp);
        fprintf(output, "0x%08X:\t%-25s\tPC=0x%08X\n", pc, instrucao, R[29] + 4);
        printf("0x%08X:\t%-25s\tPC=0x%08X\n", pc, instrucao, R[29] + 4);
        break;  


      //ble
      case 0b110010:

        pc = R[29];
        i = R[28] & 0x3FFFFFF;
        temp = ExtendedBit25To32(i);

        // ZN = 1 v SN != OV
        if (((setZN(R[31])) != 0) || ((setSN(R[31])) != (setOV(R[31])))) {
          R[29] = R[29] + (temp << 2);
          //R[29] -= 4;
        } 

        sprintf(instrucao, "ble %i", temp);
        fprintf(output, "0x%08X:\t%-25s\tPC=0x%08X\n", pc, instrucao, R[29] + 4);
        printf("0x%08X:\t%-25s\tPC=0x%08X\n", pc, instrucao, R[29] + 4);
        break; 


      //blt
      case 0b110011:

        pc = R[29];
        i = R[28] & 0x3FFFFFF;
        temp = ExtendedBit25To32(i);

        // SN != OV
        if ((setSN(R[31])) != (setOV(R[31]))) {
          R[29] = R[29] + (temp << 2);
          //R[29] -= 4;
        } 

        sprintf(instrucao, "blt %i", temp);
        fprintf(output, "0x%08X:\t%-25s\tPC=0x%08X\n", pc, instrucao, R[29] + 4);
        printf("0x%08X:\t%-25s\tPC=0x%08X\n", pc, instrucao, R[29] + 4);
        break;   


      //bne
      case 0b110100:

        pc = R[29];
        i = R[28] & 0x3FFFFFF;
        temp = ExtendedBit25To32(i);

        //printf("bneee %d\n", checkBit32(R[31], 6));

        // zn = 0
        if (setZN(R[31]) == 0) {
          R[29] = R[29] + (temp << 2);
          //R[29] -= 4;
        }

        sprintf(instrucao, "bne %i", temp);
        fprintf(output, "0x%08X:\t%-25s\tPC=0x%08X\n", pc, instrucao, R[29] + 4);
        printf("0x%08X:\t%-25s\tPC=0x%08X\n", pc, instrucao, R[29] + 4);
        break;  


      //bni
      case 0b110101:

        pc = R[29];
        i = R[28] & 0x3FFFFFF;
        temp = ExtendedBit25To32(i);

        // iv = 0
        if ((setIV(R[31])) == 0) {
          R[29] = R[29] + (temp << 2);
        } 

        sprintf(instrucao, "bni %i", temp);
        fprintf(output, "0x%08X:\t%-25s\tPC=0x%08X\n", pc, instrucao, R[29] + 4);
        printf("0x%08X:\t%-25s\tPC=0x%08X\n", pc, instrucao, R[29] + 4);
        break;  


      //bnz
      case 0b110110:

        pc = R[29];
        i = R[28] & 0x3FFFFFF;
        temp = ExtendedBit25To32(i);

        // zd = 0
        if ((setZD(R[31])) == 0) {
          R[29] = R[29] + (temp << 2);
          //R[29] -= 4;
        } 

        sprintf(instrucao, "bnz %i", temp);
        fprintf(output, "0x%08X:\t%-25s\tPC=0x%08X\n", pc, instrucao, R[29] + 4);
        printf("0x%08X:\t%-25s\tPC=0x%08X\n", pc, instrucao, R[29] + 4);
        break;


      // bun desvio incondicional
      case 0b110111:

        
        pc = R[29];
        i = R[28] & 0x3FFFFFF;
        temp = ExtendedBit25To32(i);

        R[29] = R[29] + (temp << 2); 
        //R[29] -= 4;

      //0x????????:	bun s                    	PC=0x????????
        sprintf(instrucao, "bun %i", temp);
        fprintf(output, "0x%08X:\t%-25s\tPC=0x%08X\n", pc, instrucao, R[29] + 4);
        printf("0x%08X:\t%-25s\tPC=0x%08X\n", pc, instrucao, R[29] + 4);
        break;


      //bzd
      case 0b111000:

        pc = R[29];
        i = R[28] & 0x3FFFFFF;
        temp = ExtendedBit25To32(i);

        //ZD = 1
        if (setZD(R[31]) != 0) {
          R[29] = R[29] + (temp << 2);
          //R[29] -= 4;
        }

        sprintf(instrucao, "bzd %i", temp);
        fprintf(output, "0x%08X:\t%-25s\tPC=0x%08X\n", pc, instrucao, R[29] + 4);
        printf("0x%08X:\t%-25s\tPC=0x%08X\n", pc, instrucao, R[29] + 4);
        break;


      //call tipe F
      case 0b011110:

        pc = R[29];
        sp = R[30];
        x = (R[28] & (0b11111 << 16)) >> 16;
        i = R[28] & 0xFFFF;
        temp = ExtendedBit15To32(i);

        R[29] = (R[x] + temp);
        R[29] -= 4;
        

        //0x????????:	call [rx+-s]             	PC=0x????????,MEM[0x????????]=0x????????  
        sprintf(instrucao, "call [r%u%s%i]", x, (i >= 0) ? ("+") : (""), temp);
        fprintf(output, "0x%08X:\t%-25s\tPC=0x%08X,MEM[0x%08X]=0x%08X\n", pc, instrucao, R[29] + 4, sp, MEM32[sp >> 2]);
        printf("0x%08X:\t%-25s\tPC=0x%08X,MEM[0x%08X]=0x%08X\n", pc, instrucao, R[29] + 4, sp, MEM32[sp >> 2]);
        break;


      //call tipe S
      case 0b111001:

        sp = R[30];
        pc = R[29];
        i = R[28] & 0x3FFFFFF;
        temp = ExtendedBit25To32(i);


        MEM32[R[30] >> 2] = R[29] + 4;  //armazena pc + 4 na memória, em uma posição
        R[30] = R[30] - 4;              //determinada pelo valor de sp dividido por 4.
        R[29] = R[29] + (temp << 2); 
        

        //0x????????:	call s                   	PC=0x????????,MEM[0x????????]=0x????????
        sprintf(instrucao, "call %i", temp);
        fprintf(output, "0x%08X:\t%-25s\tPC=0x%08X,MEM[0x%08X]=0x%08X\n", pc, instrucao, R[29] + 4, sp, MEM32[sp >> 2]);
        printf("0x%08X:\t%-25s\tPC=0x%08X,MEM[0x%08X]=0x%08X\n", pc, instrucao, R[29] + 4, sp, MEM32[sp >> 2]);
        break;  


      //ret
      case 0b011111:

        sp = R[30];
        pc = R[29];

        //sp = sp + 4, pc = memsp

        R[30] = R[30] + 4;
        R[29] = MEM32[R[30] >> 2];
        R[29] -= 4;

        //0x????????:	ret                      	PC=MEM[0x????????]=0x????????
        sprintf(instrucao, "ret");
        fprintf(output, "0x%08X:\t%-25s\tPC=MEM[0x%08X]=0x%08X\n", pc, instrucao, R[30], R[29] + 4);
        printf("0x%08X:\t%-25s\tPC=MEM[0x%08X]=0x%08X\n", pc, instrucao, R[30], R[29] + 4);
        break;  


    //reti
      case 0b100000:

        pc = R[29];
        //pilhaISR(MEM32, R[26], R[27], R[29], R[30], R[31], &novo_PC, &novo_SP, &novo_CR, &novo_IPC);

        novo_SP = novo_SP + 4;
        novo_IPC = MEM32[novo_SP >> 2];
        novo_SP = novo_SP + 4;
        novo_CR = MEM32[novo_SP >> 2];
        novo_SP = novo_SP + 4;
        novo_PC = MEM32[novo_SP >> 2]; 

        //reti 	
        //IPC=MEM[0x????????]=0x????????,CR=MEM[0x????????]=0x????????,PC=MEM[0x????????]=0x????????
        sprintf(instrucao, "reti");
        fprintf(output, "0x%08X:\t%-25s\tIPC=MEM[0x%08X]=0x%08X,CR=MEM[0x%08X]=0x%08X,PC=MEM[0x%08X]=0x%08X\n", pc, instrucao, novo_SP - 8, novo_IPC, novo_SP - 4, novo_CR, novo_SP, novo_PC);
        printf("0x%08X:\t%-25s\tIPC=MEM[0x%08X]=0x%08X,CR=MEM[0x%08X]=0x%08X,PC=MEM[0x%08X]=0x%08X\n", pc, instrucao, novo_SP - 8, novo_IPC, novo_SP - 4, novo_CR, novo_SP, novo_PC);
        R[26] = novo_CR;
        R[27] = novo_IPC;
        R[29] = novo_PC - 4;
        R[30] = novo_SP;
        break; 


    // int
      case 0b111111:

        pc = R[29];
        i = R[28] & 0x3FFFFFF;
        
        if (i == 0) {
            executa = 0;
            sprintf(instrucao, "int 0");
            fprintf(output, "0x%08X:\t%-25s\tCR=0x00000000,PC=0x00000000\n", pc, instrucao);
            printf("0x%08X:\t%-25s\tCR=0x00000000,PC=0x00000000\n", pc, instrucao);
        } else {
            pilhaISR(MEM32, R[26], R[27], R[29], R[30], R[31], &novo_PC, &novo_SP, &novo_CR, &novo_IPC);
            R[26] = i;
            R[27] = R[29];
            R[29] = 0x0000000C;

            sprintf(instrucao, "int %u", i);
            fprintf(output, "0x%08X:\t%-25s\tCR=0x%08X,PC=0x%08X\n", pc, instrucao, R[26], R[29]);
            printf("0x%08X:\t%-25s\tCR=0x%08X,PC=0x%08X\n", pc, instrucao, R[26], R[29]);
            printf("[SOFTWARE INTERRUPTION]\n");
            fprintf(output, "[SOFTWARE INTERRUPTION]\n");
            R[29] = R[29] - 4;
        }
        
        break;  


    // push
      case 0b001010:

        pc = R[29];
        sp = R[30];

        z = (R[28] & (0b11111 << 21)) >> 21;
        x = (R[28] & (0b11111 << 16)) >> 16;
        y = (R[28] & (0b11111 << 11)) >> 11;
        v = (R[28] & (0b11111 << 6)) >> 6; 
        w = R[28] & ((0b11111));           
        i = R[28] & 0xFFFFFFFF;

        uint8_t contador_1 = 0;

      // MEM32[R[30]] = R[i], R[30] = R[30] - 4

        if (v != 0)
        {
          MEM32[R[30] >> 2] = R[v];
          R[30] = R[30] - 4;

          contador_1++;

          if (w != 0)
          {
            MEM32[R[30] >> 2] = R[w];
            R[30] = R[30] - 4;

            contador_1++;

            if (x != 0)
            {
              MEM32[R[30] >> 2] = R[x];
              R[30] = R[30] - 4;

              contador_1++;

              if (y != 0)
              {
                MEM32[R[30] >> 2] = R[y];
                R[30] = R[30] - 4;

                contador_1++;

                if (z != 0)
                {
                  MEM32[R[30] >> 2] = R[z];
                  R[30] = R[30] - 4;

                  contador_1++;

                }
              }
            }
          }
        }

        // 0x????????:	push rv,rw,rx,ry,rz	MEM[0x????????]{0x????????,0x????????,0x????????,0x????????,0x????????}={Rv,Rw,Rx,Ry,Rz}
        if (contador_1 == 1) {
          sprintf(instrucao, "push %s", getRegisterSmaller(v));
          fprintf(output, "0x%08X:\t%-25s\tMEM[0x%08X]{0x%08X}={%s}\n", pc, instrucao, sp, R[v], getRegisterBigger(v));
          printf("0x%08X:\t%-25s\tMEM[0x%08X]{0x%08X}={%s}\n", pc, instrucao, sp, R[v], getRegisterBigger(v));

        } else if (contador_1 == 2) {
          sprintf(instrucao, "push %s,%s", getRegisterSmaller(v), getRegisterSmaller(w));
          fprintf(output, "0x%08X:\t%-25s\tMEM[0x%08X]{0x%08X,0x%08X}={%s,%s}\n", pc, instrucao, sp, R[v], R[w], getRegisterBigger(v), getRegisterBigger(w));
          printf("0x%08X:\t%-25s\tMEM[0x%08X]{0x%08X,0x%08X}={%s,%s}\n", pc, instrucao, sp, R[v], R[w], getRegisterBigger(v), getRegisterBigger(w));

        } else if (contador_1 == 3) {
          sprintf(instrucao, "push %s,%s,%s", getRegisterSmaller(v), getRegisterSmaller(w), getRegisterSmaller(x));
          fprintf(output, "0x%08X:\t%-25s\tMEM[0x%08X]{0x%08X,0x%08X,0x%08X}={%s,%s,%s}\n", pc, instrucao, sp, R[v], R[w], R[x], getRegisterBigger(v), getRegisterBigger(w), getRegisterBigger(x));
          printf("0x%08X:\t%-25s\tMEM[0x%08X]{0x%08X,0x%08X,0x%08X}={%s,%s,%s}\n", pc, instrucao, sp, R[v], R[w], R[x], getRegisterBigger(v), getRegisterBigger(w), getRegisterBigger(x));

        } else if (contador_1 == 4) {
          sprintf(instrucao, "push %s,%s,%s,%s", getRegisterSmaller(v), getRegisterSmaller(w), getRegisterSmaller(x), getRegisterSmaller(y));
          fprintf(output, "0x%08X:\t%-25s\tMEM[0x%08X]{0x%08X,0x%08X,0x%08X,0x%08X}={%s,%s,%s,%s}\n", pc, instrucao, sp, R[v], R[w], R[x], R[y], getRegisterBigger(v), getRegisterBigger(w), getRegisterBigger(x), getRegisterBigger(y));
          printf("0x%08X:\t%-25s\tMEM[0x%08X]{0x%08X,0x%08X,0x%08X,0x%08X}={%s,%s,%s,%s}\n", pc, instrucao, sp, R[v], R[w], R[x], R[y], getRegisterBigger(v), getRegisterBigger(w), getRegisterBigger(x), getRegisterBigger(y));

        } else if (contador_1 == 5) {
          sprintf(instrucao, "push %s,%s,%s,%s,%s", getRegisterSmaller(v), getRegisterSmaller(w), getRegisterSmaller(x), getRegisterSmaller(y), getRegisterSmaller(z));
          fprintf(output, "0x%08X:\t%-25s\tMEM[0x%08X]{0x%08X,0x%08X,0x%08X,0x%08X,0x%08X}={%s,%s,%s,%s,%s}\n", pc, instrucao, sp, R[v], R[w], R[x], R[y], R[z], getRegisterBigger(v), getRegisterBigger(w), getRegisterBigger(x), getRegisterBigger(y), getRegisterBigger(z));
          printf("0x%08X:\t%-25s\tMEM[0x%08X]{0x%08X,0x%08X,0x%08X,0x%08X,0x%08X}={%s,%s,%s,%s,%s}\n", pc, instrucao, sp, R[v], R[w], R[x], R[y], R[z], getRegisterBigger(v), getRegisterBigger(w), getRegisterBigger(x), getRegisterBigger(y), getRegisterBigger(z));

        } else {
          sprintf(instrucao, "push, -");
          fprintf(output, "0x%08x:\t%-25s\tMEM[0x%08X]{}={}", pc, instrucao, sp);
          printf("0x%08x:\t%-25s\tMEM[0x%08X]{}={}", pc, instrucao, sp);
        }
        break;


     // pop
      case 0b001011:

        pc = R[29];
        sp = R[30];

        z = (R[28] & (0b11111 << 21)) >> 21;
        x = (R[28] & (0b11111 << 16)) >> 16;
        y = (R[28] & (0b11111 << 11)) >> 11;
        v = (R[28] & (0b11111 << 6)) >> 6; 
        w = R[28] & ((0b11111));           
        i = R[28] & 0xFFFFFFFF;

        uint8_t contador2 = 0;

        //R[30] = R[30] + 4, R[i] =  MEM32[R[30]]

        if (v != 0)
        {
          R[30] = R[30] + 4;
          R[v] = MEM32[R[30] >> 2];

          contador2++;

          if (w != 0)
          {
            R[30] = R[30] + 4;
            R[w] = MEM32[R[30] >> 2];

            contador2++;

            if (x != 0)
            {
              R[30] = R[30] + 4;
              R[x] = MEM32[R[30] >> 2];

              contador2++;

              if (y != 0)
              {
                R[30] = R[30] + 4;
                R[y] = MEM32[R[30] >> 2];

                contador2++;

                if (z != 0)
                {
                  R[30] = R[30] + 4;
                  R[z] = MEM32[R[30] >> 2];

                  contador2++;

                }
              }
            }
          }
        }

          //0x????????:	pop rv,rw,rx,ry,rz 	{Rv,Rw,Rx,Ry,Rz}=MEM[0x????????]{0x????????,0x????????,0x????????,0x????????,0x????????}
        if (contador2 == 1) {
          sprintf(instrucao, "pop %s", getRegisterSmaller(v));
          fprintf(output, "0x%08X:\t%-25s\t{%s}=MEM[0x%08X]{0x%08X}\n", pc, instrucao, getRegisterBigger(v), sp, R[v]);
          printf("0x%08X:\t%-25s\t{%s}=MEM[0x%08X]{0x%08X}\n", pc, instrucao, getRegisterBigger(v), sp, R[v]);

        } else if (contador2 == 2) {
          sprintf(instrucao, "pop %s,%s", getRegisterSmaller(v), getRegisterSmaller(w));
          fprintf(output, "0x%08X:\t%-25s\t{%s,%s}=MEM[0x%08X]{0x%08X,0x%08X}\n", pc, instrucao, getRegisterBigger(v), getRegisterBigger(w), sp, R[v], R[w]);
          printf("0x%08X:\t%-25s\t{%s,%s}=MEM[0x%08X]{0x%08X,0x%08X}\n", pc, instrucao, getRegisterBigger(v), getRegisterBigger(w), sp, R[v], R[w]);

        } else if (contador2 == 3) {
          sprintf(instrucao, "pop %s,%s,%s", getRegisterSmaller(v), getRegisterSmaller(w), getRegisterSmaller(x));
          fprintf(output, "0x%08X:\t%-25s\t{%s,%s,%s}=MEM[0x%08X]{0x%08X,0x%08X,0x%08X}\n", pc, instrucao, getRegisterBigger(v), getRegisterBigger(w), getRegisterBigger(x), sp, R[v], R[w], R[x]);
          printf("0x%08X:\t%-25s\t{%s,%s,%s}=MEM[0x%08X]{0x%08X,0x%08X,0x%08X}\n", pc, instrucao, getRegisterBigger(v), getRegisterBigger(w), getRegisterBigger(x), sp, R[v], R[w], R[x]);

        } else if (contador2 == 4) {
          sprintf(instrucao, "pop %s,%s,%s,%s", getRegisterSmaller(v), getRegisterSmaller(w), getRegisterSmaller(x), getRegisterSmaller(y));
          fprintf(output, "0x%08X:\t%-25s\t{%s,%s,%s,%s}=MEM[0x%08X]{0x%08X,0x%08X,0x%08X,0x%08X}\n", pc, instrucao, getRegisterBigger(v), getRegisterBigger(w), getRegisterBigger(x), getRegisterBigger(y), sp, R[v], R[w], R[x], R[y]);
          printf("0x%08X:\t%-25s\t{%s,%s,%s,%s}=MEM[0x%08X]{0x%08X,0x%08X,0x%08X,0x%08X}\n", pc, instrucao, getRegisterBigger(v), getRegisterBigger(w), getRegisterBigger(x), getRegisterBigger(y), sp, R[v], R[w], R[x], R[y]);

        } else if (contador2 == 5) {
          sprintf(instrucao, "pop %s,%s,%s,%s,%s", getRegisterSmaller(v), getRegisterSmaller(w), getRegisterSmaller(x), getRegisterSmaller(y), getRegisterSmaller(z));
          fprintf(output, "0x%08X:\t%-25s\t{%s,%s,%s,%s,%s}=MEM[0x%08X]{0x%08X,0x%08X,0x%08X,0x%08X,0x%08X}\n", pc, instrucao, getRegisterBigger(v), getRegisterBigger(w), getRegisterBigger(x), getRegisterBigger(y), getRegisterBigger(z), sp, R[v], R[w], R[x], R[y], R[z]);
          printf("0x%08X:\t%-25s\t{%s,%s,%s,%s,%s}=MEM[0x%08X]{0x%08X,0x%08X,0x%08X,0x%08X,0x%08X}\n", pc, instrucao, getRegisterBigger(v), getRegisterBigger(w), getRegisterBigger(x), getRegisterBigger(y), getRegisterBigger(z), sp, R[v], R[w], R[x], R[y], R[z]);
        } else {
          sprintf(instrucao, "pop, -");
          fprintf(output, "0x%08x:\t%-25s\t{}=MEM[0x%08X]{}", pc, instrucao, sp);
          printf("0x%08x:\t%-25s\t{}=MEM[0x%08X]{}", pc, instrucao, sp);
        }
        break;	


      case 0b100001:

        z = (R[28] & (0b11111 << 21)) >> 21;
        x = (R[28] & (0b11111 << 16)) >> 16;
        i = R[28] & 0xFFFF;

        if (i == 0) {
          //cbr
          R[z] = R[z] & ~(0b1 << x);
          

        //0x????????:	cbr rz[x]                	Rz=0x????????
          sprintf(instrucao, "cbr %s[%u]", getRegisterSmaller(z), x);
          fprintf(output, "0x%08X:\t%-25s\t%s=0x%08X\n", R[29], instrucao, getRegisterBigger(z), R[z]);
          printf("0x%08X:\t%-25s\t%s=0x%08X\n", R[29], instrucao, getRegisterBigger(z), R[z]);
        } else {
          // sbr
            R[z] = R[z] | (0b1 << x);

        //0x????????:	sbr rz[x]                	Rz=0x????????
          sprintf(instrucao, "sbr %s[%u]", getRegisterSmaller(z), x);
          fprintf(output, "0x%08X:\t%-25s\t%s=0x%08X\n", R[29], instrucao, getRegisterBigger(z), R[z]);
          printf("0x%08X:\t%-25s\t%s=0x%08X\n", R[29], instrucao, getRegisterBigger(z), R[z]);
        }
        break;
          

      default:

        pc = R[29]; 
        fprintf(output, "[INVALID INSTRUCTION @ 0x%08X]\n", pc);
        printf("[INVALID INSTRUCTION @ 0x%08X]\n", pc);
        printf("[SOFTWARE INTERRUPTION]\n");
        fprintf(output, "[SOFTWARE INTERRUPTION]\n");

        pilhaISR(MEM32, R[26], R[27], R[29], R[30], R[31], &novo_PC, &novo_SP, &novo_CR, &novo_IPC);
        (bitIV(R[31], true));
        R[26] = (R[28] & (0b111111 << 26)) >> 26; 
        R[27] = pc;
        R[29] = 0x00000004;
        R[29] = R[29] - 4;
        break;

    } //fim do switch case

    //fpu    shift = (8 * (3 - (addr % 4))); 
    if ((numberOfCycles == 0) && (fpuOperation != 0) && (setIE(R[31]))) {
      fpuStatusAndOperation = 0;
      switch (fpuOperation) {

    //		  0b00001 Adição Z = X + Y
        case 0b1:
          fprintf(output, "[HARDWARE INTERRUPTION 3]\n");
          printf("[HARDWARE INTERRUPTION 3]\n");

          pilhaISR(MEM32, R[26], R[27], R[29], R[30], R[31], &novo_PC, &novo_SP, &novo_CR, &novo_IPC);
          R[27] = pc; //ARMAZENA O ENDEREÇO DA INSTRUÇÃO ONDE A INTERRUPÇÃO FOI GERADA
          R[29] = 0x00000018;
          R[26] = 0x01EEE754; //ARMAZENA O CÓDIGO IDENTIFICADOR DAS INTERRUPÇÕES
         
          fpuZ = fpuX + fpuY;
          shiftIEEE = true;

          
          break;

  //				0b10 Subtração Z = X − Y
        case 0b00010:

          fprintf(output, "[HARDWARE INTERRUPTION 3]\n");
          printf("[HARDWARE INTERRUPTION 3]\n");

          pilhaISR(MEM32, R[26], R[27], R[29], R[30], R[31], &novo_PC, &novo_SP, &novo_CR, &novo_IPC);
          R[27] = pc; //ARMAZENA O ENDEREÇO DA INSTRUÇÃO ONDE A INTERRUPÇÃO FOI GERADA
          R[29] = 0x00000018;
          R[26] = 0x01EEE754; //ARMAZENA O CÓDIGO IDENTIFICADOR DAS INTERRUPÇÕES
          fpuZ = fpuX - fpuY;
          shiftIEEE = true;

          break;

  //				00011 Multiplicação Z = X × Y
        case 0b00011:
          fprintf(output, "[HARDWARE INTERRUPTION 3]\n");
          printf("[HARDWARE INTERRUPTION 3]\n");

          pilhaISR(MEM32, R[26], R[27], R[29], R[30], R[31], &novo_PC, &novo_SP, &novo_CR, &novo_IPC);
          R[27] = pc; //ARMAZENA O ENDEREÇO DA INSTRUÇÃO ONDE A INTERRUPÇÃO FOI GERADA
          R[29] = 0x00000018;
          R[26] = 0x01EEE754; //ARMAZENA O CÓDIGO IDENTIFICADOR DAS INTERRUPÇÕES
          fpuZ = fpuX * fpuY;
          shiftIEEE = true;

          break;

  //				00100 Divisão Z = X ÷ Y
        case 0b00100:

          if (fpuY != 0) {

            fprintf(output, "[HARDWARE INTERRUPTION 3]\n");
            printf("[HARDWARE INTERRUPTION 3]\n");
            pilhaISR(MEM32, R[26], R[27], R[29], R[30], R[31], &novo_PC, &novo_SP, &novo_CR, &novo_IPC);
            R[27] = pc; //ARMAZENA O ENDEREÇO DA INSTRUÇÃO ONDE A INTERRUPÇÃO FOI GERADA
            R[29] = 0x00000018;
            R[26] = 0x01EEE754; //ARMAZENA O CÓDIGO IDENTIFICADOR DAS INTERRUPÇÕES
            fpuZ = fpuX / fpuY;
            shiftIEEE = true;

          } else {

            fprintf(output, "[HARDWARE INTERRUPTION 2]\n");
            printf("[HARDWARE INTERRUPTION 2]\n");

            pilhaISR(MEM32, R[26], R[27], R[29], R[30], R[31], &novo_PC, &novo_SP, &novo_CR, &novo_IPC);
            R[27] = pc; //ARMAZENA O ENDEREÇO DA INSTRUÇÃO ONDE A INTERRUPÇÃO FOI GERADA
            R[29] = 0x00000014;
            R[26] = 0x01EEE754; //ARMAZENA O CÓDIGO IDENTIFICADOR DAS INTERRUPÇÕES
            statusFpu(fpuStatusAndOperation, true);
          }

          break;

  //				0b00101 Atribuição X = Z
        case 0b00101:
          fprintf(output, "[HARDWARE INTERRUPTION 4]\n");
          printf("[HARDWARE INTERRUPTION 4]\n");

          pilhaISR(MEM32, R[26], R[27], R[29], R[30], R[31], &novo_PC, &novo_SP, &novo_CR, &novo_IPC);
          R[27] = pc; //ARMAZENA O ENDEREÇO DA INSTRUÇÃO ONDE A INTERRUPÇÃO FOI GERADA
          R[29] = 0x0000001C;
          R[26] = 0x01EEE754; //ARMAZENA O CÓDIGO IDENTIFICADOR DAS INTERRUPÇÕES
          fpuX = fpuZ;

          shiftIEEEOfX = (shiftIEEE != 0) ? true : false;

          break;

  //				0b00110 Atribuição Y = Z
        case 0b00110:
          fprintf(output, "[HARDWARE INTERRUPTION 4]\n");
          printf("[HARDWARE INTERRUPTION 4]\n");

          pilhaISR(MEM32, R[26], R[27], R[29], R[30], R[31], &novo_PC, &novo_SP, &novo_CR, &novo_IPC);
          R[27] = pc; //ARMAZENA O ENDEREÇO DA INSTRUÇÃO ONDE A INTERRUPÇÃO FOI GERADA
          R[29] = 0x0000001C;
          R[26] = 0x01EEE754; //ARMAZENA O CÓDIGO IDENTIFICADOR DAS INTERRUPÇÕES
          fpuY = fpuZ;

          shiftIEEEOfY = (shiftIEEE != 0) ? true : false;

          break;

  //				0b00111 Teto ⌈Z⌉
        case 0b00111:
          fprintf(output, "[HARDWARE INTERRUPTION 4]\n");
          printf("[HARDWARE INTERRUPTION 4]\n");

          pilhaISR(MEM32, R[26], R[27], R[29], R[30], R[31], &novo_PC, &novo_SP, &novo_CR, &novo_IPC);
          R[27] = pc; //ARMAZENA O ENDEREÇO DA INSTRUÇÃO ONDE A INTERRUPÇÃO FOI GERADA
          R[29] = 0x0000001C;
          R[26] = 0x01EEE754; //ARMAZENA O CÓDIGO IDENTIFICADOR DAS INTERRUPÇÕES
          shiftIEEE = false;

          break;

    //				0b01000 Piso ⌊Z⌋
        case 0b01000:

          fprintf(output, "[HARDWARE INTERRUPTION 4]\n");
          printf("[HARDWARE INTERRUPTION 4]\n");

          pilhaISR(MEM32, R[26], R[27], R[29], R[30], R[31], &novo_PC, &novo_SP, &novo_CR, &novo_IPC);
          R[27] = pc; //ARMAZENA O ENDEREÇO DA INSTRUÇÃO ONDE A INTERRUPÇÃO FOI GERADA
          R[29] = 0x0000001C;
          R[26] = 0x01EEE754; //ARMAZENA O CÓDIGO IDENTIFICADOR DAS INTERRUPÇÕES
          shiftIEEE = false;

          break;

    //				0b01001 Arredondamento ∥Z ∥
        case 0b01001:
          fprintf(output, "[HARDWARE INTERRUPTION 4]\n");
          printf("[HARDWARE INTERRUPTION 4]\n");

          pilhaISR(MEM32, R[26], R[27], R[29], R[30], R[31], &novo_PC, &novo_SP, &novo_CR, &novo_IPC);
          R[27] = pc; //ARMAZENA O ENDEREÇO DA INSTRUÇÃO ONDE A INTERRUPÇÃO FOI GERADA
          R[29] = 0x0000001C;
          R[26] = 0x01EEE754; //ARMAZENA O CÓDIGO IDENTIFICADOR DAS INTERRUPÇÕES
          shiftIEEE = false;
          break;


          // invalid
        default:
          fprintf(output, "[HARDWARE INTERRUPTION 2]\n");
          printf("[HARDWARE INTERRUPTION 2]\n");
          pilhaISR(MEM32, R[26], R[27], R[29], R[30], R[31], &novo_PC, &novo_SP, &novo_CR, &novo_IPC);
          R[27] = pc; //ARMAZENA O ENDEREÇO DA INSTRUÇÃO ONDE A INTERRUPÇÃO FOI GERADA
          R[29] = 0x00000014;
          R[26] = 0x01EEE754; //ARMAZENA O CÓDIGO IDENTIFICADOR DAS INTERRUPÇÕES

          // Error
          statusFpu(fpuStatusAndOperation, true);

          break;

      }
      fpuOperation = 0;
      R[29] = R[29] - 4;
    }

    // Decrementando o número de ciclos
    if (numberOfCycles != 0) {
      numberOfCycles--;
    }

    R[29] = R[29] + 4;
    
  } 

  printf("[TERMINAL]\n");
  fprintf(output, "[TERMINAL]\n");

  for (int i = 0; i < count; i++) {
    fprintf(output, "%c", teste[i]);
    printf("%c", teste[i]);
  }

  printf("\n[END OF SIMULATION]\n");
  fprintf(output, "\n[END OF SIMULATION]\n");
  fclose(input);
  fclose(output);
  return 0;
}
