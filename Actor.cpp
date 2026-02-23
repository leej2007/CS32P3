#include "Actor.h"
#include "StudentWorld.h"

// Students:  Add code to this file, Actor.h, StudentWorld.h, and StudentWorld.cpp
void IceMonster::doSomething() {
	int direction = GraphObject::getDirection();
	Coord position = GraphObject::getCoord();
	switch (direction) {
	case right:
		//check if you can move right
		if (!myWorld()->somethingSolid(Coord(position.x + 1, position.y)) && myWorld()->somethingSolid(Coord(position.x + 1, position.y - 1)))
			moveTo(right);
		else
			setDirection(left);
		break;
	case left:
		//check if you can move left
		if (!myWorld()->somethingSolid(Coord(position.x - 1, position.y)) && myWorld()->somethingSolid(Coord(position.x - 1, position.y - 1)))
			moveTo(left);
		else
			setDirection(right);
		break;
	}
}