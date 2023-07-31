//
// Created by Yifei Yao on 7/31/23.
//

#include "EntityManager.h"
#include "Utility.h"
#include "Mob.h"
#include "Player.h"
EntityManager::EntityManager(Background background,
                             Map map,
                             Player *player,
                             std::vector<Mob *> mobs
)
    : background_(std::move(
    background)),
      map_(std::move(map)),
      player_(player),
      mobs_(std::move(mobs)) {

}
void EntityManager::RenderAll(ShaderProgram *shader) const {
  background_.Render(shader);
  map_.Render(shader);
  for (auto &&mob : mobs_) {
    mob->Render(shader);
  }
  player_->Render(shader);
  switch (result) {
    case GameResult::OnGoing: {
      break;
    }
    case GameResult::Win: {
      utility::RenderText("You win", shader, 1.0, glm::vec3(-3.5, 2, 0));
      break;
    };

    case GameResult::Lose: {
      utility::RenderText("You lose", shader, 1.0, glm::vec3(-3.5, 2, 0));
      break;
    };
  }

}
const Map &EntityManager::map() const {
  return map_;
}
void EntityManager::UpdateAll(float delta_t) {
  player_->Update(delta_t, *this);
  if (!player_->IsActive()) {
    result = GameResult::Lose;
    return;
  }
  bool all_killed = true;
  for (auto &&mob : mobs_) {
    if (mob->IsActive()) {
      all_killed = false;
      mob->Update(delta_t, *this);
    }
  }
  if (all_killed) {
    result = GameResult::Win;
    return;
  }
}
std::vector<Mob *> &EntityManager::mobs() {
  return mobs_;
}