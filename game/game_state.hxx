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

#ifndef OVENBRICK_GAME_STATE_HXX
#define OVENBRICK_GAME_STATE_HXX

#include <memory>

namespace sf {
  class Event;

  class Time;
}

class GameStateManager;

struct GameState
{
  explicit inline GameState(std::shared_ptr<GameStateManager> gsm)
      : m_gsm {gsm}
  {
  }

  virtual ~GameState() = default;

  /**
   * @brief Lifecycle function called by the GameStateManager when the state is added.
   */
  virtual void set_up() = 0;

  /**
   * @brief Lifecycle function called whenever an event occurs.
   * @param event The event which occurred.
   */
  virtual void handle_event(sf::Event const & event) = 0;

  /**
   * @brief Lifecycle function called whenever the world should be updated.
   * @param elapsed Time elapsed since the last call to update.
   */
  virtual void update(sf::Time const & elapsed) = 0;

  /**
   * @brief Lifecycle function called when the state is removed.
   */
  virtual void tear_down() = 0;

protected:
  std::shared_ptr<GameStateManager> const m_gsm;
};

#endif // OVENBRICK_GAME_STATE_HXX
