#include <SFML/Graphics.hpp>
#include <cmath>
#include <iostream>
#include <vector>

#define PI 3.14159265
#define PLAYER_ROTATION_SPEED 0.5f
#define PLAYER_MOVEMENT_SPEED 0.5f
#define ROWS 10
#define COLUMNS 10
#define CELL_SIZE 50.0f
#define RAY_COUNT 100
#define FOV 90.0f

double degToRad(double deg)
{
  return deg * (PI / 180);
}

double radToDeg(double rad)
{
  return rad / (PI / 180);
}

bool checkCollisionX(const sf::FloatRect& playerBounds, const std::vector<sf::RectangleShape>& objects, double& x)
{
  sf::FloatRect futureBounds = playerBounds;
  futureBounds.left += x;
  for (const auto& cell : objects)
  {
    if (futureBounds.intersects(cell.getGlobalBounds()))
    {
      return true;
    }
  }
  return false;
}

bool checkCollisionY(const sf::FloatRect& playerBounds, const std::vector<sf::RectangleShape>& objects, double& y)
{
  sf::FloatRect futureBounds = playerBounds;
  futureBounds.top += y;
  for (const auto& cell : objects)
  {
    if (futureBounds.intersects(cell.getGlobalBounds()))
    {
      return true;
    }
  }
  return false;
}

class Ray
{
  public: 
    Ray(void) :
    _angle(0.0f),
    _distance(0.0f)
    {;}

    Ray(double angle_) :
    _angle(angle_),
    _distance(0.0f)
    {;}
    
    void setAngle(double angle_)
    {
      this->_angle = angle_;
    }

    double getAngle() const
    {
      return this->_angle;
    }

    void setCollision(sf::RectangleShape collision_)
    {
      this->_collision = collision_;
    }

    sf::RectangleShape getCollision(void)
    {
      return this->_collision;
    }

  private:
    double _angle;
    double _distance;
    sf::RectangleShape _collision;
};

int main(void)
{
  sf::RenderWindow window(sf::VideoMode(500, 500), "SFML Lesson");

  sf::RectangleShape player(sf::Vector2f(2, 2)); // create a circle with radius 100
  player.setFillColor(sf::Color::Red);
  player.setOrigin(player.getLocalBounds().width / 2, player.getLocalBounds().height / 2);
  player.setPosition(window.getSize().x / 2, window.getSize().y / 2);


  double direction = 90.0;

  std::vector<int> walls = {1, 1, 1, 1, 1, 1, 1, 1, 1, 1,
                            1, 0, 0, 0, 0, 0, 0, 0, 0, 1,
                            1, 0, 0, 0, 0, 0, 0, 0, 0, 1,
                            1, 0, 0, 0, 0, 0, 0, 0, 0, 1,
                            1, 0, 0, 0, 0, 0, 0, 0, 0, 1,
                            1, 0, 0, 0, 0, 0, 0, 0, 0, 1,
                            1, 0, 0, 0, 0, 0, 0, 0, 0, 1,
                            1, 0, 0, 0, 0, 0, 0, 0, 0, 1,
                            1, 0, 0, 0, 0, 0, 0, 0, 0, 1,
                            1, 1, 1, 1, 1, 1, 1, 1, 1, 1};
  std::vector<sf::RectangleShape> cells;

  for (int row = 0; row <= ROWS; ++row)
  {
    for (int column = 0; column <= COLUMNS; ++column)
    {
      if (walls[(row * 10) + column])
      {
        sf::RectangleShape cell(sf::Vector2f(CELL_SIZE, CELL_SIZE));
        cell.setFillColor(sf::Color::White);
        cell.setOutlineThickness(1);
        cell.setOutlineColor(sf::Color::Green);
        cell.setPosition(column * CELL_SIZE, row * CELL_SIZE);
        cells.push_back(cell);
      }
    }
  }

  std::vector<Ray> rayList;

  // Create ray objects
  for (int i = 0; i < FOV; ++i)
  {
    Ray ray;
    rayList.push_back(ray);
  }


  // Main loop
  while (window.isOpen())
  {
    sf::Event event;
    while (window.pollEvent(event))
    {
      if (event.type == sf::Event::Closed)
      {
        window.close();
      }
    }

    if (sf::Keyboard::isKeyPressed(sf::Keyboard::Up))
    {
      double radians = degToRad(direction);
      double x = (std::cos(radians)) * PLAYER_MOVEMENT_SPEED;
      double y = (std::sin(radians) * -1) * PLAYER_MOVEMENT_SPEED;
      sf::FloatRect playerBounds = player.getGlobalBounds();

      bool xCollision = checkCollisionX(playerBounds, cells, x);
      bool yCollision = checkCollisionY(playerBounds, cells, y);

      if (!xCollision && !yCollision)
      {
        player.move(x, y);
      }
      else if (xCollision && !yCollision)
      {
        player.move(0, y);
      }
      else if (yCollision && !xCollision)
      {
        player.move(x, 0);
      }
    }
    if (sf::Keyboard::isKeyPressed(sf::Keyboard::Down))
    {
      double radians = degToRad(direction);
      double x = (std::cos(radians) * -1) * PLAYER_MOVEMENT_SPEED;
      double y = (std::sin(radians)) * PLAYER_MOVEMENT_SPEED;
      sf::FloatRect playerBounds = player.getGlobalBounds();

      bool xCollision = checkCollisionX(playerBounds, cells, x);
      bool yCollision = checkCollisionY(playerBounds, cells, y);

      if (!xCollision && !yCollision)
      {
        player.move(x, y);
      }
      else if (xCollision && !yCollision)
      {
        player.move(0, y);
      }
      else if (yCollision && !xCollision)
      {
        player.move(x, 0);
      }
    }
    if (sf::Keyboard::isKeyPressed(sf::Keyboard::Left))
    {
      direction = std::fmod(direction + PLAYER_ROTATION_SPEED, 360);
    }
    if (sf::Keyboard::isKeyPressed(sf::Keyboard::Right))
    {
      if (direction - PLAYER_ROTATION_SPEED <= 0)
      {
        direction = 360 - PLAYER_ROTATION_SPEED;
      }
      else
      {
        direction = direction - PLAYER_ROTATION_SPEED;
      }
    }

    //populate rays
    for (int i = 0; i < FOV; ++i)
    {
      double angle = 0.0f;
      if (i < FOV / 2)
      {
        angle = degToRad(direction) + degToRad((FOV / 2) - i);
      }
      else 
      {
        angle = degToRad(direction) - degToRad(i % 45);

        // Keep angle between 0 and 2 * PI
        if (angle < 0)
        {
          angle = angle + (2 * PI);
        }
      }

      rayList[i].setAngle(angle);
    }


    window.clear(); // Clear the screen
    
    for (auto& cell : cells)
    {
      window.draw(cell);
    }

    window.draw(player);
    window.display(); // Display the updated frame
  }

  return 0;
}
