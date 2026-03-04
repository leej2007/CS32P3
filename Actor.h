#ifndef ACTOR_H_
#define ACTOR_H_

#include "GraphObject.h"
class StudentWorld;

class Actor : public GraphObject
{
public:
	Actor(int imageID, Coord startCoord, int dir, StudentWorld* world) : GraphObject(imageID, startCoord, dir), m_studentWorld(world), m_alive(true), ticksSinceMove(0) {}
	virtual void doSomething() {}
	virtual ~Actor() {}
	virtual bool isSolid() { return false; }
	bool isAlive() { return m_alive; }
	void setDead() { m_alive = false; }
	StudentWorld* myWorld() { return m_studentWorld; }
	void incrementTicks() { ticksSinceMove++; }
	void resetTicks() { ticksSinceMove = 0; }
	int ticks() { return ticksSinceMove; }
	virtual bool canBeSaved() { return false; }
	virtual bool canBeBounced() { return false; }
	virtual bool canBeKilled() { return false; }
	virtual bool isLemmingAttractor() { return false; }
	virtual bool isClimbable() { return false; }
	bool attemptMove(int dir);
	virtual void setBounce(int bounceheight) { }
	virtual int gimmeFallDistance() { return -999; }
private:
	StudentWorld* m_studentWorld;
	bool m_alive;
	int ticksSinceMove;
};

class FloorBrick : public Actor
{
public:
	FloorBrick(Coord startCoord, int dir, StudentWorld* world) : Actor(IID_FLOOR, startCoord, dir, world) {}
	virtual bool isSolid() { return true; }
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
	virtual bool canBeSaved() { return true; }
	virtual bool canBeBounced() {
		if (m_state == 3)
			return false;
		return true;
	}
	virtual bool canBeKilled() { return true; }
	virtual void setBounce(int bounceheight) { m_state = 3; m_upwardSteps = bounceheight; }
	int lemmingState() { return m_state; }
	int gimmeFallDistance() { return m_fallDistance; }
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
	virtual bool isClimbable() { return true; }
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
	virtual bool isLemmingAttractor() { return true; }
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
