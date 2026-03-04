#ifndef STUDENTWORLD_H_
#define STUDENTWORLD_H_

#include "GameWorld.h"
#include "Level.h"
#include <string>
#include <vector>
#include "Actor.h"
// Students:  Add code to this file, StudentWorld.cpp, Actor.h, and Actor.cpp

class Actor;
class Cursor;

class StudentWorld : public GameWorld
{
public:
  StudentWorld(std::string assetPath);
  virtual int init();
  virtual int move();
  virtual void cleanUp();
  bool somethingSolid(Coord p);
  virtual ~StudentWorld() { cleanUp(); }
  void incrementNumSpawned() { m_lemmingsSpawned++; }
  int numSpawned() { return m_lemmingsSpawned; }
  void addActor(Actor* newActor);
  bool killHere(Coord p);
  bool isEmpty(Coord p);
  bool somethingClimbable(Coord p);
  void Bounce(Coord p, int type);
  void bounceHelper(Actor* bounceguy, int bounceheight);
  int checkPheromones(Coord p);
  void kill(Actor* deadguy);
  void save(Actor* savedguy);
  void turn(Coord p, int dir);
  void saveHere(Coord p);
  bool useTool(char tool);
  void updateDisplay();
private:
	std::vector<Actor*> m_vA;
	Actor* m_cursor;
	int m_leveltimer;
	int m_lemmingsSaved;
	int m_lemmingsDead;
	int m_lemmingsSpawned;
	std::string m_tools;
};

#endif // STUDENTWORLD_H_
