#!/usr/local/bin/bash

#A script for compiling and running the .bl files in the specified directory

#Resources
#http://www.cyberciti.biz/faq/bash-for-loop-array/

cd /home/sl136/course/cs031/comp/tests

#Array of files
files=( 'fibs.bl' 'countdown.bl' 'stats.bl' )

#For all .bl files in the directory
#for file in `dir -d *.bl` ; do
for file in "${files[@]}" ; do
	#Remove the .bl file extension and replace it with _demo.s
	#This is where the demo will store its output
	mipsDemo=$file
	mipsDemo=${mipsDemo/.bl/_demo.s}	

	printf "\n"$file

	printf "\n\nCompiler Demo\n\n"

	#Run the compiler demo
	compiler-demo $file $mipsDemo

	#Run the produced code
	printf "\n\nDemo Outputs\n\n"
	mipscope -nox $mipsDemo

	printf "\nMyCompiler\n\n" 

	#Remove the .bl file extension and replace it with .s
	#This is where my compiler will store its output
	mipsMy=$file
	mipsMy=${mipsMy/.bl/.s}	

	#Run my compiler
	java -classpath ~/workspace/Compiler/bin comp.Compiler $file $mipsMy

	#Run the code produced by my compiler
	printf "\n\nMy Compiler Outputs\n\n"
	mipscope -nox $mipsMy
done