#ifndef OVENBRICK_GAME_STATE_HXX
#define OVENBRICK_GAME_STATE_HXX

struct GameState {
  virtual ~GameState() = default;

  virtual void run() = 0;
};

#endif // OVENBRICK_GAME_STATE_HXX
