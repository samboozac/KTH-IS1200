  # hexmain.asm
  # Written 2015-09-04 by F Lundevall
  # Copyright abandonded - this file is in the public domain.

	.text
main:
	li	$t0,17		# change this to test different values

	jal	hexasc		# call hexasc
	nop			# delay slot filler (just in case)	

	move	$a0,$v0		# copy return value to argument register

	li	$v0,11		# syscall with v0 = 11 will print out
	syscall			# one byte from a0 to the Run I/O window
	
stop:	j	stop		# stop after one run
	nop			# delay slot filler (just in case)

  # You can write your own code for hexasc here
  #
hexasc: 
  	 andi $a0, 0xf	#masking lsByte in the parameter
  	 li $t0, 10	# for the if-condtion in ble
  	 li $t1, 48	#ASCI-first index of digits
  	 li $t2, 55	#ASCI-first index of char
  	 ble $a0, $t0, char	#conditioin 
  	 nop 
  	 add $v0, $a0, $t2	#add the value to index and load the return register
  	 jr $ra
char: 
	 add $v0, $a0, $t1
	 jr $ra
			 
	    	 
  	 
  	 
  	

