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

#include <SFML/System/Clock.hpp>

#include <SFML/Window/Event.hpp>
#include <SFML/Window/Window.hpp>
#include <SFML/Window/WindowStyle.hpp>
#include <SFML/Window/VideoMode.hpp>

#include <cstdlib>

#include "game/game_state_manager.hxx"
#include "game/game_state.hxx"
#include "game/dummy_game_state.hxx"

int main()
{
  auto const desktopMode = sf::VideoMode::getDesktopMode();
  auto const style = sf::Style::Fullscreen;

  sf::Window mainWindow {desktopMode, "Oven Brick", style};
  mainWindow.setVerticalSyncEnabled(true);
  mainWindow.setMouseCursorVisible(false);

  auto & gsm = GameStateManager::singleton();
  gsm.push_state(gsl::make_not_null(new DummyGameState));

  sf::Clock clock {};
  while (mainWindow.isOpen())
  {
    sf::Event event {};
    while (mainWindow.pollEvent(event))
    {
      if (event.type == sf::Event::Closed)
        mainWindow.close();

      gsm.handle_event(event);
    }

    gsm.update(clock.restart());
    mainWindow.display();

    if (gsm.is_empty())
      mainWindow.close();
  }
  return EXIT_SUCCESS;
}
