.entry LIST W
.extern
MAIN: add r2 LIST
LOOP: prn #48
lea W r6
inc r6
mov r3
sub r1 r4
bne END
cmp k #-6
bne &END
dec W
ab
.entry MAIN
jump &LOOP
add 
END: stop
STR: .string abcd
"vvv"
LIST: .data 6 -9
.data -100
k: .data 31
 L3
