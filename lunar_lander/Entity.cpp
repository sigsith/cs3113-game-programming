/**
* Author: Yifei Yao
* Assignment: Lunar Lander
* Date due: 2023-07-07, 11:59pm
* I pledge that I have completed this assignment without
* collaborating with anyone else, in conformance with the
* NYU School of Engineering Policies and Procedures on
* Academic Misconduct.
**/

#include "Entity.h"

namespace ship {
    void Ship::Update(float delta_time) {
        _position += _velocity * delta_time;
        _velocity += _acceleration * delta_time;
        _acceleration = _is_engine_on ? GRAVITY +
                                        VectorByAngle(THRUST, _orientation)
                                      : GRAVITY;
        _orientation += _angular_velocity * delta_time;
        _angular_velocity += _angular_acceleration * delta_time;
        _angular_acceleration = static_cast<float>(_rcs_state) * RCS_THRUST;
    }

    void Ship::Render(ShaderProgram &program) {
        const auto state_index = _rcs_state + 1 + (_is_engine_on ? 3 : 0);
    }

    Ship::Ship(glm::vec3 position, glm::vec3 velocity, float orientation,
               GLuint texture_id) :
            _position(position), _velocity(velocity),
            _orientation(orientation), _texture_id(texture_id) {}

    void Ship::SetEngine(bool is_on) {
        _is_engine_on = is_on;
    }

    void Ship::SetRcs(int state) {
        _rcs_state = state;
    }

    Ship::Ship() = default;

    glm::vec3 VectorByAngle(float scalar, float angle_in_degree) {
        float angle_in_radians = glm::radians(angle_in_degree);
        float x = scalar * glm::cos(angle_in_radians);
        float y = scalar * glm::sin(angle_in_radians);
        return {x, y, 0.0f};
    }
}