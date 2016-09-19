#include "stdafx.h"

using namespace std;

class Application
{
public:
    bool initialize()
    {
        try
        {
            loadFont();
            createWindow();
            createDots();
            createNumbers();
            createClockCircle();
            createCenterCircle();
            createHands();
        }
        catch (const exception & e)
        {
            cerr << e.what() << endl;
            return false;
        }

        return true;
    }

    void run()
    {
        while (m_window->isOpen())
        {
            handleWindowEvents();
            updateHandsPositions();
            updateView();
        }
    }

private:
    unique_ptr<sf::RenderWindow> m_window;
    unique_ptr<sf::CircleShape> m_clockCircle;
    unique_ptr<sf::CircleShape> m_centerCircle;
    unique_ptr<sf::RectangleShape> m_hourHand;
    unique_ptr<sf::RectangleShape> m_minuteHand;
    unique_ptr<sf::RectangleShape> m_secondsHand;

    vector<sf::CircleShape> m_dots;
    vector<sf::Text> m_numbers;
    sf::Vector2f m_windowCenter;
    sf::Font m_font;

    const unsigned m_screenWidth = 800;
    const unsigned m_screenHeight = 600;
    const float m_clockCircleSize = 250.0f;
    const float m_clockCircleThickness = 2.0f;
    const float m_dotsCenterOffset = 240.0f;

    void handleWindowEvents()
    {
        sf::Event event;
        while (m_window->pollEvent(event))
        {
            if (event.type == sf::Event::Closed)
            {
                m_window->close();
            }
        }
    }

    void updateHandsPositions()
    {
        std::time_t currentTime = std::time(NULL);

        struct tm ptm;
        localtime_s(&ptm, &currentTime);

        m_hourHand->setRotation(ptm.tm_hour * 30 + (ptm.tm_min / 2.0f));
        m_minuteHand->setRotation(ptm.tm_min * 6 + (ptm.tm_sec / 12.0f));
        m_secondsHand->setRotation(ptm.tm_sec * 6.0f);
    }

    void updateView()
    {
        m_window->clear(sf::Color::White);

        m_window->draw(*m_clockCircle);

        drawCollection(m_dots);
        drawCollection(m_numbers);

        m_window->draw(*m_hourHand);
        m_window->draw(*m_minuteHand);
        m_window->draw(*m_secondsHand);
        m_window->draw(*m_centerCircle);
        m_window->display();
    }

    template <typename Item>
    void drawCollection(const vector<Item> & collection)
    {
        for_each(collection.begin(), collection.end(), [&](const Item& item) {
            m_window->draw(item);
        });
    }

    void loadFont()
    {
        if (!m_font.loadFromFile("resources/fonts/mySuperFont.ttf"))
        {
            throw runtime_error("Failed to load font");
        }
    }

    void createWindow()
    {
        sf::ContextSettings settings;
        settings.antialiasingLevel = 8;

        m_window = make_unique<sf::RenderWindow>(
            sf::VideoMode(m_screenWidth, m_screenHeight),
            "SFML Analog Clock",
            sf::Style::Close,
            settings
        );

        m_windowCenter = sf::Vector2f(m_window->getSize().x / 2.0f, m_window->getSize().y / 2.0f);
    }

    void createDots()
    {
        const unsigned dotsCount = 60;

        m_dots.reserve(dotsCount);
        iteratePositions(m_dotsCenterOffset, dotsCount, [&](float x, float y, unsigned i) {
            sf::CircleShape dot((i % 5 == 0) ? 3.0f : 1.0f);
            dot.setFillColor(sf::Color::Black);
            dot.setOrigin(dot.getGlobalBounds().width / 2, dot.getGlobalBounds().height / 2);
            dot.setPosition(x, y);
            m_dots.push_back(move(dot));
        });
    }

    void createNumbers()
    {
        const float numbersCenterOffset = m_dotsCenterOffset - 20;
        const unsigned numbersCount = 12;

        m_numbers.reserve(numbersCount);
        iteratePositions(numbersCenterOffset, numbersCount, [&](float x, float y, unsigned i) {
            unsigned value = ((3 + i) % numbersCount);
            value = (value == 0) ? 12 : value;
            unsigned fontSize = 30;

            sf::Text text;
            text.setString(to_string(value));
            text.setFont(m_font);
            text.setCharacterSize(fontSize);
            text.setFillColor(sf::Color::Black);
            text.setOrigin(text.getGlobalBounds().width / 2.0f, text.getGlobalBounds().height / 2.0f);
            text.setPosition(x, y - (fontSize / 2));
            m_numbers.push_back(move(text));
        });
    }

    void iteratePositions(float radius, unsigned count, const function<void(float x, float y, unsigned i)> & fn)
    {
        float angle = 0.0;
        for (unsigned i = 0; i < count; ++i)
        {
            auto x = radius * cos(angle);
            auto y = radius * sin(angle);

            fn(m_windowCenter.x + x, m_windowCenter.y + y, i);

            angle += ((2 * static_cast<float>(M_PI)) / count);
        }
    }

    void createClockCircle()
    {
        m_clockCircle = make_unique<sf::CircleShape>(m_clockCircleSize);
        m_clockCircle->setPointCount(100);
        m_clockCircle->setOutlineThickness(m_clockCircleThickness);
        m_clockCircle->setOutlineColor(sf::Color::Black);
        m_clockCircle->setOrigin(m_clockCircle->getGlobalBounds().width / 2, m_clockCircle->getGlobalBounds().height / 2);
        m_clockCircle->setPosition(m_windowCenter.x + m_clockCircleThickness, m_windowCenter.y + m_clockCircleThickness);
    }

    void createCenterCircle()
    {
        m_centerCircle = make_unique<sf::CircleShape>(10.0f);
        m_centerCircle->setPointCount(100);
        m_centerCircle->setFillColor(sf::Color::Black);
        m_centerCircle->setOrigin(m_centerCircle->getGlobalBounds().width / 2, m_centerCircle->getGlobalBounds().height / 2);
        m_centerCircle->setPosition(m_windowCenter);
    }

    auto createHand(const sf::Vector2f& size, const sf::Color& color = sf::Color::Black)
    {
        auto hand = make_unique<sf::RectangleShape>(size);
        hand->setFillColor(color);
        hand->setOrigin(hand->getGlobalBounds().width / 2, hand->getGlobalBounds().height - 25);
        hand->setPosition(m_windowCenter);

        return move(hand);
    }

    void createHands()
    {
        m_hourHand    = createHand(sf::Vector2f(5, 150));
        m_minuteHand  = createHand(sf::Vector2f(3, 210));
        m_secondsHand = createHand(sf::Vector2f(2, 230), sf::Color::Red);
    }
};

int main()
{
    Application app;
    if (!app.initialize())
    {
        return EXIT_FAILURE;
    }

    app.run();

    return EXIT_SUCCESS;
}

