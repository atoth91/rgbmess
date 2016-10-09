#include "stdafx.h"
#include "ColorUtil.h"
using namespace std;

void ColorUtil::setRegionFromHsv(KeyRegion keyregion, HsvColor color) {
	RgbColor rgbColor = ColorUtil::hsv2rgb(color);
	setRegionFromRgb(keyregion, rgbColor);
}

void ColorUtil::setRegionFromRgb(KeyRegion keyregion, RgbColor color) {

	for (int col = keyregion.topLeftKey.col; col <= keyregion.bottomRightKey.col; col++) {

		for (int row = keyregion.topLeftKey.row; row <= keyregion.bottomRightKey.row; row++) {

			if (!SetLedColor(row, col, color.r * 255, color.g * 255, color.b * 255)) {
				//cout << "setting led failed: col: " << col << " row: " << row << "\n";
			}
			else {
				//cout << "setting led: col: " << col << " row: " << row << "\n";
			}

		}

	}
}

void ColorUtil::setFullFromHsv(HsvColor color) {
	RgbColor rgbcolor = ColorUtil::hsv2rgb(color);
	SetFullLedColor(rgbcolor.r * 255, rgbcolor.g * 255, rgbcolor.b * 255);
}

void ColorUtil::setFullFromRgb(RgbColor color) {
	SetFullLedColor(color.r * 255, color.g * 255, color.b * 255);
}

HsvColor ColorUtil::rgb2hsv(RgbColor in)
{
	HsvColor         out;
	double      min, max, delta;

	min = in.r < in.g ? in.r : in.g;
	min = min < in.b ? min : in.b;

	max = in.r > in.g ? in.r : in.g;
	max = max > in.b ? max : in.b;

	out.v = max;                                // v
	delta = max - min;
	if (delta < 0.00001)
	{
		out.s = 0;
		out.h = 0; // undefined, maybe nan?
		return out;
	}
	if (max > 0.0) { // NOTE: if Max is == 0, this divide would cause a crash
		out.s = (delta / max);                  // s
	}
	else {
		// if max is 0, then r = g = b = 0              
		// s = 0, v is undefined
		out.s = 0.0;
		out.h = NAN;                            // its now undefined
		return out;
	}
	if (in.r >= max)                           // > is bogus, just keeps compilor happy
		out.h = (in.g - in.b) / delta;        // between yellow & magenta
	else
		if (in.g >= max)
			out.h = 2.0 + (in.b - in.r) / delta;  // between cyan & yellow
		else
			out.h = 4.0 + (in.r - in.g) / delta;  // between magenta & cyan

	out.h *= 60.0;                              // degrees

	if (out.h < 0.0)
		out.h += 360.0;

	return out;
}

RgbColor ColorUtil::hsv2rgb(HsvColor in)
{
	double      hh, p, q, t, ff;
	long        i;
	RgbColor         out;

	if (in.s <= 0.0) {       // < is bogus, just shuts up warnings
		out.r = in.v;
		out.g = in.v;
		out.b = in.v;
		return out;
	}
	hh = in.h;
	if (hh >= 360.0) hh = 0.0;
	hh /= 60.0;
	i = (long)hh;
	ff = hh - i;
	p = in.v * (1.0 - in.s);
	q = in.v * (1.0 - (in.s * ff));
	t = in.v * (1.0 - (in.s * (1.0 - ff)));

	switch (i) {
	case 0:
		out.r = in.v;
		out.g = t;
		out.b = p;
		break;
	case 1:
		out.r = q;
		out.g = in.v;
		out.b = p;
		break;
	case 2:
		out.r = p;
		out.g = in.v;
		out.b = t;
		break;

	case 3:
		out.r = p;
		out.g = q;
		out.b = in.v;
		break;
	case 4:
		out.r = t;
		out.g = p;
		out.b = in.v;
		break;
	case 5:
	default:
		out.r = in.v;
		out.g = p;
		out.b = q;
		break;
	}
	return out;
}