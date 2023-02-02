///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//! \file
//! \author Reiex
//! \copyright The MIT License (MIT)
//! \date 2020-2022
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

#pragma once


#include <DejaVu/Core/CoreTypes.hpp>

#include <SciPP/SciPPDecl.hpp>
#include <Diskon/DiskonDecl.hpp>


#include <DejaVu/Core/Shape.hpp>
#include <DejaVu/Core/Pixel.hpp>
#include <DejaVu/Core/Image.hpp>


#pragma region djvPixelMacros

#pragma region djvPixelRight

#define DJV_PIXEL_RIGHT(BBehaviour, it, itRight, width, height, preWidth, preHeight, i, j)	\
if (i == 0)																					\
{																							\
	itRight = it + 1;																		\
}																							\
else if (i < preWidth)																		\
{																							\
	++itRight;																				\
}																							\
else																						\
{																							\
	if constexpr (BBehaviour == scp::BorderBehaviour::Zero)									\
	{																						\
		itRight = &_zeroColor;																\
	}																						\
	else if constexpr (BBehaviour == scp::BorderBehaviour::Continuous)						\
	{																						\
		itRight = it;																		\
	}																						\
	else if constexpr (BBehaviour == scp::BorderBehaviour::Periodic)						\
	{																						\
		itRight = it - preWidth;															\
	}																						\
}

#pragma endregion

#pragma region djvPixelLeft

#define DJV_PIXEL_LEFT(BBehaviour, it, itLeft, width, height, preWidth, preHeight, i, j)	\
if (i > 1)																					\
{																							\
	++itLeft;																				\
}																							\
else if (i == 1)																			\
{																							\
	itLeft = it - 1;																		\
}																							\
else																						\
{																							\
	if constexpr (BBehaviour == scp::BorderBehaviour::Zero)									\
	{																						\
		itLeft = &_zeroColor;																\
	}																						\
	else if constexpr (BBehaviour == scp::BorderBehaviour::Continuous)						\
	{																						\
		itLeft = it;																		\
	}																						\
	else if constexpr (BBehaviour == scp::BorderBehaviour::Periodic)						\
	{																						\
		itLeft = it + preWidth;																\
	}																						\
}

#pragma endregion

#pragma region djvPixelDown

#define DJV_PIXEL_DOWN(BBehaviour, it, itDown, width, height, preWidth, preHeight, i, j)			\
if (i == 0 && j == 0)																				\
{																									\
	itDown = it + width;																			\
}																									\
else if (j < preHeight || (j == preHeight && i != 0 && BBehaviour != scp::BorderBehaviour::Zero))	\
{																									\
	++itDown;																						\
}																									\
else if (i == 0 && j == preHeight)																	\
{																									\
	if constexpr (BBehaviour == scp::BorderBehaviour::Zero)											\
	{																								\
		itDown = &_zeroColor;																		\
	}																								\
	else if constexpr (BBehaviour == scp::BorderBehaviour::Continuous)								\
	{																								\
		itDown = it;																				\
	}																								\
	else if constexpr (BBehaviour == scp::BorderBehaviour::Periodic)								\
	{																								\
		itDown = it - preHeight * width;															\
	}																								\
}

#pragma endregion

#pragma region djvPixelUp

#define DJV_PIXEL_UP(BBehaviour, it, itUp, width, height, preWidth, preHeight, i, j)					\
if (j > 1 || (j == 1 && i != 0) || ((i != 0 || j != 0) && BBehaviour != scp::BorderBehaviour::Zero))	\
{																										\
	++itUp;																								\
}																										\
else if (i == 0 && j == 0)																				\
{																										\
	if constexpr (BBehaviour == scp::BorderBehaviour::Zero)												\
	{																									\
		itUp = &_zeroColor;																				\
	}																									\
	else if constexpr (BBehaviour == scp::BorderBehaviour::Continuous)									\
	{																									\
		itUp = it;																						\
	}																									\
	else if constexpr (BBehaviour == scp::BorderBehaviour::Periodic)									\
	{																									\
		itUp = it + preHeight * width;																	\
	}																									\
}																										\
else if (j == 1 && i == 0)																				\
{																										\
	itUp = it - width;																					\
}

#pragma endregion

#pragma region djvPixelRightDown

#define DJV_PIXEL_RIGHT_DOWN(BBehaviour, it, itRightDown, width, height, preWidth, preHeight, i, j)	\
if (i == 0)																							\
{																									\
	if (j == preHeight)																				\
	{																								\
		if constexpr (BBehaviour == scp::BorderBehaviour::Zero)										\
		{																							\
			itRightDown = &_zeroColor;																\
		}																							\
		else if constexpr (BBehaviour == scp::BorderBehaviour::Continuous)							\
		{																							\
			itRightDown = it + 1;																	\
		}																							\
		else if constexpr (BBehaviour == scp::BorderBehaviour::Periodic)							\
		{																							\
			itRightDown = it - preHeight * width + 1;												\
		}																							\
	}																								\
	else																							\
	{																								\
		itRightDown = it + width + 1;																\
	}																								\
}																									\
else if (i < preWidth)																				\
{																									\
	if constexpr (BBehaviour == scp::BorderBehaviour::Zero)											\
	{																								\
		if (j < preHeight)																			\
		{																							\
			++itRightDown;																			\
		}																							\
	}																								\
	else																							\
	{																								\
		++itRightDown;																				\
	}																								\
}																									\
else																								\
{																									\
	if constexpr (BBehaviour == scp::BorderBehaviour::Zero)											\
	{																								\
		/* Nothing */																				\
	}																								\
	else if constexpr (BBehaviour == scp::BorderBehaviour::Continuous)								\
	{																								\
		if (j == preHeight)																			\
		{																							\
			itRightDown = it;																		\
		}																							\
		else																						\
		{																							\
			itRightDown = it + width;																\
		}																							\
	}																								\
	else if constexpr (BBehaviour == scp::BorderBehaviour::Periodic)								\
	{																								\
		if (j == preHeight)																			\
		{																							\
			itRightDown = it - width * height + 1;													\
		}																							\
		else																						\
		{																							\
			itRightDown = it + 1;																	\
		}																							\
	}																								\
}

#pragma endregion

#pragma region djvPixelLeftDown

#define DJV_PIXEL_LEFT_DOWN(BBehaviour, it, itLeftDown, width, height, preWidth, preHeight, i, j)	\
if (i > 1)																							\
{																									\
	if constexpr (BBehaviour == scp::BorderBehaviour::Zero)											\
	{																								\
		if (j != preHeight)																			\
		{																							\
			++itLeftDown;																			\
		}																							\
	}																								\
	else																							\
	{																								\
		++itLeftDown;																				\
	}																								\
}																									\
else if (i == 1)																					\
{																									\
	if (j == preHeight)																				\
	{																								\
		if constexpr (BBehaviour == scp::BorderBehaviour::Zero)										\
		{																							\
			/* Nothing */																			\
		}																							\
		else if constexpr (BBehaviour == scp::BorderBehaviour::Continuous)							\
		{																							\
			itLeftDown = it - 1;																	\
		}																							\
		else if constexpr (BBehaviour == scp::BorderBehaviour::Periodic)							\
		{																							\
			itLeftDown = it - preHeight * width - 1;												\
		}																							\
	}																								\
	else																							\
	{																								\
		itLeftDown = it + preWidth;																	\
	}																								\
}																									\
else																								\
{																									\
	if constexpr (BBehaviour == scp::BorderBehaviour::Zero)											\
	{																								\
		itLeftDown = &_zeroColor;																	\
	}																								\
	else if constexpr (BBehaviour == scp::BorderBehaviour::Continuous)								\
	{																								\
		if (j == preHeight)																			\
		{																							\
			itLeftDown = it;																		\
		}																							\
		else																						\
		{																							\
			itLeftDown = it + width;																\
		}																							\
	}																								\
	else if constexpr (BBehaviour == scp::BorderBehaviour::Periodic)								\
	{																								\
		if (j == preHeight)																			\
		{																							\
			itLeftDown = it - preHeight * width + preWidth;											\
		}																							\
		else																						\
		{																							\
			itLeftDown = it + width + preWidth;														\
		}																							\
	}																								\
}

#pragma endregion

#pragma region djvPixelRightUp

#define DJV_PIXEL_RIGHT_UP(BBehaviour, it, itRightUp, width, height, preWidth, preHeight, i, j)	\
if (i == 0)																						\
{																								\
	if (j == 0)																					\
	{																							\
		if constexpr (BBehaviour == scp::BorderBehaviour::Zero)									\
		{																						\
			itRightUp = &_zeroColor;															\
		}																						\
		else if constexpr (BBehaviour == scp::BorderBehaviour::Continuous)						\
		{																						\
			itRightUp = it + 1;																	\
		}																						\
		else if constexpr (BBehaviour == scp::BorderBehaviour::Periodic)						\
		{																						\
			itRightUp = it + preHeight * width + 1;												\
		}																						\
	}																							\
	else																						\
	{																							\
		itRightUp = it - width + 1;																\
	}																							\
}																								\
else if (i < preWidth)																			\
{																								\
	if constexpr (BBehaviour == scp::BorderBehaviour::Zero)										\
	{																							\
		if (j != 0)																				\
		{																						\
			++itRightUp;																		\
		}																						\
	}																							\
	else																						\
	{																							\
		++itRightUp;																			\
	}																							\
}																								\
else																							\
{																								\
	if constexpr (BBehaviour == scp::BorderBehaviour::Zero)										\
	{																							\
		itRightUp = &_zeroColor;																\
	}																							\
	else if constexpr (BBehaviour == scp::BorderBehaviour::Continuous)							\
	{																							\
		if (j == 0)																				\
		{																						\
			itRightUp = it;																		\
		}																						\
		else																					\
		{																						\
			itRightUp = it - width;																\
		}																						\
	}																							\
	else if constexpr (BBehaviour == scp::BorderBehaviour::Periodic)							\
	{																							\
		if (j == 0)																				\
		{																						\
			itRightUp = it + preHeight * width - preWidth;										\
		}																						\
		else																					\
		{																						\
			itRightUp = it - width - preWidth;													\
		}																						\
	}																							\
}

#pragma endregion

#pragma region djvPixelLeftUp

#define DJV_PIXEL_LEFT_UP(BBehaviour, it, itLeftUp, width, height, preWidth, preHeight, i, j)	\
if (i > 1)																						\
{																								\
	if constexpr (BBehaviour == scp::BorderBehaviour::Zero)										\
	{																							\
		if (j != 0)																				\
		{																						\
			++itLeftUp;																			\
		}																						\
	}																							\
	else																						\
	{																							\
		++itLeftUp;																				\
	}																							\
}																								\
else if (i == 1)																				\
{																								\
	if (j == 0)																					\
	{																							\
		if constexpr (BBehaviour == scp::BorderBehaviour::Zero)									\
		{																						\
			/* Nothing */																		\
		}																						\
		else if constexpr (BBehaviour == scp::BorderBehaviour::Continuous)						\
		{																						\
			itLeftUp = it - 1;																	\
		}																						\
		else if constexpr (BBehaviour == scp::BorderBehaviour::Periodic)						\
		{																						\
			itLeftUp = it - 1 + preHeight * width;												\
		}																						\
	}																							\
	else																						\
	{																							\
		itLeftUp = it - width - 1;																\
	}																							\
}																								\
else																							\
{																								\
	if constexpr (BBehaviour == scp::BorderBehaviour::Zero)										\
	{																							\
		itLeftUp = &_zeroColor;																	\
	}																							\
	else if constexpr (BBehaviour == scp::BorderBehaviour::Continuous)							\
	{																							\
		if (j == 0)																				\
		{																						\
			itLeftUp = it;																		\
		}																						\
		else																					\
		{																						\
			itLeftUp = it - width;																\
		}																						\
	}																							\
	else if constexpr (BBehaviour == scp::BorderBehaviour::Periodic)							\
	{																							\
		if (j == 0)																				\
		{																						\
			itLeftUp = it + width * height - 1;													\
		}																						\
		else																					\
		{																						\
			itLeftUp = it - 1;																	\
		}																						\
	}																							\
}

#pragma endregion

#pragma endregion
