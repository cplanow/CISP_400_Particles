#include "Particle.h"


bool Particle::almostEqual(double a, double b, double eps)
{
	return fabs(a - b) < eps;
}

void Particle::unitTests()
{
    int score = 0;

    cout << "Testing RotationMatrix constructor...";
    double theta = PI / 4.0;
    RotationMatrix r(PI / 4);
    if (r.getRows() == 2 && r.getCols() == 2 && almostEqual(r(0, 0), cos(theta))
        && almostEqual(r(0, 1), -sin(theta))
        && almostEqual(r(1, 0), sin(theta))
        && almostEqual(r(1, 1), cos(theta)))
    {
        cout << "Passed.  +1" << endl;
        score++;
    }
    else
    {
        cout << "Failed." << endl;
    }

    cout << "Testing ScalingMatrix constructor...";
    ScalingMatrix s(1.5);
    if (s.getRows() == 2 && s.getCols() == 2
        && almostEqual(s(0, 0), 1.5)
        && almostEqual(s(0, 1), 0)
        && almostEqual(s(1, 0), 0)
        && almostEqual(s(1, 1), 1.5))
    {
        cout << "Passed.  +1" << endl;
        score++;
    }
    else
    {
        cout << "Failed." << endl;
    }

    cout << "Testing TranslationMatrix constructor...";
    TranslationMatrix t(5, -5, 3);
    if (t.getRows() == 2 && t.getCols() == 3
        && almostEqual(t(0, 0), 5)
        && almostEqual(t(1, 0), -5)
        && almostEqual(t(0, 1), 0)
        && almostEqual(t(1, 1), 0)
        && almostEqual(t(0, 2), 0)
        && almostEqual(t(1, 2), 0))
    {
        cout << "Passed.  +1" << endl;
        score++;
    }
    else
    {
        cout << "Failed." << endl;
    }

    
    cout << "Testing Particles..." << endl;
    cout << "Testing Particle mapping to Cartesian origin..." << endl;
    Vector2f zero = {0,0};
    if (m_centerCoordinate != zero)
    {
        cout << "Failed.  Expected (0,0).  Received: (" << m_centerCoordinate.x << "," << m_centerCoordinate.y << ")" << endl;
    }
    else
    {
        cout << "Passed.  +1" << endl;
        score++;
    }

    cout << "Applying one rotation of 90 degrees about the origin..." << endl;
    Matrix initialCoords = m_A;
    rotate(PI / 2.0);
    bool rotationPassed = true;
    for (int j = 0; j < initialCoords.getCols(); j++)
    {
        if (!almostEqual(m_A(0, j), -initialCoords(1, j)) || !almostEqual(m_A(1, j), initialCoords(0, j)))
        {
            cout << "Failed mapping: ";
            cout << "(" << initialCoords(0, j) << ", " << initialCoords(1, j) << ") ==> (" << m_A(0, j) << ", " << m_A(1, j) << ")" << endl;
            rotationPassed = false;
        }
    }
    if (rotationPassed)
    {
        cout << "Passed.  +1" << endl;
        score++;
    }
    else
    {
        cout << "Failed." << endl;
    }

    cout << "Applying a scale of 0.5..." << endl;
    initialCoords = m_A;
    scale(0.5);
    bool scalePassed = true;
    for (int j = 0; j < initialCoords.getCols(); j++)
    {
        if (!almostEqual(m_A(0, j), 0.5 * initialCoords(0,j)) || !almostEqual(m_A(1, j), 0.5 * initialCoords(1, j)))
        {
            cout << "Failed mapping: ";
            cout << "(" << initialCoords(0, j) << ", " << initialCoords(1, j) << ") ==> (" << m_A(0, j) << ", " << m_A(1, j) << ")" << endl;
            scalePassed = false;
        }
    }
    if (scalePassed)
    {
        cout << "Passed.  +1" << endl;
        score++;
    }
    else
    {
        cout << "Failed." << endl;
    }

    cout << "Applying a translation of (10, 5)..." << endl;
    initialCoords = m_A;
    translate(10, 5);
    bool translatePassed = true;
    for (int j = 0; j < initialCoords.getCols(); j++)
    {
        if (!almostEqual(m_A(0, j), 10 + initialCoords(0, j)) || !almostEqual(m_A(1, j), 5 + initialCoords(1, j)))
        {
            cout << "Failed mapping: ";
            cout << "(" << initialCoords(0, j) << ", " << initialCoords(1, j) << ") ==> (" << m_A(0, j) << ", " << m_A(1, j) << ")" << endl;
            translatePassed = false;
        }
    }
    if (translatePassed)
    {
        cout << "Passed.  +1" << endl;
        score++;
    }
    else
    {
        cout << "Failed." << endl;
    }

    cout << "Score: " << score << " / 7" << endl;
}
Particle::Particle(RenderTarget& target, int numPoints, Vector2i mouseClickPosition) : m_A(2, numPoints)
{
    m_ttl = TTL;
    m_numPoints = numPoints;
    // 2. Angular velocity: [0, PI]
    float randomFraction = static_cast<float>(rand()) / RAND_MAX;
    m_radiansPerSec = randomFraction * PI;

    // setting up the Cartesian view
    m_cartesianPlane.setCenter(0, 0);
    m_cartesianPlane.setSize(target.getSize().x, (-1.0f) * target.getSize().y);

    // converting pixel to Cartesian and store particle center
    m_centerCoordinate = target.mapPixelToCoords(mouseClickPosition, m_cartesianPlane);

    // random pixel velocities 100–500
    m_vx = 100 + rand() % 401; // horizontal
    if (rand() % 2 != 0) { m_vx *= -1; } // // negative random numbers
    m_vy = 100 + rand() % 401; // vertical

    m_color1 = Color :: White ;
    //m_color = Color (rand() % 255 , rand() % 255, rand() % 255);
    m_color2 = Color(rand() % 255, rand() % 255, rand() % 255);

    // generating shape
    float theta = static_cast <float> (rand () % RAND_MAX * (PI / 2)); // angle between [0: PI/ 2]
    float dTheta = 2 * PI / numPoints;

    for (int j = 0; j < numPoints; j++)
    {
        int r = 20 + rand() % (80 - 20 + 1);
        float dx = r * cos(theta);
        float dy = r * sin(theta);

        m_A(0, j) = dx;
        m_A(1, j) = dy;

        // move it to the next location
        theta += dTheta;
    }
}
void Particle::draw(RenderTarget& target, RenderStates states) const
{
    VertexArray lines(TriangleFan, m_numPoints + 1);
    
    // Center vertex
    Vector2f worldPos = m_centerCoordinate;
    Vector2i pixelPos = target.mapCoordsToPixel(worldPos, m_cartesianPlane);
    lines[0].position = Vector2f(pixelPos.x, pixelPos.y);
    lines[0].color = m_color1;

    // Outer vertices
    for (int j = 1; j <= m_numPoints; j++)
    {
        // Get the vertex position in world coordinates
        Vector2f vertexWorldPos(m_A(0, j-1), m_A(1, j-1));
        vertexWorldPos += m_centerCoordinate; // Add center offset
        
        // Convert to pixel coordinates
        Vector2i vertexPixelPos = target.mapCoordsToPixel(vertexWorldPos, m_cartesianPlane);
        lines[j].position = Vector2f(vertexPixelPos.x, vertexPixelPos.y);
        lines[j].color = m_color2;
    }

    target.draw(lines, states);
}

void Particle :: update (float dt)
{
    // Subtract dt from m_ttl
    m_ttl -= dt;
    
    // Rotate particle by dt * m_radiansPerSec
    rotate(dt * m_radiansPerSec);
    
    // Scale particle using SCALE constant
    scale(SCALE);
    
    // Calculate translation distances
    float dx = m_vx * dt;
    float dy;
    
    // Apply gravity effect to vertical velocity
    m_vy -= G * dt;
    dy = m_vy * dt;
    
    // Translate particle by calculated distances
    translate(dx, dy);
}
void Particle::translate(double xShift, double yShift)
{
    // Create translation matrix with the shifts and same number of columns as m_A
    TranslationMatrix T(xShift, yShift, m_A.getCols());
    
    // Add translation matrix to m_A
    m_A = T + m_A;
    
    // Update the particle's center coordinate
    m_centerCoordinate.x += xShift;
    m_centerCoordinate.y += yShift;
}
void Particle :: rotate(double theta)
{
   // Create rotation matrix and multiply with m_A
    RotationMatrix R(theta);
    m_A = R * m_A;
}
void Particle :: scale(double c)
{
    // Create rotation matrix and multiply with m_A
    ScalingMatrix S(c);
    m_A = S * m_A;
}