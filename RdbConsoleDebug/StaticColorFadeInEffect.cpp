#include "stdafx.h"
#include "StaticColorFadeInEffect.h"
#include "ColorUtil.h"

using namespace std;

StaticColorFadeInEffect::StaticColorFadeInEffect(KeyRegion keyRegion, RgbColor targetColor, double valueStep) : ContinuousEffect(keyRegion) {
	HsvColor hsvColor = ColorUtil::rgb2hsv(targetColor);
	hsvColor.v = 0.1;
	this->targetColor = hsvColor;
	this->valueStep = valueStep;
};

COLOR_MATRIX StaticColorFadeInEffect::applyStep(COLOR_MATRIX currentState) {

	if (fadeIn) {
		targetColor.v = targetColor.v + valueStep;
	}
	else {
		targetColor.v = targetColor.v - valueStep;
	}

	if (targetColor.v >= 1.0) {
		fadeIn = false;
	}
	else if (targetColor.v <= 0.1) {
		fadeIn = true;
	}

	KEY_COLOR newColor = ColorUtil::hsvColorToKeyColor(targetColor);
	currentState = ColorUtil::setRegionColor(currentState, keyRegion, newColor);

	if (this->stopFlagState && ColorUtil::isEqualColorMatrixInRegion(currentState, stopState, keyRegion)) {
		this->stopFlag = true;
	}

	return currentState;
}