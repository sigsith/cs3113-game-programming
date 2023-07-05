/**
* Author: Yifei Yao
* Assignment: Lunar Lander
* Date due: 2023-07-07, 11:59pm
* I pledge that I have completed this assignment without
* collaborating with anyone else, in conformance with the
* NYU School of Engineering Policies and Procedures on
* Academic Misconduct.
**/

#ifndef LUNAR_LANDER_ENTITY_H
#define LUNAR_LANDER_ENTITY_H

#include "ShaderProgram.h"

class Entity {
private:

public:
    Entity();

    void Update(float delta_time);

    void Render(ShaderProgram *program);
};


#endif //LUNAR_LANDER_ENTITY_H
