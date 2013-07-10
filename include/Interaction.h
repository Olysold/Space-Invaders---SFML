#ifndef INTERACTION_H_INCLUDED
#define INTERACTION_H_INCLUDED

#include <SFML\Graphics.hpp>

#include "AnimatedSprite.h"
#include "Invaders.h"
#include "Player.h"
#include "Bullets.h"

#include <vector>
#include <memory>
#include <string>

class Interaction
{
public:
    Interaction(sf::Text&);
    bool gameRunning(Player&, Invaders&);
    void resetRound(const sf::Time&, Player&, Invaders&, Bullets&, AnimatedSprite&);
    void invaderCollision(Invaders&, Bullets&);
    void playerCollision(Player&, Bullets&);
    void updateScoreBoard();
    std::shared_ptr<sf::Text> getScore() const;

private:
    sf::Time m_resetTime;
    bool m_gameState = true;
    bool m_collision = false;

    //Scoreboard
    std::shared_ptr<sf::Text> m_score;
    std::string m_strScore;
    unsigned m_numScore = 000000;

    unsigned m_chances = 3;
};

#endif // INTERACTION_H_INCLUDED
