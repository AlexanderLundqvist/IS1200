  # analyze.asm
  # This file written 2015 by F Lundevall
  # Copyright abandoned - this file is in the public domain.

	.text
main:
	li	$s0,0x30	# Loads in the first ASCII-character into s0
loop:
	move	$a0,$s0		# copy from s0 to a0 
	
	li	$v0,11		# syscall with v0 = 11 will print out
	syscall			# one byte from a0 to the Run I/O window (One byte since ASCII is 1 byte)

	addi	$s0,$s0,3	# what happens if the constant is changed?
	
	li	$t0,0x5a	# Load in the character Z
	ble	$s0,$t0,loop	# This line checks if we have reached Z (Will go to stop if We already have printed Z)
	nop			# delay slot filler (just in case)

stop:	j	stop		# loop forever here
	nop			# delay slot filler (just in case)



