#include "BresenhamsAlgorithm.h"


void BresenhamsAlgorithm::DrawParallelogram(int aX, int aY, int bX, int bY, int cX, int cY, std::function<void(int, int)> setPixel)
{
    std::vector<std::pair<int, int>> acLine;
    auto acLineDraw = [&](int x, int y)
    {
        acLine.push_back(std::make_pair(x - aX, y - aY));
    };

    DrawLine(aX, aY, cX, cY, LINE_OVERLAP_NONE, acLineDraw);


    auto abDraw = [&](int x, int y)
    {
        for (auto [dx, dy] : acLine)
        {
            setPixel(x + dx, y + dy);
        }
    };

    DrawLine(aX, aY, bX, bY, LINE_OVERLAP_MINOR, abDraw);

}

void BresenhamsAlgorithm::DrawThickLine(int aXStart, int aYStart, int aXEnd, int aYEnd, int aThickness, std::function<void(int, int)> setPixel)
{
    int16_t i, tDeltaX, tDeltaY, tDeltaXTimes2, tDeltaYTimes2, tError, tStepX, tStepY;

    if (aThickness <= 1) {
        DrawLine(aXStart, aYStart, aXEnd, aYEnd, LINE_OVERLAP_NONE, setPixel);
    }
   

    /**
     * For coordinate system with 0.0 top left
     * Swap X and Y delta and calculate clockwise (new delta X inverted)
     * or counterclockwise (new delta Y inverted) rectangular direction.
     * The right rectangular direction for LINE_OVERLAP_MAJOR toggles with each octant
     */
    tDeltaY = aXEnd - aXStart;
    tDeltaX = aYEnd - aYStart;
    // mirror 4 quadrants to one and adjust deltas and stepping direction
    bool tSwap = true; // count effective mirroring
    if (tDeltaX < 0) {
        tDeltaX = -tDeltaX;
        tStepX = -1;
        tSwap = !tSwap;
    }
    else {
        tStepX = +1;
    }
    if (tDeltaY < 0) {
        tDeltaY = -tDeltaY;
        tStepY = -1;
        tSwap = !tSwap;
    }
    else {
        tStepY = +1;
    }
    tDeltaXTimes2 = tDeltaX << 1;
    tDeltaYTimes2 = tDeltaY << 1;
    bool tOverlap;
    // adjust for right direction of thickness from line origin
    int tDrawStartAdjustCount = aThickness / 2;
  

    /*
     * Now tDelta* are positive and tStep* define the direction
     * tSwap is false if we mirrored only once
     */
     // which octant are we now
    if (tDeltaX >= tDeltaY) {
        // Octant 1, 3, 5, 7 (between 0 and 45, 90 and 135, ... degree)
        if (tSwap) {
            tDrawStartAdjustCount = (aThickness - 1) - tDrawStartAdjustCount;
            tStepY = -tStepY;
        }
        else {
            tStepX = -tStepX;
        }
        /*
         * Vector for draw direction of the starting points of lines is rectangular and counterclockwise to main line direction
         * Therefore no pixel will be missed if LINE_OVERLAP_MAJOR is used on change in minor rectangular direction
         */
         // adjust draw start point
        tError = tDeltaYTimes2 - tDeltaX;
        for (i = tDrawStartAdjustCount; i > 0; i--) {
            // change X (main direction here)
            aXStart -= tStepX;
            aXEnd -= tStepX;
            if (tError >= 0) {
                // change Y
                aYStart -= tStepY;
                aYEnd -= tStepY;
                tError -= tDeltaXTimes2;
            }
            tError += tDeltaYTimes2;
        }
        //draw start line
        DrawLine(aXStart, aYStart, aXEnd, aYEnd, LINE_OVERLAP_NONE, setPixel);
        // draw aThickness number of lines
        tError = tDeltaYTimes2 - tDeltaX;
        for (i = aThickness; i > 1; i--) {
            // change X (main direction here)
            aXStart += tStepX;
            aXEnd += tStepX;
            tOverlap = LINE_OVERLAP_NONE;
            if (tError >= 0) {
                // change Y
                aYStart += tStepY;
                aYEnd += tStepY;
                tError -= tDeltaXTimes2;
                /*
                 * Change minor direction reverse to line (main) direction
                 * because of choosing the right (counter)clockwise draw vector
                 * Use LINE_OVERLAP_MAJOR to fill all pixel
                 *
                 * EXAMPLE:
                 * 1,2 = Pixel of first 2 lines
                 * 3 = Pixel of third line in normal line mode
                 * - = Pixel which will additionally be drawn in LINE_OVERLAP_MAJOR mode
                 *           33
                 *       3333-22
                 *   3333-222211
                 * 33-22221111
                 *  221111                     /\
                 *  11                          Main direction of start of lines draw vector
                 *  -> Line main direction
                 *  <- Minor direction of counterclockwise of start of lines draw vector
                 */
                tOverlap = LINE_OVERLAP_MAJOR;
            }
            tError += tDeltaYTimes2;
            DrawLine(aXStart, aYStart, aXEnd, aYEnd, tOverlap, setPixel);
        }
    }
    else {
        // the other octant 2, 4, 6, 8 (between 45 and 90, 135 and 180, ... degree)
        if (tSwap) {
            tStepX = -tStepX;
        }
        else {
            tDrawStartAdjustCount = (aThickness - 1) - tDrawStartAdjustCount;
            tStepY = -tStepY;
        }
        // adjust draw start point
        tError = tDeltaXTimes2 - tDeltaY;
        for (i = tDrawStartAdjustCount; i > 0; i--) {
            aYStart -= tStepY;
            aYEnd -= tStepY;
            if (tError >= 0) {
                aXStart -= tStepX;
                aXEnd -= tStepX;
                tError -= tDeltaYTimes2;
            }
            tError += tDeltaXTimes2;
        }
        //draw start line
        DrawLine(aXStart, aYStart, aXEnd, aYEnd, LINE_OVERLAP_NONE, setPixel);
        // draw aThickness number of lines
        tError = tDeltaXTimes2 - tDeltaY;
        for (i = aThickness; i > 1; i--) {
            aYStart += tStepY;
            aYEnd += tStepY;
            tOverlap = LINE_OVERLAP_NONE;
            if (tError >= 0) {
                aXStart += tStepX;
                aXEnd += tStepX;
                tError -= tDeltaYTimes2;
                tOverlap = LINE_OVERLAP_MAJOR;
            }
            tError += tDeltaXTimes2;
            DrawLine(aXStart, aYStart, aXEnd, aYEnd, tOverlap, setPixel);
        }
    }
}

void BresenhamsAlgorithm::DrawLine(int aXStart, int aYStart, int aXEnd, int aYEnd, uint8_t aOverlap, std::function<void(int, int)> setPixel)
{
	int16_t tDeltaX, tDeltaY, tDeltaXTimes2, tDeltaYTimes2, tError, tStepX, tStepY;


	//calculate direction
	tDeltaX = aXEnd - aXStart;
	tDeltaY = aYEnd - aYStart;
	if (tDeltaX < 0) {
		tDeltaX = -tDeltaX;
		tStepX = -1;
	}
	else {
		tStepX = +1;
	}
	if (tDeltaY < 0) {
		tDeltaY = -tDeltaY;
		tStepY = -1;
	}
	else {
		tStepY = +1;
	}
	tDeltaXTimes2 = tDeltaX << 1;
	tDeltaYTimes2 = tDeltaY << 1;
	//draw start pixel
	setPixel(aXStart, aYStart);
	if (tDeltaX > tDeltaY) {
		// start value represents a half step in Y direction
		tError = tDeltaYTimes2 - tDeltaX;
		while (aXStart != aXEnd) {
			// step in main direction
			aXStart += tStepX;
			if (tError >= 0) {
				if (aOverlap & LINE_OVERLAP_MAJOR) {
					// draw pixel in main direction before changing
					setPixel(aXStart, aYStart);
				}
				// change Y
				aYStart += tStepY;
				if (aOverlap & LINE_OVERLAP_MINOR) {
					// draw pixel in minor direction before changing
					setPixel(aXStart - tStepX, aYStart);
				}
				tError -= tDeltaXTimes2;
			}
			tError += tDeltaYTimes2;
			setPixel(aXStart, aYStart);
		}
	}
	else {
		tError = tDeltaXTimes2 - tDeltaY;
		while (aYStart != aYEnd) {
			aYStart += tStepY;
			if (tError >= 0) {
				if (aOverlap & LINE_OVERLAP_MAJOR) {
					// draw pixel in main direction before changing
					setPixel(aXStart, aYStart);
				}
				aXStart += tStepX;
				if (aOverlap & LINE_OVERLAP_MINOR) {
					// draw pixel in minor direction before changing
					setPixel(aXStart, aYStart - tStepY);
				}
				tError -= tDeltaYTimes2;
			}
			tError += tDeltaXTimes2;
			setPixel(aXStart, aYStart);
		}
	}

}
