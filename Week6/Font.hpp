#pragma once

#include <memory>
#include <string>
#include <math/vector.hpp>

typedef unsigned char byte;

/* Provides information to find glyph bitmap within font map. */
class Glyph
{
public:
	Glyph()
	{
	}
	
	Glyph(vec2<size_t> const topLeft, vec2<size_t> const size) :
		_topLeft(topLeft), _size(size)
	{
	}

	Glyph& operator=(Glyph const & rhs)
	{
		_topLeft = rhs._topLeft;
		_size = rhs._size;

		return *this;
	}

	vec2<size_t> const topLeft() const { return _topLeft; }

	vec2<size_t> const size() const { return _size; }

private:
	vec2<size_t> _topLeft;
	vec2<size_t> _size;
};

/* Provides an interface to render glyphs to an image buffer and read glyphs
   back from the image buffer. */
class IFont
{
public:
	virtual ~IFont() {}

	/* Render glyphs */
	virtual void preload(std::string const & characters) = 0;

	/* Access image buffer */
	virtual byte const * const buffer() const = 0;

	/* Pitch of image buffer */
	virtual const int pitch() const = 0;

	/* Height of image buffer */
	virtual const size_t height() const = 0;

	/* Bits per pixel */
	virtual const size_t bpp() const = 0;

	/* Get glyph info */
	virtual Glyph const getGlyph(char const character) = 0;
};

class FontFactoryImpl;

/* Loads the font */
class FontFactory
{
public:
	FontFactory();
	void loadFont(std::string const & face, size_t const pixelHeight,
		std::shared_ptr<IFont> & font);

private:
	std::shared_ptr<FontFactoryImpl> _pimpl;
};
