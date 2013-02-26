#pragma once

#include <ostream>

template <class T>
class vec4
{
private:
	static const size_t _x = 0;
	static const size_t _y = 1;
	static const size_t _z = 2;
	static const size_t _w = 3;

	T _values[4];

	void ctor(T const & x, T const & y, T const & z, T const & w)
	{
		_values[_x] = x;
		_values[_y] = y;
		_values[_z] = z;
		_values[_w] = w;
	}

	void ctor(T const & x, T const & y, T const & z)
	{
		ctor(x, y, z, 1);
	}

public:
	T const & X() const
	{
		return _values[_x];
	}

	void X(T const & x)
	{
		_values[_x] = x;
	}
	
	T const & Y() const
	{
		return _values[_y];
	}

	void Y(T const & y)
	{
		_values[_y] = y;
	}

	T const & Z() const
	{
		return _values[_z];
	}

	void Z(T const & z)
	{
		_values[_z] = z;
	}

	T const & W() const
	{
		return _values[_w];
	}

	void W(T const & w)
	{
		_values[_w] = w;
	}

	vec4()
	{
		ctor(0, 0, 0);
	}

	vec4(T const & x, T const & y, T const & z)
	{
		ctor(x, y, z);
	}
	
	vec4(T const & x, T const & y, T const & z, T const & w)
	{
		ctor(x, y, z, w);
	}

	vec4(vec4 const & rhs)
	{
		ctor(rhs.X(), rhs.Y(), rhs.Z(), rhs.W());
	}

	vec4& operator=(vec4 const & rhs)
	{
		ctor(rhs.X(), rhs.Y(), rhs.Z(), rhs.W());
		return *this;
	}

	vec4 makeHomogeneous()
	{
		return vec4(X()/W(), Y()/W(), Z()/W(), W()/W());
	}

	friend std::ostream& operator<< (std::ostream& stream, vec4 const & vec)
	{
		return stream << "{ " << vec.X() << ", " << vec.Y() << ", " <<
			vec.Z() << " }";
	}
};

template <class T>
class vec2
{
private:
	static const size_t _x = 0;
	static const size_t _y = 1;

	T _values[2];

	void ctor(T const & x, T const & y)
	{
		_values[_x] = x;
		_values[_y] = y;
	}

public:
	T const & X() const
	{
		return _values[_x];
	}

	void X(T const & x)
	{
		_values[_x] = x;
	}
	
	T const & Y() const
	{
		return _values[_y];
	}

	void Y(T const & y)
	{
		_values[_y] = y;
	}

	vec2()
	{
		ctor(0, 0);
	}

	vec2(T const & x, T const & y)
	{
		ctor(x, y);
	}


	vec2(vec2 const & rhs)
	{
		ctor(rhs.X(), rhs.Y());
	}

	vec2& operator=(vec2 const & rhs)
	{
		ctor(rhs.X(), rhs.Y());
		return *this;
	}

	friend std::ostream& operator<< (std::ostream& stream, vec2 const & vec)
	{
		return stream << "{ " << vec.X() << ", " << vec.Y() << " }";
	}
};
