#include <SFML\graphics.hpp>
#include <SFML\audio.hpp>

#include "AnimatedSprite.h"
#include "Invaders.h"
#include "Player.h"
#include "Bullets.h"
#include "Interaction.h"

#include <vector>
using std::vector;

#include <iostream>

int main()
{
    sf::RenderWindow window(sf::VideoMode(600, 700), "Space Invaders", sf::Style::Default);
    window.setFramerateLimit(60);

    /*******************************************************
            ANIMATION, SPRITE AND TEXTURES DECLARATIONS
    ********************************************************/
    ///START - Invaders setup
    //Set up textures
    sf::Texture invaderTexture;
    invaderTexture.loadFromFile("images/Invaders.png");
    sf::Texture deathTexture;
    deathTexture.loadFromFile("images/Death.png");

    //Set up animation
    Animation invaderAnimation;
    invaderAnimation.setSpriteSheet(invaderTexture);
    invaderAnimation.addFrame(sf::IntRect(0, 0, 34, 25));
    invaderAnimation.addFrame(sf::IntRect(39, 0, 31, 24));

    //Set up sprites
    AnimatedSprite invaderSprite;
    invaderSprite.setAnimation(invaderAnimation);
    sf::Sprite deathSprite;
    deathSprite.setTexture(deathTexture);
    ///END - Invaders setup

    ///START - Player setup
    //Set up texture
    sf::Texture playerTexture;
    playerTexture.loadFromFile("images/Player.png");

    //Set up sprite
    sf::Sprite player;
    player.setTexture(playerTexture);

    //Positioning
    player.setPosition(330, 600);
    ///END - Player setup

    ///START - Bullets setup
    //Set up texture
    sf::Texture bulletTexture_player;
    bulletTexture_player.loadFromFile("images/PlayerBullets.png");
    sf::Texture bulletTexture_invader;
    bulletTexture_invader.loadFromFile("images/InvaderBullets.png");

    //Set up sprite
    sf::Sprite bulletSprite_player;
    bulletSprite_player.setTexture(bulletTexture_player);
    sf::Sprite bulletSprite_invader;
    bulletSprite_invader.setTexture(bulletTexture_invader);
    ///END - Bullets setup

    /*******************************************************
                                AUDIO
    ********************************************************/
    sf::SoundBuffer invaderDead;
    invaderDead.loadFromFile("audio/invaderkilled.wav");
    sf::Sound iDeathSound;
    iDeathSound.setBuffer(invaderDead);

    sf::SoundBuffer playerShoot;
    playerShoot.loadFromFile("audio/shoot.wav");
    sf::Sound pShootSound;
    pShootSound.setBuffer(playerShoot);

    /*******************************************************
                                TEXT
    ********************************************************/
    sf::Font font;
    font.loadFromFile("fonts/arial.ttf");

    sf::Text scoreBoard;
    scoreBoard.setFont(font);
    scoreBoard.setString("000000");
    scoreBoard.setPosition(20, 650);
    scoreBoard.setCharacterSize(30);
    scoreBoard.setColor(sf::Color::White);
    scoreBoard.setStyle(sf::Text::Bold);
    /*******************************************************
                     SPRITE/OVERALL CONTROLS
    ********************************************************/
    //Invaders
    Invaders invaderOps;
    invaderOps.populateInvaders(invaderSprite);
    invaderOps.setDeathAnimation(deathSprite);
    invaderOps.setDeathAudio(iDeathSound);

    //Player
    Player playerOps;
    playerOps.setPlayer(player);
    playerOps.setPlayerShootAudio(pShootSound);

    //Bullets
    Bullets bulletOps;
    bulletOps.setPlayerBullet(bulletSprite_player);
    bulletOps.setInvaderBullets(bulletSprite_invader);

    //Interaction
    Interaction events(scoreBoard);

    sf::Clock gameClock;

    while(window.isOpen())
    {
        sf::Event event;

        while(window.pollEvent(event))
        {
            switch (event.type)
            {
            case sf::Event::Closed:
                window.close();
            }
        }

        /*******************************************************
                             INTERACTIONS
        ********************************************************/
        if(events.gameRunning(playerOps, invaderOps))
        {
             //Invader movement
            invaderOps.multiMove(gameClock.getElapsedTime());
            //Invader shooting
            invaderOps.multiShoot(gameClock.getElapsedTime(), bulletOps);
            //Player movement
            playerOps.move(gameClock.getElapsedTime());
            //Player shooting
            playerOps.shoot(gameClock.getElapsedTime(), bulletOps);

            //Events
            events.invaderCollision(invaderOps, bulletOps);
            events.playerCollision(playerOps, bulletOps);
            events.updateScoreBoard();

            //Updates
            invaderOps.updateDead(gameClock.getElapsedTime());
            bulletOps.update();
        }
        else
        {
            events.resetRound(gameClock.getElapsedTime(), playerOps, invaderOps, bulletOps, invaderSprite);
        }

        gameClock.restart();

        /*******************************************************
                             DRAWING DISPLAY
        ********************************************************/
        window.clear(sf::Color::Black);

        //Drawing invader bullets
        if(!bulletOps.getInvaderBullets()->empty())
        {
            for(auto bullets : *bulletOps.getInvaderBullets())
            {
                window.draw(bullets);
            }
        }

        //Drawing player bullet
        if(bulletOps.m_playerBulletActive)
        {
            window.draw(*bulletOps.getPlayerBullet());
        }

         //Drawing death animation
        if(!invaderOps.getDeathNumber()->empty())
        {
            for(auto deaths : *invaderOps.getDeathNumber())
            {
                window.draw(deaths);
            }
        }

        //Drawing invaders
        if(!invaderOps.getInvaders()->empty())
        {
            for(auto col : *invaderOps.getInvaders())
            {
                for(auto row : col)
                {
                    window.draw(row);
                }
            }
        }

        //Drawing player
        if(playerOps.getPlayerState())
        {
            window.draw(*playerOps.getPlayer());
        }

        //Drawing score
        window.draw(*events.getScore());

        window.display();
    }

    return 0;
}

//Personal notes: 1) Figure out a way to prevent accessing/running empty containers
//                2) Figure out a better way to handle death animations
//                3) Might as well construct Invaders, Bullets and Player with the needed data
//                   instead of setting it.
//                4) Issues with naming. Refer to player and how stupidly the functions are named
//                   ie. getPlayerState() no shit "player" state.
//                5) Apparently, player controls need extra "guards" compared to computer controls
//                   ie. return bool for spawnPlayerBullet to check when to play the audio
