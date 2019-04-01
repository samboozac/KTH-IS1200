  # labwork.S
  # Written 2015-2017 by F Lundevall
  # Skeleton file for IS1200/IS1500 lab 1.
  # The macros PUSH and POP are in the public domain.
  # Please add your own code at the end of the file.

  #
  # Please keep the two macros PUSH and POP unchanged
  #
.macro	PUSH reg		
	addi	$sp,$sp,-4
	sw	\reg,0($sp)
.endm

.macro	POP reg
	lw	\reg,0($sp)
	addi	$sp,$sp,4
.endm

# Please add your own code below this line

.data	
.global delay, time2string		# to find the subroutines delay and time2string from main
.text

# Pseudokod ändrad till Equivalent basic instructions

# HEXASCFUNCTION
hexasc:
 	andi	$t1, $a0, 0xf	# "maskar" fram bitarna 0-3
  	slti 	$t0, $t1, 10	# Sätter t0 till 1 om a0 är mellan 0 - 9, annars t1 = 0.
  	beq	$t0, $0, aTillF	# if the argument is bigger then 9, the branch is taken. If branch taken, jump to aTillF.
  	nop
  				# case 0-9
  	addi	$t1, $t1, 0x30	# add the constant 30 to recive the ASCII code for 0-9
  	add	$v0, $t1, $0	# copy ASCII-code to v0
  	
  	jr	$ra		# return to main
  	nop
  	
aTillF:				# case A-F
  	addi	$t1, $t1, 0x37	# add the constant 37 to recive the ASCII code for A-F.
  	add	$v0, $t1, $0	# copy ASCII-code to v0
	
  	jr	$ra		# return to main
  	nop
  	
# DELAYFUNCTION
delay:
	li	$t1, 4711		# delayconstant for the loop
	#lui	$at, 0x0000
	#ori	$t1, $at, 0xffff
	
	#li	$t0, 1			# constant to subtract with register. Subtract variabel. Used in while to sub a0 with 1 for each loop.
	lui	$at, 0x0000
	ori	$t0, $at, 0x0001	
	
while_loop_1:				# label
	nop				# delay slot filler
	#ble	$a0, $zero, while_done	# condition for the while loop.
	slt	$at, $0, $a0		
	beq	$at, $0, while_done
	nop
	
	sub	$a0, $a0, $t0		# $a0 = $a0 - 1
	#li	$t2, 0			# reset the for-loop. Resets the increment variable in the for-loop.
	lui	$at, 0x0
	ori	$t2, $at, 0x0		
	
for_loop_1:				# Label, jump-I
	nop
	#ble	$t2, $t1, for_done	# condition for-loop
	slt	$at, $t2, $t1
	beq	$at, $0, for_done	
	nop
	addi	$t2, $t2, 1		# increment the for-counter
	
	# do something
	
	j	for_loop_1		# jump back to the top of the for-loop (for_loop_1)
	nop
for_done:
	j	while_loop_1		# jump back to the top of the while loop
	nop
while_done:				# loop done. jumps out of the function
 	jr 	$ra
	nop				# delay slot filler

# TIMESTRINGFUNCTION
time2string:
 	PUSH $ra			# Spara return address till mai
 	PUSH $s0
 	PUSH $s1
 	PUSH $s2
 	
	add	$s2, $a0, $0		# Kopierar adressen i a0 till $t9. $t9 används i samband med sb för att spara de hexkodade karaktärerna på minnesplatsen som $a0 initial pekade på.
 	
 	andi	$s0, $a1, 0xffff	# Hour condition
 	beq	$s0, $0, hour		# branch taken if hour == 00:00
 	nop				# Jumps to hour if current time == 00:00
 	
 	# m1
 	andi	$s0, $a1, 0xf000	# Maska bitarna 15:12 från a1
 	srl	$a0, $s0, 12		# Shifta 4 bitarna så att de blir lsb's
 	jal	hexasc			# jump (and link) to hexasc. 
 	nop				# Return value i v0 representerar givna siffror i Hex-kodning. En hexkodad siffra representeras av 8 bits.
 	sb	$v0, 0($s2)		# Spara m1 i minnet, strängadressen som $t9($a0) pekar på med offset 0, då det är den första karaktären i strängen så läggs den på index 0 i minnesplatsen.


 	# m2
 	andi	$s0, $a1, 0x0f00	# Maska bitarna 11:8 från a1
 	srl	$a0, $s0, 8		# Shifta 4 bitarna så att de blir lsb's
 	jal	hexasc			# jump( and link) till hexasc. 
 	nop				# Return value i v0 representerar givna siffror i Hex-kodning. En hexkodad siffra representeras av 8 bits.
 	sb	$v0, 1($s2)		# Spara m2 i minnet, strängadressen som a0 pekar på med offset 1, lägger sig på minnesplatsen med index 2
 	
 	
 	# "kolon"
	li	$s0, 0x3A		# Hex-kodning för kolon. load immediate in i t1, som i sin tur sparas i minnes som t9 pekar på
 	sb	$s0, 2($s2)		# Spara m1 i minnet, strängadressen som $t9($a0) pekar på med offset 3
 	

	# s1
 	andi	$s0, $a1, 0x00f0	# Maska bitarna 7:4 från a1
 	srl	$a0, $s0, 4		# Shifta 4 bitarna så att de blir lsb's 
 	jal	hexasc			# jump( and link) till hexasc. 
 	nop				# Return value i v0 representerar givna siffror i Hex-kodning. En hexkodad siffra representeras av 8 bits.
 	sb	$v0, 3($s2)		# Spara s1 i minnet, strängadressen som $t9($a0) pekar på, offset 4. "sista" platsen i första minnesadressen
 	
 	
 	# s2
 	add	$a0, $a1, $0		# kopiera a1 till a0. Skicka till hexasc, detta fungerar då hexasc bara tar emot de fyra minst signifikanta bitarna i a0
	jal	hexasc			# dvs. resterande bitar ignoreras
 	nop				#
 	sb	$v0, 4($s2)		# Spara s2 i minnet, strängadressen som a0 pekar på. Minnet i 0x10010004 är för oss fyllt, därför kommer $v0, 4($t9) att lägga sig på 0x10010004 + 4
 					# = 0x10010008, med indexplats 0.
 	
 	# NUL				# NULLbyte, tells the 
 	#li	$t1, 0x00
 	lui	$at, 0x0
	ori	$s0, $at, 0x0
 	sb	$s0, 5($s2)		# Spara m1 i minnet, strängadressen som a0 pekar på med offset 5 -> Lägger sig i minnet på addressen 0x10010008 med index 1
 	
 	POP $s2		# a0
	POP $s1		# t1
 	POP $s0		# t0 
 	POP $ra 	# hämtar tillbaka jumpaddressen, då den överskrivits i samband med functioncall till hexasc
  
  	jr	$ra
	nop
	
# HOUR
 	
hour:
 	li	$s0, 0x48	# H
 	sb	$s0, 0($s2)
 	
 	li	$s0, 0x4f	# O
 	sb	$s0, 1($s2)
 	
 	li	$s0, 0x55	# U
 	sb	$s0, 2($s2)
 	
 	li	$s0, 0x52	# R
 	sb	$s0, 3($s2)
 	
 	li	$s0, 0x00	# NUL
 	sb	$s0, 4($s2)			
 	
  	POP $s2		# a0
	POP $s1		# t1
 	POP $s0		# t0 
 	POP $ra		# POP return address till main
 	
  	jr	$ra
	nop
