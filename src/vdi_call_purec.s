	XDEF vdi_call

	TEXT
vdi_call:
	move.l a0,d1
	move.l #$73,d0
	trap   #2
	rts
