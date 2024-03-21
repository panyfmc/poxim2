#include <stdbool.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>


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
    // Verifica se bitPosition é inválido (negativo OU maior que 31)
    if (bitPosition < 0 || bitPosition > 31) {
        return -1; // Valor inválido
    }
    // Cria uma máscara para isolar o bit
    uint32_t mask = 1u << bitPosition;
    // Aplica a máscara e verifica se o bit é 0 ou 1
    return (value & mask) != 0;
}


int checkBit64(uint64_t value2, int bitPosition2) {
    if (bitPosition2 < 0 || bitPosition2 > 63) {
        return -1; // Valor inválido
    }
    // Cria uma máscara para isolar o bit
    uint32_t mask2 = 1u << bitPosition2;
    // Aplica a máscara e verifica se o bit é 0 ou 1
    return (value2 & mask2) != 0;
}



uint32_t ExtensaoBit21To32(uint32_t hexa) {
  if (checkBit32(hexa, 20)) {
    return hexa | 0xFFF00000;
  } else {
    return hexa;
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


bool bitZN(uint32_t R) {
  // if checkBit32 is 0 or 1
  return checkBit32(R, 6);
}


// ZD 5
bool bitZD(uint32_t R) {
  return checkBit32(R, 5);
}

// SN 4
bool bitSN(uint32_t R) {
  return checkBit32(R, 4);
}

// OV 3
bool bitOV(uint32_t R) {
  return checkBit32(R, 3);
}

// IV 2
bool bitIV(uint32_t R) {
  return checkBit32(R, 2);
}

// CY 0
bool bitCY(uint32_t R) {
  return checkBit32(R, 0);
}
 


int main(int argc, char *argv[]) { 

  FILE *input = fopen(argv[1], "r");
  FILE *output = fopen(argv[2], "w");

  uint32_t R[32] = { 0 };
  uint32_t watchdog = 0;

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
    uint32_t pc = 0, xyl = 0, sp = 0, tmpSubi = 0, i = 0, ipc = 0, cr = 0, counter = 0, temp = 0, hardwareValue = 0;
    uint64_t tmpSla_1 = 0, tmpSll_1 = 0,tmpSra_1 = 0, tmpSrl_1 = 0, cmp1 = 0, cmpi1 = 0, tmpMul_1 = 0, tmpMuls_1 = 0, tmpAdd_1 = 0;

    R[28] = ((MEM8[R[29] + 0] << 24) | (MEM8[R[29] + 1] << 16) | (MEM8[R[29] + 2] << 8) | (MEM8[R[29] + 3] << 0)) | MEM32[R[29] >> 2];

    uint8_t opcode = (R[28] & (0b111111 << 26)) >> 26;
    uint8_t subcode = (R[28] & (0b111 << 8)) >> 8;


    //watchdog

      //10000000000000000000000000000000 AND ADDRESS OF WACTHDOG TRUE
    if (0x80000000 & watchdog) {
      //couter 0000000000000...001 and address -1 
      counter = (0x7FFFFFFF & watchdog) - 1;
      //address = 00000...001 and address or counter == 0
      watchdog = 0x80000000 & watchdog | counter;
      if(counter == 0) {
        //and IE (bit 1  in status register SR) 
        if (R[31] & 0b10) {
          watchdog = 0;
          printf("[HARDWARE INTERRUPTION 1]\n");
          fprintf(output, "[HARDWARE INTERRUPTION 1]\n");
          R[26] = 0xE1AC04DA;
          R[27] = R[29];
          MEM32[R[30] >> 2] = R[29] + 4, R[30] = R[30] - 4;
          MEM32[R[30] >> 2] = R[26], R[30] = R[30] - 4;
          MEM32[R[30] >> 2] = R[27], R[30] = R[30] - 4;
          R[29] = 0x00000010;
  
          continue;
        }
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
        fprintf(output, "0x%08X:\t%-25s\t%s=0x%08X\n", R[29], instrucao, getRegisterBigger(z), xyl);
        printf("0x%08X:\t%-25s\t%s=0x%08X\n", R[29], instrucao, getRegisterBigger(z), xyl);
        break;


    // movs (tipe U)
      case 0b000001:
        pc = R[29];
        z = (R[28] & (0b11111 << 21)) >> 21;
        x = (R[28] & (0b11111 << 16)) >> 21;
        y = (R[28] & (0b11111 << 11)) >> 11;
        xyl = R[28] & 0x1FFFFF;
        R[z] = ExtensaoBit21To32(xyl);

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

        tmpAdd_1 = (uint64_t)(R[x]) + (uint64_t)(R[y]);
        R[z] = tmpAdd_1;

        //zn rz = 0
        if (tmpAdd_1 != 0) {
          R[31] = R[31] & ~0b1000000;
        } else {
          R[31] = R[31] | 0b1000000;
        }

        //sn rz31 = 1
        if ((checkBit64(tmpAdd_1, 31)) != 0) {
          R[31] = R[31] | 0b10000;
        } else {
          R[31] = R[31] & ~0b10000;
        }

        //ov
        if ((checkBit64(tmpAdd_1, 31) == checkBit64(R[y], 31)) && (checkBit64(tmpAdd_1, 31) != checkBit64(R[x], 31))) {
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

            // Extracting the 32 most significant bits
            R[z] = (uint32_t)setRegistrador(z, (tmpSla_1 >> 32) & 0xFFFFFFFF);
             // Extracting the least significant bits
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
            pc = R[29];
            xyl = R[28] & 0b11111;
            z = (R[28] & (0b11111 << 21)) >> 21;
            x = (R[28] & (0b11111 << 16)) >> 16;
            y = (R[28] & (0b11111 << 11)) >> 11;

            // R[l]= R[x] % R[y], R[z] = R[x] / R[y]
            if (R[y] == 0) {
              R[31] = R[31] | 0b100000;
            } else {
              R[xyl] = ((int)R[x] % R[y]); 
              R[z] = ((int)(R[x] / R[y]));
            }

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

            // 0x????????:	div rl,rz,rx,ry        Rl=Rx%Ry=0x????????,Rz=Rx/Ry=0x????????,SR=0x????????
            sprintf(instrucao, "div %s,%s,%s,%s", getRegisterSmaller(xyl), getRegisterSmaller(z), getRegisterSmaller(x), getRegisterSmaller(y));
            fprintf(output, "0x%08X:\t%-25s\t%s=%s%%%s=0x%08X,%s=%s/%s=0x%08X,SR=0x%08X\n", pc, instrucao, getRegisterBigger(xyl), getRegisterBigger(x), getRegisterBigger(y), R[xyl], getRegisterBigger(z), getRegisterBigger(x), getRegisterBigger(y), R[z], R[31]);
            printf("0x%08X:\t%-25s\t%s=%s%%%s=0x%08X,%s=%s/%s=0x%08X,SR=0x%08X\n", pc, instrucao, getRegisterBigger(xyl), getRegisterBigger(x), getRegisterBigger(y), R[xyl], getRegisterBigger(z), getRegisterBigger(x), getRegisterBigger(y), R[z], R[31]);
            break;


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

            // R[l]= R[x] % R[y], R[z] = R[x] / R[y]
            if (R[y] == 0) {
              R[31] = R[31] | 0b100000;
            } else {
              R[xyl] = ((int)R[x] % R[y]); 
              R[z] = ((int)(R[x] / R[y]));
            }

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

            // ov rl != 0
            if (R[xyl] != 0) {
              R[31] = R[31] | 0b1000;
            } else {
              R[31] = R[31] & ~0b1000;
            }

            // 0x????????:	divs rl,rz,rx,ry // Rl=Rx%Ry=0x????????,Rz=Rx/Ry=0x????????,SR=0x????????
            sprintf(instrucao, "divs %s,%s,%s,%s", getRegisterSmaller(xyl), getRegisterSmaller(z), getRegisterSmaller(x), getRegisterSmaller(y));
            fprintf(output, "0x%08X:\t%-25s\t%s=%s%%%s=0x%08X,%s=%s/%s=0x%08X,SR=0x%08X\n", pc, instrucao, getRegisterBigger(xyl), getRegisterBigger(x), getRegisterBigger(y), R[xyl], getRegisterBigger(z), getRegisterBigger(x), getRegisterBigger(y), R[z], R[31]);
            printf("0x%08X:\t%-25s\t%s=%s%%%s=0x%08X,%s=%s/%s=0x%08X,SR=0x%08X\n", pc, instrucao, getRegisterBigger(xyl), getRegisterBigger(x), getRegisterBigger(y), R[xyl], getRegisterBigger(z), getRegisterBigger(x), getRegisterBigger(y), R[z], R[31]);
            break;


          // sra
          case 0b111:

            pc = R[29];
            i = R[28] & 0b11111;
            z = (R[28] & (0b11111 << 21)) >> 21;
            x = (R[28] & (0b11111 << 16)) >> 16;
            y = (R[28] & (0b11111 << 11)) >> 11;

            //printf("ry = 0x%016X\n", R[y]);

            // R[z]:R[x]=(R[z]:R[y])*(2(l + 1))
            tmpSra_1 = (extendTo64(R[z]) | extendTo64(R[y]));
            tmpSra_1 = (tmpSra_1 >> (i + 1));
            
            //printf("sra = 0x%016lX\n", tmpSra_1);

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
            fprintf(output, "[INVALID INSTRUCTION @ 0x%08X]\n", R[29]);
            printf("[INVALID INSTRUCTION @ 0x%08X]\n", R[29]);
            executa = 0;
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

        R[z] = (R[x] / ExtendedBit15To32(i));
        
      // ZN rz = 0
        if (R[z] != 0) {
          R[31] = R[31] & ~0b1000000;
        } else {
            R[31] = R[31] | 0b1000000;
        }

          // ZD ry = 0
        if (ExtendedBit15To32(i) != 0) {
          R[31] = R[31] & ~0b100000;
        } else {
            R[31] = R[31] | 0b100000;
        }

        // ov rl != 0
        R[31] = R[31] & ~0b1000;
        

      //0x????????:	divi rz,rx,s             	Rz=Rx/0x????????=0x????????,SR=0x????????
        sprintf(instrucao, "divi %s,%s,%i", getRegisterSmaller(z), getRegisterSmaller(x), ExtendedBit15To32(i));
        fprintf(output, "0x%08X:\t%-25s\t%s=%s/0x%08X=0x%08X,SR=0x%08X\n", R[29], instrucao, getRegisterBigger(z), getRegisterBigger(x), ExtendedBit15To32(i), R[z], R[31]);
        printf("0x%08X:\t%-25s\t%s=%s/0x%08X=0x%08X,SR=0x%08X\n", R[29], instrucao, getRegisterBigger(z), getRegisterBigger(x), ExtendedBit15To32(i), R[z], R[31]);
        break;


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

        z = (R[28] & (0b11111 << 21)) >> 21;
        x = (R[28] & (0b11111 << 16)) >> 16;
        i = R[28] & 0xFFFF;
        uint32_t addr = R[x] + i;
        uint32_t shift = (8 * (3 - (addr % 4))); 
        R[z] = ((MEM32[addr >> 2]) & (0xFF << (shift))) >> shift; 
        

        sprintf(instrucao, "l8 r%u,[r%u%s%i]", z, x, (i >= 0) ? ("+") : (""), i);
        fprintf(output, "0x%08X:\t%-25s\tR%u=MEM[0x%08X]=0x%02X\n", R[29], instrucao, z, R[x] + i, R[z]);
        printf("0x%08X:\t%-25s\tR%u=MEM[0x%08X]=0x%02X\n", R[29], instrucao, z, R[x] + i, R[z]);
        break;


      //l16
      case 0b011001:

        z = (R[28] & (0b11111 << 21)) >> 21;
        x = (R[28] & (0b11111 << 16)) >> 16;
        i = R[28] & 0xFFFF;

        uint32_t addr1 = R[x] + i;

        uint32_t shift1 = (16 * (1 - (addr1 % 2))); 

        R[z] = ((MEM32[addr1 >> 1]) & (0xFFFF << (shift1))) >> shift1; 


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


      //s8
      case 0b011011:

        z = (R[28] & (0b11111 << 21)) >> 21;
        x = (R[28] & (0b11111 << 16)) >> 16;
        i = R[28] & 0xFFFF;

        R[z] = (((0xFF << (shift))) >> shift) & (MEM32[addr >> 2]); 


        //0x????????:	s8 [rx+-s],rz            	MEM[0x????????]=Rz=0x??
        sprintf(instrucao, "s8 [r%u%s%i],r%u", x, (i >= 0) ? ("+") : (""), i, z);
        fprintf(output, "0x%08X:\t%-25s\tMEM[0x%08X]=R%u=0x%02X\n", R[29], instrucao, R[x] + i, z, R[z]);
        printf("0x%08X:\t%-25s\tMEM[0x%08X]=R%u=0x%02X\n", R[29], instrucao, R[x] + i, z, R[z]);
        break;


      //s16
      case 0b011100:

        z = (R[28] & (0b11111 << 21)) >> 21;
        x = (R[28] & (0b11111 << 16)) >> 16;
        i = R[28] & 0xFFFF;

        R[z] = (((0xFFFF << (shift1))) >> shift1) & (MEM32[addr1 >> 1]); 



      //0x????????:	s16 [rx+-s],rz           	//MEM[0x????????]=Rz=0x????
        sprintf(instrucao, "s16 [r%u%s%i],r%u", x, (i >= 0) ? ("+") : (""), i, z);
        fprintf(output, "0x%08X:\t%-25s\tMEM[0x%08X]=R%u=0x%04X\n", R[29], instrucao, (R[x] + i) << 1, z, R[z]);
        printf("0x%08X:\t%-25s\tMEM[0x%08X]=R%u=0x%04X\n", R[29], instrucao, (R[x] + i) << 1, z, R[z]);
        break;


      //s32
      case 0b011101:

        z = (R[28] & (0b11111 << 21)) >> 21;
        x = (R[28] & (0b11111 << 16)) >> 16;  
        i = R[28] & 0xFFFF;  

       // R[z] = MEM32[R[x] + ExtendedBit15To32(i)];      
        if  ((R[x] + ExtendedBit15To32(i)) == 0x20202020) {
          watchdog = R[z];
        } else {
          MEM32[R[x] + ExtendedBit15To32(i)] = R[z];
        }
               

        //0x????????:	s32 [rx+-s],rz           	MEM[0x????????]=Rz=0x???????? 
        sprintf(instrucao, "s32 [r%u%s%i],%s", x, (i >= 0) ? ("+") : (""), i, getRegisterSmaller(z));
        fprintf(output, "0x%08X:\t%-25s\tMEM[0x%08X]=%s=0x%08X\n", R[29], instrucao, (R[x] + i) << 2, getRegisterBigger(z), R[z]);
        printf("0x%08X:\t%-25s\tMEM[0x%08X]=%s=0x%08X\n", R[29], instrucao, (R[x] + i) << 2, getRegisterBigger(z), R[z]);
        break;


      //bae
      case 0b101010:

        pc = R[29];
        i = R[28] & 0x3FFFFFF;
        temp = ExtendedBit25To32(i);

        // cy = 0
        if (bitCY(R[31]) == 0) {
          R[29] = R[29] + (temp << 2);
          R[29] -= 4;
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
        if (((bitZN(R[31])) == 0) && ((bitCY(R[31])) == 0)) {
          R[29] = R[29] + (temp << 2);
        } else {
          R[29] = R[29];
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
        if (((bitZN(R[31])) != 0) || ((bitCY(R[31])) != 0)) {
          R[29] = R[29] + (temp << 2);
        } else {
          R[29] = R[29];
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
        if ((bitCY(R[31])) != 0) {
          R[29] = R[29] + (temp << 2);
        } else {
          R[29] = R[29];
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
        if ((bitZN(R[31])) != 0) {
          R[29] = R[29] + (temp << 2);
        } else {
          R[29] = R[29];
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
        if ((bitSN(R[31])) == (bitOV(R[31]))) {
          R[29] = R[29] + (temp << 2);
        } else {
          R[29] = R[29];
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
        if (((bitZN(R[31])) == 0) && ((bitSN(R[31])) == (bitOV(R[31])))) {
          R[29] = R[29] + (temp << 2);
        } else {
          R[29] = R[29];
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

        R[29] = R[29] + (temp << 2);

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
        if (((bitZN(R[31])) != 0) || ((bitSN(R[31])) != (bitOV(R[31])))) {
          R[29] = R[29] + (temp << 2);
        } else {
          R[29] = R[29];
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
        if ((bitSN(R[31])) != (bitOV(R[31]))) {
          R[29] = R[29] + (temp << 2);
        } else {
          R[29] = R[29];
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
        if (bitZN(R[31]) == 0) {
          R[29] = R[29] + (temp << 2);
          R[29] -= 4;
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
        if ((bitIV(R[31])) == 0) {
          R[29] = R[29] + (temp << 2);
        } else {
          R[29] = R[29];
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
        if ((bitZD(R[31])) == 0) {
          R[29] = R[29] + (temp << 2);
        } else {
          R[29] = R[29];
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

        R[29] = R[29] + (temp << 2); 

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



      //call tipe S
      case 0b111001:

        sp = R[30];
        pc = R[29];
        i = R[28] & 0x3FFFFFF;
        temp = ExtendedBit25To32(i);


        MEM32[R[30] >> 2] = R[29] + 4;  //armazena pc + 4 na memória, em uma posição
        R[30] = R[30] - 4;              //determinada pelo valor de sp dividido por 4.
        R[29] = R[29] + (temp << 2); 
        R[29] -= 4;
        

        //0x????????:	call s                   	PC=0x????????,MEM[0x????????]=0x????????
        sprintf(instrucao, "call %i", temp);
        fprintf(output, "0x%08X:\t%-25s\tPC=0x%08X,MEM[0x%08X]=0x%08X\n", pc, instrucao, R[29] + 4, sp, MEM32[sp >> 2]);
        printf("0x%08X:\t%-25s\tPC=0x%08X,MEM[0x%08X]=0x%08X\n", pc, instrucao, R[29] + 4, sp, MEM32[sp >> 2]);
        break;  


      //ret
      case 0b011111:

        sp = R[30];
        pc = R[29];

        R[30] = R[30] + 4;
        MEM32[R[30] >> 2] = R[29];

        //0x????????:	ret                      	PC=MEM[0x????????]=0x????????
        sprintf(instrucao, "ret");
        fprintf(output, "0x%08X:\t%-25s\tPC=MEM[0x%08X]=0x%08X\n", pc, instrucao, sp, MEM32[sp >> 2]);
        printf("0x%08X:\t%-25s\tPC=MEM[0x%08X]=0x%08X\n", pc, instrucao, sp, MEM32[sp >> 2]);
        break;  


    //reti
      case 0b100000:

        cr = R[26];
        ipc = R[27];
        pc = R[29];
        sp = R[30];

        R[30] = R[30] + 4, R[27] = MEM32[R[30] >> 2];
        R[30] = R[30] + 4, R[26] = MEM32[R[30] >> 2];
        R[30] = R[30] + 4, R[29] = MEM32[R[30] >> 2];

        //reti 	
        //IPC=MEM[0x????????]=0x????????,CR=MEM[0x????????]=0x????????,PC=MEM[0x????????]=0x????????
        sprintf(instrucao, "reti");
        fprintf(output, "0x%08X:\t%-25s\tIPC=MEM[0x%08X]=0x%08X,CR=MEM[0x%08X]=0x%08X,PC=MEM[0x%08X]=0x%08X\n", pc, instrucao, sp, R[27], sp, R[26], sp, R[29]);
        printf("0x%08X:\t%-25s\tIPC=MEM[0x%08X]=0x%08X,CR=MEM[0x%08X]=0x%08X,PC=MEM[0x%08X]=0x%08X\n", pc, instrucao, sp, R[27], sp, R[26], sp, R[29]);
        break; 



    // int
      case 0b111111:

        i = R[28] & 0x3FFFFFF;
        cr = R[26];
        ipc = R[27];
        pc = R[29];
        sp = R[30];
        
        if (i == 0) {
          executa = 0;
          R[29] = 0 - 4;
          R[26] = 0; 
          // sprintf(instrucao, "int 0");
          // fprintf(output, "0x%08X:\t%-25s\tCR=0x00000000,PC=0x00000000\n", R[29], instrucao);
          // printf("0x%08X:\t%-25s\tCR=0x00000000,PC=0x00000000\n", R[29], instrucao);
        } else {
          R[26] = i;
          R[27] = R[29];
          R[29] = 0x0000000C - 4;
        }
        sprintf(instrucao, "int %d", i);
        fprintf(output, "0x%08X:\t%-25s\tCR=0x%08X,PC=0x%08X\n", pc, instrucao, R[26], R[29] + 4);
        printf("0x%08X:\t%-25s\tCR=0x%08X,PC=0x%08X\n", pc, instrucao, R[26], R[29] + 4);
        
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
          //___
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
        fprintf(output, "[INVALID INSTRUCTION @ 0x%08X]\n", R[29]);
        printf("[INVALID INSTRUCTION @ 0x%08X]\n", R[29]);
        executa = 0;
        break;


    }

    R[29] = R[29] + 4;
  }

  printf("[END OF SIMULATION]\n");
  fprintf(output, "[END OF SIMULATION]\n");
  fclose(input);
  fclose(output);
  return 0;
}
