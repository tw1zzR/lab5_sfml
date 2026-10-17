#include <SFML/Graphics.hpp>
#include <SFML/Window.hpp>
#include <SFML/System.hpp>
#include <optional>
#include <string>
#include <cmath>
#include <algorithm>

const float PI = 3.14159265f;

struct PhysicsState {
    float speed = 300.f;
    float angleDeg = 45.f;
    sf::Vector2f velocity;
    sf::Vector2f position;
    int collisions = 0;
    bool isPaused = false;

    void updateVelocity() {
        float rad = angleDeg * PI / 180.f;
        velocity.x = speed * std::cos(rad);
        velocity.y = speed * std::sin(rad);
    }

    void reset(sf::Vector2f startPos) {
        position = startPos;
        collisions = 0;
        updateVelocity();
    }
};

int main() {
    sf::RenderWindow window(sf::VideoMode({800, 600}), "SFML Physics Demo - Variant 5");
    window.setFramerateLimit(60);

    sf::Font font;
    if (!font.openFromFile("../resources/fonts/arial.ttf")) {
        font.openFromFile("C:/Windows/Fonts/arial.ttf");
    }

    sf::Text infoText(font, "", 20);
    infoText.setFillColor(sf::Color::White);
    infoText.setPosition({20.f, 20.f});

    sf::RectangleShape bounds({700.f, 500.f});
    bounds.setPosition({50.f, 50.f});
    bounds.setFillColor(sf::Color::Transparent);
    bounds.setOutlineColor(sf::Color(100, 100, 100));
    bounds.setOutlineThickness(5.f);

    float ballRadius = 20.f;
    sf::CircleShape ball(ballRadius);
    ball.setFillColor(sf::Color::Green);
    ball.setOrigin({ballRadius, ballRadius});

    PhysicsState state;
    state.reset({400.f, 300.f});

    sf::Clock clock;

    while (window.isOpen()) {
        float dt = clock.restart().asSeconds();

        while (const std::optional event = window.pollEvent()) {
            if (event->is<sf::Event::Closed>()) {
                window.close();
            } else if (const auto* keyEvent = event->getIf<sf::Event::KeyPressed>()) {
                if (keyEvent->code == sf::Keyboard::Key::Space) {
                    state.isPaused = !state.isPaused;
                } else if (keyEvent->code == sf::Keyboard::Key::R) {
                    state.reset({400.f, 300.f});
                } else if (keyEvent->code == sf::Keyboard::Key::Up) {
                    state.speed += 50.f;
                    state.updateVelocity();
                } else if (keyEvent->code == sf::Keyboard::Key::Down) {
                    state.speed = std::max(50.f, state.speed - 50.f);
                    state.updateVelocity();
                } else if (keyEvent->code == sf::Keyboard::Key::Right) {
                    state.angleDeg += 10.f;
                    state.updateVelocity();
                } else if (keyEvent->code == sf::Keyboard::Key::Left) {
                    state.angleDeg -= 10.f;
                    state.updateVelocity();
                }
            }
        }

        if (!state.isPaused) {
            state.position.x += state.velocity.x * dt;
            state.position.y += state.velocity.y * dt;

            float leftEdge = bounds.getPosition().x;
            float rightEdge = leftEdge + bounds.getSize().x;
            float topEdge = bounds.getPosition().y;
            float bottomEdge = topEdge + bounds.getSize().y;

            if (state.position.x - ballRadius <= leftEdge) {
                state.position.x = leftEdge + ballRadius;
                state.velocity.x = -state.velocity.x;
                state.collisions++;
            } else if (state.position.x + ballRadius >= rightEdge) {
                state.position.x = rightEdge - ballRadius;
                state.velocity.x = -state.velocity.x;
                state.collisions++;
            }

            if (state.position.y - ballRadius <= topEdge) {
                state.position.y = topEdge + ballRadius;
                state.velocity.y = -state.velocity.y;
                state.collisions++;
            } else if (state.position.y + ballRadius >= bottomEdge) {
                state.position.y = bottomEdge - ballRadius;
                state.velocity.y = -state.velocity.y;
                state.collisions++;
            }
        }

        ball.setPosition(state.position);

        std::string status = state.isPaused ? "PAUSED" : "RUNNING";
        infoText.setString(
            "Status: " + status + "\n" +
            "Speed (Up/Down): " + std::to_string(static_cast<int>(state.speed)) + "\n" +
            "Angle (Left/Right): " + std::to_string(static_cast<int>(state.angleDeg)) + " deg\n" +
            "Collisions: " + std::to_string(state.collisions) + "\n" +
            "Press 'R' to Restart, 'Space' to Pause"
        );

        window.clear(sf::Color::Black);
        window.draw(bounds);
        window.draw(ball);
        window.draw(infoText);
        window.display();
    }

    return 0;
}