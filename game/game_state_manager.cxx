#include "game_state_manager.hxx"
#include "game_state.hxx"

GameStateManager & GameStateManager::singleton()
{
  static GameStateManager instance{};
  return instance;
}

void GameStateManager::push_state(gsl::not_null<GameState *> state)
{
  m_states.push(std::unique_ptr<GameState>{state});
}

void GameStateManager::pop_state()
{
  m_states.pop();
}

GameState & GameStateManager::current() const
{
  return *m_states.top();
}
