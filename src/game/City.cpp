/*
 * City.cpp
 *
 *  Created on: 17.11.2013
 *      Author: David
 */

#include "../../hdr/game/City.h"

City::City(string name, CITYCOLOUR cityColour, short number, Vector place) :
		Vector(place), name(name), cityColour(cityColour), number(number) {
}

City::~City() {
}