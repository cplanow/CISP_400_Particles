#pragma once
#pragma once
#include <SFML/Graphics.hpp>
#include <SFML/Audio.hpp> // for audio
#include "Particle.h"
using namespace sf;
using namespace std;

class Engine
{
private:
	// A regular RenderWindow
	RenderWindow m_Window;

	//vector for Particles
	vector<Particle> m_particles;


    // Sound members
    SoundBuffer m_bufferClick;
    Sound m_clickSound;

    //background music
    Music m_backgroundMusic;


    // Private functions for internal use only
	void input();
	void update(float dtAsSeconds);
	void draw();

public:
	// The Engine constructor
	Engine();

	// Run will call all the private functions
	void run();

};

