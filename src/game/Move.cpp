/*
 * Move.cpp
 *
 *  Created on: 02.12.2013
 *      Author: David
 */

#include "../../hdr/game/Move.h"
#include "../../hdr/game/Coordinate.h"

/*Zug::Zug(short playerColor) :
 gueltigkeit(false), gueltigkeitUEberprueft(false), richtigBelegt(true), playerColor(
 playerColor), anzahlSchienen(0) {
 }

 Zug::Zug(short playerColor, const Connection &belegt) :
 gueltigkeit(false), gueltigkeitUEberprueft(false), richtigBelegt(true), playerColor(
 playerColor), anzahlSchienen(1) {
 Belegt[0] = &belegt;
 Belegt[1] = 0;
 }*/ //TODO
Move::Move(PLAYERCOLOR spielerfarbe, const Connection* belegt1,
		const Connection* belegt2) :
		gueltigkeit(false), gueltigkeitUEberprueft(false), richtigBelegt(true), spielerfarbe(
				spielerfarbe), anzahlSchienen(2), sumRailValue(0), bannedStatus(
				NOT_BANNED) {
	Belegt[0] = belegt1;
	Belegt[1] = belegt2;
	if (belegt1 == 0) {
		Belegt[0] = Belegt[1];
		Belegt[1] = 0;
		anzahlSchienen--;
	}
	if (belegt2 == 0)
		anzahlSchienen--;
}

Move::Move(const Move& copy) :
		gueltigkeit(copy.gueltigkeit), gueltigkeitUEberprueft(
				copy.gueltigkeitUEberprueft), richtigBelegt(copy.richtigBelegt), spielerfarbe(
				copy.spielerfarbe), anzahlSchienen(copy.anzahlSchienen), sumRailValue(
				copy.sumRailValue), bannedStatus(copy.bannedStatus) {
	Belegt[0] = copy.Belegt[0];
	Belegt[1] = copy.Belegt[1];
}

Move::~Move() {
}

bool Move::valid(State currentState, PLAYERCOLOR spielerfarb) {
	gueltigkeitUEberprueft = true;
	//checken, ob der Spieler richtige Farbe verwendet hat
	if (spielerfarb != this->spielerfarbe) {
		cout << "Spieler hat falsche Farbe initialisiert" << endl;
		bannedStatus = MOVE_WRONG_COLOR;
		gueltigkeit = false;
		return false;
	}
	//checken, ob min. eine Schiene gelegt wurde
	if (anzahlSchienen == 0) {
		cout << "Es wurde ein Zug mit keiner Schiene initialisiert" << endl;
		bannedStatus = MOVE_WITHOUT_RAILS;
		gueltigkeit = false;
		return false;
	}
	//checken, dass keine Schiene auf einer vorhandenen Schiene platziert wurde
	if (currentState.railSet[Belegt[0]->first->x][Belegt[0]->first->y][Belegt[0]->direction]) {
		cout << "The first connection is already set." << endl;
		bannedStatus = MOVE_RAILS_ALREADY_EXIST;
		gueltigkeit = false;
		return false;
	}
	if (anzahlSchienen == 2) {
		if (currentState.railSet[Belegt[1]->first->x][Belegt[1]->first->y][Belegt[1]->direction]) {
			cout << "The second connection is already set." << endl;
			bannedStatus = MOVE_RAILS_ALREADY_EXIST;
			gueltigkeit = false;
			return false;
		}
		//checken, dass die gesetzten Schienen verschieden sind
		if (Belegt[0] == Belegt[1]) {
			cout << "Both connections of the move are the same." << endl;
			bannedStatus = MOVE_RAILS_ALREADY_EXIST;
			gueltigkeit = false;
			return false;
		}
	}
	//erste Schiene checken
	//schienennr von Spieler
	short schienennr = currentState.getPawn(spielerfarb).schienennetznummer;
	//pr�fe, ob Connection an schienennr anliegt
	if (currentState.isRailwayNumberOfConnectionEqualsNumber(*Belegt[0],
			schienennr)) {
		richtigBelegt = true;
		//wenn ja, schiene legen, neuer State, also neue schienennr und dann zweite schiene analog
		if (anzahlSchienen == 1) {
			gueltigkeit = true;
		} else {
			currentState.setRail(Belegt[0]);
			schienennr = currentState.getPawn(spielerfarbe).schienennetznummer;
			if (currentState.isRailwayNumberOfConnectionEqualsNumber(*Belegt[1],
					schienennr)) {
				gueltigkeit = true;
			} else {
				cout
						<< "The second connection of this move isn't connected to the railway system."
						<< endl;
				bannedStatus = MOVE_NOT_CONNECTED_RAILWAY;
				gueltigkeit = false;
				return false;
			}
		}
	} else if (anzahlSchienen == 2
			&& currentState.isRailwayNumberOfConnectionEqualsNumber(*Belegt[1],
					schienennr)) {
		richtigBelegt = false;
		//wenn ja, schiene legen, neuer State, also neue schienennr und dann zweite schiene analog
		currentState.setRail(Belegt[1]);
		schienennr = currentState.getPawn(spielerfarbe).schienennetznummer;
		if (currentState.isRailwayNumberOfConnectionEqualsNumber(*Belegt[0],
				schienennr)) {
			gueltigkeit = true;
		} else {
			cout
					<< "The second connection of this move isn't connected to the railway system."
					<< endl;
			bannedStatus = MOVE_NOT_CONNECTED_RAILWAY;
			gueltigkeit = false;
			return false;
		}
	} else {
		cout
				<< "The connections of this move aren't connected to the railway system."
				<< endl;
		bannedStatus = MOVE_NOT_CONNECTED_RAILWAY;
		gueltigkeit = false;
		return false;
	}
	sumRailValue = 0;
	for (int i = 0; i < 2; i++) {
		if (Belegt[i] != 0) {
			if (Belegt[i]->barrier)
				sumRailValue += 2;
			else
				sumRailValue += 1;
		}
	}
	if (sumRailValue > 2) {
		cout
				<< "This move is invalid, because of containing two connections, but at least one of them is a barrier."
				<< endl;
		bannedStatus = MOVE_TOO_MANY_BARRIERS;
		gueltigkeit = false;
		return false;
	}
	return gueltigkeit;
}

void Move::execute(State& akt) const { //TODO execute je nach Anzahl schienen
	akt.playerStatus[spielerfarbe] = bannedStatus;
	if (!gueltigkeitUEberprueft) {
		cout
				<< "Zug::ausfuehren:Zug sollte ohne Ueberpruefung ausgefuehrt werden"
				<< endl;
		return;
	}
	if (!gueltigkeit) {
		cout
				<< "Zug::ausfuehren: Es sollte ein ungueltiger Zug ausgefuehrt werden!"
				<< endl;
		return;
	}
	if (richtigBelegt) {
		akt.setRail(Belegt[0]);
		if (anzahlSchienen == 2)
			akt.setRail(Belegt[1]);
	} else {
		akt.setRail(Belegt[1]);
		akt.setRail(Belegt[0]);
	}
}

void Move::dump() const {
	cout << "First" << endl;
	cout << Belegt[0]->first->x << " " << Belegt[0]->first->y << endl;
	cout << "Richtung" << endl;
	cout << Belegt[0]->direction << endl;
}

BANNED_STATUS Move::getBannedStatus() const {
	return bannedStatus;
}

PLAYERCOLOR Move::getSpielerfarbe() const {
	return spielerfarbe;
}

const Connection* const * Move::getBelegt() const {
	return Belegt;
}

short Move::getSumRailValue() const {
	return sumRailValue;
}
