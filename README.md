# Brain Tuck -- A Brainf**k inspired Language
## Instructions and Binary Representation

pppooooo


### Increment and Decrement Register Pointers
#### Instructions
* `incr [axy]?>`
* `decr [axy]?<`
#### Format
`0rrooooo`

### Jump
#### Instructions
* `jz [` Jump to `]` if memory pointed to by `a` is zero
* `jnz ]` Jump to `[` if memory pointed to by `a` is zero
#### Format
`s00ooooo 0xDDDD`

### Unsigned Arithmetic and Logical Operations
#### Instructions
* `inc [bwdq]i +`
* `dec [bwdq]d -`
* `add [bwdq]+`
* `sub [bwdq]-`
* `mult [bwdq]*`
* `div [bwdq]/`
* `mod [bwdq]%`
* `neg [bwdq]n`
* `band [bwdq]&`
* `bor [bwdq]|`
* `bxor [bwdq]^`
* `land [bwdq]a`
* `lor [bwdq]o`
* `bnot [bwdq]~`
* `lnot [bwdq]!`
* `mov [bwdq]=`
* `eq [bwdq]e`
#### Format
`0llooooo`

### Bit Shift
#### Instructions
* `s [bwdq][su][{}]`
#### Format
`sd0ooooo`

### Signed Arithmetic and Logical Operations
#### Instructions
* `lt [bwdq][su]<`
* `gt [bwdq][su]>`
#### Format
`sllooooo`

### IO
#### Instructions
* `in [bwdqs],`
* `out [bwdqs].`
#### Format
* `0llooooo`
* `1s0ooooo`


