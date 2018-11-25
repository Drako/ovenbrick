#include <catch.hpp>

#include <array>

#include "../game/game_state_manager.hxx"
#include "../game/game_state.hxx"

TEST_CASE("GameStateManager is a singleton", "[GameStateManager]")
{
  REQUIRE(!std::is_constructible<GameStateManager>::value);

  auto const & a = GameStateManager::singleton();
  auto const & b = GameStateManager::singleton();
  REQUIRE(std::addressof(a) == std::addressof(b));
}

namespace {
  class MockGameState final : public GameState
  {
    bool * m_destructed;

  public:
    explicit MockGameState(bool * destructed = nullptr)
        : m_destructed {destructed}
    {
      if (m_destructed)
      {
        *m_destructed = false;
      }
    }

    ~MockGameState() override
    {
      if (m_destructed)
      {
        *m_destructed = true;
      }
    }

    void run() override
    {
    }
  };
}

TEST_CASE("Current GameState works", "[GameStateManager]")
{
  auto & gsm = GameStateManager::singleton();

  constexpr auto const NUM_GAME_STATES = 5;

  std::array<GameState *, NUM_GAME_STATES> states{};

  for (auto n = NUM_GAME_STATES; n--;)
  {
    states[n] = new MockGameState;
    gsm.push_state(gsl::make_not_null(states[n]));
    REQUIRE(std::addressof(gsm.current()) == states[n]);
  }

  for (auto n = 1; n < NUM_GAME_STATES - 1; ++n) {
    gsm.pop_state();
    REQUIRE(std::addressof(gsm.current()) == states[n]);
  }
}

TEST_CASE("GameStates are cleaned up", "[GameStateManager]")
{
  auto & gsm = GameStateManager::singleton();

  constexpr auto const NUM_GAME_STATES = 5;

  std::array<GameState *, NUM_GAME_STATES> states{};
  bool destructed[NUM_GAME_STATES];

  for (auto n = NUM_GAME_STATES; n--;)
  {
    states[n] = new MockGameState{destructed + n};
    gsm.push_state(gsl::make_not_null(states[n]));
    REQUIRE(!destructed[n]);
  }

  for (auto n = 0; n < NUM_GAME_STATES; ++n) { // NOLINT(modernize-loop-convert)
    gsm.pop_state();
    REQUIRE(destructed[n]);
  }
}
