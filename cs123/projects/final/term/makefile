# Brett Foster 0239553

CFLAGS =  -DDOUBLE_BUFFER  -DLAB_MACHINES_SUCK -Wall
LFLAGS =  -lGL -lglut -lGLU -lm

CC = g++

all: pongish tree animate
		
pongish: pongish.cc gltree.cc props.cc circle.cc
	$(CC)  $(CFLAGS) -o pongish pongish.cc gltree.cc props.cc circle.cc $(LFLAGS)
tree: tree.cc
	$(CC)  $(CFLAGS) -o tree tree.cc $(LFLAGS)
animate: animate.cc
	$(CC)  $(CFLAGS) -o animate animate.cc $(LFLAGS)
	
test: all
	cat test.dna noanimate.dna noanimate.dna noanimate.dna noanimate.dna | ./pongish 0,0,0,0,0
test-precompute: all
	cat tree-stringy.dna noanimate.dna noanimate.dna noanimate.dna animate-long-fern.dna weirdo.dna noanimate.dna noanimate.dna noanimate.dna animate-long.dna tree-3.dna noanimate.dna noanimate.dna noanimate.dna noanimate.dna tree-2.dna noanimate.dna noanimate.dna noanimate.dna noanimate.dna | ./pongish 0,-50,100,0,160 1,-400,0,0,0 2,800,100,0,0 2,600,300,0,180 3,-800,100,0,0
tree-type: all
	@echo GENERATING tDNA SEQUENCES tree
	echo -n > test.dna
	@echo DEFAULT_SCALE
	./tree 1 30 0 >> test.dna
	@echo STEM_DETAIL
	./tree 1 5 0 >> test.dna
	@echo STEM_RADIUS
	./tree 128 0.5 0 >> test.dna
	@echo BRANCHING_SEGMENTS
	./tree 128 2  0 >> test.dna
	@echo BRANCHING_AROUND
	./tree 128 6 3 >> test.dna
	@echo STEM_STEM_RATIO
	./tree 128 0.95 0.005 >> test.dna
	@echo BRANCH_STEM_RATIO
	./tree 128 1 0 >> test.dna
	@echo BRANCHING_ANGLE_V
	./tree 128 30 20 >> test.dna
	@echo BRANCHING_ANGLE_H
	./tree 128 120 100 >> test.dna
	@echo HELICAL_TWIST_X
	./tree 128 0 8 >> test.dna
	@echo HELICAL_TWIST_Y
	./tree 128 0 8 >> test.dna
	@echo HELICAL_TWIST_LEN
	./tree 128 16 0 >> test.dna
	@echo COLOR_RED
	./tree 128 0.486 0.1 >> test.dna
	@echo COLOR_GREEN
	./tree 128 0.23 0.1 >> test.dna
	@echo COLOR_BLUE
	./tree 128 0.23 0.1 >> test.dna
	@echo DRAW_MODE
	./tree 1 1 0 >> test.dna
fern-type: all
	@echo GENERATING tDNA SEQUENCES fern
	echo -n > test.dna
	@echo DEFAULT_SCALE
	./tree 1 100 0 >> test.dna
	@echo STEM_DETAIL
	./tree 1 0 0 >> test.dna
	@echo STEM_RADIUS
	./tree 1 0 0 >> test.dna
	@echo BRANCHING_SEGMENTS
	./tree 1 2  0 >> test.dna
	@echo BRANCHING_AROUND
	./tree 1 16 0 >> test.dna
	@echo STEM_STEM_RATIO
	./tree 1 0.9 0 >> test.dna
	@echo BRANCH_STEM_RATIO
	./tree 1 0.8 0 >> test.dna
	@echo BRANCHING_ANGLE_V
	./tree 1 30 0 >> test.dna
	@echo BRANCHING_ANGLE_H
	./tree 1 180 0 >> test.dna
	@echo HELICAL_TWIST_X
	./tree 16 0 16 >> test.dna
	@echo HELICAL_TWIST_Y
	./tree 16 0 16 >> test.dna
	@echo HELICAL_TWIST_LEN
	./tree 16 4 4 >> test.dna
	@echo COLOR_RED
	./tree 1 0 0 >> test.dna
	@echo COLOR_GREEN
	./tree 16 0.7 0.3 >> test.dna
	@echo COLOR_BLUE
	./tree 16 0.7 0.3 >> test.dna
	@echo DRAW_MODE
	./tree 1 0 0 >> test.dna
clean:
	rm -f pongish tree animate
