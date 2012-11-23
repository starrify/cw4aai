#branch less than
.def blt r1, r2, label
{
    slt $at, r1, r2
    bne $at, $zero, label
}
#branch less than(unsigned)
.def bltu r1, r2, label
{
    sltu $at, r1, r2
    bne $at, $zero, label
}
#branch less than or equal to
.def ble r1, r2, label
{
    slt $at, r2, r1
    beq $at, $zero, label
}
#branch less than or equal to(unsigned)
.def bleu r1, r2, label
{
    sltu $at, r2, r1
    beq $at, $zero, label
}
#branch greater than
.def bgt r1, r2, label
{
    slt $at, r2, r1
    bne $at, $zero, label
}
#branch greater than(unsigned)
.def bgtu r1, r2, label
{
    sltu $at, r2, r1
    bne $at, $zero, label
}
#branch greater than or equal to
.def bge r1, r2, label
{
    slt $at, r1, r2
    beq $at, $zero, label
}
#branch greater than or equal to(unsigned)
.def bgeu r1, r2, label
{
    sltu $at, r1, r2
    beq $at, $zero, label
}
#load large immediate
.def lli r1, imm
{
    lui r1, (imm >> 16) & 0xffff
    ori r1, $zero, inst & 0xffff
}
#store word by immediate address
.def swia r1, base_addr, offset
{
    ori $at, $zero, base_addr
    sw r1, offset($at)
}
#load word by immediate address
.def lwia r1, base_addr, offset
{
    ori $at, $zero, base_addr
    lw r1, offset($at)
}
#store word by large immediate address
.def swlia r1, base_addr, offset
{
    lli $at, base_addr
    sw r1, offset($at)
}
#load word by large immediate address
.def lwlia r1, base_addr, offset
{
    lli $at, base_addr
    lw r1, offset($at)
}
#shift left round by reg
.def slrv rd, rs, rt
{
    ori $rd, $zero, 32
    sub $rd, $rd, $rt
    srl $rd, $rs, $rd
    sll $at, $rs, $rt
    or $rd, $rd, $at
}
#shift right round by reg
.def srrv rd, rs, rt
{
    ori $rd, $zero, 32
    sub $rd, $rd, $rt
    sll $rd, $rs, $rd
    srl $at, $rs, $rt
    or $rd, $rd, $at
}
#branch when not aligned
.def bnaln rs, label
{
    srl $at, rs, 2
    sll $at, $at, 2
    bne $at, rs, label
}
#branch when aligned
.def baln rs, label
{
    srl $at, rs, 2
    sll $at, $at, 2
    beq $at, rs, label
}
#set when overlapped
.def sovlp r1, raddr1, raddr2, rsize
{
    add r1, raddr1, rsize
    sltu r1, raddr2, r1
    add $at, raddr2, rsize
    sltu $at, raddr1, $at
    #is overlap if (raddr1 + rsize > raddr2 and raddr2 + rsize > raddr1) != 0
    and r1, r1, $at
}
