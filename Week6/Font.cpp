#include <iostream>
#include <map>
#include <cmath>

#include <ft2build.h>
#include <freetype/freetype.h>

#include "Font.hpp"

void printError(std::string const & errorStr)
{
	std::cerr << errorStr << std::endl;
}

class Font : public IFont
{
public:
	Font(FT_Face face) : _face(face), _pitch(1024), _height(1024), _bpp(8),
						 _buffer(new byte[_pitch * _height]), _insert(1,1),
						 _lineHeight(0)
	{
		memset(_buffer, 0, _pitch * _height);
	}

	virtual ~Font()
	{
		FT_Done_Face(_face);
		delete [] _buffer;
	}

	virtual void preload(std::string const & characters)
	{
		for(size_t i = 0; i < characters.size(); i++) {
			insertCharacter(characters[i]);
		}
	}

	virtual byte const * const buffer() const
	{
		return _buffer;
	}

	virtual const int pitch() const
	{
		return _pitch;
	}

	virtual const size_t height() const
	{
		return _height;
	}

	virtual const size_t bpp() const
	{
		return _bpp;
	}

	virtual Glyph const getGlyph(char const character)
	{
		return _cachedChars[character];
	}

private:
	void insertCharacter(char character)
	{
		FT_UInt glyphIndex = FT_Get_Char_Index(_face, character);
		if(glyphIndex == 0) {
			printError("Undefined character.");
			return;
		}

		FT_Error error = FT_Load_Glyph(_face, glyphIndex, FT_LOAD_DEFAULT);
		if(error) {
			printError("Could not load glyph.");
			return;
		}

		FT_GlyphSlot slot = _face->glyph;
		if(slot->format != FT_GLYPH_FORMAT_BITMAP) {
			error = FT_Render_Glyph(slot, FT_RENDER_MODE_NORMAL);
			if(error) {
				printError("Could not render glyph.");
				return;
			}
		}

		if(_insert.X() + slot->bitmap.width > static_cast<size_t>(_pitch)) {
			_insert = vec2<size_t>(_insert.X(), _insert.Y() + _lineHeight + 1);
			_lineHeight = 0;
		}


		byte * bufferPos = _buffer + _pitch * _insert.Y() + _insert.X();

		byte * const bitmapEnd = abs(slot->bitmap.pitch) * slot->bitmap.rows +
			slot->bitmap.buffer;
		size_t const bitmapPitch = abs(slot->bitmap.pitch);
		for(byte * bitmapPos = slot->bitmap.buffer; bitmapPos < bitmapEnd;
			bitmapPos += bitmapPitch) {
			memcpy(bufferPos, bitmapPos, slot->bitmap.width);

			bufferPos += _pitch;
		}

		vec2<size_t> size(slot->bitmap.width, slot->bitmap.rows);
		_cachedChars[character] = Glyph(_insert, size);

		_insert = vec2<size_t>(_insert.X() + size.X() + 1, _insert.Y());
		if(size.Y() > _lineHeight) {
			_lineHeight = size.Y();
		}
	}

	FT_Face _face;
	std::map<char, Glyph> _cachedChars;
	int const _pitch;
	size_t const _height;
	size_t const _bpp;
	byte* _buffer;
	vec2<size_t> _insert;
	size_t _lineHeight;
};

class FontFactoryImpl
{
public:
	FontFactoryImpl()
	{
		FT_Error error = FT_Init_FreeType(&Library);
		if(error) {
			printError("Failed to load freetype.");
		}
	}

	~FontFactoryImpl()
	{
		FT_Error error = FT_Done_FreeType(Library);
		if(error) {
			printError("Failed to load freetype.");
		}
	}

	FT_Library Library;
};

FontFactory::FontFactory() : _pimpl(new FontFactoryImpl())
{
}

void FontFactory::loadFont(std::string const & facePath,
	size_t const pixelHeight, std::shared_ptr<IFont> & font)
{
	FT_Face face;
	FT_Error error = FT_New_Face(_pimpl->Library, facePath.c_str(), 0,
		&face);
	if(error) {
		if(error == FT_Err_Unknown_File_Format) {
			printError("Unknown file format...");
		}

		printError("Failed to open file...");
	}

	error = FT_Set_Pixel_Sizes(face, 0, pixelHeight);
	if(error) {
		printError("Failed to set pixel height...");
	}

	font = std::make_shared<Font>(face);
}
