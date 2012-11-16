// define targa public constants

#define TARGA_COLOR_RED										1
#define TARGA_COLOR_GREEN									2
#define TARGA_COLOR_BLUE									3
#define TARGA_COLOR_ALPHA									4


// define targa public data types

struct Targa {
    int width;
    int height;
    int imageLength;
    unsigned char *image;
};


// declare targa public functions

/**
 * targa_init()
 *
 * Initilize the Targa structure for utilization.
 *
 * @param	targa(in)	The Targa struct to initialize.
 *
 * @return	An integer where zero is pass, less than zero is failure.
 */
int targa_init(Targa *targa);


/**
 * targa_free()
 *
 * Free the Targa structure.
 *
 * @param	targa(in)	The Targa struct to free.
 *
 * @return	An integer where zero is pass, less than zero is failure.
 */
int targa_free(Targa *targa);


/**
 * targa_getDimensions()
 *
 * Obtain the width and height in pixels of a loaded Targa image.
 *
 * @param	targa(in)	The Targa struct of a loaded image.
 *
 * @param	width(out)	The width in pixels of a loaded Targa image.
 *
 * @param	height(out)	The height in pixels of a loaded Targa image.
 *
 * @return	An integer where zero is pass, less than zero is failure.
 */
int targa_getDimensions(Targa *targa, int *width, int *height);


/**
 * targa_getImageLength()
 *
 * Obtain the length in bytes of the serialized RGBA image.
 *
 * @param	targa(in)			The Targa struct of a loaded image.
 *
 * @param	imageLength(out)	The length in bytes of the RGBA image.
 *
 * @return	An integer where zero is pass, less than zero is failure.
 */
int targa_getImageLength(Targa *targa, int *imageLength);


/**
 * targa_getRgbaTexture()
 *
 * Obtain the serialized RGBA texture and its' length from a Targa image.
 *
 * @param	targa(in)			The Targa struct of a loaded image.
 *
 * @param	texture(out)		The serialized RGBA image pointer.
 *
 * @param	textureLength(out)	The serialized RGBA image length in bytes.
 *
 * @return	An integer where zero is pass, less than zero is failure.
 */
int targa_getRgbaTexture(Targa *targa, char **texture, int *textureLength);


/**
 * targa_loadFromFile()
 *
 * Load a targa file and decode it into a 32-bit RGBA serialized image.
 *
 * @param	targa(in)		The Targa struct of an image to load.
 *
 * @param	filename(in)	The filename of the image to load.
 *
 * @return	An integer where zero is pass, less than zero is failure.
 */
int targa_loadFromFile(Targa *targa, char *filename);


/**
 * targa_loadFromData()
 *
 * Load the targa from an in-memory location and decode it into a 32-bit RGBA
 * serialize image.
 *
 * @param	targa(in)		The Targa struct an image to load.
 *
 * @param	data(in)		A pointer to an in-memory location containing a
 *							Targa image.
 *
 * @param	dataLength(in)	The length of the Targa in-memory image.
 *
 * @return	An integer where zero is pass, less than zero is failure.
 */
int targa_loadFromData(Targa *targa, unsigned char *data, int dataLength);


/**
 * targa_applyRgbaMask()
 *
 * Apply a red, green, blue or alpha-channel additive color-mask to a loaded
 * Targa image.
 *
 * @param	targa(in)		The Targa struct of a loaded image.
 *
 * @param	colorType(in)	The color channel to mask.
 *
 * @param	value(in)		The color code (0 - 255) to mask.
 *
 * @return	An integer where zero is pass, less than zero is failure.
 */
int targa_applyRgbaMask(Targa *targa, int colorType, unsigned char value);


/**
 * targa_setRgbaChannel()
 *
 * Apply a red, green, blue or alpha-channel additive color-channel
 * replacement to a loaded Targa image.
 *
 * @param	targa(in)		The Targa struct of a loaded image.
 *
 * @param	colorType(in)	The color channel to replace.
 *
 * @param	value(in)		The color code (0 - 255) to replace.
 *
 * @return	An integer where zero is pass, less than zero is failure.
 */
int targa_setRgbaChannel(Targa *targa, int colorType, unsigned char value);
