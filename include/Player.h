#ifndef PLAYER_H_INCLUDED
#define PLAYER_H_INCLUDED

#include <SFML\Graphics.hpp>
#include <SFML\Audio.hpp>
#include "Bullets.h"

#include <memory>

class Player
{
public:
    //Setters and getters
    std::shared_ptr<sf::Sprite> getPlayer() const;
    void setPlayer(const sf::Sprite&);
    bool getPlayerState() const;
    void setPlayerShootAudio(const sf::Sound&);
    void resetPlayerState();

    //Interactions
    void move(const sf::Time&);
    void shoot(const sf::Time&, Bullets&);
    void kill();

private:
    std::shared_ptr<sf::Sprite> m_player;
    sf::Time m_currTime;
    sf::Time m_currBulletTime;

    //Audio
    sf::Sound m_shoot;

    bool m_playerAlive = true;
};


#endif // PLAYER_H_INCLUDED
