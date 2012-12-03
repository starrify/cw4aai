.inc "pseudo_inst.inc"
.def STKVAR_S r1, var {sw r1, var($sp)}
.def STKVAR_L r1, var {lw r1, var($sp)}

.def SETFRM pos {ori $fp, $zero, pos}
.def SETFRML pos {lli $fp, pos}
.def SETFRMLB lb {lla $fp, lb}
