/*
 * Zustand.cpp
 *
 *  Created on: 03.12.2013
 *      Author: David
 */

#include "Zustand.h"

Zustand::Zustand(Brett &board) :
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

Zustand::Zustand(const Zustand &copy) :
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

Zustand::~Zustand() {
	for (int i = 0; i < MAX_SPIELER; i++)
		delete poeppelListe[i];
}

Poeppel Zustand::getPoeppel(const short spielerfarbe) const {
	return *poeppelListe[-spielerfarbe - 1];
}

bool Zustand::schienenNetzNummerVon_Ist_(const Verbindung &von,
		const short schienennr) const {
	bool ruckgabe = false;
	if (getSchienenNetzNummer(von.first) == schienennr)
		ruckgabe = ruckgabe || true;
	if (getSchienenNetzNummer(von.second) == schienennr)
		ruckgabe = ruckgabe || true;
	return ruckgabe;
}

short Zustand::getSchienenNetzNummer(const Vector &koo) const {
	return schienenNetzNummer[koo.x][koo.y];
}

void Zustand::setSchienenNetzNummer(const Coordinate &koo, const short nr) {
	schienenNetzNummer[koo.x][koo.y] = nr;
}

void Zustand::setSchiene(const Verbindung& input) {
	schieneGelegt[input.first.x][input.first.y][RichtungsWert(input.richtung)] =
			true;
}

short Zustand::RichtungsWert(const Vector & richtung) {
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

void Zustand::resetNr_ZuNr_(const short von, const short zu) {
	for (int i = 0; i < MAX_SPIELER; i++)
		if (this->poeppelListe[i]->schienennetznummer == von)
			this->poeppelListe[i]->schienennetznummer = zu;
	for (int i = 0; i < MAX_X; i++)
		for (int j = 0; j < MAX_Y; j++)
			if (this->schienenNetzNummer[i][j] == von)
				this->schienenNetzNummer[i][j] = zu;
}

void Zustand::schieneLegen(const Verbindung &sollGelegt) {
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

const Verbindung &Zustand::getVerbindung(Vector a, Vector b) const {
	Vector eins = a;
	Vector zwei = b;
	if ((b - a).x < 0 || (b - a).y < 0) {
		eins = b;
		zwei = a;
	}
	const Verbindung &ruckgabe =
			*this->Spielbrett.Kanten[eins.x][eins.y][this->RichtungsWert(
					zwei - eins)];
	return ruckgabe;
}

void Zustand::addPoeppel(Poeppel insert) {
	this->anzahlPoeppel++;
	this->schienenNetzNummer[insert.startposition.x][insert.startposition.y] =
			insert.spielerfarbe;
	poeppelListe[-insert.spielerfarbe - 1] = new Poeppel(insert);
}

void Zustand::resetAll() {
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

unsigned short** Zustand::evaluateBoard(Vector target) const {
	unsigned short **index = new unsigned short*[20];
	for (int i = 0; i < 20; i++) {
		index[i] = new unsigned short[13];
	}
	for (int i = 0; i < 20; i++) {
		for (int j = 0; j < 13; j++) {
			index[i][j] = std::numeric_limits<unsigned short>::max();
		}
	}
	index[target.x][target.y] = 0;

	vector<Vector> old_changed;
	vector<Vector> new_changed;
	old_changed.push_back(target);
	vector<Vector>::iterator it;
	while(old_changed.size()!=0){
		for(it=old_changed.begin();it!=old_changed.end();it++){
			calculate_surround(*it,index,new_changed);
		}
		old_changed=new_changed;
		new_changed.clear();
	}
	return index;
}

void Zustand::calculate_surround(Vector actual,
		unsigned short ** &index, vector<Vector> &new_changed) const {
	Vector w(actual.x + 1, actual.y), sw(actual.x + 1, actual.y + 1), se(
			actual.x, actual.y + 1), e(actual.x - 1, actual.y), ne(actual.x - 1,
			actual.y - 1), nw(actual.x, actual.y - 1);
//TODO vector mit den 6 elementen machen

	Vector vecs[6]={Vector(1,0),Vector(0,1),Vector(1,1),Vector(-1,0),Vector(0,-1),Vector(-1,-1)};

	//actual+vecs[i];

	unsigned short wi = find_min(w, index);
	if (index[w.x][w.y] > wi) {
		index[w.x][w.y] = wi;
		new_changed.push_back(w);
	};
	unsigned short swi = find_min(sw, index);
	if (index[sw.x][sw.y] > swi) {
		index[sw.x][sw.y] = swi;
		new_changed.push_back(sw);
	};
	unsigned short sei = find_min(se, index);
	if (index[se.x][se.y] > sei) {
		index[se.x][se.y] = sei;
		new_changed.push_back(se);
	};
	unsigned short ei = find_min(e, index);
	if (index[e.x][e.y] > ei) {
		index[e.x][e.y] = ei;
		new_changed.push_back(e);
	};
	unsigned short nei = find_min(ne, index);
	if (index[ne.x][ne.y] > nei) {
		index[ne.x][ne.y] = nei;
		new_changed.push_back(ne);
	};
	unsigned short nwi = find_min(nw, index);
	if (index[nw.x][nw.y] > nwi) {
		index[nw.x][nw.y] = nwi;
		new_changed.push_back(nw);
	};
}

unsigned short Zustand::find_min(Vector actual,
		unsigned short ** &index) const {
	unsigned short min = 1000, a, b, c, d, e, f;
	/*
	 *	 e   f
	 *	  \ /
	 * d-- x --a
	 *	  / \
	 *   c   b
	 */
//TODO vector mit abcdef und Schleife machen
	if (this->Spielbrett.Kanten[actual.x][actual.y][0])
		a = 2;
	else if (this->schieneGelegt[actual.x][actual.y][0])
		a = 0;
	else
		a = 1;
	a = a + index[actual.x + 1][actual.y];

	if (this->Spielbrett.Kanten[actual.x][actual.y][2])
		b = 2;
	else if (this->schieneGelegt[actual.x][actual.y][2])
		b = 0;
	else
		b = 1;
	b = b + index[actual.x + 1][actual.y + 1];

	if (this->Spielbrett.Kanten[actual.x][actual.y][1])
		c = 2;
	else if (this->schieneGelegt[actual.x][actual.y][1])
		c = 0;
	else
		c = 1;
	c = c + index[actual.x][actual.y + 1];

	if (this->Spielbrett.Kanten[actual.x - 1][actual.y][0])
		d = 2;
	else if (this->schieneGelegt[actual.x - 1][actual.y][0])
		d = 0;
	else
		d = 1;
	d = d + index[actual.x - 1][actual.y];

	if (this->Spielbrett.Kanten[actual.x - 1][actual.y - 1][2])
		e = 2;
	else if (this->schieneGelegt[actual.x - 1][actual.y - 1][2])
		e = 0;
	else
		e = 1;
	e = e + index[actual.x - 1][actual.y - 1];

	if (this->Spielbrett.Kanten[actual.x][actual.y - 1][1])
		f = 2;
	else if (this->schieneGelegt[actual.x][actual.y - 1][1])
		f = 0;
	else
		f = 1;
	f = f + index[actual.x][actual.y - 1];

	min = a;
	if (b < min)
		min = b;
	if (c < min)
		min = c;
	if (d < min)
		min = d;
	if (e < min)
		min = e;
	if (f < min)
		min = f;

	return min;
}


vector<Vector> Zustand::pointsBelongingToRailwaySystem(short playercolour) const{
	int playersRailwayID=this->getPoeppel(playercolour).schienennetznummer;
	vector<Vector> returnval;
	for(int i=0;i<MAX_X;i++)
		for(int j=0;j<MAX_Y;j++)
			if(playersRailwayID==this->schienenNetzNummer[i][j])
				returnval.push_back(Vector(i,j));
	return returnval;
}


unsigned short Zustand::distance(Vector target,const vector<Vector> &possibleStarts) const{
	unsigned short distance=std::numeric_limits<unsigned short>::max();
	unsigned short ** array=this->evaluateBoard(target);
	for(int i=0;i<possibleStarts.size();i++){
		Vector act=possibleStarts[i];
		if(array[act.x][act.y] < distance)
			distance=array[act.x][act.y];
	}
	return distance;
}