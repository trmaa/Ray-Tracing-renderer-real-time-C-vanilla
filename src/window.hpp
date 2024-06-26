#ifndef WINDOW_HPP
#define WINDOW_HPP

#include <SFML/Graphics.hpp>
#include <SFML/Window/Keyboard.hpp>
#include <cmath>
#include <glm/ext/vector_int2.hpp>
#include <glm/geometric.hpp>
#include <iostream>
#include <glm/glm.hpp>
#include <string>
#include "./camera.hpp"
#include "./scene.hpp"

sf::Color shader(int* x, int* y, glm::ivec2* buff_v, Camera* cam, Scene* scn, sf::Color* lastCol);

class Window {
private:
    glm::ivec2 m_viewport;
    sf::Image m_buffer;
    sf::RenderWindow m_display;

    sf::Font m_font;
    sf::Text m_fpsText;
    uint16_t m_fps;
    uint8_t m_fpsLimit = 15;

    sf::Texture m_texture;  
    sf::Sprite m_sprite;
public:
    sf::Image* buffer() { return &this->m_buffer; }
    sf::RenderWindow* display() { return &this->m_display; }

    Window(int w, int h, std::string text): m_viewport(w, h) {
        this->m_display.create(sf::VideoMode(w, h), text);
        this->m_buffer.create(w, h);

        if (!this->m_font.loadFromFile("./fonts/pixelmix.ttf")) {
            std::cerr << ":) no font" << std::endl;
        }

        this->m_fpsText.setFont(this->m_font);
        this->m_fpsText.setCharacterSize(24);
        this->m_fpsText.setFillColor(sf::Color(0xff00ffff));
        this->m_fpsText.setPosition(10.f, 10.f);

        //this->m_display.setFramerateLimit(this->m_fpsLimit);
    }
    ~Window() = default;
public:
    void repaint(float *dt, Camera *cam, Scene *scn, sf::Event* ev) {
        this->m_fps = static_cast<uint16_t>(1.f / *dt);
        this->m_fpsText.setString("FPS: " + std::to_string(this->m_fps));
        
        glm::ivec2* buff_v = new glm::ivec2(this->m_buffer.getSize().x, this->m_buffer.getSize().y);

        sf::Color lastCol;
        for (int y = 0; y < buff_v->y; y++) {
            for (int x = 0; x < buff_v->x; x++) {
                sf::Color col = shader(&x, &y, buff_v, cam, scn, &lastCol);
                this->m_buffer.setPixel(x, y, col);
            }
        }

        this->m_texture.loadFromImage(this->m_buffer);
        this->m_sprite.setTexture(this->m_texture);
        float scale = static_cast<float>(this->m_display.getSize().x) / buff_v->x;
        this->m_sprite.setScale(scale, scale);

        this->m_display.clear();
        this->m_display.draw(this->m_sprite);
        this->m_display.draw(this->m_fpsText);
        this->m_display.display();

        if (ev->key.code == sf::Keyboard::Tab) {
            this->m_buffer.saveToFile("./bin/screenshot.jpg");
        }

        delete buff_v;
    }    
};

#endif
