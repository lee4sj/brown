#!/usr/local/bin/bash

#http://www.linuxconfig.org/Bash_scripting_Tutorial#3-1-global-vs-local-variables
#http://tldp.org/LDP/abs/html/string-manipulation.html
#http://www.linuxquestions.org/questions/programming-9/bash-script-to-access-all-files-in-a-directory-283582/
#http://ubuntuforums.org/showthread.php?t=708438
#http://www.cyberciti.biz/faq/bash-loop-over-file/


cd /home/sl136/course/cs031/comp/tests/myTests/Semantic_Tests

#File definition
#file="output.bl"

for file in `dir -d *.bl` ; do
	cd /home/sl136/course/cs031/comp/

	#Remove the .bl file extension and replace it with .s
	mips=$file
	mips=${mips/.bl/.s}	

	printf "\n"$file
	printf "\n"$mips

	printf "\n\nCompiler Demo\n\n"

	#Run the compiler demo
	compiler-demo tests/myTests/Semantic_Tests/$file tests/myTests/Semantic_Tests/$mips

	printf "\nMyCompiler\n\n"

	
	#Run my compiler
	ant run -Dblaise=tests/myTests/Semantic_Tests/$file -Dmal=tests/myTests/Semantic_Tests/$mips

	#Wait for the user to give input before continuing
# 	read
done
