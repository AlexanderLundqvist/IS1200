  # labwork.S
  # Written 2015-2017 by F Lundevall
  # Skeleton file for IS1200/IS1500 lab 1.
  # The macros PUSH and POP are in the public domain.
  # Please add your own code at the end of the file.


.global hexasc

.global delay

.global time2string
  
# Please keep the two macros PUSH and POP unchanged
  
.macro	PUSH reg
	addi	$sp,$sp,-4
	sw	\reg,0($sp)
.endm

.macro	POP reg
	lw	\reg,0($sp)
	addi	$sp,$sp,4
.endm

.data
.text


# Code from task 1 to 4

hexasc:
	andi $t0, $a0, 0xf		# Bitwise AND with 0xf to keep the 4 LSB in the argument, store in t0
	
	ble $t0, 0x9, number		# Branch if the input is less than or equal to 9, else continue
	nop				# delay slot filler
	
	ble $t0, 0xf, char		# Branch if the input is less than or equal to 15, but greater than 9
	nop				# Delay slot filler
	
	number:				# Subroutine for input 0-9
		addi $v0, $t0, 0x30	# ASCII numbers start at 30, therefore we add the input to zero (0x30)
		jr $ra			# Jump to return adress that was linked with function call		
		nop			# delay slot filler
		
	char:				# Subroutine for input 10-15
		addi $v0, $t0, 0x37	# Since ASCII letters start at 0x41, we add an offset of 7 
		jr $ra			# Jump to return adress that was linked with function call		
		nop			# delay slot filler

# New delay funtion (Task 4)
delay:
	PUSH	$ra
	move 	$t1, $a0			# Store argument in temp so we can use it
	
	while:
		ble	$t1, $zero, exit	# Check if ms > 0
		nop
		sub	$t1, $t1, 1 		# ms--;
		
	li	$t2, 0				# int i	= 0		
	for:
		bge	$t2, 28, while		# Check if i < 28 (Can be changed for speed), then jump or continue
		nop
		add	$t2, $t2, 1		# i++;
		j	for			# Go to next iteration of for loop
		nop
		
	j	while				# Go back to next iteration in while loop
	nop
			
	exit:					# End of subroutine
		POP	$ra			# Restore the return adress
		jr	$ra			# Jump back to caller
		nop
		
# Converts time-info into a string of printable characters, with a null-byte as an end-of-string-marker
# $a0 contains the adress to the section of memory where we will store the result.
# $a1 conains the NBCD-encoded time info, where we only consider the 16 LSB.
time2string:
	PUSH	$s1				# Save contents of s1 to restore it after the function ends
	PUSH	$ra				# Save the return adress on the stack
	
	move	$s1, $a0				# Move contents of $a0 to $s1 so we can work with it
	PUSH	$a0				# Save the contents of $a0 so we can restore it later 
						# and use it for hexasc now
	
	# First digit
	andi 	$t1, $a1, 0xf000		# Masking out bit from index 15 to 12
	srl 	$a0, $t1, 12			# Shifting the bits to lowest position and store it in $a0 for hexasc
	jal	hexasc				# Calling the hexasc that will transform the decimal into hexadecimal
	nop
	sb 	$v0, 0($s1)		 	# Save the return value from hexasc in the first byte location $s1 
						# points to
						
	
	# Second digit
	andi 	$t1, $a1, 0x0f00		# Masking out bit from index 11 to 8
	srl 	$a0, $t1, 8			# Shifting the bits to lowest position and store it in $a0 for hexasc
	jal	hexasc				# Calling the hexasc that will transform the decimal into hexadecimal
	nop
	sb 	$v0, 1($s1)		 	# Save the return value from hexasc in the second byte location $s1 
						# points to
	
	# Adding the colon
	li 	$t1, 0x3a			# Loading the ASCII code for colon
	sb 	$t1, 2($s1)		 	# Save the return value from hexasc in the third byte location $s1 
						# points to
	
	# Third digit
	andi 	$t1, $a1, 0x00f0		# Masking out bit from index 7 to 4
	srl 	$a0, $t1, 4			# Shifting the bits to lowest position and store it in $a0 for hexasc
	jal	hexasc				# Calling the hexasc that will transform the decimal into hexadecimal
	nop
	sb 	$v0, 3($s1)		 	# Save the return value from hexasc in the fourth byte location $s1 
						# points to
	
	# Forth digit
	andi 	$t1, $a1, 0x000f		# Masking out bit from index 3 to 0
	move 	$a0, $t1			# No need for shifting, just move it to the argument.
	jal	hexasc				# Calling the hexasc that will transform the decimal into hexadecimal
	nop
	sb 	$v0, 4($s1)		 	# Save the return value from hexasc in the fifth byte location $s1 
						# points to
																										
	# Adding the NUL byte
	li	$t1, 0x00			# Loading the ASCII code for NUL
	sb 	$t1, 5($s1)		 	# Save the return value from hexasc in the sixth byte location $s1 
						# points to
	
	# End of subroutine. Restoring registers and jumping back to caller.																																																																																										
	POP	$a0
	POP	$ra
	POP	$s1
	
 	jr 	$ra
 	nop	
