#include "Interaction.h"

#include <iostream>

#include <iomanip>
using std::setw;
using std::setfill;

#include <vector>
using std::vector;

#include <sstream>
using std::istringstream;

using std::make_shared;

Interaction::Interaction(sf::Text& score): m_score(make_shared<sf::Text>(score)) { }

bool Interaction::gameRunning(Player& playerOps, Invaders& invaderOps)
{
    if(!playerOps.getPlayerState())
    {
        m_gameState = false;
        return false;
    }

    return true;
}

void Interaction::resetRound(const sf::Time& deltaTime, Player& playerOps, Invaders& invaderOps, Bullets& bulletOps, AnimatedSprite& animatedInvader)
{
    if(!m_gameState)
    {
        m_resetTime += deltaTime;

        if(m_resetTime.asSeconds() >= 1.0)
        {
            auto player     = playerOps.getPlayer();
            auto invaders   = invaderOps.getInvaders();
            auto pBullet    = bulletOps.getPlayerBullet();
            auto iBullets   = bulletOps.getInvaderBullets();
            auto iDeaths    = invaderOps.getDeathNumber();

            //Reset positions
            player->setPosition(330, 600);
            playerOps.resetPlayerState();

            invaders->clear();
            invaderOps.populateInvaders(animatedInvader);

            pBullet->setPosition(0, 0);

            iBullets->clear();
            bulletOps.m_playerBulletActive = false;

            iDeaths->clear();

            m_gameState = true;

            m_resetTime = sf::Time::Zero;
        }
    }
}

void Interaction::invaderCollision(Invaders& invaderOps, Bullets& bulletOps)
{
    m_collision = false;

    if(!invaderOps.getInvaders()->empty())
    {
        auto invaderPop = invaderOps.getInvaders();
        auto bullet = bulletOps.getPlayerBullet();

        for(auto col = invaderPop->begin(); col != invaderPop->end(); ++col)
        {
            for(auto row = col->begin(); row != col->end(); ++row)
            {
                if(bullet->getGlobalBounds().intersects(row->getGlobalBounds()))
                {
                    invaderOps.kill(col, row);
                    bullet->setPosition(0, 0);
                    bulletOps.m_playerBulletActive = false;

                    m_collision = true;

                    return;
                }
            }
        }
    }
}

void Interaction::playerCollision(Player& playerOps, Bullets& bulletOps)
{
    if(!bulletOps.getInvaderBullets()->empty())
    {
        auto player = playerOps.getPlayer();

        for(auto bullets : *bulletOps.getInvaderBullets())
        {
            if(bullets.getGlobalBounds().intersects(player->getGlobalBounds()))
            {
                playerOps.kill();
            }
        }
    }
}

void Interaction::updateScoreBoard()
{
    if(m_collision)
    {
        m_numScore += 10;

        std::ostringstream oss;
        oss << setw(6) << setfill('0') << m_numScore;
        m_strScore = oss.str();

        m_score->setString(m_strScore);
    }
}

std::shared_ptr<sf::Text> Interaction::getScore() const
{
    return m_score;
}
