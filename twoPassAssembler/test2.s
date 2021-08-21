.section ivt
.global c
.word a, 5, 11
.skip 10
.word a, c
a:jeq %c
jne *5
c: jgt a
.end