///////////////////////////////////////////////////////////////////////////////
//
// Copyright (c) 2018, Felix Bytow <drako@drako.guru>
//
// This code is licensed under the MIT License (MIT).
//
// THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
// IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
// FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
// AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
// LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
// OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
// THE SOFTWARE.
//
///////////////////////////////////////////////////////////////////////////////

#include <catch.hpp>

#include <array>

#include "../game/game_state_manager.hxx"
#include "../game/game_state.hxx"

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

TEST_CASE("GameStateManager", "[core][GameStateManager]")
{
  auto & gsm = GameStateManager::singleton();

  SECTION("GameStateManager is a singleton")
  {
    REQUIRE(!std::is_constructible<GameStateManager>::value);

    auto const & other = GameStateManager::singleton();
    REQUIRE(std::addressof(gsm) == std::addressof(other));
  }

  SECTION("Current GameState works")
  {
    constexpr auto const NUM_GAME_STATES = 5;

    std::array<GameState *, NUM_GAME_STATES> states {};

    for (auto n = NUM_GAME_STATES; n--;)
    {
      states[n] = new MockGameState;
      gsm.push_state(gsl::make_not_null(states[n]));
      REQUIRE(std::addressof(gsm.current()) == states[n]);
    }

    for (auto n = 0; n < NUM_GAME_STATES; ++n)
    {
      REQUIRE(std::addressof(gsm.current()) == states[n]);
      gsm.pop_state();
    }

    REQUIRE(gsm.is_empty());
  }

  SECTION("GameStates are cleaned up")
  {
    constexpr auto const NUM_GAME_STATES = 5;

    std::array<GameState *, NUM_GAME_STATES> states {};
    bool destructed[NUM_GAME_STATES];

    for (auto n = NUM_GAME_STATES; n--;)
    {
      states[n] = new MockGameState {destructed + n};
      gsm.push_state(gsl::make_not_null(states[n]));
      REQUIRE(!destructed[n]);
    }

    for (auto n = 0; n < NUM_GAME_STATES; ++n) // NOLINT(modernize-loop-convert)
    {
      gsm.pop_state();
      REQUIRE(destructed[n]);
    }

    REQUIRE(gsm.is_empty());
  }
}
