#pragma once
#include <functional>

#define LINE_OVERLAP_NONE 0 	// No line overlap, like in standard Bresenham
#define LINE_OVERLAP_MAJOR 0x01 // Overlap - first go major then minor direction. Pixel is drawn as extension after actual line
#define LINE_OVERLAP_MINOR 0x02 // Overlap - first go minor then major direction. Pixel is drawn as extension before next line
#define LINE_OVERLAP_BOTH 0x03  // Overlap - both

#define LINE_THICKNESS_MIDDLE 0                 // Start point is on the line at center of the thick line
#define LINE_THICKNESS_DRAW_CLOCKWISE 1         // Start point is on the counter clockwise border line
#define LINE_THICKNESS_DRAW_COUNTERCLOCKWISE 2  // Start point is on the clockwise border line

class BresenhamsAlgorithm
{
public:
	static void DrawParallelogram(int aX, int aY, int bX, int bY, int cX, int cY, std::function<void(int, int)> setPixel);

	static void DrawThickLine(
		int aXStart, int aYStart, int aXEnd, int aYEnd, int aThickness,
		std::function<void(int, int)> setPixel
	);

	static void DrawLine(int aXStart, int aYStart, int aXEnd, int aYEnd, uint8_t aOverlap,
		std::function<void(int, int)> setPixel);
};