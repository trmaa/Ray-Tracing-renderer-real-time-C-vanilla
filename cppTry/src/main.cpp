#include <SFML/System.hpp>
#include <SFML/Window.hpp>
#include <iostream>
#include <string>
#include "../include/window.hpp"
#include "../include/camera.hpp"
#include "../include/scene.hpp"

int w = 320;
int h = 180;
Window* win = new Window(w, h, "rtx");
Camera* cam = new Camera(w, h);
Scene* scn = new Scene("./scene.json");

void loop(float* dt, sf::Event* ev) {
    win->repaint(dt, cam, scn);
    cam->move(dt, ev);
}

int main() {
    std::cout << "starting..." << std::endl;

    sf::Clock clck;
    sf::Event ev;
    sf::Time elapsed;
    float dt;
    while (win->display()->isOpen()) { 
        while (win->display()->pollEvent(ev)) {
            if (ev.type == sf::Event::Closed) {
                win->display()->close();
            } else if (ev.type == sf::Event::Resized) {
                sf::FloatRect visibleArea(0, 0, ev.size.width, ev.size.height);
                win->display()->setView(sf::View(visibleArea));
            }
        }
        elapsed = clck.restart();
        dt = elapsed.asSeconds();

        loop(&dt, &ev);
    }

    delete win;
}