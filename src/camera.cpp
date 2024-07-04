#include "camera.hpp"

Camera::Camera(int w, int h)
    : m_position(0.0f), m_angle(glm::vec3(0.0f, 3.14159f * 0.5f, 0.0f)), m_direction(0.0f), m_speed(100.0f) {
    for (int y = 0; y < h; y++) {
        for (int x = 0; x < w; x++) {
            m_ray.push_back(Ray(
                m_position,
                glm::vec3(x - w / 2, y - h / 2, 200.0f)
            ));
        }
    }
}

glm::vec3* Camera::direction() {
    m_direction = glm::vec3(
        std::cos(m_angle.x) * std::cos(m_angle.y),
        std::sin(m_angle.x),
        std::cos(m_angle.x) * std::sin(m_angle.y)
    );
    return &m_direction;
}

void Camera::cast(int* x, int* y, glm::ivec2* buff_v) {
    int id = (*x) + (*y) * buff_v->x;

    glm::vec3 idle((*x) - buff_v->x / 2, (*y) - buff_v->y / 2, 200.0f);
    glm::vec3 idleA(
        std::atan2(idle.y, idle.z),
        std::atan2(idle.x, idle.z),
        0.0f
    );
    glm::vec3 ang = m_angle + idleA;
    glm::vec3 direction(
        2000.0f * std::cos(ang.x) * std::cos(ang.y),
        2000.0f * std::sin(ang.x),
        2000.0f * std::cos(ang.x) * std::sin(ang.y)
    );

    m_ray[id] = Ray(m_position, direction);
}

void Camera::move(float* dt, sf::Event* ev) {
    float fixedSpeed = m_speed * (*dt);
    if (ev->type != sf::Event::KeyPressed) {
        return;
    }
    if (ev->key.code == sf::Keyboard::Up) {
        m_position.x += std::cos(m_angle.y) * fixedSpeed;
        m_position.z += std::sin(m_angle.y) * fixedSpeed;
    }
    if (ev->key.code == sf::Keyboard::Down) {
        m_position.x -= std::cos(m_angle.y) * fixedSpeed;
        m_position.z -= std::sin(m_angle.y) * fixedSpeed;
    }
    if (ev->key.code == sf::Keyboard::Left) {
        m_position.x += std::cos(m_angle.y - 3.14159f / 2) * fixedSpeed;
        m_position.z += std::sin(m_angle.y - 3.14159f / 2) * fixedSpeed;
    }
    if (ev->key.code == sf::Keyboard::Right) {
        m_position.x -= std::cos(m_angle.y - 3.14159f / 2) * fixedSpeed;
        m_position.z -= std::sin(m_angle.y - 3.14159f / 2) * fixedSpeed;
    }
    if (ev->key.code == sf::Keyboard::RControl) {
        m_position.y -= fixedSpeed;
    }
    if (ev->key.code == sf::Keyboard::RShift) {
        m_position.y += fixedSpeed;
    }
    if (ev->key.code == sf::Keyboard::LAlt) {
        m_angle.y += 0.1f;
    }
    if (ev->key.code == sf::Keyboard::LShift) {
        m_angle.y -= 0.1f;
    }
    if (ev->key.code == sf::Keyboard::LControl) {
        m_angle.x += 0.1f;
    }
    if (ev->key.code == 40) {
        m_angle.x -= 0.1f;
    }
}
