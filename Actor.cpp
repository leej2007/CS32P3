#include "Actor.h"
#include "StudentWorld.h"

bool Actor::attemptMove(int dir)
{
	Coord position = GraphObject::getCoord();
	switch (dir) {
	case right:
		//check if you can move right
		if (!myWorld()->somethingSolid(Coord(position.x + 1, position.y))) {
			moveTo(right);
			return true;
		}
		else {
			setDirection(left);
			return false;
		}
		break;
	case left:
		//check if you can move left
		if (!myWorld()->somethingSolid(Coord(position.x - 1, position.y))) {
			moveTo(left);
			return true;
		}
		else {
			setDirection(right);
			return false;
		}
		break;
	case up:
		//check if you can move up
		if (!myWorld()->somethingSolid(Coord(position.x, position.y + 1))) {
			moveTo(up);
			return true;
		}
		else
			return false;
		break;
	case down:
		//check if you can move down
		if (!myWorld()->somethingSolid(Coord(position.x, position.y - 1))) {
			moveTo(down);
			return true;
		}
		else
			return false;
		break;
	}
	return false;
}


void IceMonster::doSomething() {
	//add 1 to the counter of time since last moved
	incrementTicks();
	
	//kill lemming if there is one here, if killed something, no action this turn
	if (myWorld()->killHere(getCoord()))
		return;

	//if it didn't kill a lemming, check if it can move
	if (ticks() % 10 == 0) {
		int direction = GraphObject::getDirection();
		Coord position = GraphObject::getCoord();
		//icemonsters also change direction on ledges
		if (direction == right) {
			if (myWorld()->somethingSolid(Coord(position.x + 1, position.y - 1)))
				attemptMove(direction);
			else
				setDirection(left);
		}
		else
			if (myWorld()->somethingSolid(Coord(position.x - 1, position.y - 1)))
				attemptMove(direction);
			else
				setDirection(right);
	}
}

void Factory::doSomething() {
	//track how long it's been since it last spawned a lemming
	incrementTicks();

	if (ticks() % 100 == 0 && myWorld()->numSpawned()<10) {
		Actor* newLemming = new Lemming(getCoord(), right, myWorld());
		myWorld()->addActor(newLemming);
	}
}

void Cursor::doSomething() {
	int value = -1;
	//switch statement for control of player actions
	if (myWorld()->getKey(value)) {
		switch (value) {
			//movement of cursor, staying within bounds
		case KEY_PRESS_LEFT:
			if (GraphObject::getCoord().x > 1)
				GraphObject::moveTo(left);
			break;
		case KEY_PRESS_RIGHT:
			if (GraphObject::getCoord().x < VIEW_WIDTH-2)
				GraphObject::moveTo(right);
			break;
		case KEY_PRESS_DOWN:
			if (GraphObject::getCoord().y > 1)
				GraphObject::moveTo(down);
			break;
		case KEY_PRESS_UP:
			if (GraphObject::getCoord().y < VIEW_HEIGHT-2)
				GraphObject::moveTo(up);
			break;
			//create actors from tools
		case 't':
		case 'T':
			if (myWorld()->isEmpty(GraphObject::getCoord()) && myWorld()->useTool('T')) {
				Actor* newTrampoline = new Trampoline(GraphObject::getCoord(), right, myWorld());
				myWorld()->addActor(newTrampoline);
			}
			break;

		case 'n':
		case 'N':
			if (myWorld()->isEmpty(GraphObject::getCoord()) && myWorld()->useTool('N')) {
				Actor* newNet = new Net(GraphObject::getCoord(), right, myWorld());
				myWorld()->addActor(newNet);
			}
			break;

		case 'p':
		case 'P':
			if (myWorld()->isEmpty(GraphObject::getCoord()) && myWorld()->useTool('P')) {
				Actor* newPheromone = new Pheromone(GraphObject::getCoord(), right, myWorld());
				myWorld()->addActor(newPheromone);
			}
			break;

		case 's':
		case 'S':
			if (myWorld()->isEmpty(GraphObject::getCoord()) && myWorld()->useTool('S')) {
				Actor* newSpring = new Spring(GraphObject::getCoord(), right, myWorld());
				myWorld()->addActor(newSpring);
			}
			break;

		case '<':
			if (myWorld()->isEmpty(GraphObject::getCoord()) && myWorld()->useTool('<')) {
				Actor* newDoor = new Door(GraphObject::getCoord(), left, myWorld());
				myWorld()->addActor(newDoor);
			}
			break;
		case '>':
			if (myWorld()->isEmpty(GraphObject::getCoord()) && myWorld()->useTool('>')) {
				Actor* newDoor = new Door(GraphObject::getCoord(), right, myWorld());
				myWorld()->addActor(newDoor);
			}
			break;
		}

	}
}

void Lemming::doSomething() {
	//keep track of ticks since last move
	incrementTicks();

	if (m_state == WALKING) {
		if (ticks() % 4 != 0)
			return;
	}
	else if (m_state == FALLING || m_state == CLIMBING || m_state == BOUNCING) {
		if (ticks() % 2 != 0)
			return;
	}
	//it is about to move, so reset its ticks since last move to 0
	resetTicks();

	//check for pheromones
	int command = myWorld()->checkPheromones(getCoord());
	if (command != -1) {
		//command is either 0(right) or 180(left)
		setDirection(command);
	}

	if (m_state == WALKING) {
		if (myWorld()->somethingClimbable(getCoord())) {
			m_state = CLIMBING;
			return;
		}

		//move to the next space or turn around if facing a wall
		int direction = GraphObject::getDirection();
		attemptMove(direction);

		//transition into falling if there's no floor under lemming now
		Coord position = GraphObject::getCoord();
		if (!myWorld()->somethingSolid(Coord(position.x, position.y - 1)))
			m_state = FALLING;
		return;
	}

	if (m_state == FALLING) {
		if (myWorld()->somethingClimbable(getCoord())) {
			m_state = CLIMBING;
			m_fallDistance = 0;
			return;
		}

		//attempt to fall
		if (attemptMove(down))
			//successfully moves down
			m_fallDistance++;
		//hit the ground
		else if (m_fallDistance > 5) {
			//kill the lemming
			myWorld()->kill(this);
		}
		else //it survives the fall
		{
			m_fallDistance = 0;
			m_state = WALKING;
		}

		return;
	}
	//climbing actions
	if (m_state == CLIMBING) {
		if (!myWorld()->somethingClimbable(getCoord())) {
			m_state = WALKING;
			return;
		}
		attemptMove(up);
		return;
	}
	//bouncing actions
	if (m_state == BOUNCING) {
		if (myWorld()->somethingClimbable(getCoord())) {
			m_state = CLIMBING;
			return;
		}
		if (m_upwardSteps > 0) {
			if (!attemptMove(up)) {
				//give up bouncing if you hit a ceiling
				m_upwardSteps = 0;
			}
			else
				m_upwardSteps--;
		}
		//move at apex
		if (m_upwardSteps == 0) {
			int direction = GraphObject::getDirection();
			attemptMove(direction);

			//transition to fall
			m_state = FALLING;
			m_fallDistance = 0;
		}
	}
}

//short actions
void Bonfire::doSomething() { myWorld()->killHere(GraphObject::getCoord()); }
void Trampoline::doSomething() { myWorld()->Bounce(GraphObject::getCoord(), 0); }
void Door::doSomething() { myWorld()->turn(GraphObject::getCoord(), GraphObject::getDirection()); }
void Spring::doSomething() { myWorld()->Bounce(GraphObject::getCoord(), 1); }
void Exit::doSomething() { myWorld()->saveHere(GraphObject::getCoord()); }