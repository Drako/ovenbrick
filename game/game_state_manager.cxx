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

#include <cassert>

#include "game_state_manager.hxx"
#include "game_state.hxx"

void GameStateManager::push_state(std::shared_ptr<GameState> state)
{
  assert(state != nullptr);
  m_states.push(state);
  current().set_up();
}

void GameStateManager::pop_state()
{
  current().tear_down();
  m_states.pop();
}

bool GameStateManager::is_empty() const
{
  return m_states.empty();
}

GameState & GameStateManager::current() const
{
  return *m_states.top();
}

void GameStateManager::handle_event(sf::Event const & event) const
{
  if (!is_empty())
    current().handle_event(event);
}

void GameStateManager::update(sf::Time const & elapsed) const
{
  if (!is_empty())
    current().update(elapsed);
}
