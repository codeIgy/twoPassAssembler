.section ivt
halt
int r4
iret
call 5
ret
jmp a
a:jeq %b
b: jne *5
c: jgt *c
push psw
pop r0
xchg r1, r2
add r5, r7
sub sp, pc
mul r0, r0
div r2, r3
cmp r3, pc
not r6
and r1, r2
or r1, r2
xor r1, r2
test r1, r2
shl r1, r2
shr r1, r2
jmp *r1
jmp *[ r1 ]
jmp *[r7 + 3]
jmp *[pc + a]
ldr r0, $4
ldr r0, $e
ldr r0, 0x10
e: ldr r0, a
str r0, %a
str pc, psw
str sp, [r4]
str psw, [r4 + 0]
str r4, [r4 + e]
.end