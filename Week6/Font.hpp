#pragma once

#include <memory>
#include <string>
#include <math/vector.hpp>

typedef unsigned char byte;

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

class IFont
{
public:
	virtual ~IFont() {}

	virtual void preload(std::string const & characters) = 0;
	virtual byte const * const buffer() const = 0;
	virtual const int pitch() const = 0;
	virtual const size_t height() const = 0;
	virtual const size_t bpp() const = 0;
	virtual Glyph const getGlyph(char const character) = 0;
};

class FontFactoryImpl;

class FontFactory
{
public:
	FontFactory();
	void loadFont(std::string const & face, size_t const pixelHeight,
		std::shared_ptr<IFont> & font);

private:
	std::shared_ptr<FontFactoryImpl> _pimpl;
};
