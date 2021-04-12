// Author: Román Chérrez Mala
// Email: cherrezmar@coventry.ac.uk

#include "user_main.h"

#include "Core/window.h"

#include "Game/game.h"

namespace Aloe {

  Game game;

  void Aloe::Start() {

    CreateWindowContext(900, 600, "Aloe Server");

    game.Start();

  }

  void Aloe::Update() {
    game.Update();
  }

  void Aloe::Close() {
  }
}