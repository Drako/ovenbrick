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
