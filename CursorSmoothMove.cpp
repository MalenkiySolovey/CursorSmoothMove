#include <SFML/Graphics.hpp>
#include <cmath>
#include <cstdlib>
#include <ctime>
#include <thread>
#include <vector>
#include <fstream>
#include <iostream>

std::vector<sf::Vector2f> path;

void readTxtTarget(int& x, int& y) {
    std::fstream File("Target.txt");
    File >> x >> y;
    File.close();
}

int random(int min, int max) {
    return min + rand() % (max - min + 1);
}

void moveMouseSmoothly(int targetX, int targetY) {
    while (true) {
        double speed = random(5, 15);

        sf::Vector2i currentPos = sf::Mouse::getPosition();

        int DeltaX = targetX - currentPos.x;
        int DeltaY = targetY - currentPos.y;
        double remainingDistance = std::sqrt(DeltaX * DeltaX + DeltaY * DeltaY);

        if (remainingDistance <= speed) {
            sf::Mouse::setPosition(sf::Vector2i(targetX, targetY));
            path.push_back({ static_cast<float>(targetX), static_cast<float>(targetY) });
            break;
        }

        double progress = speed / remainingDistance;
        int nextX = currentPos.x + static_cast<int>(DeltaX * progress) + random(-2, 2);
        int nextY = currentPos.y + static_cast<int>(DeltaY * progress) + random(-2, 2);

        sf::Mouse::setPosition(sf::Vector2i(nextX, nextY));
        path.push_back({ static_cast<float>(nextX), static_cast<float>(nextY) });

        using namespace std::chrono_literals;
        std::this_thread::sleep_for(random(0, 10) * 1ms);
    }
}

int main() {
    srand(static_cast<unsigned int>(time(0)));

    sf::RenderWindow window(sf::VideoMode::getDesktopMode(), "Mouse Path", sf::Style::Fullscreen);
    window.setFramerateLimit(60);

    int x, y;
    readTxtTarget(x, y);
    std::thread(moveMouseSmoothly, x, y).detach();

    while (window.isOpen()) {
        sf::Event event;
        while (window.pollEvent(event)) {
            if (event.type == sf::Event::Closed)
                window.close();
        }
        window.clear(sf::Color::White);

        sf::VertexArray lines(sf::LinesStrip, path.size());
        for (size_t i = 0; i < path.size(); ++i) {
            lines[i].position = path[i];
            lines[i].color = sf::Color::Red;
        }
        window.draw(lines);

        window.display();
    }
    return 0;
}