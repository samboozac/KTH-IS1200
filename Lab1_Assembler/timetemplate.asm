  # timetemplate.asm
  # Written 2015 by F Lundevall
  # Copyright abandonded - this file is in the public domain.

.macro	PUSH (%reg)		#MACRO-setup for PUSH
	addi	$sp,$sp,-4	#Increment pointer and addd value to the stack
	sw	%reg,0($sp)	#Refer to incrementing stackpointer
.end_macro

.macro	POP (%reg)		#MACRO-setup for POP
	lw	%reg,0($sp)	#Take value from the stack and decrement
	addi	$sp,$sp,4
.end_macro

	.data			#Assembler directives (datasection)
	.align 2		#Assembler directives (ASCII string declaration)
mytime:	.word 0x5957		#Variable 'mytime' with word value declaration of 0x 5957
timstr:	.ascii "text more text lots of text\0"
	.text			#Assembler directives (cide section)
main:
	# print timstr
	la	$a0,timstr	#Load adress (lui/ori), label timstr
	li	$v0,4		#Load immediate (lui/ori), constant
	syscall
	nop			#No operation, to fill branch-delay slot of jumps or branches
	# wait a little
	li	$a0,1		#Load value 2 into parameter $a0 register 
	jal	delay		#Jump And Link to delay, jump right back with jr $ra
	nop			#No operation 
	# call tick
	la	$a0,mytime	#Load address of variable 'mytime' into $a0
	jal	tick		#Jump And Link to 'tick' which updates time pointed to by $a0
	nop			#No operation
	# call your function time2string
	la	$a0,timstr	#Load adress of $a0, call it 'timstr' ($at register)
	la	$t0,mytime	#Load adress of $t0 of label 'mytime'
	lw	$a1,0($t0)	#Load Word into $a1, $t0 
	jal	time2string	#Jump And Link to 'time2String'
	nop
	# print a newline
	li	$a0,10		
	li	$v0,11		
	syscall
	nop			#No operation 
	# go back and do it all again
	j	main		#Jump  back to main
	nop			#No operation 
# tick: update time pointed to by $a0
tick:	lw	$t0,0($a0)	# get time
	addiu	$t0,$t0,1	# increase
	andi	$t1,$t0,0xf	# check lowest digit
	sltiu	$t2,$t1,0xa	# if digit < a, okay
	bnez	$t2,tiend
	nop
	addiu	$t0,$t0,0x6	# adjust lowest digit
	andi	$t1,$t0,0xf0	# check next digit
	sltiu	$t2,$t1,0x60	# if digit < 6, okay
	bnez	$t2,tiend
	nop
	addiu	$t0,$t0,0xa0	# adjust digit
	andi	$t1,$t0,0xf00	# check minute digit
	sltiu	$t2,$t1,0xa00	# if digit < a, okay
	bnez	$t2,tiend
	nop
	addiu	$t0,$t0,0x600	# adjust digit
	andi	$t1,$t0,0xf000	# check last digit
	sltiu	$t2,$t1,0x6000	# if digit < 6, okay
	bnez	$t2,tiend
	nop
	addiu	$t0,$t0,0xa000	# adjust last digit
tiend:	sw	$t0,0($a0)	# save updated result
	jr	$ra		# return
	nop

  # you can write your code for subroutine "hexasc" below this line
  #
hexasc: 
  	 andi $a0, 0xf		#masking lsByte in the parameter
  	 andi $v0, 0x7f		#masking lsbit(7) in return value
  	 li $t0, 10		# for the if-condtion in ble
  	 li $t1, 48		#ASCI-first index of digits
  	 li $t2, 55		#ASCI-first index of char
  	 ble $a0, $t0, char	#conditioin 
  	 nop 			#No operation
  	 add $v0, $a0, $t2	#add the value to index and load the return register
  	 jr $ra			#Jump Register $ra, jump back under jal(Jump And Link in main)
char: 				#Runs if argument $a0 < 10
	 add $v0, $a0, $t1	#Load the value of input and index to $v0
	 jr $ra

delay: 				#Delay function
	li	$t1, 4711		# delayconstant for the loop
	li	$t0, 1			# to subtract with register
while_loop_1:				# label
	nop				# delay slot filler
	ble	$a0, $zero, while_done	# condition (while)
	sub	$a0, $a0, $t0		# $a0 = $a0 - 1
	li	$t2, 0			# reset the for-loop
for_loop_1:	
	nop			# Label, jump-I
	ble	$t2, $t1, for_done	# condition for-loop
	addi	$t2, $t2, 1		# increment for-counter
	
	j	for_loop_1
	nop
for_done:
	j	while_loop_1
	nop
while_done:
 	jr 	$ra
 	nop

time2string:
	PUSH($ra)
	move $t9, $a0		#saves the adress of $a0 in $s0
	
	andi $t0, $a1, 0xf000	#mask the first digit
	srl $a0, $t0, 12	#Shift Right Logic 12 to lsb
	jal hexasc		#Call Hexasc
	nop
	sb $v0, 0($t9)		#Store the byte(hexasc...) using the pointer to the correctr string Adress ($t9) 
	
	andi $t0, $a1, 0x0f00	#mask the first digit
	srl $a0, $t0, 8		#Shift Right Logic 12 to lsb
	jal hexasc		#Call Hexasc
	nop
	sb $v0, 1($t9)		#Store the byte(hexasc...) using the pointer to the correctr string Adress ($t9)
	
	li $t4, 0x3a
	sb $t4, 2($t9)
	
	andi $t0, $a1, 0x00f0	#mask the first digit
	srl $a0, $t0, 4		#Shift Right Logic 12 to lsb
	jal hexasc		#Call Hexasc
	nop
	sb $v0, 3($t9)		#Store the byte(hexasc...) using the pointer to the correctr string Adress ($t9) 
	
	move $a0, $a1		#Prep $a0 to the masked value in $t0
	jal hexasc		#Call Hexasc
	nop
	sb $v0, 4($t9)		#Store the byte(hexasc...) using the pointer to the correctr string Adress ($t9)  
	
	li $t8, 0x0
	sb $t8, 5($t9)
	POP($ra)
	
	jr $ra
	nop

