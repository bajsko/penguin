//
//  cpu.h
//  chip8
//
//  Created by bajsko on 2015-04-03.
//  Copyright (c) 2015 bajsko. All rights reserved.
//

#ifndef chip8_cpu_h
#define chip8_cpu_h

#define CPU_FAIL 1
#define FALSE 0
#define TRUE 1

//im lazy
typedef unsigned char byte;
typedef unsigned short ushort;

//struct representing the Chip8 CPU
typedef struct Chip8
{
    //storage, 4K max
    byte memory[4096];
    
    //16 registers
    byte V[16];
    
    //graphics, 64*32
    byte gfx[2048];
    
    //the stack, 16 levels
    ushort stack[16];
    
    //keyboard
    byte keyboard[16];
    
    //current opcode
    ushort opcode;
    
    //stack pointer
    ushort sp;
    
    //program counter
    ushort pc;
    
    //index reg
    ushort I;
    
    //delay timer, used for timing
    byte delay_timer;
    
    //sound timer
    byte sound_timer;
    
    //size of ROM (in bytes)
    ushort romSize;
    
    //we good?
    byte status;
    
    int drawFlag;
    
} Chip8;

//loads specified ROM and returns a functional Chip8 struct.
Chip8* loadRom(const char* filename);

//emulate specified CPU (1 cycle)
int emulate(Chip8* cpu);

//writes buffer to memory starting at specified adress
int writemem(Chip8* cpu, ushort adress, byte* contents, long length);

//dumps contents of all registers of specified CPU
int dumpregisters(Chip8 *cpu);

//updates chip8 timers
int updateTimers(Chip8 *cpu);

//releases the CPU
int releaseCPU(Chip8* cpu);

//a bunch of opcodes
void OP0(Chip8 *cpu);
void OP1(Chip8 *cpu);
void OP2(Chip8 *cpu);
void OP3(Chip8 *cpu);
void OP4(Chip8 *cpu);
void OP5(Chip8 *cpu);
void OP6(Chip8 *cpu);
void OP7(Chip8 *cpu);
void OP8(Chip8 *cpu);
void OP9(Chip8 *cpu);
void OPA(Chip8 *cpu);
void OPB(Chip8 *cpu);
void OPC(Chip8 *cpu);
void OPD(Chip8 *cpu);
void OPE(Chip8 *cpu);
void OPF(Chip8 *cpu);


#endif
