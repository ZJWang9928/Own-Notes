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
	5 * 5 = 25
	0001|1001 = 9 = 25 mod 16
    
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
