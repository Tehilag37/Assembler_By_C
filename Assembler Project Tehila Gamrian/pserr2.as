.extern W
.entry
MAIN: addd r3, LIST
LOOP: prn #48
lea W, r6
inc 
moov r3, K
sub r1  r4
bne END
cmp
bne &END
dec W ,W
.entry MAIN
jmp &LOOP
add L3 L3
END: stop
STR: .string "abcd
LIST: .data 6, -9
.data -100
k: .data 31
.extern W
.extern

