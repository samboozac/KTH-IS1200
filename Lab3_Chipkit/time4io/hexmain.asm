  # hexmain.asm
  # Written 2015-09-04 by F Lundevall
  # Copyright abandonded - this file is in the public domain.

	.text
main:
	li	$a0, 99		# change this to test different values

	jal	hexasc		# call hexasc
	nop			# delay slot filler (just in case)	

	move	$a0,$v0		# copy return value to argument register

	li	$v0,11		# syscall with v0 = 11 will print out
	syscall			# one byte from a0 to the Run I/O window
	
stop:	j	stop		# stop after one run
	nop			# delay slot filler (just in case)

  # You can write your own code for hexasc here
  
  hexasc:
  	slti 	$t0, $a0, 10	# Sätter t0 till 1 om a0 är mellan 0 - 9, annars t1 = 0.
  	beq	$t0, $0, aTillF	# if the argument is bigger then 9, branch.
  	
  				# case 0-9
  	addi	$t1, $a0, 0x30	# add the constant 30 to recive the ASCII code for 0-9
  	ori	$v0, $t1, 0x0	# mask away unwanted bits (setting them to zero)
  	
  	jr	$ra		# return to main
  	
aTillF:				# case A-F
  	addi	$t1, $a0, 0x37	# add the constant 37 to recive the ASCII code for A-F.
	ori	$v0, $t1, 0x0	# mask away unwanted bits (setting them to zero)
	
  	jr	$ra		# return to main
  	
  	


