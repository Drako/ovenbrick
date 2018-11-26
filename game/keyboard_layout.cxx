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

#include "keyboard_layout.hxx"

KeyboardLayout KeyboardLayout::current;

KeyboardLayout const KeyboardLayout::XBOX {
    sfk::Up,
    sfk::Down,
    sfk::Left,
    sfk::Right,

    sfk::J,
    sfk::K,
    sfk::U,
    sfk::I,

    sfk::Escape,

    sfk::Enter,
    sfk::Space,

    "xbox"
};

KeyboardLayout const KeyboardLayout::SNES {
    sfk::Up,
    sfk::Down,
    sfk::Left,
    sfk::Right,

    sfk::J,
    sfk::K,
    sfk::U,
    sfk::I,

    sfk::Escape,

    sfk::Enter,
    sfk::Space,

    "snes"
};
