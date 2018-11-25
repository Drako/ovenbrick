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

#include "dummy_game_state.hxx"
#include "game_state_manager.hxx"

#include <SFML/System/Time.hpp>

#include <SFML/Window/Event.hpp>

void DummyGameState::set_up()
{
}

void DummyGameState::handle_event(sf::Event const & event)
{
  if (event.type == sf::Event::KeyPressed && event.key.code == sf::Keyboard::Escape)
    GameStateManager::singleton().pop_state();
}

void DummyGameState::update(sf::Time const & /* elapsed */)
{
}

void DummyGameState::tear_down()
{
}
