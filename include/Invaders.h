#ifndef INVADERS_H_INCLUDED
#define INVADERS_H_INCLUDED

#include <SFML\Graphics.hpp>
#include <SFML\Audio.hpp>

#include "AnimatedSprite.h"
#include "Bullets.h"

#include <vector>
#include <list>
#include <memory>

class Invaders
{
public:
    typedef std::vector<std::vector<AnimatedSprite>> InvaderMatrixVec;
    typedef std::vector<std::vector<AnimatedSprite>>::iterator VecColIter;
    typedef std::vector<AnimatedSprite>::iterator              VecRowIter;

    Invaders();
    //Getters and setters
    std::shared_ptr<InvaderMatrixVec> getInvaders() const;
    AnimatedSprite getRandomInvader();
    void setInvaderSpeed(const sf::Time);
    std::shared_ptr<std::list<sf::Sprite>> getDeathNumber() const;
    void setDeathAnimation(const sf::Sprite&);
    void setDeathAudio(const sf::Sound&);

    //Setup and update work
    void populateInvaders(AnimatedSprite&);
    void updateDead(const sf::Time&);
    //Interactions
    void kill(VecColIter, VecRowIter);
    void multiMove(const sf::Time&);
    void multiShoot(const sf::Time&, Bullets&);

private:
    std::default_random_engine m_engine;
    std::uniform_int_distribution<unsigned> m_engineSeed;

    //Invader population
    std::shared_ptr<InvaderMatrixVec> m_invaderPop;
    int m_posX;
    int m_posY;

    //Movement
    int m_moveX;
    int m_moveY;
    sf::Time m_currTime;
    sf::Time m_invaderSpeed;
    bool m_movingLeft = false;

    //Bullets
    sf::Time m_bulletFrequency;
    sf::Time m_currbulletTime;
    sf::Time m_bulletSpeed;

    std::uniform_real_distribution<double> m_bulletSpawnChance;

    //Death
    sf::Sprite m_death;
    std::shared_ptr<std::list<sf::Sprite>> m_deathNumber;
    std::list<sf::Time> m_deathDurations;

    //Audio
    sf::Sound m_deathSound;
};


#endif // INVADERS_H_INCLUDED
