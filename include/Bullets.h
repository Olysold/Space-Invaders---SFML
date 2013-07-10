#ifndef BULLET_H_INCLUDED
#define BULLET_H_INCLUDED

#include <SFML\Graphics.hpp>

#include <vector>
#include <random>
#include <memory>

class Bullets
{
public:
    Bullets();
    //Getters and setters
    std::shared_ptr<std::vector<sf::Sprite>> getInvaderBullets();
    void setInvaderBullets(const sf::Sprite&);
    std::shared_ptr<sf::Sprite> getPlayerBullet();
    void setPlayerBullet(const sf::Sprite&);

    void spawnBullet_invader(const float, const float);
    bool spawnBullet_player(const float, const float);
    void update();

    bool m_playerBulletActive = false;

private:
    std::shared_ptr<std::vector<sf::Sprite>> m_invaderBullets;
    std::shared_ptr<sf::Sprite> m_pBullet;
    std::shared_ptr<sf::Sprite> m_iBullet;

};

#endif // BULLET_H_INCLUDED
