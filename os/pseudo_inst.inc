.def blt r1, r2, label
{
	slt	$at, r1, r2
	bne	$at, $zero, label
}
.def bltu r1, r2, label
{
	sltu	$at, r1, r2
	bne	$at, $zero, label
}
.def ble r1, r2, label
{
	slt	$at, r2, r1
	beq	$at, $zero, label
}
.def bleu r1, r2, label
{
	sltu	$at, r2, r1
	beq	$at, $zero, label
}
.def bgt r1, r2, label
{
	slt	$at, r2, r1
	bne	$at, $zero, label
}
.def bgtu r1, r2, label
{
	sltu	$at, r2, r1
	bne	$at, $zero, label
}
.def bge r1, r2, label
{
	slt	$at, r1, r2
	beq	$at, $zero, label
}
.def bgeu r1, r2, label
{
	sltu	$at, r1, r2
	beq	$at, $zero, label
}
.def lil r1, inst
{
        lui     r1, (inst >> 16) & 0xff
        addi    r1, $zero, inst & 0xff
}
.def swia r1, base_addr, offset
{
        lil $at, base_addr
        sw r1, offset($at)
}
.def lwia r1, base_addr, offset
{
        lil $at, base_addr
        lw r1, offset($at)
}