#ifndef ACTOR_H_
#define ACTOR_H_

#include "GraphObject.h"
class StudentWorld;

//states a lemming can be in
const int WALKING = 0;
const int FALLING = 1;
const int CLIMBING = 2;
const int BOUNCING = 3;

class Actor : public GraphObject
{
public:
	Actor(int imageID, Coord startCoord, int dir, StudentWorld* world) : GraphObject(imageID, startCoord, dir), m_studentWorld(world), m_alive(true), ticksSinceMove(0) {}
	virtual void doSomething() {}
	virtual ~Actor() {}
	virtual bool isSolid() const { return false; }
	bool isAlive() const { return m_alive; }
	void setDead() { m_alive = false; }
	StudentWorld* myWorld() const { return m_studentWorld; }
	void incrementTicks() { ticksSinceMove++; }
	void resetTicks() { ticksSinceMove = 0; }
	int ticks() const { return ticksSinceMove; }
	virtual bool canBeSaved() const { return false; }
	virtual bool canBeBounced() const { return false; }
	virtual bool canBeKilled() const { return false; }
	virtual bool isLemmingAttractor() const { return false; }
	virtual bool isClimbable() const { return false; }
	bool attemptMove(int dir);
	virtual void setBounce(int bounceheight) { }
	virtual int gimmeFallDistance() const { return -999; }
private:
	StudentWorld* m_studentWorld;
	bool m_alive;
	int ticksSinceMove;
};

class FloorBrick : public Actor
{
public:
	FloorBrick(Coord startCoord, int dir, StudentWorld* world) : Actor(IID_FLOOR, startCoord, dir, world) {}
	virtual bool isSolid() const { return true; }
private:
};

class IceMonster : public Actor 
{
public:
	IceMonster(Coord startCoord, int dir, StudentWorld* world) : Actor(IID_ICE_MONSTER, startCoord, dir, world) {}
	virtual void doSomething();
private:
};

class Cursor : public Actor
{
public:
	Cursor(StudentWorld* world) : Actor(IID_PLAYER, Coord(VIEW_WIDTH/2, VIEW_HEIGHT/2), GraphObject::right, world) {}
	virtual void doSomething();
private:
};

class Factory : public Actor
{
public:
	Factory(Coord startCoord, int dir, StudentWorld* world) : Actor(IID_LEMMING_FACTORY, startCoord, dir, world) {}
	virtual void doSomething();
private:
};

class Lemming : public Actor
{
public:
	Lemming(Coord startCoord, int dir, StudentWorld* world) : Actor(IID_LEMMING, startCoord, dir, world), m_state(0), m_fallDistance(0), m_upwardSteps(0) {}
	virtual void doSomething();
	virtual bool canBeSaved() const { return true; }
	virtual bool canBeBounced() const {
		if (m_state == 3)
			return false;
		return true;
	}
	virtual bool canBeKilled() const { return true; }
	virtual void setBounce(int bounceheight) { m_state = 3; m_upwardSteps = bounceheight; }
	int lemmingState() const { return m_state; }
	int gimmeFallDistance() const { return m_fallDistance; }
private:
	int m_state;
	int m_fallDistance;
	int m_upwardSteps;
};

class Bonfire : public Actor
{
public:
	Bonfire(Coord startCoord, int dir, StudentWorld* world) : Actor(IID_BONFIRE, startCoord, dir, world) {}
	virtual void doSomething();
private:
};

class Trampoline : public Actor
{
public:
	Trampoline(Coord startCoord, int dir, StudentWorld* world) : Actor(IID_TRAMPOLINE, startCoord, dir, world) {}
	virtual void doSomething();
private:
};


class Net : public Actor
{
public:
	Net(Coord startCoord, int dir, StudentWorld* world) : Actor(IID_NET, startCoord, dir, world) {}
	virtual bool isClimbable() const { return true; }
private:
};

class Door : public Actor
{
public:
	Door(Coord startCoord, int dir, StudentWorld* world) : Actor(IID_ONE_WAY_DOOR, startCoord, dir, world) {}
	virtual void doSomething();
private:
};

class Pheromone : public Actor
{
public:
	Pheromone(Coord startCoord, int dir, StudentWorld* world) : Actor(IID_PHEROMONE, startCoord, dir, world) {}
	virtual bool isLemmingAttractor() const { return true; }
private:
};

class Spring : public Actor
{
public:
	Spring(Coord startCoord, int dir, StudentWorld* world) : Actor(IID_SPRING, startCoord, dir, world) {}
	virtual void doSomething();
private:
};

class Exit : public Actor
{
public:
	Exit(Coord startCoord, int dir, StudentWorld* world) : Actor(IID_EXIT, startCoord, dir, world) {}
	virtual void doSomething();
private:
};

#endif // ACTOR_H_
