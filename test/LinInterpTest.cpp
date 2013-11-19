//!
//! @file 		LinInterpTest.h
//! @author 	Geoffrey Hunter <gbmhunter@gmail.com> (www.cladlab.com)
//! @edited 	n/a
//! @date 		2013/05/23
//! @brief 		Performs unit tests on LinInterp library.
//! @details
//!				See README.rst in the repo's root dir for more info.

// Relys on point library working correctly

#include <stdio.h>
#include <stdint.h>

// LinInterp library (library under test)
#include "../src/include/LinInterp.hpp"

// Uses UnitTest++ library for testing.
#include "./UnitTest++/src/UnitTest++.h"

using namespace LinInterpNs;

TEST(TestInPositiveNumberSpaceWithPositiveGradient)
{
	LinInterp<double, double> linInterp;
	Point<double, double> pointA[2];
	pointA[0].xVal = 0.0;
	pointA[0].yVal = 0.0;
	pointA[1].xVal = 1.0;
	pointA[1].yVal = 1.0;

	linInterp.pointA = pointA;
	linInterp.numPoints = 2;
	
	InterpResult<double> result = linInterp.Interp(0.5);

	CHECK_EQUAL(OK, result.status);
	CHECK_EQUAL(0.5, result.yVal);
}

TEST(TestInPositiveNumberSpaceWithNegativeGradient)
{
	LinInterp<double, double> linInterp;
	Point<double, double> pointA[2];
	pointA[0].xVal = 0.0;
	pointA[0].yVal = 1.0;
	pointA[1].xVal = 1.0;
	pointA[1].yVal = 0.0;

	linInterp.pointA = pointA;
	linInterp.numPoints = 2;
	
	InterpResult<double> result = linInterp.Interp(0.5);

	CHECK_EQUAL(OK, result.status);
	CHECK_EQUAL(0.5, result.yVal);
}

TEST(TestInNegativeNumberSpaceWithPositiveGradient)
{
	LinInterp<double, double> linInterp;
	Point<double, double> pointA[2];
	pointA[0].xVal = 0.0;
	pointA[0].yVal = -1.0;
	pointA[1].xVal = 1.0;
	pointA[1].yVal = 0.0;

	linInterp.pointA = pointA;
	linInterp.numPoints = 2;
	
	InterpResult<double> result = linInterp.Interp(0.5);
	
	CHECK_EQUAL(OK, result.status);
	CHECK_EQUAL(-0.5, result.yVal);
}

TEST(TestInNegativeNumberSpaceWithNegativeGradient)
{
	LinInterp<double, double> linInterp;
	Point<double, double> pointA[2];
	pointA[0].xVal = 0.0;
	pointA[0].yVal = 0.0;
	pointA[1].xVal = 1.0;
	pointA[1].yVal = -1.0;

	linInterp.pointA = pointA;
	linInterp.numPoints = 2;
	
	InterpResult<double> result = linInterp.Interp(0.5);
	
	CHECK_EQUAL(OK, result.status);
	CHECK_EQUAL(-0.5, result.yVal);
}

TEST(ZeroGradientTest)
{
	LinInterp<double, double> linInterp;
	Point<double, double> pointA[2];
	pointA[0].xVal = 0.0;
	pointA[0].yVal = 1.0;
	pointA[1].xVal = 1.0;
	pointA[1].yVal = 1.0;

	linInterp.pointA = pointA;
	linInterp.numPoints = 2;
	
	InterpResult<double> result = linInterp.Interp(0.5);
	
	CHECK_EQUAL(OK, result.status);
	CHECK_EQUAL(1.0, result.yVal);
}

/*
TEST(XValuesDoNotIncreaseMonotomicallyTest)
{
	LinInterp<double, double> linInterp;
	Point<double, double> pointA[2];
	pointA[0].xVal = 1.0;
	pointA[0].yVal = 0.0;
	pointA[1].xVal = 0.0;
	pointA[1].yVal = 1.0;

	linInterp.pointA = pointA;
	linInterp.numPoints = 2;
	
	InterpResult<double> result = linInterp.Interp(0.5);
	
	// LinInterp.Interp() should of returned success == false,
	// and yVal == 0.
	CHECK_EQUAL(OK, result.status);
	CHECK_CLOSE(0, result.yVal, 0.01);
}
*/

TEST(xValueBelowMinimumTest)
{
	LinInterp<double, double> linInterp;
	Point<double, double> pointA[2];
	pointA[0].xVal = 0.0;
	pointA[0].yVal = 0.0;
	pointA[1].xVal = 1.0;
	pointA[1].yVal = 1.0;

	linInterp.pointA = pointA;
	linInterp.numPoints = 2;
	
	// Ask for interpolation above maximum x-value in point array
	InterpResult<double> result = linInterp.Interp(-1.0);
	
	// LinInterp.Interp() should of returned closest y-value, and status X_VALUE_OUT_OF_RANGE
	CHECK_EQUAL(X_VALUE_OUT_OF_RANGE, result.status);
	CHECK_CLOSE(0.0, result.yVal, 0.01);
}

TEST(xValueAboveMaximumTest)
{
	LinInterp<double, double> linInterp;
	Point<double, double> pointA[2];
	pointA[0].xVal = 0.0;
	pointA[0].yVal = 0.0;
	pointA[1].xVal = 1.0;
	pointA[1].yVal = 1.0;

	linInterp.pointA = pointA;
	linInterp.numPoints = 2;
	
	// Ask for interpolation above maximum x-value in point array
	InterpResult<double> result = linInterp.Interp(2.0);
	
	// LinInterp.Interp() should of returned closest y-value, and status X_VALUE_OUT_OF_RANGE
	CHECK_EQUAL(X_VALUE_OUT_OF_RANGE, result.status);
	CHECK_CLOSE(1.0, result.yVal, 0.01);
}

TEST(OnlyLooksAtNumPointsTest)
{
	LinInterp<double, double> linInterp;
	Point<double, double> pointA[3];
	pointA[0].xVal = 0.0;
	pointA[0].yVal = 0.0;
	pointA[1].xVal = 1.0;
	pointA[1].yVal = 1.0;
	pointA[2].xVal = 2.0;
	pointA[2].yVal = 2.0;

	linInterp.pointA = pointA;
	
	// Only tell the engine about 2 of the 3 points
	linInterp.numPoints = 2;
	
	// Ask for interpolation above maximum x-value in point array
	InterpResult<double> result = linInterp.Interp(1.5);
	
	// LinInterp.Interp() should of returned closest y-value, and status X_VALUE_OUT_OF_RANGE
	CHECK_EQUAL(X_VALUE_OUT_OF_RANGE, result.status);
	CHECK_CLOSE(1.0, result.yVal, 0.01);
}

TEST(SectionNumTest)
{
	LinInterp<double, double> linInterp;
	Point<double, double> pointA[3];
	pointA[0].xVal = 0.0;
	pointA[0].yVal = 0.0;
	pointA[1].xVal = 1.0;
	pointA[1].yVal = 1.0;
	pointA[2].xVal = 2.0;
	pointA[2].yVal = 2.0;

	linInterp.pointA = pointA;
	
	linInterp.numPoints = 3;
	
	// Ask for interpolation below minimum x-value in point array
	InterpResult<double> result = linInterp.Interp(-1);	
	CHECK_EQUAL(0, result.sectionNum);
	
	// Ask for interpolation between pointA[0] and pointA[1]
	result = linInterp.Interp(0.5);
	CHECK_EQUAL(1, result.sectionNum);
	
	// Ask for interpolation between pointA[1] and pointA[2]
	result = linInterp.Interp(1.5);	
	CHECK_EQUAL(2, result.sectionNum);
	
	// Ask for interpolation above pointA[2]
	result = linInterp.Interp(2.5);	
	CHECK_EQUAL(3, result.sectionNum);
}

int main()
{
	// Run unit tests
	return UnitTest::RunAllTests();
}
