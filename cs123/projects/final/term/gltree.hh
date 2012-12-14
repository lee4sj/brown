#ifndef GLTREE_H
#define GLTREE_H

#include "props.hh"

enum {
	DEFAULT_SCALE = 0,
	STEM_DETAIL,
	STEM_RADIUS,
	BRANCHING_SEGMENTS,
	BRANCHING_AROUND,
	STEM_STEM_RATIO,
	BRANCH_STEM_RATIO,
	BRANCHING_ANGLE_V,
	BRANCHING_ANGLE_H,
	HELICAL_TWIST_X,
	HELICAL_TWIST_Y,
	HELICAL_TWIST_LENGTH,
	COLOR_RED,
	COLOR_GREEN,
	COLOR_BLUE,
	DRAW_MODE,
	ANIMATION_BRANCHING_ANGLE_V,
	ANIMATION_HELICAL_TWIST_X,
	ANIMATION_HELICAL_TWIST_Y,
	ANIMATION_BRANCHING_ANGLE_H,
	TDNA_END
};

#define TDNA_ANIMATION ANIMATION_BRANCHING_ANGLE_V

class GLtree {
	protected:
		struct tdna dna [TDNA_END];
		
		void transform_stem ();
		void transform_branch ();
		void transform_branch_multi ();
		void transform_helical ();
	public:
		GLtree	();
		~GLtree ();
		void load (FILE * properties);
		void draw (int depth = 0);
};
#endif
