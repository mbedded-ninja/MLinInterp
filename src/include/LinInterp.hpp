//!
//! @file 		LinInterp.h
//! @author 	Geoffrey Hunter <gbmhunter@gmail.com> (www.cladlab.com)
//! @edited 	n/a
//! @date 		2013/04/17
//! @brief 		Performs fast linear interpolation.
//! @details
//!				See README.rst in the repo's root dir for more info.

//===============================================================================================//
//====================================== HEADER GUARD ===========================================//
//===============================================================================================//

#ifndef LIN_INTERP_H
#define LIN_INTERP_H

#ifndef __cplusplus
	#error Please build with C++ compiler
#endif

#include <stdint.h>
#include <cstdio>
#include <stdio.h>

#include "./Point.hpp"
//#include "./Comms/include/UartDebug.h"

//===============================================================================================//
//======================================== NAMESPACE ============================================//
//===============================================================================================//

namespace LinInterpNs
{
	using namespace PointNs;

	//! Prints general debug messages.
	#define linInterpPRINT_DEBUG						(0)		

	//===============================================================================================//
	//=================================== PUBLIC TYPEDEFS ===========================================//
	//===============================================================================================//

	//! @brief		Enumeration of possible status's that can be returned in the InterpResult variable when 
	//!				an interpolation is run.
	typedef enum{
		OK,
		X_VALUE_OUT_OF_RANGE
	} status_t;
	
	//===============================================================================================//
	//================================== PUBLIC TEMPLATES ===========================================//
	//===============================================================================================//
	
	//! @brief		Helper class for LinInterp class. Used as the returned object
	//!				from LinInterp.Interp().
	template <class yType> class InterpResult
	{
		public:
			
			//! @brief		Stores the "status" of the conversion.
			//! @sa			status_t
			status_t status;

			//! @brief		The result of the linear interpolation.
			yType yVal;
			
			//! @brief		The section number that the interpolation occurred in.
			//! @details	Section 0 is before the first x-value in pointA, section 1 is between the 1st and 2nd
			//!				x-value, ..., section numPoints is after the last x-value in the pointA.
			uint32_t sectionNum;
	};

	template <class xType, class yType> class LinInterp
	{
		public:
			//! @brief		Constructor
			LinInterp();
			
			//! @brief		Performs linear interpolation of xVal, using the 
			//!				points defined in pointA.
			//! @returns	The result, of type InterpResult<yType> (a helper class).
			InterpResult<yType> Interp(xType xVal);
		
			//! @brief		Total number of points stored in pointA
			//! @todo		If user forgets to do this, memory leaks could occur.
			//! 				Make this automatic (use std::vector???)
			uint32_t numPoints;
			
			//! @brief 		Pointer to an array of x,y data points.
			Point<xType, yType>* pointA;
			
			//! @brief		Buffer for formulating debug messages.
			//! @todo		Make private
			char debugBuff[100];

		private:
			//! @brief		Prints debug information.
			//! @details	Platform dependant implementation (uses pre-compiler checks).
			void PrintDebug(const char* msg);
	};

	//! @brief 		Constructor
	template <class xType, class yType> LinInterp<xType, yType>::LinInterp()
	{
		#if(linInterpPRINT_DEBUG == 1)
			PrintDebug((char*)"LIN-INTERP: Initialising...\r\n");
		#endif
		
		// Nothing to do here
		
		#if(linInterpPRINT_DEBUG == 1)
			PrintDebug((char*)"LIN-INTERP: Initialisation complete.\r\n");
		#endif
	}

	//! @brief 		Interpolates xVal and returns yVal
	template <class xType, class yType> InterpResult<yType> LinInterp<xType, yType>::Interp(xType xVal)
	{
		// Check if xVal is below minimum xVal in pointA
		if(xVal < pointA[0].xVal)
		{
			InterpResult<yType> result;
				
			// x-value is beyond range in pointA
			result.status = X_VALUE_OUT_OF_RANGE;
			
			// Return the closest y-value, which is the one
			// at the start of the array
			result.yVal = pointA[0].yVal;
			
			// Section number is 0, which is the section before the first x-value in pointA
			result.sectionNum = 0;
			
			return result;
		}
	
		// Find which segment xVal is in
		// Start at i=1, since can't interpolate
		// with just one point
		uint32_t i = 1;

		while(pointA[i].xVal < xVal)
		{
			// Look for first x value in array which is greater than
			// x value of point to interpolate
			
			// Protection against exceeding array count
			if(i == numPoints - 1)
			{
				InterpResult<yType> result;
				
				// x-value is beyond range in pointA
				result.status = X_VALUE_OUT_OF_RANGE;
				
				// Return the closest y-value, which is the one
				// at the end of the array
				result.yVal = pointA[i].yVal;
				
				// The section is the one past the last x-value in the pointA, which is equal
				// to the number numPoints (which at this point in time is i + 1).
				result.sectionNum = i + 1;
				
				return result;
			}
			
			i++;
		}
		
		// xVal is now above pointA[i-1].xVal and below pointA[i].xVal
		
		#if(linInterpPRINT_DEBUG == 1)
			snprintf(debugBuff, sizeof(debugBuff), "LIN-INTERP: Index = %u\r\n", 
				i);
			PrintDebug(debugBuff);
		#endif

		// Calculate x and y differences
		int32_t xDiff = (int32_t)pointA[i].xVal - (int32_t)pointA[i-1].xVal;
		yType yDiff = pointA[i].yVal - pointA[i-1].yVal;
		
		#if(linInterpPRINT_DEBUG == 1)
			snprintf(debugBuff, sizeof(debugBuff), "LIN-INTERP: xDiff = %u, yDiff = %f\r\n", 
				xDiff, yDiff);
			PrintDebug(debugBuff);
		#endif
		
		InterpResult<yType> result;
		
		// If code has reached here, interpolation must of been successful
		result.status = OK;	
		
		//result.yVal = fp<FP_08>(((double)(xVal-pointA[i-1].xVal)*Fix2Float<FP_08>(yDiff.intValue))/(double)xDiff) + pointA[i-1].yVal;
		result.yVal = (yType)(((double)(xVal-pointA[i-1].xVal)*(yDiff))/(double)xDiff) + pointA[i-1].yVal;
		
		// When the x-value is within the range of pointA, sectionNum is equal to i
		result.sectionNum = i;

		return result;
	}

	template <class xType, class yType> void LinInterp<xType, yType>::PrintDebug(const char* msg)
	{
		// Implementation is platform-dependant
		#if(__linux__)
			printf(msg);
		#elif(PSOC)
			UartDebug::PutString(msg);
		#endif
	}

} // namespace LinInterpNs

#endif // #ifndef LIN_INTERP_H

// EOF
