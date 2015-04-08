//
//  cpu.c
//  chip8
//
//  Created by bajsko on 2015-04-03.
//  Copyright (c) 2015 bajsko. All rights reserved.
//

// OP 0 is bad.

#include "cpu.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

Chip8* loadRom(const char* filename)
{
    //allocate cpu struct memory
    Chip8* cpu = calloc(sizeof(Chip8), 1);
    
    //reset cpu memory
    for(int i = 0; i < 4096; i++)
    {
        cpu->memory[i] = 0;
    }
    
    //reset gfx
    for(int i = 0; i < 2048; i++)
    {
        cpu->gfx[i] = 0;
    }
    
    //reset keyboard, registers and the stack
    for(int i = 0; i < 16; i++)
    {
        cpu->keyboard[i] = 0;
        cpu->V[i] = 0;
        cpu->stack[i] = 0;
    }
    
    //game ROM is stored at offset 0x200
    cpu->pc = 0x200;
    cpu->sp = 0;
    cpu->I = 0;
    
    cpu->status = 0;
    
    cpu->drawFlag = FALSE;
    
    //open ROM using (r)read (b)binary
    FILE *rom = fopen(filename, "rb");
    
    if(!rom)
    {
        printf("ROM could not be loaded..\n");
        return 0;
    }
    
    //get the game size
    fseek(rom, 0L, SEEK_END);
    long length = ftell(rom);
    fseek(rom, 0L, SEEK_SET);
    
    //ROM contents
    byte* buffer = malloc(length);
    fread(buffer, length, 1, rom);

    //write ROM contents to cpu memory
    writemem(cpu, cpu->pc, buffer, length);
    cpu->romSize = length;
    
    //release the buffer
    free(buffer);
    
    //release the ROM file
    fclose(rom);
    
    //Chip 8 internal fontset
	unsigned char fontset[80] ={
		0xF0, 0x90, 0x90, 0x90, 0xF0, // 0
		0x20, 0x60, 0x20, 0x20, 0x70, // 1
		0xF0, 0x10, 0xF0, 0x80, 0xF0, // 2
		0xF0, 0x10, 0xF0, 0x10, 0xF0, // 3
		0x90, 0x90, 0xF0, 0x10, 0x10, // 4
		0xF0, 0x80, 0xF0, 0x10, 0xF0, // 5
		0xF0, 0x80, 0xF0, 0x90, 0xF0, // 6
		0xF0, 0x10, 0x20, 0x40, 0x40, // 7
		0xF0, 0x90, 0xF0, 0x90, 0xF0, // 8
		0xF0, 0x90, 0xF0, 0x10, 0xF0, // 9
		0xF0, 0x90, 0xF0, 0x90, 0x90, // A
		0xE0, 0x90, 0xE0, 0x90, 0xE0, // B
		0xF0, 0x80, 0x80, 0x80, 0xF0, // C
		0xE0, 0x90, 0x90, 0x90, 0xE0, // D
		0xF0, 0x80, 0xF0, 0x80, 0xF0, // E
		0xF0, 0x80, 0xF0, 0x80, 0x80  // F
	};
    
    writemem(cpu, 0, fontset, 80);
    
    printf("CPU loaded and ready\n");
    
    return cpu;
}

int writemem(Chip8* cpu, ushort adress, byte* contents, long length)
{
    if(adress > 4096 || adress < 0)
    {
        printf("writemem: Outside of memory bounds\n");
        return CPU_FAIL;
    }
    
    for(int i = 0; i < length; i++)
    {
        cpu->memory[adress+i] = contents[i];
    }
    printf("writemem: Wrote %ld bytes to memory starting at adress 0x%02X\n", length, adress);
    return 0;
}

int emulate(Chip8* cpu)
{
    //merge 2 bytes into a short
    cpu->opcode = cpu->memory[cpu->pc] << 8 | cpu->memory[cpu->pc + 1];
    printf("Executing OPCode: 0x%02X (PC %d)\n", cpu->opcode, cpu->pc);
    
    switch(cpu->opcode & 0xF000)
    {
        case 0x0000:
            OP0(cpu);
            break;
            
        case 0x1000:
            OP1(cpu);
            break;
            
        case 0x2000:
            OP2(cpu);
            break;
            
        case 0x3000:
            OP3(cpu);
            break;
            
        case 0x4000:
            OP4(cpu);
            break;
            
        case 0x5000:
            OP5(cpu);
            break;
            
        case 0x6000:
            OP6(cpu);
            break;
            
        case 0x7000:
            OP7(cpu);
            break;
            
        case 0x8000:
            OP8(cpu);
            break;
            
        case 0x9000:
            OP9(cpu);
            break;
            
        case 0xA000:
            OPA(cpu);
            break;
            
        case 0xB000:
            OPB(cpu);
            break;
            
        case 0xC000:
            OPC(cpu);
            break;
            
        case 0xD000:
            OPD(cpu);
            break;
            
        case 0xE000:
            OPE(cpu);
            break;
            
        case 0xF000:
            OPF(cpu);
            break;
        
        default:
            printf("Unknown OPCode: 0x%02X (pc 0x%02X)\n", cpu->opcode, cpu->pc);
            cpu->status = CPU_FAIL;
            break;
        
    }
    
    return 0;
}

int dumpregisters(Chip8 *cpu)
{
    printf("dumpregisters: Dump data:\n");
    for(int i = 0; i < 16; i++)
        printf("Register %X: %d\n", i, cpu->V[i]);
    
    return 0;
}

int updateTimers(Chip8 *cpu)
{
    if(cpu->delay_timer > 0)
        cpu->delay_timer--;
    
    return 0;
}

int releaseCPU(Chip8* cpu)
{
    free(cpu);
    printf("CPU freed\n");
    
    return 0;
}

void OP0(Chip8 *cpu)
{
    switch (cpu->opcode & 0x000F) {
        case 0x0000:
        {
            for(int i = 0; i < 2048; i++)
            {
                cpu->gfx[i] = 0;
            }
            cpu->pc += 2;
            cpu->drawFlag = TRUE;
        }
        break;
            
        case 0x000E:
        {
            cpu->pc = cpu->stack[cpu->sp];
            cpu->sp--;
            cpu->pc += 2;
        }
        break;
            
        default:
            printf("Unknown OPCode 0x%02X (PC %d)\n", cpu->opcode, cpu->pc);
            cpu->status = CPU_FAIL;
            break;
    }
}

void OP1(Chip8 *cpu)
{
    ushort adress = cpu->opcode & 0x0FFF;
    cpu->pc = adress;
}

void OP2(Chip8 *cpu)
{
    ushort adress = cpu->opcode & 0x0FFF;
    
    cpu->sp++;
    cpu->stack[cpu->sp] = cpu->pc;
    cpu->pc = adress;
}
void OP3(Chip8 *cpu)
{
    ushort x = (cpu->opcode & 0x0F00) >> 8;
    if(cpu->V[x] == (cpu->opcode & 0x00FF))
        cpu->pc += 4;
    else
        cpu->pc += 2;
}
void OP4(Chip8 *cpu)
{
    ushort x = (cpu->opcode & 0x0F00) >> 8;
    
    if(cpu->V[x] != (cpu->opcode & 0x00FF))
        cpu->pc += 4;
    else
        cpu->pc += 2;
}
void OP5(Chip8 *cpu)
{
    ushort x = (cpu->opcode & 0x0F00) >> 8;
    ushort y = (cpu->opcode & 0x00F0) >> 4;
    
    if(cpu->V[x] == cpu->V[y])
        cpu->pc += 4;
    else
        cpu->pc += 2;
    
}
void OP6(Chip8 *cpu)
{
    ushort x = (cpu->opcode & 0x0F00) >> 8;
    ushort constant = cpu->opcode & 0x00FF;
    
    cpu->V[x] = constant;
    
    cpu->pc += 2;
}
void OP7(Chip8 *cpu)
{
    ushort x = (cpu->opcode & 0x0F00) >> 8;
    ushort constant = cpu->opcode & 0x00FF;
    
    cpu->V[x] += constant;
    
    cpu->pc += 2;
}
void OP8(Chip8 *cpu)
{
    ushort x = (cpu->opcode & 0x0F00) >> 8;
    ushort y = (cpu->opcode & 0x00F0) >> 4;
    
    switch (cpu->opcode & 0x000F) {
        case 0x0000:
            cpu->V[x] = cpu->V[y];
            cpu->pc += 2;
            break;
        
        case 0x0001:
            cpu->V[x] |= cpu->V[y];
            cpu->pc += 2;
            break;
            
        case 0x0002:
            cpu->V[x] &= cpu->V[y];
            cpu->pc += 2;
            break;
            
        case 0x0003:
            cpu->V[x] ^= cpu->V[y];
            cpu->pc += 2;
            break;
            
        case 0x0004:
            
            if((cpu->V[x] + cpu->V[y]) > 0xFF)
                cpu->V[0xF] = 1;
            else
                cpu->V[0xF] = 0;
            
            cpu->V[x] += cpu->V[y];
            cpu->pc += 2;
            break;
            
        case 0x0005:
            
            if(cpu->V[y] > cpu->V[x])
                cpu->V[0xF] = 0;
            else
                cpu->V[0xF] = 1;
            
            cpu->V[x] -= cpu->V[y];
            cpu->pc += 2;
            break;
            
        case 0x0006:
        {
            byte lsb = cpu->V[x] & 0x1;
            cpu->V[0xF] = lsb;
            
            cpu->V[x] >>= 1;
            cpu->pc += 2;
            break;
        }
            
        case 0x0007:
            
            if((cpu->V[y] - cpu->V[x]) > cpu->V[x])
                cpu->V[0xF] = 0;
            else
                cpu->V[0xF] = 1;
            
            cpu->V[x] = (cpu->V[y] - cpu->V[x]);
            cpu->pc += 2;
            break;
            
        case 0x000E:
        {
            byte msb = cpu->V[x] >> 7;
            cpu->V[0xF] = msb;
            
            cpu->V[x] <<= 1;
            cpu->pc += 2;
            break;
        }
            
        default:
            printf("Unknown OPcode 0x%02X (PC 0x%02X)\n", cpu->opcode, cpu->pc);
            cpu->status = CPU_FAIL;
            break;
    }

}
void OP9(Chip8 *cpu)
{
    ushort x = (cpu->opcode & 0x0F00) >> 8;
    ushort y = (cpu->opcode & 0x00F0) >> 4;
    
    if(cpu->V[x] != cpu->V[y])
        cpu->pc += 4;
    else
        cpu->pc += 2;
}
void OPA(Chip8 *cpu)
{
    cpu->I = cpu->opcode & 0x0FFF;
    cpu->pc += 2;
}
void OPB(Chip8 *cpu)
{
    ushort adress = cpu->opcode & 0x0FFF;
    cpu->pc = adress + cpu->V[0x0];
}
void OPC(Chip8 *cpu)
{
    ushort x = cpu->opcode & 0x0F00 >> 8;
    ushort constant = cpu->opcode & 0x00FF;
    
    cpu->V[x] = (rand() % 0xFF) & constant;
    cpu->pc += 2;
}
void OPD(Chip8 *cpu)
{
    unsigned short x = cpu->V[(cpu->opcode & 0x0F00) >> 8];
    unsigned short y = cpu->V[(cpu->opcode & 0x00F0) >> 4];
    unsigned short height = (cpu->opcode & 0x000F);
    unsigned short pixel;
    
    cpu->V[0xF] = 0;
    for (int yline = 0; yline < height; yline++)
    {
        pixel = cpu->memory[cpu->I + yline];
        for(int xline = 0; xline < 8; xline++)
        {
            if((pixel & (0x80 >> xline)) != 0)
            {
                if(cpu->gfx[(x + xline + ((y + yline) * 64))] == 1)
                    cpu->V[0xF] = 1;
                cpu->gfx[x + xline + ((y + yline) * 64)] ^= 1;
            }
        }
    }
    
    cpu->pc += 2;
    cpu->drawFlag = TRUE;
}
void OPE(Chip8 *cpu)
{
    ushort x = cpu->opcode & 0x0F00 >> 8;
    
    switch (cpu->opcode & 0x000F) {
        case 0x000E:
            if(cpu->keyboard[cpu->V[x]] != 0)
                cpu->pc += 4;
            else
                cpu->pc += 2;
            break;
            
        case 0x0001:
            if(cpu->keyboard[cpu->V[x]] == 0)
                cpu->pc += 4;
            else
                cpu->pc += 2;
            break;
            
        default:
            printf("Unknown OPcode 0x%02X (PC 0x%02X)\n", cpu->opcode, cpu->pc);
            cpu->status = CPU_FAIL;
            break;
    }
}
void OPF(Chip8 *cpu)
{
    ushort x = cpu->opcode & 0x0F00 >> 8;
    
    switch (cpu->opcode & 0x00FF) {
        case 0x0007:
            cpu->V[x] = cpu->delay_timer;
            cpu->pc += 2;
            break;
            
        case 0x000A:
        {
            int pressed = 0;
            
            for(int i = 0; i < 16; i++)
            {
                if(cpu->keyboard[i] != 0)
                {
                    cpu->V[x] = i;
                    pressed = 1;
                }
            }
            
            if(pressed == 0)
                break;
            
            cpu->pc += 2;
            break;
        }
            
        case 0x0015:
            cpu->delay_timer = cpu->V[x];
            cpu->pc += 2;
            break;
            
        case 0x0018:
            cpu->sound_timer = cpu->V[x];
            cpu->pc += 2;
            break;
            
        case 0x001E:
            cpu->I += cpu->V[x];
            cpu->pc += 2;
            break;
            
        case 0x0029:
            cpu->I = cpu->V[x] * 0x5;
            cpu->pc += 2;
            break;
            
        case 0x0033:
        {
            ushort bcd = cpu->V[x];
            byte hundreds = bcd / 100;
            byte tens = (bcd / 10) % 10;
            byte ones = (bcd % 100) % 10;
            
            cpu->memory[cpu->I] = hundreds;
            cpu->memory[cpu->I+1] = tens;
            cpu->memory[cpu->I+2] = ones;
            
            cpu->pc += 2;
            break;
        }
            
        case 0x0055:
        {
            byte contents[x];
            
            for(int i = 0; i <= x; i++)
                contents[i] = cpu->V[i];
            
            writemem(cpu, cpu->I, contents, x);
            cpu->pc += 2;
            break;
        }
            
        case 0x0065:
        {
            for(int i = 0; i <= x; i++)
                cpu->V[i] = cpu->memory[cpu->I+i];
            
            cpu->I += cpu->V[x] + 1;
            cpu->pc += 2;
            break;
        }
            
        default:
            printf("Unknown OPcode 0x%02X (PC 0x%02X)\n", cpu->opcode, cpu->pc);
            cpu->status = CPU_FAIL;
            break;
    }
}