#ifndef OVENBRICK_GAME_STATE_MANAGER_HXX
#define OVENBRICK_GAME_STATE_MANAGER_HXX

#include <memory>
#include <stack>

#include <gsl/pointers>

struct GameState;

class GameStateManager
{
  std::stack<std::unique_ptr<GameState>> m_states;

  GameStateManager() = default;

  ~GameStateManager() = default;

public:
  static GameStateManager & singleton();

  /**
   * @brief Push the new state on top of the game state stack.
   * @param state The new state.
   * @remarks The manager takes ownership of the state and will delete it.
   */
  void push_state(gsl::not_null<GameState *> state);

  /**
   * @brief Pop the current state from the game state stack.
   * @remarks This deletes the state.
   */
  void pop_state();

  /**
   * @brief Acquire the current game state.
   * @return The current game state.
   */
  GameState & current() const;
};

#endif // OVENBRICK_GAME_STATE_MANAGER_HXX
