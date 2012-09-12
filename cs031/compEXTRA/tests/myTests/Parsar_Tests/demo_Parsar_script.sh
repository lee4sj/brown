cd /home/sl136/course/cs031/comp/tests/myTests/Parsar_Tests

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
	compiler-demo tests/myTests/Parsar_Tests/$file tests/myTests/Parsar_Tests/$mips

	printf "\nMyCompiler\n\n"

	
	#Run my compiler
	ant run -Dblaise=tests/myTests/Parsar_Tests/$file -Dmal=tests/myTests/Parsar_Tests/$mips

	#Wait for the user to give input before continuing
# 	read
done
