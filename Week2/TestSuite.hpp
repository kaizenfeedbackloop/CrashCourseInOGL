#pragma once

#include <cxxtest/TestSuite.h>
#include <math/vector.hpp>
#include <math/matrix.hpp>
#include <cmath>

const float pi = acos(-1.0f);

class TextSuite : public CxxTest::TestSuite
{
public:
	void testSetup(void)
	{
		TS_ASSERT(true);
	}

	void testVec4(void)
	{
		vec4<int> vector(0, 1, 2);
	 	TS_ASSERT_EQUALS(vector.X(), 0);
		TS_ASSERT_EQUALS(vector.Y(), 1);
		TS_ASSERT_EQUALS(vector.Z(), 2);
	}

	void testMat4(void)
	{
		mat4<int> mat;
		std::cout << std::endl << mat << std::endl;

		TS_ASSERT_EQUALS(mat(0,0), 1);
		TS_ASSERT_EQUALS(mat(1,1), 1);
		TS_ASSERT_EQUALS(mat(2,2), 1);
		TS_ASSERT_EQUALS(mat(3,3), 1);
	}

	void testMatMultiply(void)
	{
		mat4<int> mat(0,1,2,3,
					  4,5,6,7,
					  8,9,10,11,
					  12,13,14,15);

		mat4<int> res = mat * mat4<int>::Identity;
		std::cout << std::endl << res << std::endl;

		TS_ASSERT_EQUALS(mat(0,0), 0);
		TS_ASSERT_EQUALS(mat(1,1), 5);
		TS_ASSERT_EQUALS(mat(2,2), 10);
		TS_ASSERT_EQUALS(mat(3,3), 15);
	}

	void testVecMultiply(void)
	{
		vec4<int> vec(0,1,2);

		vec4<int> res = mat4<int>::Identity * vec;
		std::cout << std::endl << res << std::endl;

		TS_ASSERT_EQUALS(res.X(), 0);
		TS_ASSERT_EQUALS(res.Y(), 1);
		TS_ASSERT_EQUALS(res.Z(), 2);
		TS_ASSERT_EQUALS(res.W(), 1);
	}
    
    void testAssignment(void)
    {
		mat4<float> scale(4.0f, 0.0f, 0.0f, 0.0f,
						  0.0f, 4.0f, 0.0f, 0.0f,
						  0.0f, 0.0f, 4.0f, 0.0f,
						  0.0f, 0.0f, 0.0f, 1.0f);

		std::cout << pi << std::endl;
		float theta = pi / 4.0f; 
		mat4<float> rotate(cos(theta), -sin(theta), 0.0f, 0.0f,
						   sin(theta), cos(theta), 0.0f, 0.0f,
						   0.0f, 0.0f, 1.0f, 0.0f,
						   0.0f, 0.0f, 0.0f, 1.0f);

		mat4<float> translate(1.0f, 0.0f, 0.0f, 0.0f,
							  0.0f, 1.0f, 0.0f, 0.0f,
							  0.0f, 0.0f, 1.0f, -8.0f,
							  0.0f, 0.0f, 0.0f, 1.0f);

		mat4<float> projection(1.0f, 0.0f, 0.0f, 0.0f,
							   0.0f, 1.0f, 0.0f, 0.0f,
							   0.0f, 0.0f, 1.0f, 0.0f,
							   0.0f, 0.0f, -0.25f, 0.0f);

		mat4<float> wvp;
		wvp = scale * wvp;
		std::cout << "scale:" << std::endl << wvp << std::endl;
		wvp = rotate * wvp;
		std::cout << "rotate:" << std::endl << wvp << std::endl;
		wvp = translate * wvp;
		std::cout << "translation:" << std::endl << wvp << std::endl;
		wvp = projection * wvp;
		std::cout << "projection:" << std::endl << wvp << std::endl;

		std::cout << "projected vertices:" << std::endl;
		std::cout << (wvp * vec4<float>(0.5f, 0.5f, 0.5f)).makeHomogeneous();
		std::cout << std::endl;
		std::cout << (wvp * vec4<float>(0.5f, 0.5f, -0.5f)).makeHomogeneous();
		std::cout << std::endl;
		std::cout << (wvp * vec4<float>(0.5f, -0.5f, 0.5f)).makeHomogeneous();
		std::cout << std::endl;
		std::cout << (wvp * vec4<float>(0.5f, -0.5f, -0.5f)).makeHomogeneous();
		std::cout << std::endl;
		std::cout << (wvp * vec4<float>(-0.5f, 0.5f, 0.5f)).makeHomogeneous();
		std::cout << std::endl;
		std::cout << (wvp * vec4<float>(-0.5f, 0.5f, -0.5f)).makeHomogeneous();
		std::cout << std::endl;
		std::cout << (wvp * vec4<float>(-0.5f, -0.5f, 0.5f)).makeHomogeneous();
		std::cout << std::endl;
		std::cout << (wvp * vec4<float>(-0.5f, -0.5f,-0.5f)).makeHomogeneous();
		std::cout << std::endl;
	}
};
