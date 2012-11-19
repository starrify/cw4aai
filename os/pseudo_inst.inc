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
