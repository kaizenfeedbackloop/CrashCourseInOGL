#pragma once

#include <ostream>

#include "vector.hpp"

template <class T>
class mat4
{
private:
	static const size_t _dimR = 4;
	static const size_t _dimC = 4;

	T _values[_dimC][_dimR];

	inline T & get(size_t const & row, size_t const & col)
	{
		return _values[col][row];
	}

	inline T const & get(size_t const & row, size_t const & col) const
	{
		return _values[col][row];
	}

	void ctor(T const & _00, T const & _01, T const & _02, T const & _03,
			  T const & _10, T const & _11, T const & _12, T const & _13,
			  T const & _20, T const & _21, T const & _22, T const & _23,
			  T const & _30, T const & _31, T const & _32, T const & _33)
	{
		get(0, 0) = _00;
		get(0, 1) = _01;
		get(0, 2) = _02;
		get(0, 3) = _03;
		get(1, 0) = _10;
		get(1, 1) = _11;
		get(1, 2) = _12;
		get(1, 3) = _13;
		get(2, 0) = _20;
		get(2, 1) = _21;
		get(2, 2) = _22;
		get(2, 3) = _23;
		get(3, 0) = _30;
		get(3, 1) = _31;
		get(3, 2) = _32;
		get(3, 3) = _33;
	}

	void ctor(mat4 const & rhs)
	{
		ctor(rhs(0,0), rhs(0,1), rhs(0,2), rhs(0,3),
			 rhs(1,0), rhs(1,1), rhs(1,2), rhs(1,3),
			 rhs(2,0), rhs(2,1), rhs(2,2), rhs(2,3),
			 rhs(3,0), rhs(3,1), rhs(3,2), rhs(3,3));
	}

public:
	T & operator()(size_t const & row, size_t const & col)
	{
		return get(row,col);
	}

	const T & operator()(size_t const & row, size_t const & col) const
	{
		return get(row,col);
	}

	static const mat4 Identity;

	mat4()
	{
		ctor(1, 0, 0, 0,
			 0, 1, 0, 0,
			 0, 0, 1, 0,
			 0, 0, 0, 1);
	}
	
	mat4(mat4 const & rhs)
	{
		ctor(rhs);
	}

	mat4(T const & _00, T const & _01, T const & _02, T const & _03,
			  T const & _10, T const & _11, T const & _12, T const & _13,
			  T const & _20, T const & _21, T const & _22, T const & _23,
			  T const & _30, T const & _31, T const & _32, T const & _33)
	{
		ctor(_00, _01, _02, _03,
			 _10, _11, _12, _13,
			 _20, _21, _22, _23,
			 _30, _31, _32, _33);
	}

	mat4 & operator=(mat4 const & rhs)
	{
		ctor(rhs);
		return *this;
	}

	mat4 operator*(mat4 const & rhs) const
	{
		mat4 res(
				 /* left.row0 & right.col0 */ 
				 get(0,0)*rhs(0,0) + get(0,1)*rhs(1,0) +
				 get(0,2)*rhs(2,0) + get(0,3)*rhs(3,0),
				 /* left.row0 & right.col1 */ 
				 get(0,0)*rhs(0,1) + get(0,1)*rhs(1,1) +
				 get(0,2)*rhs(2,1) + get(0,3)*rhs(3,1),
				 /* left.row0 & right.col2 */ 
				 get(0,0)*rhs(0,2) + get(0,1)*rhs(1,2) +
				 get(0,2)*rhs(2,2) + get(0,3)*rhs(3,2),
				 /* left.row0 & right.col3 */ 
				 get(0,0)*rhs(0,3) + get(0,1)*rhs(1,3) +
				 get(0,2)*rhs(2,3) + get(0,3)*rhs(3,3),

				 /* left.row1 & right.col0 */ 
				 get(1,0)*rhs(0,0) + get(1,1)*rhs(1,0) +
				 get(1,2)*rhs(2,0) + get(1,3)*rhs(3,0),
				 /* left.row1 & right.col1 */ 
				 get(1,0)*rhs(0,1) + get(1,1)*rhs(1,1) +
				 get(1,2)*rhs(2,1) + get(1,3)*rhs(3,1),
				 /* left.row1 & right.col2 */ 
				 get(1,0)*rhs(0,2) + get(1,1)*rhs(1,2) +
				 get(1,2)*rhs(2,2) + get(1,3)*rhs(3,2),
				 /* left.row1 & right.col3 */ 
				 get(1,0)*rhs(0,3) + get(1,1)*rhs(1,3) +
				 get(1,2)*rhs(2,3) + get(1,3)*rhs(3,3),

				 /* left.row1 & right.col0 */ 
				 get(2,0)*rhs(0,0) + get(2,1)*rhs(1,0) +
				 get(2,2)*rhs(2,0) + get(2,3)*rhs(3,0),
				 /* left.row2 & right.col1 */ 
				 get(2,0)*rhs(0,1) + get(2,1)*rhs(1,1) +
				 get(2,2)*rhs(2,1) + get(2,3)*rhs(3,1),
				 /* left.row2 & right.col2 */ 
				 get(2,0)*rhs(0,2) + get(2,1)*rhs(1,2) +
				 get(2,2)*rhs(2,2) + get(2,3)*rhs(3,2),
				 /* left.row2 & right.col3 */ 
				 get(2,0)*rhs(0,3) + get(2,1)*rhs(1,3) +
				 get(2,2)*rhs(2,3) + get(2,3)*rhs(3,3),

				 /* left.row3 & right.col0 */ 
				 get(3,0)*rhs(0,0) + get(3,1)*rhs(1,0) +
				 get(3,2)*rhs(2,0) + get(3,3)*rhs(3,0),
				 /* left.row3 & right.col1 */ 
				 get(3,0)*rhs(0,1) + get(3,1)*rhs(1,1) +
				 get(3,2)*rhs(2,1) + get(3,3)*rhs(3,1),
				 /* left.row3 & right.col2 */ 
				 get(3,0)*rhs(0,2) + get(3,1)*rhs(1,2) +
				 get(3,2)*rhs(2,2) + get(3,3)*rhs(3,2),
				 /* left.row3 & right.col3 */ 
				 get(3,0)*rhs(0,3) + get(3,1)*rhs(1,3) +
				 get(3,2)*rhs(2,3) + get(3,3)*rhs(3,3)
				 );
		return res;
	}


	vec4<T> operator*(vec4<T> const & rhs) const
	{
		return vec4<T>(
					   /* left.row0 & vector */
					   get(0,0)*rhs.X() + get(0,1)*rhs.Y() +
					   get(0,2)*rhs.Z() + get(0,3)*rhs.W(),
					   /* left.row1 & vector */
					   get(1,0)*rhs.X() + get(1,1)*rhs.Y() +
					   get(1,2)*rhs.Z() + get(1,3)*rhs.W(),
					   /* left.row2 & vector */
					   get(2,0)*rhs.X() + get(2,1)*rhs.Y() +
					   get(2,2)*rhs.Z() + get(2,3)*rhs.W(),
					   /* left.row3 & vector */
					   get(3,0)*rhs.X() + get(3,1)*rhs.Y() +
					   get(3,2)*rhs.Z() + get(3,3)*rhs.W()
					   );
	}

	friend std::ostream& operator<< (std::ostream& stream, mat4 const & vec)
	{
		return stream <<
			"{{"<<vec(0,0)<<","<<vec(0,1)<<","<<vec(0,2)<<","<<vec(0,3)<<"}"<<
			std::endl <<
			" {"<<vec(1,0)<<","<<vec(1,1)<<","<<vec(1,2)<<","<<vec(1,3)<<"}"<<
			std::endl <<
			" {"<<vec(2,0)<<","<<vec(2,1)<<","<<vec(2,2)<<","<<vec(2,3)<<"}"<<
			std::endl <<
			" {"<<vec(3,0)<<","<<vec(3,1)<<","<<vec(3,2)<<","<<vec(3,3)<<"}}";
	}
};

template <class T>
const mat4<T> mat4<T>::Identity;
