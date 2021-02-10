
        # pointers.asm
# By David Broman, 2015-09-14
# This file is in the public domain


.macro	PUSH (%reg)
	addi	$sp,$sp,-4
	sw	%reg,0($sp)
.end_macro

.macro	POP (%reg)
	lw	%reg,0($sp)
	addi	$sp,$sp,4
.end_macro

.data


text1: 	  .asciiz "This is a string."	# 18 Byte string
text2:	  .asciiz "Yet another thing."	# 19 Byte string

.align 2
list1: 	.space 80 	# Here we define the storage!
list2: 	.space 80
count:	.word  0

.text
main:
	jal	work
stop:	j	stop

# function work()
work:
	PUSH	($ra)
	la 	$a0,text1	# Load in the memory adress to the string
	la	$a1,list1	# Load in the memory adress to the array
	la	$a2,count	# Load in the memory adress to the count value
	jal	copycodes	# Call the copycodes function

	la 	$a0,text2	# Load in the memory adress to the string
	la	$a1,list2	# Load in the memory adress to the array
	la	$a2,count	# Load in the memory adress to the count value
	jal	copycodes	# Call the copycodes function
	POP	($ra)


# function copycodes()
copycodes:
loop:
	# Copying the values
	lb	$t0,0($a0)	# Load in the value at the first byte position in $a0, store it in temp
	beq	$t0,$0,done	# If we get to NUL byte, branch to done (Exit the subroutine)
	sw	$t0,0($a1)	# Store the temp in the first byte position in $a1

	addi	$a0,$a0,1	# For char increment 1 byte
	addi	$a1,$a1,4	# For int increment 4 byte

	# Increment the counter
	lw	$t1,0($a2)	# Load the value to temp
	addi	$t1,$t1,1	# Increase the count
	sw	$t1,0($a2)	# Overwrite the old count value
	j	loop
done:

	jr	$ra
