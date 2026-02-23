#include "StudentWorld.h"
#include "GameConstants.h"
#include <string>
#include "Coord.h"
using namespace std;

GameWorld* createStudentWorld(string assetPath)
{
    return new StudentWorld(assetPath);
}

// Students:  Add code to this file, StudentWorld.h, Actor.h, and Actor.cpp
// Do not change or remove the createStudentWorld implementation above.

StudentWorld::StudentWorld(string assetPath)
: GameWorld(assetPath)
{
}

int StudentWorld::init()
{
    Level lev(assetPath()); // note: GameWorld provides assetPath()
    Level::LoadResult result = lev.loadLevel("level00.txt");
    for (int i = 0; i < 20; i++) {
        for (int j = 0; j < 20; j++) {
            Coord p(i, j);
            Level::MazeEntry item = lev.getContentsOf(p);
            switch (item) {
            case Level::floor:
                m_vA.push_back(new FloorBrick(p, GraphObject::right, this));
                break;
            case Level::ice_monster:
                m_vA.push_back(new IceMonster(p, GraphObject::right, this));
                break;
            }

        }
    }
    return GWSTATUS_CONTINUE_GAME;
}

int StudentWorld::move()
{
    for (size_t k = 0; k < m_vA.size(); k++) {
        m_vA[k]->doSomething();
    }
    
    return GWSTATUS_CONTINUE_GAME;
}

void StudentWorld::cleanUp()
{
    for (size_t k = 0; k < m_vA.size(); k++) {
        delete m_vA[k];
    }
}



bool StudentWorld::somethingSolid(Coord p)
{
    for (size_t k = 0; k < m_vA.size(); k++) {
        if (m_vA[k]->GraphObject::getCoord() == p && m_vA[k]->isSolid())
            return true;
    }
    return false;
}