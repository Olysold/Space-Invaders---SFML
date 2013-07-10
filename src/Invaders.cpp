#include "Invaders.h"

#include <iostream>

#include<vector>
using std::vector;

using std::list;
using std::shared_ptr;
using std::make_shared;
using std::uniform_int_distribution;
using std::default_random_engine;

Invaders::Invaders(): m_invaderPop(make_shared<InvaderMatrixVec>()),
                      m_deathNumber(make_shared<list<sf::Sprite>>()),
                      m_posX(100),
                      m_posY(70),
                      m_moveX(10),
                      m_moveY(20),
                      m_invaderSpeed(sf::seconds(0.5)),
                      m_bulletSpeed(sf::seconds(0.05)),
                      m_bulletSpawnChance(0.5, 2.5) { }

shared_ptr<Invaders::InvaderMatrixVec> Invaders::getInvaders() const
{
    return m_invaderPop;
}

AnimatedSprite Invaders::getRandomInvader()
{
    default_random_engine engine;
    engine.seed(m_engineSeed(m_engine));
    uniform_int_distribution<unsigned> distribution(0, (m_invaderPop->size() - 1));

    if(!m_invaderPop->empty())
    {
        auto selectedCol = m_invaderPop->at(distribution(engine));

        if(!selectedCol.empty())
        {
            if(selectedCol.size() == 1)
            {
                return *selectedCol.begin();
            }
            else
                return *(selectedCol.end() - 1);
        }
    }
}

void Invaders::setInvaderSpeed(const sf::Time newTime)
{
    m_invaderSpeed = newTime;
}

std::shared_ptr<std::list<sf::Sprite>> Invaders::getDeathNumber() const
{
    return m_deathNumber;
}

void Invaders::setDeathAnimation(const sf::Sprite& death)
{
    m_death = death;
}

void Invaders::setDeathAudio(const sf::Sound& sound)
{
    m_deathSound = sound;
}

void Invaders::populateInvaders(AnimatedSprite& invader)
{
    vector<AnimatedSprite> invaderCol;

    for(int col = 0; col < 10; col++)
    {
        for(int row = 0; row < 5; row++)
        {
            invader.setPosition(m_posX, m_posY);
            invaderCol.push_back(invader);
            m_posY += 40;
        }

        m_invaderPop->push_back(invaderCol);
        invaderCol.clear();
        m_posY -= 200;
        m_posX += 40;
    }

    m_posX = 100;
    m_posY = 70;
}

void Invaders::kill(VecColIter col, VecRowIter row)
{
    sf::Time deathTime;

    m_deathSound.play();

    m_death.setPosition(row->getPosition().x, row->getPosition().y);
    m_deathNumber->push_back(m_death);
    m_deathDurations.push_back(deathTime);

    //Erase in the row
    col->erase(row, row+1);

    //If the column has all rows erased, erase column itself
    if(col->empty())
    {
        m_invaderPop->erase(col, col+1);
    }
}

void Invaders::updateDead(const sf::Time& deltaTime)
{
    if(!m_deathDurations.empty() && !m_deathNumber->empty())
    {
        for(auto& srch : m_deathDurations)
        {
            srch += deltaTime;
        }

        for(auto srch : m_deathDurations)
        {
            if(srch.asSeconds() >= 1)
            {
                m_deathDurations.pop_front();
                m_deathNumber->pop_front();
                return;
            }
        }
    }
}

void Invaders::multiMove(const sf::Time& deltaTime)
{
    //Used to determine when the invaders should move bottom
    auto mostRightInvader = (m_invaderPop->end() - 1)->begin();
    auto mostLeftInvader = m_invaderPop->begin()->begin();

    m_currTime += deltaTime;

    if(m_currTime >= m_invaderSpeed && !m_invaderPop->empty())
    {
        //Moving horizontally
        if(!m_movingLeft && mostRightInvader->getPosition().x != 540)
        {
            for(auto& col : *m_invaderPop)
            {
                for(auto& row : col)
                {
                    row.update(m_currTime);
                    row.move(m_moveX, 0);
                }
            }
        }
        else if(m_movingLeft && mostLeftInvader->getPosition().x != 20)
        {
            for(auto& col : *m_invaderPop)
            {
                for(auto& row : col)
                {
                    row.update(m_currTime);
                    row.move(-m_moveX, 0);
                }
            }
        }
        //Moving vertically
        else if(mostRightInvader->getPosition().x == 540)
        {
            for(auto& col : *m_invaderPop)
            {
                for(auto& row : col)
                {
                    row.move(0, m_moveY);
                }
            }
            m_movingLeft = true;

        }
        else if(mostLeftInvader->getPosition().x == 20)
        {
            for(auto& col : *m_invaderPop)
            {
                for(auto& row : col)
                {
                    row.move(0, m_moveY);
                }
            }
            m_movingLeft = false;

        }
        m_currTime = sf::Time::Zero;
    }
}

void Invaders::multiShoot(const sf::Time& deltaTime, Bullets& bulletOps)
{
    m_bulletFrequency += deltaTime;

    if(m_bulletFrequency.asSeconds() >= m_bulletSpawnChance(m_engine) && !m_invaderPop->empty())
    {
        auto invader = getRandomInvader();
        bulletOps.spawnBullet_invader(invader.getGlobalBounds().left + (invader.getGlobalBounds().width/2),
                                      invader.getGlobalBounds().top + (invader.getGlobalBounds().height/2));

        m_bulletFrequency = sf::Time::Zero;
    }

    if(!bulletOps.getInvaderBullets()->empty())
    {
        m_currbulletTime += deltaTime;

        if(m_currbulletTime >= m_bulletSpeed)
        {
            for(auto& srch : *bulletOps.getInvaderBullets())
            {
                srch.move(0, 10);
            }
            m_currbulletTime = sf::Time::Zero;
        }
    }
}
