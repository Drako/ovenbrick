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

#ifndef OVENBRICK_DUMMY_GAME_STATE_HXX
#define OVENBRICK_DUMMY_GAME_STATE_HXX

#include "game_state.hxx"

struct DummyGameState final : public GameState
{
  DummyGameState() = default;

  ~DummyGameState() override = default;

  void set_up() override;

  void handle_event(sf::Event const & event) override;

  void update(sf::Time const & elapsed) override;

  void tear_down() override;
};

#endif // OVENBRICK_DUMMY_GAME_STATE_HXX
