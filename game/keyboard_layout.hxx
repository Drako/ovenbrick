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

#ifndef OVENBRICK_KEYBOARD_LAYOUT_HXX
#define OVENBRICK_KEYBOARD_LAYOUT_HXX

#include <SFML/Window/Keyboard.hpp>

#include <string>

struct KeyboardLayout final
{
  using key_type = sf::Keyboard::Key;
  using sfk = sf::Keyboard;

  key_type m_keyUp;
  key_type m_keyDown;
  key_type m_keyLeft;
  key_type m_keyRight;

  key_type m_keyA;
  key_type m_keyB;
  key_type m_keyX;
  key_type m_keyY;

  key_type m_keyMenu;

  key_type m_keyStart;
  key_type m_keySelect;

  char const * m_name;

  static KeyboardLayout current;

  static KeyboardLayout const XBOX;
  static KeyboardLayout const SNES;
};

#endif // OVENBRICK_KEYBOARD_LAYOUT_HXX
