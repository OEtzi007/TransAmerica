/*
 * templateAI.cpp
 *
 *  Created on: 12.03.2014
 *      Author: David
 */

//==============================
// forward declared dependencies
//==============================
// included dependencies
#include "../../hdr/game/templateAI.h" /*TODO adapt*/

#include "../../hdr/game/Constants.h"
#include "../../hdr/game/City.h"
#include "../../hdr/game/Move.h"
#include "../../hdr/game/Pawn.h"
//==============================
// the actual code

templateAI::templateAI(PLAYERCOLOR playerColor) : /*TODO adapt*/
		AI(playerColor) {
	owner = "noOwner";
	AIname = "templateAI";
}

templateAI::~templateAI() { /*TODO adapt*/
}

Move templateAI::doMove(State &currentState, vector<Move*> moveList) { /*TODO adapt*/
}

const Coordinate* templateAI::setPawn(State &currentState) { /*TODO adapt*/
}

bool templateAI::countPoints(State& currentState,
		vector<const Connection*>& returnPath) { /*TODO adapt*/
}

void templateAI::gatherInformationEndOfRound(
		const RoundLogger* informationAboutGame) { /*TODO adapt*/
}
