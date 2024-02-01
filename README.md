# Brain Tuck -- A Brainf**k inspired Language
## Overview
Brain Tuck can be deemed as the assembly language for a machine with 10000 bytes
of data memory and 10000 bytes of code memory. The machine has three data
memory pointers (`a`, `x` and `y`) and an implicit instruction pointer which
normally increments on instructions, except for the jump instructions, where
it can be set to an arbitrary indicated by the matching bracket. The instruction
set itself is compatible with Brain F**k. All arithmetic and logical operations
must be prepended with a length prefix (`b` for 1 byte, `w` for 2 bytes, `d`
for 4 bytes and `q` for 8 bytes), while some operations (greater than, less
than and bit-wise right shift) must also be prepended with a signed-ness prefix
(`u` for unsigned and `s` for signed). All operations work as
`mem[a] = op(mem[x] + mem[y])`.

## Instructions and Binary Representation
`pppooooo`

All instructions, except for jump and load, are represented by a single byte,
where the lower 5 bits are the op-code and the upper 3 bits serve as
"parameters" (length, signed-ness, etc.)

### Increment and Decrement Memory Pointers
#### Format
`0rrooooo`

The 6th and 5th bit indicate the memory pointer to modify, while the 7th bit
is fixed at 0.

#### Instructions
* `incr [axy]?>`: Increment the memory pointer
* `decr [axy]?<`: Decrement the memory pointer


### Jump
#### Format
`s00ooooo 0xDDDD`

The 7th bit is used to distinguish the 2 instructions, where 1 indicates `jnz`.
It is then followed by a 2 byte absolute address.

#### Instructions
* `jz [` Jump to `]` if memory pointed to by `a` is zero
* `jnz ]` Jump to `[` if memory pointed to by `a` is not zero


### Unsigned Arithmetic and Logical Operations
#### Format
`0llooooo`

The 6th and 5th bit indicate length.

#### Instructions
* `inc [bwdq]i`: `mem[a] = mem[a] + 1`
* `dec [bwdq]d`: `mem[a] = mem[a] - 1`
* `add [bwdq]+`: `mem[a] = mem[x] + mem[y]`
* `sub [bwdq]-`: `mem[a] = mem[x] - mem[y]`
* `mult [bwdq]*`: `mem[a] = mem[x] * mem[y]`
* `div [bwdq]/`: `mem[a] = mem[x] / mem[y]`
* `mod [bwdq]%`: Unsigned modulo, `mem[a] = mem[x] % mem[y]`
* `neg [bwdq]n`: 2's complement, `mem[a] = -mem[x]`
* `band [bwdq]&`: Bit-wise and, `mem[a] = mem[x] & mem[y]`
* `bor [bwdq]|`: Bit-wise or, `mem[a] = mem[x] | mem[y]`
* `bxor [bwdq]^`: Bit-wise xor, `mem[a] = mem[x] ^ mem[y]`
* `land [bwdq]a`: Logical and, `mem[a] = mem[x] && mem[y]`
* `lor [bwdq]o`: Logical or, `mem[a] = mem[x] || mem[y]`
* `bnot [bwdq]~`: Bit-wise negate, `mem[a] = ~mem[x]`
* `lnot [bwdq]!`: Logical negate, `mem[a] = !mem[x]`
* `mov [bwdq]=`: Assignment, `mem[a] = mem[x]`
* `eq [bwdq]e`: Equality, `mem[a] = mem[x] == mem[y]`
* `sl [bwdq]{`: Left shift, `mem[a] = mem[x] << mem[y]`


### Signed Arithmetic and Logical Operations
#### Format
`sllooooo`

The 6th and 5th bit indicate length, while the 7th bit indicates signed-ness,
where 1 indicates signed.

#### Instructions
* `lt [bwdq][su]l`: `mem[a] = mem[x] < mem[y]`
* `gt [bwdq][su]g`: `mem[a] = mem[x] > mem[y]`
* `sr [bwdq][su]}`: Right shift, `mem[a] = mem[x] >> mem[y]`


### Load Immediate Byte
#### Format
`000ooooo 0xDD`

Top 3 bits are fixed at 0. It is then followed by a byte with which to set the
memory pointed to by `a` to.
##### Instruction
* `ld [0-9A-F]{2}`


### IO
#### Format
* `0s0ooooo`
* `1llooooo`

If top bit is at 0, output null-terminated string pointed to by `a` if the 6th
bit is 1, otherwise output the character pointed to by `a`.

If top bit is at 1, output the number pointed to by `a` in hexadecimal notation,
where the 6th and 5th bit indicate length.

#### Instructions
* `in [bwdqs]?,`: Input, s for string. Single character if no prefix is present.
* `out [bwdqs]?.`:
Output, s for string. Single character if no prefix is present.

## Build Instructions
* `cd bti`
* `make`
