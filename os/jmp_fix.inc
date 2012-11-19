.def j label
{
        j label
        sll $zero, $zero, 0
}
.def jr r1
{
        jr r1
        sll $zero, $zero, 0
}
.def jal label
{
        jal label
        sll $zero, $zero, 0
}
.def beq r1, r2, label
{
        beq r1, r2, label
        sll $zero, $zero, 0
}
.def bne r1, r2, label
{
        bne r1, r2, label
        sll $zero, $zero, 0
}
