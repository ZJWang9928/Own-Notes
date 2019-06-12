# Bits, Bytes and Integers cont

## Unsigned Addition
Operands: w bits    
True Sum: w+1 bits    
Discard Carry: w bits    

### Standard Addition Function
+ Ignores carry output
### Implements Modular Arithmetic
+ s = UADDw(u, v) = u + v mod 2^w
+ Overflow (P61)

## Two's Complement Addition
Operands: w bits    
True Sum: w+1 bits    
Discard Carry: w bits    
	
	 1101 -3
	+0101  5
	---------
	 0010  2

	 1011 -5
	+0011  3
	---------
	 1110 -2 

	 1101 -3
	+1010 -6
	---------
	 0111  7  (Negative Overflow)

	 0111  7
	+0101  5
	---------
	 1100 -4  (Positive Overflow)
### TAdd Overflow
**Functionality**    
+ True sum requires w+1 bits
+ Drop off MSB
+ Treat remaining bits as 2's comp. integer
+ Overflow(P/N) (P64)

## Multiplication
**Exact results can be bigger than w bits, so, maintaining exact results.**      
### Unsigned Multiplication in C
+ UMULTw(u, v) = u * v mod 2^w   

> 5 * 5 = 25    
> 0001|1001 = 9 = 25 mod 16    
    
### Signed Multiplication in C
Operands: w bits    
True Sum: 2\*w bits    
Discard w bits: w bits    
+ Ignores high order w bits
+ Some of which are different for signed vs unsigned multiplication
+ **Lower bits are the same**
+ **Whatever bit left in the sign-bit position will determine the sign regardless of the signs of the original two operands**

		 1101 -3 (13)
		 1110 -2 (14)
	 ____|0110  6 (182)

## Power-of-2 Multiply with Shift
Operands: w bits    
True Sum: w+k bits    
Discard k bits: w bits    
### Opetarion
+ u << k gives u * 2^k
+ Both signed and unsigned    
    
## Unsigned Power-of-2 Divide with Shift
+ u \>\> k gives floor(u / 2^k)
+ Use logical shift
    
## Unsigned Power-of-2 Divide with Shift
+ Use logical shift (\>\>\> in Java)
+ May add a bias

## Negate a number (x -> -x)
+ complement and increment (contrast + 1)

## Counting Down with Unsigned
### Proper way to use unsigned as loop index
> unsigned i;     
> for (i = cnt-2; **i < cnt**; i--)   
>	a[i] += a[i+1];
**C Standard guarantees that unsigned addition will behave like modular arithmetic**    
+ 0-1 -> UMax
### Even better
> size\_t i;     
> for (i = cnt-2; **i < cnt**; i--)   
>	a[i] += a[i+1];
+ Data type size\_t defined as unsiged value with length = word size
+ Code will work even if cnt = UMax
+ What if cnt is signed and <0? 

## Why Should I Use Unsigned?(cont.)
### Perform Modular Arithmetic
+ Mult. arithmetic
### When Using Bits to Represent Sets
+ Logical right shift, no sign extension

## Byte-Oritented Memory Organization
### Programs refer to data by address
+ Conceptually, envision it as a very large array of bytes(while in reality it's not)
+ An address is like an index into that array

### Note: System provides private address spaces to each "precess"
+ Think of a precess as a program being executed
+ So, a program can clobber its own data, but not that of others

### a Useful Trick: 2^10 is around 10^3

## Machine Words
### Any given computer has a "Word Size"
+ Nominal size of the integer-value data and of address
+ Machines still support multiple data formats

## Word-Oritented Memory Organization
### Addresses Specify Byte Locations
+ Address of first byte in a word

### How are the bytes within a multi-byte word ordered in memory?
**Conventions**    
+ Big Endian: Least significant byte has highest address(Sun, PPC Mac, Internet)
+ Little Endian: Least significant byte has lowest address(x86, ARM processors running Androidm iOS, Windows)
