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
   * @brief Check whether there are any game states.
   * @return false, if there are game states on the stack, true otherwise.
   */
  bool is_empty() const;

  /**
   * @brief Acquire the current game state.
   * @return The current game state.
   */
  GameState & current() const;
};

#endif // OVENBRICK_GAME_STATE_MANAGER_HXX
