#ifndef ACTOR_H_
#define ACTOR_H_

#include "GraphObject.h"
// Students:  Add code to this file, Actor.cpp, StudentWorld.h, and StudentWorld.cpp
class StudentWorld;

class Actor : public GraphObject
{
public:
	Actor(int imageID, Coord startCoord, int dir, StudentWorld* world) : GraphObject(imageID, startCoord, dir), m_studentWorld(world) {}
	virtual void doSomething() = 0;
	virtual ~Actor() {}
	virtual bool isSolid() = 0;
	StudentWorld* myWorld() { return m_studentWorld; }
private:
	StudentWorld* m_studentWorld;
};

class FloorBrick : public Actor
{
public:
	FloorBrick(Coord startCoord, int dir, StudentWorld* world) : Actor(IID_FLOOR, startCoord, dir, world) {}
	virtual void doSomething() {}
	virtual bool isSolid() { return true; }
private:
};

class IceMonster : public Actor 
{
public:
	IceMonster(Coord startCoord, int dir, StudentWorld* world) : Actor(IID_ICE_MONSTER, startCoord, dir, world) {}
	virtual void doSomething();
	virtual bool isSolid() { return false; }
private:
};
#endif // ACTOR_H_
