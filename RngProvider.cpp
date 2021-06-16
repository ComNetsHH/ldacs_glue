//
// Created by seba on 6/16/21.
//

#include "RngProvider.hpp"

using namespace TUHH_INTAIRNET_MCSOTDMA;

IntegerUniformRng::IntegerUniformRng() : random_device(), generator(random_device()) {}

int IntegerUniformRng::get(int min, int max) {
	dist = std::uniform_int_distribution<>(min, max - 1);;
	return dist(generator);
}

void IRng::signupInt() {
	RngProvider::getInstance().signupInt(this);
}

int IRng::getRandomInt(int min, int max) {
	return RngProvider::getInstance().getInt(this, min, max);
}
