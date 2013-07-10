#include "Bullets.h"

#include <iostream>

using std::vector;
using std::shared_ptr;
using std::make_shared;

Bullets::Bullets(): m_invaderBullets(make_shared<vector<sf::Sprite>>()) { }

shared_ptr<vector<sf::Sprite>> Bullets::getInvaderBullets()
{
    return m_invaderBullets;
}

void Bullets::setInvaderBullets(const sf::Sprite& bullet)
{
    m_iBullet = make_shared<sf::Sprite>(bullet);
}

shared_ptr<sf::Sprite> Bullets::getPlayerBullet()
{
    return m_pBullet;
}

void Bullets::setPlayerBullet(const sf::Sprite& bullet)
{
    m_pBullet = make_shared<sf::Sprite>(bullet);
}

void Bullets::spawnBullet_invader(const float posX, const float posY)
{
    m_iBullet->setPosition(posX, posY);
    m_invaderBullets->push_back(*m_iBullet);
}

bool Bullets::spawnBullet_player(const float posX, const float posY)
{
    if(!m_playerBulletActive)
    {
        m_pBullet->setPosition(posX, posY);
        m_playerBulletActive = true;
    }
}

void Bullets::update()
{
    if(m_pBullet->getPosition().y <= -14)
    {
        m_playerBulletActive = false;
    }

    if(!m_invaderBullets->empty())
    {
        for(auto iter = m_invaderBullets->begin(); iter != m_invaderBullets->end(); ++iter)
        {
            if(iter->getPosition().y >= 710)
            {
                m_invaderBullets->erase(iter, iter + 1);
                return;
            }
        }
    }
}
