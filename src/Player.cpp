#include "Player.h"

using std::make_shared;

std::shared_ptr<sf::Sprite> Player::getPlayer() const
{
    return m_player;
}

void Player::setPlayer(const sf::Sprite& player)
{
    m_player = make_shared<sf::Sprite>(player);
}

bool Player::getPlayerState() const
{
    return m_playerAlive;
}

void Player::setPlayerShootAudio(const sf::Sound& sound)
{
    m_shoot = sound;
}

void Player::resetPlayerState()
{
    m_playerAlive = true;
}

void Player::move(const sf::Time& deltaTime)
{
    if(m_playerAlive)
    {
        m_currTime += deltaTime;

        if(m_currTime.asSeconds() >= 0.03)
        {
            if(sf::Keyboard::isKeyPressed(sf::Keyboard::Right) && m_player->getPosition().x != 540)
            {
                m_player->move(10, 0);
            }
            if(sf::Keyboard::isKeyPressed(sf::Keyboard::Left) && m_player->getPosition().x != 20)
            {
                m_player->move(-10, 0);
            }

            m_currTime = sf::Time::Zero;
        }
    }

}

void Player::shoot(const sf::Time& deltaTime, Bullets& bulletOps)
{
    if(m_playerAlive)
    {
        if(sf::Keyboard::isKeyPressed(sf::Keyboard::Space))
        {
            if(bulletOps.spawnBullet_player(m_player->getGlobalBounds().left + (m_player->getGlobalBounds().width/2),
                                            m_player->getGlobalBounds().top))
            {
                m_shoot.play();
            }
        }

        if(bulletOps.m_playerBulletActive)
        {
            m_currBulletTime += deltaTime;

            if(m_currBulletTime.asSeconds() >= 0.01)
            {
                bulletOps.getPlayerBullet()->move(0, -10);
                m_currBulletTime = sf::Time::Zero;
            }
        }
    }
}

void Player::kill()
{
    m_playerAlive = false;
}
