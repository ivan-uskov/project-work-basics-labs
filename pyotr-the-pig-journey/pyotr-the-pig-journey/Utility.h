#pragma once

#include <SFML/Window/Keyboard.hpp>
#include <SFML/System/Vector2.hpp>
#include <SFML/Graphics/Rect.hpp>

#include <sstream>

#define MY_PI 3.141592653589793238462643383f

namespace sf
{
    class Sprite;
    class Text;
}

class Animation;

// Since std::to_string doesn't work on MinGW we have to implement
// our own to support all platforms.
template <typename T>
std::string toString(const T & value)
{
    std::stringstream stream;
    stream << value;
    return stream.str();
}

// Convert enumerators to strings
std::string toString(sf::Keyboard::Key key);

// Call setOrigin() with the center of the object
void centerOrigin(sf::Sprite& sprite);
void centerOrigin(sf::Text& text);
void centerOrigin(Animation& animation);

// Degree/radian conversion
float toDegree(float radian);
float toRadian(float degree);

// Random number generation
int randomInt(int exclusiveMax);

// Vector operations
float length(sf::Vector2f vector);
sf::Vector2f normalize(sf::Vector2f vector);

sf::FloatRect max(const sf::FloatRect & lhs, const sf::FloatRect & rhs);
sf::Vector2f max(const sf::Vector2f & lhs, const sf::Vector2f & rhs);
sf::Vector2f min(const sf::Vector2f & lhs, const sf::Vector2f & rhs);
