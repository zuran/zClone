#include "Actor.h"

void Actor::Update(int dt) {}

void Actor::Draw(Screen& screen) {}

void Actor::Stop() { set_direction(MovementDirection::kNone); }
