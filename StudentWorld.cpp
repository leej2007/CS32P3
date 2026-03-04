#include "StudentWorld.h"
#include "GameConstants.h"
#include <string>
#include "Coord.h"
#include "Actor.h"
using namespace std;

GameWorld* createStudentWorld(string assetPath)
{
    return new StudentWorld(assetPath);
}

// Students:  Add code to this file, StudentWorld.h, Actor.h, and Actor.cpp
// Do not change or remove the createStudentWorld implementation above.

StudentWorld::StudentWorld(string assetPath)
    : GameWorld(assetPath), m_leveltimer(2000), m_lemmingsSaved(0), m_lemmingsDead(0), m_lemmingsSpawned(0), m_cursor(nullptr), m_tools("")
{
}

void StudentWorld::addActor(Actor* newActor) {
    if(newActor->canBeSaved())
        incrementNumSpawned();
    m_vA.push_back(newActor);
}
int StudentWorld::init()
{
    Level lev(assetPath()); // note: GameWorld provides assetPath()
    string expression = "";
    int level = getLevel();
    //convert the level to a string for the file name
    if (level >= 0 && level < 10) {
        string levelstring = "";
        levelstring += level + '0';
        expression = "level0" + levelstring + ".txt";
    }
    //convert a level with multiple digits to a string
    else if (level >= 10) {
        int temp = level;
        string levelstring = "";
        while (temp > 0) {
            char thing = temp % 10 + '0';
            levelstring = thing + levelstring;
            temp = temp/10;
        }
        expression = "level" +levelstring+ ".txt";
    }
    Level::LoadResult result = lev.loadLevel(expression);
    //no next level
   if(result == Level::load_fail_file_not_found)
       return GWSTATUS_PLAYER_WON;
   //wrong format
   else if(result == Level::load_fail_bad_format) //or if there is not one lemming factory/at least 1 exit
       return GWSTATUS_LEVEL_ERROR;

    //load successful, load each item in the file
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
            case Level::bonfire:
                m_vA.push_back(new Bonfire(p, GraphObject::right, this));
                break;
            case Level::lemming_factory:
                m_vA.push_back(new Factory(p, GraphObject::right, this));
                break;
            case Level::trampoline:
                m_vA.push_back(new Trampoline(p, GraphObject::right, this));
                break;
            case Level::net:
                m_vA.push_back(new Net(p, GraphObject::right, this));
                break;
            case Level::left_one_way_door:
                m_vA.push_back(new Door(p, GraphObject::left, this));
                break;
            case Level::right_one_way_door:
                m_vA.push_back(new Door(p, GraphObject::right, this));
                break;
            case Level::pheromone:
                m_vA.push_back(new Pheromone(p, GraphObject::right, this));
                break;
            case Level::spring:
                m_vA.push_back(new Spring(p, GraphObject::right, this));
                break;
            case Level::lemming_exit:
                m_vA.push_back(new Exit(p, GraphObject::right, this));
                break;
            }
        }
    }
    //reset stats for this level
    m_tools = lev.getTools();
    m_leveltimer = 2000;
    m_lemmingsSaved = 0;
    m_lemmingsDead = 0;
    m_lemmingsSpawned = 0;
    m_cursor = new Cursor(this);

    return GWSTATUS_CONTINUE_GAME;
}
//helper function to fit the display numbers
string formatWithZeros(int number, int targetwidth) {
    string str = to_string(number);

    //insert 0s until it is the right width
    if (str.length() < targetwidth) {
        str.insert(0, targetwidth - str.length(), '0');
    }
    return str;
}

void StudentWorld::updateDisplay()
{
    string m_tools2;
    //if m_tools empty, print "none" instead
    if (m_tools == "")
        m_tools2 = "None";
    else
        m_tools2 = m_tools;
    //create the overall string using helper function
    string Displaytext =
        "Score: " + formatWithZeros(getScore(), 5) +
        "  Level: " + formatWithZeros(getLevel(), 2) +
        "  Lives: " + formatWithZeros(getLives(), 2) +
        "  Saved: " + formatWithZeros(m_lemmingsSaved, 2) +
        "  Tools: " + m_tools2 +
        "  Time left: " + formatWithZeros(m_leveltimer, 4);

    //update the display
    setGameStatText(Displaytext);
}

//
int StudentWorld::move()
{
    //decrement the timer
    m_leveltimer--;

    //update display
    updateDisplay();

    //move cursor
    m_cursor->doSomething();

    //make all the actors move
    for (size_t k = 0; k < m_vA.size(); k++) {
        if (m_vA[k]->isAlive())
            m_vA[k]->doSomething();
    }

    //cleanup dead actors
    for (int i = m_vA.size() - 1; i >= 0; i--) {
        if (!m_vA[i]->isAlive()) {
            //count the number of dead lemmings
            m_lemmingsDead++;
            delete m_vA[i];
            m_vA.erase(m_vA.begin() + i);
        }
    }

    //level end conditions
    // too many lemmings died
    if (m_lemmingsDead >= 6) {
        decLives();
        return GWSTATUS_PLAYER_DIED;
    }
    //time runs out
    if (m_leveltimer <= 0)
    {
        if (m_lemmingsSaved >= 5)
        {
            playSound(SOUND_FINISHED_LEVEL);
            return GWSTATUS_FINISHED_LEVEL;
        }
        else
        {
            decLives();
            return GWSTATUS_PLAYER_DIED;
        }
    }
    //end if all possible spawned lemmings are saved or dead
    if (m_lemmingsSaved + m_lemmingsDead == 10) {
        playSound(SOUND_FINISHED_LEVEL);
        increaseScore(m_leveltimer);
        return GWSTATUS_FINISHED_LEVEL;
    }
    //continue otherwise
    return GWSTATUS_CONTINUE_GAME;
}

void StudentWorld::cleanUp()
{
    //delete all the objects
    for (size_t k = 0; k < m_vA.size(); k++) {
        delete m_vA[k];
    }
    //clear dangling pointers
    size_t vectorsize = m_vA.size();
    for (size_t k = 0; k < vectorsize; k++) {
        m_vA.pop_back();
    }
    //delete player
    if (m_cursor != nullptr)
        delete m_cursor;
    m_cursor = nullptr;
}


//for moving purposes
bool StudentWorld::somethingSolid(Coord p)
{
    for (size_t k = 0; k < m_vA.size(); k++) {
        if (m_vA[k]->GraphObject::getCoord() == p && m_vA[k]->isSolid())
            return true;
    }
    return false;
}

//saving helper function
void StudentWorld::save(Actor* savedguy) {
    playSound(SOUND_LEMMING_SAVED);
    increaseScore(100);
    m_lemmingsSaved++;
    savedguy->setDead();
}
void StudentWorld::saveHere(Coord p) {
    for (size_t k = 0; k < m_vA.size(); k++) {
        if (m_vA[k]->GraphObject::getCoord() == p && m_vA[k]->canBeSaved()) {
            save(m_vA[k]);
            //make sure only 1 gets saved per tick
            return;
        }
    }
}

//killing helper function
void StudentWorld::kill(Actor* deadguy) {
    playSound(SOUND_LEMMING_DIE);
    m_lemmingsDead++;
    deadguy->setDead();
}

//for bonfire and icemonster
bool StudentWorld::killHere(Coord p) {
    bool didIKill = false;
    for (size_t k = 0; k < m_vA.size(); k++) {
        if (m_vA[k]->GraphObject::getCoord() == p && m_vA[k]->canBeKilled()) {
            kill(m_vA[k]);
            didIKill = true;
        }
    }
    return didIKill;
}

//for cursor
bool StudentWorld::isEmpty(Coord p)
{
    for (size_t k = 0; k < m_vA.size(); k++) {
        if (m_vA[k]->GraphObject::getCoord() == p)
            return false;
    }
    return true;
}

//for lemming movement
bool StudentWorld::somethingClimbable(Coord p)
{
    for (size_t k = 0; k < m_vA.size(); k++) {
        if (m_vA[k]->GraphObject::getCoord() == p && m_vA[k]->isClimbable())
            return true;
    }
    return false;
}

void StudentWorld::bounceHelper(Actor* bounceguy, int bounceheight) {
    playSound(SOUND_BOUNCE);
    bounceguy->setBounce(bounceheight);
}

void StudentWorld::Bounce(Coord p, int type)
{
    for (size_t k = 0; k < m_vA.size(); k++) {
        if (m_vA[k]->GraphObject::getCoord() == p && m_vA[k]->canBeBounced()) {
            //type 0 == trampoline
            if (type == 0) {
                int fallDistance = m_vA[k]->gimmeFallDistance();
                if (fallDistance > 0)
                    bounceHelper(m_vA[k], fallDistance - 1);
                else
                    bounceHelper(m_vA[k], 0);
            }
            //type 1 == spring
            else if (type == 1) {
                bounceHelper(m_vA[k], 15);
            }
        }
    }
}

//check for pheromones, returns -1 if none valid, 180 to turn left, and 0 to turn right
int StudentWorld::checkPheromones(Coord p) {
    int lowestdistance = 999;
    int rightorleft = -1;
    for (size_t k = 0; k < m_vA.size(); k++) {
        if (m_vA[k]->isLemmingAttractor()) {
            //get distance between lemmingattractor and lemming
            int x = p.x - m_vA[k]->GraphObject::getCoord().x;
            //get the absolute value of the distance
            int absval = 0;
            if (x < 0)
                absval = -x;
            else
                absval = x;
            //check if this is the closest pheromone, that it isnt on the same coord as the lemming, and that the distance is 5 or less
            if (absval < lowestdistance && x !=0 && absval <=5) {
                lowestdistance = absval;
                //turn left if lemming is on the right of the pheromone
                if (x > 0)
                    rightorleft = 180;
                //right
                else
                    rightorleft = 0;
            }

        }
    }
    return rightorleft;
}


//for doors
void StudentWorld::turn(Coord p, int dir)
{
    for (size_t k = 0; k < m_vA.size(); k++) {
        if (m_vA[k]->GraphObject::getCoord() == p)
            m_vA[k]->setDirection(dir);
    }
}

bool StudentWorld::useTool(char tool) {
    for (int i = 0; i < m_tools.size(); i++) {
        //tool is in m_tools
        if (m_tools[i] == tool) {
            //use up that tool
            m_tools.erase(i, 1);
            //only erase 1 tool
            return true;
        }
    }
    //tool not in m_tools
    return false;
}