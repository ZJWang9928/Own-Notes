# Machine Level Programming

## Intel x86 Processors
### Complex instruction set computer(CISC)
+ Many different instructions with many different formats
+ But only small subset encountered with Linux programs
+ Hard to match performance of Reduced Instruction Set Computers(RISC)
+ But Intel has done just that! In terms of speed. Less so for low power.
### ARM: Acorn RISC Machine

## Definations
### Architecture(alse ISA:instruction set architecture)(指令集):
**The parts of a processor design that one needs to understand or write assembly/machine code.**
+ Examples: instrucion set spectification, registers.

### Microarchitecture:
**Implementation of the architecture**
+ Examples: cache sizes and core frequency.

### Code Forms:
+ Machine Code
+ Assembly Code

## Assembly Characteristics: Data Types
+ Integer data of 1, 2, 4, or 8 bytes
+ Floating point data of 4, 8, or 10 bytes
+ Code: Byte sequences encoding series of instructions
+ No aggregate types such as arrays or structures

## leaq Src, Dst
+ Load Effective Address
+ Src is address mode expression
+ Set Dst to address denoted by expression
> SI=1000H , DS=5000H, (1000H)=1234H   
> LEA BX , [SI] --> BX=1000H   
> MOV BX , [SI] --> BX=1234H   
### Computing arithmetic expressions of the form x+k\*y(k=1,2,4,8)
	Example
	long m12(long x) {
		return x*12;
	}
	leaq (%rdi, %rdi, 2), %rax
	salq  $2, %rax
