#include "Engine.h"
#include <cstdlib>
#include <ctime> // for random number

Engine::Engine()
{
    VideoMode desktop = VideoMode::getDesktopMode();
    m_Window.create(desktop, "Particles By Chris and Ali");
}

void Engine::run()
{
    Clock clock;

    cout << "Starting Particle unit tests..." << endl;
    Particle p(m_Window, 4, { (int)m_Window.getSize().x / 2, (int)m_Window.getSize().y / 2 });
    p.unitTests();
    cout << "Unit tests complete. Starting engine..." << endl;

    while (m_Window.isOpen())
    {
        float dt = clock.restart().asSeconds();
        input();
        update(dt);
        draw();
    }
}

void Engine::input()
{
    Event event;
    while (m_Window.pollEvent(event))
    {
        if (event.type == Event::Closed) { m_Window.close(); }

        if (event.type == Event::KeyPressed && event.key.code == Keyboard::Escape)
        {
            m_Window.close();
        }

        if (event.type == Event::MouseButtonPressed && event.mouseButton.button == Mouse::Left)
        {
            Vector2i mousePos = Mouse::getPosition(m_Window);

            for (int i = 0; i < 5; i++)
            {
                int numPoints = 50 + rand() % 100 ;

                Particle newParticle(m_Window, numPoints, mousePos);

                m_particles.push_back(newParticle);
            }
        }
    }
}

void Engine::update(float dtAsSeconds)
{
    auto it = m_particles.begin();
    while (it != m_particles.end())
    {
        if (it->getTTL() > 0.0f)
        {
            it->update(dtAsSeconds);
            ++it;
        }
        else { it = m_particles.erase(it); }
    }
}

void Engine::draw()
{
    m_Window.clear();

    for (const auto& Particle : m_particles)
    {
        m_Window.draw(Particle);
    }

    m_Window.display();
}
