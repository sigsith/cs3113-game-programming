/**
* Author: Yifei Yao
* Assignment: Platformer
* Date due: 2023-08-01, 11:59pm
* I pledge that I have completed this assignment without
* collaborating with anyone else, in conformance with the
* NYU School of Engineering Policies and Procedures on
* Academic Misconduct.
**/
#ifndef CS3113_GAME_PROGRAMMING_PLATFORMER_PLAYER_H_
#define CS3113_GAME_PROGRAMMING_PLATFORMER_PLAYER_H_

#include "Tank.h"
#include "Mob.h"
#include "Projectile.h"
#include "StaticEntity.h"

enum class PlayerFeedback {
  NoOp,
  TakeDamage
};

class EventFrame;

class Player : public Tank {
 private:
  uint immune_time_out = 0;
  uint8_t life_ = 3;
 public:
  uint Update(float delta_t, const EventFrame &event_frame,
              const Map &map,
              std::vector<Mob> &mobs,
              std::vector<std::unique_ptr<Projectile>> &,
              const std::vector<StaticEntity> &);
  Player();
  uint8_t life() const;
};

#endif //CS3113_GAME_PROGRAMMING_PLATFORMER_PLAYER_H_
