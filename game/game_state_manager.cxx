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
