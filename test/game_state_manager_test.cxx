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
  struct MockGameState final : public GameState
  {
    struct Info
    {
      bool m_destructor = false;
      int m_setup = 0;
      int m_update = 0;
      int m_event = 0;
      int m_teardown = 0;
    };

  private:
    Info * m_info;

  public:
    explicit MockGameState(std::shared_ptr<GameStateManager> gsm, Info * info = nullptr)
        : GameState {std::move(gsm)}, m_info {info}
    {
      if (m_info)
        *m_info = Info {};
    }

    ~MockGameState() override
    {
      if (m_info)
        m_info->m_destructor = true;
    }

    void set_up() override
    {
      if (m_info)
        ++m_info->m_setup;
    }

    void update(sf::Time const & elapsed) override
    {
      if (m_info)
        ++m_info->m_update;
    }

    void handle_event(sf::Event const & event) override
    {
      if (m_info)
        ++m_info->m_event;
    }

    void tear_down() override
    {
      if (m_info)
        ++m_info->m_teardown;
    }
  };
}

TEST_CASE("GameStateManager", "[core][GameStateManager]")
{
  auto const gsm = std::make_shared<GameStateManager>();

  SECTION("GameStateManager is not a singleton, but still not copyable")
  {
    REQUIRE(std::is_constructible<GameStateManager>::value);
    REQUIRE(!std::is_copy_constructible<GameStateManager>::value);
    REQUIRE(!std::is_copy_assignable<GameStateManager>::value);
    REQUIRE(!std::is_move_constructible<GameStateManager>::value);
    REQUIRE(!std::is_move_assignable<GameStateManager>::value);
  }

  SECTION("Current GameState works")
  {
    constexpr auto const NUM_GAME_STATES = 5;

    std::array<std::shared_ptr<GameState>, NUM_GAME_STATES> states {};

    for (auto n = NUM_GAME_STATES; n--;)
    {
      states[n] = std::make_shared<MockGameState>(gsm);
      gsm->push_state(states[n]);
      REQUIRE(std::addressof(gsm->current()) == states[n].get());
    }

    for (auto n = 0; n < NUM_GAME_STATES; ++n)
    {
      REQUIRE(std::addressof(gsm->current()) == states[n].get());
      gsm->pop_state();
    }

    REQUIRE(gsm->is_empty());
  }

  SECTION("GameStates are cleaned up")
  {
    constexpr auto const NUM_GAME_STATES = 5;

    MockGameState::Info mockInfos[NUM_GAME_STATES];

    for (auto n = NUM_GAME_STATES; n--;)
    {
      gsm->push_state(std::make_shared<MockGameState>(gsm, mockInfos + n));
      REQUIRE(mockInfos[n].m_setup == 1);
    }

    for (auto n = 0; n < NUM_GAME_STATES; ++n) // NOLINT(modernize-loop-convert)
    {
      gsm->pop_state();
      REQUIRE(mockInfos[n].m_teardown == 1);
      REQUIRE(mockInfos[n].m_destructor);
    }

    REQUIRE(gsm->is_empty());
  }
}
