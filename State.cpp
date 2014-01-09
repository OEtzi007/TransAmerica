/*
 * State.cpp
 *
 *  Created on: 03.12.2013
 *      Author: David
 */

#include "State.h"
#include <limits>

State::State(Brett &board) :
		anzahlPoeppel(0), Spielbrett(board) {
	for (int i = 0; i < MAX_SPIELER; i++)
		poeppelListe[i] = new Poeppel;
	for (int i = 0; i < MAX_X; i++)
		for (int j = 0; j < MAX_Y; j++) {
			schienenNetzNummer[i][j] = KEINSCHIENENNETZ;
			for (int k = 0; k < 3; k++)
				schieneGelegt[i][j][k] = false;
		}
}

State::State(const State &copy) :
		Spielbrett(copy.Spielbrett) {
	anzahlPoeppel = copy.anzahlPoeppel;
	for (int i = 0; i < MAX_SPIELER; i++)
		poeppelListe[i] = new Poeppel(*copy.poeppelListe[i]);
	for (int i = 0; i < MAX_X; i++)
		for (int j = 0; j < MAX_Y; j++)
			for (int k = 0; k < 3; k++)
				schieneGelegt[i][j][k] = copy.schieneGelegt[i][j][k];
	for (int i = 0; i < MAX_X; i++)
		for (int j = 0; j < MAX_Y; j++)
			schienenNetzNummer[i][j] = copy.schienenNetzNummer[i][j];
}

/*
 State& State::operator=(const State& copy) const{
 this->Spielbrett=copy.Spielbrett;
 this->anzahlPoeppel = copy.anzahlPoeppel;
 for (int i = 0; i < MAX_SPIELER; i++)
 poeppelListe[i] = new Poeppel(*copy.poeppelListe[i]);
 for (int i = 0; i < MAX_X; i++)
 for (int j = 0; j < MAX_Y; j++)
 for (int k = 0; k < 3; k++)
 schieneGelegt[i][j][k] = copy.schieneGelegt[i][j][k];
 for (int i = 0; i < MAX_X; i++)
 for (int j = 0; j < MAX_Y; j++)
 schienenNetzNummer[i][j] = copy.schienenNetzNummer[i][j];
 return *this;
 }
 */

State::~State() {
	for (int i = 0; i < MAX_SPIELER; i++)
		delete poeppelListe[i];
}

Poeppel State::getPoeppel(const short spielerfarbe) const {
	return *poeppelListe[-spielerfarbe - 1];
}

bool State::schienenNetzNummerVon_Ist_(const Verbindung &von,
		const short schienennr) const {
	bool ruckgabe = false;
	if (getSchienenNetzNummer(von.first) == schienennr)
		ruckgabe = ruckgabe || true;
	if (getSchienenNetzNummer(von.second) == schienennr)
		ruckgabe = ruckgabe || true;
	return ruckgabe;
}

short State::getSchienenNetzNummer(const Vector &koo) const {
	return schienenNetzNummer[koo.x][koo.y];
}

void State::setSchienenNetzNummer(const Coordinate &koo, const short nr) {
	schienenNetzNummer[koo.x][koo.y] = nr;
}

void State::setSchiene(const Verbindung& input) {
	schieneGelegt[input.first.x][input.first.y][RichtungsWert(input.richtung)] =
			true;
}

short State::RichtungsWert(const Vector & richtung) {
	//zu jeder Coordinate: 0=(1,0); 1=(0,1); 2=(1,1) s. RichtungsWert
	short summe = richtung.x * 2 + richtung.y;
	switch (summe) {
	case 1:
		return 1;
	case 2:
		return 0;
	case 3:
		return 2;
	default:
		cout << "RichtungsWert hat keine Richtung uebergeben bekommen" << endl;
		return -1;
	}
}

void State::resetNr_ZuNr_(const short von, const short zu) {
	for (int i = 0; i < MAX_SPIELER; i++)
		if (this->poeppelListe[i]->schienennetznummer == von)
			this->poeppelListe[i]->schienennetznummer = zu;
	for (int i = 0; i < MAX_X; i++)
		for (int j = 0; j < MAX_Y; j++)
			if (this->schienenNetzNummer[i][j] == von)
				this->schienenNetzNummer[i][j] = zu;
}

void State::schieneLegen(const Verbindung &sollGelegt) {
	setSchiene(sollGelegt);
	short nummerFirst = getSchienenNetzNummer(sollGelegt.first);
	short nummerSecond = getSchienenNetzNummer(sollGelegt.second);
	if (nummerFirst == KEINSCHIENENNETZ) {
		setSchienenNetzNummer(sollGelegt.first, nummerSecond);
		return;
	} else if (nummerSecond == KEINSCHIENENNETZ) {
		setSchienenNetzNummer(sollGelegt.second, nummerFirst);
		return;
	}
	if (nummerFirst == nummerSecond) {
		return;
	}
	if (nummerFirst < nummerSecond)
		resetNr_ZuNr_(nummerFirst, nummerSecond);
	else if (nummerSecond < nummerFirst)
		resetNr_ZuNr_(nummerSecond, nummerFirst);
	else
		cout << "Zustand::schieneLegen ist fehlerhaft" << endl;
//TODO Exceptions
}

const Verbindung* State::getVerbindung(Vector a, Vector b) const {
	Vector eins = a;
	Vector zwei = b;
	const Verbindung* ruckgabe = 0;
	if ((b - a).x < 0 || (b - a).y < 0) {
		eins = b;
		zwei = a;
	}
	if (eins.x >= 0 && eins.y >= 0 && zwei.x >= 0 && zwei.y >= 0
			&& eins.x < MAX_X && eins.y < MAX_Y && zwei.x < MAX_X
			&& zwei.y < MAX_Y)
		ruckgabe = this->Spielbrett.Kanten[eins.x][eins.y][this->RichtungsWert(
				zwei - eins)];
	return ruckgabe;
}

void State::addPoeppel(Poeppel insert) {
	this->anzahlPoeppel++;
	this->schienenNetzNummer[insert.startposition.x][insert.startposition.y] =
			insert.spielerfarbe;
	poeppelListe[-insert.spielerfarbe - 1] = new Poeppel(insert);
}

void State::resetAll() {
	anzahlPoeppel = 0;
	for (int i = 0; i < MAX_SPIELER; i++)
		poeppelListe[i] = new Poeppel;
	for (int i = 0; i < MAX_X; i++)
		for (int j = 0; j < MAX_Y; j++) {
			schienenNetzNummer[i][j] = KEINSCHIENENNETZ;
			for (int k = 0; k < 3; k++)
				schieneGelegt[i][j][k] = false;
		}
}

/** This function creates a two-dimensional array.
 * In the end each entry 'array[x][y]' shows off the distance between the vector (x,y) of the Board
 * and the requested target vector.
 * [Here distance means how many rails one has to use to connect the two positions.]
 *
 * Therefore the six values around the target coordinate are calculated first. From now on
 * the surrounding of all vectors that were recently assigned with a new value are calculated until the complete
 * board is calculated.
 */
unsigned short** State::evaluateBoard(Vector target) const {
	unsigned short **index = new unsigned short*[MAX_X];
	for (int i = 0; i < MAX_X; i++) {
		index[i] = new unsigned short[MAX_Y];
	}
	for (int i = 0; i < MAX_X; i++) {
		for (int j = 0; j < MAX_Y; j++) {
			index[i][j] = std::numeric_limits<unsigned short>::max() / 2;
		}
	}
	index[target.x][target.y] = 0;

	vector<Vector> old_changed;
	vector<Vector> new_changed;
	old_changed.push_back(target);
	vector<Vector>::iterator it;
	while (old_changed.size() != 0) {
		for (it = old_changed.begin(); it != old_changed.end(); it++) {
			calculate_surround(*it, index, new_changed);
		}
		old_changed = new_changed;
		new_changed.clear();
	}
	return index;
}

/** As the name says this function just dumps the 2D-array calculated in the evaluateBoard function.
 */
#include <iomanip>
void State::dumpEvaluateBoard(unsigned short ** & index) {
	for (int j = 0; j < MAX_Y; j++) {
		for (int i = 0; i < MAX_X; i++) {
			cout << std::setw(6) << index[i][j];
		}
		cout << endl;
	}
}

/** This function is used in the evaluateBoard function.
 * The values for the 2D-array of the surrounding 6 positions are calculated.
 */
void State::calculate_surround(Vector actual, unsigned short ** &index,
		vector<Vector> &new_changed) const {
	Vector vecs[6] = { Vector(1, 0), Vector(1, 1), Vector(0, 1), Vector(-1, 0),
			Vector(-1, -1), Vector(0, -1) };
	unsigned short results[6];
	Vector now;
	for (int i = 0; i < 6; i++) {
		now = (actual + vecs[i]);
		if (now.x >= 0 && now.x < MAX_X && now.y >= 0 && now.y < MAX_Y) {
			results[i] = find_min(now, index);
			if (index[now.x][now.y] > results[i]) {
				index[now.x][now.y] = results[i];
				new_changed.push_back(now);
			}
		}
	}
}

/** This function is used in the evaluateBoard function.
 * It finds the minimum value for requested vector of the 2D-array taking into account the six surrounding values.
 */
unsigned short State::find_min(Vector actual, unsigned short ** &index) const {
	unsigned short min = std::numeric_limits<unsigned short>::max();
	Vector richtungsvektoren[] = { Vector(1, 0), Vector(1, 1), Vector(0, 1),
			Vector(-1, 0), Vector(-1, -1), Vector(0, -1) };
	for (Vector vec : richtungsvektoren) {
		const Verbindung* connection = getVerbindung(actual + vec, actual);
		if (connection != 0) {
			unsigned short value = index[(actual + vec).x][(actual + vec).y];
// what kind of connection is it?
			if (!this->schieneGelegt[connection->first.x][connection->first.y][this->RichtungsWert(
					connection->richtung)]) {
				if (connection->hindernis)
					value += 2;
				else
					value += 1;
			}
			if (value < min) {
				min = value;
			}
		}
	}
	return min;
}

/** This function returns a vector with all vectors belonging to one's railway system.
 */
vector<Vector> State::pointsBelongingToRailwaySystem(short playercolour) const {
	int playersRailwayID = this->getPoeppel(playercolour).schienennetznummer;
	vector<Vector> returnval;
	for (int i = 0; i < MAX_X; i++)
		for (int j = 0; j < MAX_Y; j++)
			if (playersRailwayID == this->schienenNetzNummer[i][j])
				returnval.push_back(Vector(i, j));
	return returnval;
}

/** The lengths of the shortest route between the given railway system
 * the target coordinate gets calculated.
 */
unsigned short State::distance(Vector target,
		const vector<Vector> &possibleStarts) const {
	unsigned short distance = std::numeric_limits<unsigned short>::max();
	unsigned short ** array = this->evaluateBoard(target);
	dumpEvaluateBoard(array);
	for (int i = 0; i < possibleStarts.size(); i++) {
		Vector act = possibleStarts[i];
		if (array[act.x][act.y] < distance)
			distance = array[act.x][act.y];
	}
	return distance;
}