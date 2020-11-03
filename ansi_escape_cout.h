
#include <array>
#include <cstdint>
#include <string>
#include <iostream>

#pragma once
namespace ansi_escape_codes {
/**
 * @class color_base_t
 * @brief colors 0-7. ansi colors start at 30 for foreground, and 40 for
 * background, The functions accept these and just add the difference for the
 * setting type.
 */
enum class color_base_t : u_int16_t {
  Black = 0,
  Red = 1,
  Green = 2,
  Yellow = 3,
  Blue = 4,
  Magenta = 5,
  Cyan = 6,
  White = 7
};

/**
 * @class ansi_escape_attribute_t
 * @brief one to one match of ansi text attributes.
 */
enum class ansi_escape_attribute_t : u_int16_t {
  normal = 0,
  bold = 1,
  underscore = 4,
  blink = 5,
  reverse_video = 7,
  concealed = 8
};

/**
 * @class ansi_cout_t
 * @brief public member functions to emit ansi escape
 * code pattern.
 */

class ansi_escape_cout_t {
public:
  // from http://ascii-table.com/ansi-escape-sequences.php
  //  Moves the cursor to the specified position (coordinates).
  void position(unsigned int _cur_x, unsigned int _cur_y) {
    std::cout << "\x1b[" << _cur_x << ";" << _cur_y;
  }

  //   Moves the cursor up by the specified number of lines without changing
  //   columns. If the cursor is already on the top line, ANSI.SYS ignores
  //   this sequence.
  void cursor_up(unsigned int n = 1) { std::cout << "\x1b[" << n << "A"; }

  //  Moves the cursor down by the specified number of lines without changing
  //  columns. If the cursor is already on the bottom line, ANSI.SYS ignores
  //  this sequence.
  void cursor_down(unsigned int n = 1) { std::cout << "\x1b[" << n << "B"; }

  // Moves the cursor forward by the specified number of columns without
  // changing lines. If the cursor is already in the rightmost column, ANSI.SYS
  // ignores this sequence.
  void cursor_forward(unsigned int n = 1) { std::cout << "\x1b[" << n << "C"; }

  // Moves the cursor back by the specified number of columns without changing
  // lines. If the cursor is already in the leftmost column, ANSI.SYS ignores
  // this sequence.
  void cursor_backward(unsigned int n = 1) { std::cout << "\x1b[" << n << "D"; }

  //  Saves the current cursor position. You can move the cursor to the saved
  //  cursor position by using the Restore Cursor Position sequence.
  void cursor_save() {
    std::cout << "\x1b["
              << "s";
  }

  //  Returns the cursor to the position stored by the Save Cursor Position
  //  sequence.
  void cursor_restore() {
    std::cout << "\x1b["
              << "u";
  }

  //  Clears the screen and moves the cursor to the home position (line 0,
  //  column 0).
  void erase_display() {
    std::cout << "\x1b["
              << "2J";
  }

  // Clears all characters from the cursor position to the end of the line
  // (including the character at the cursor position).
  void erase_line() {
    std::cout << "\x1b["
              << "K";
  }

  void foreground(color_base_t _c) {
    set_graphics_mode(static_cast<u_int16_t>(_c) + 40);
  }

  void background(color_base_t _c) {
    set_graphics_mode(static_cast<u_int16_t>(_c) + 30);
  }

  //,fold expression from
  // https://stackoverflow.com/questions/13562823/parameter-pack-aware-stdis-base-of/36194445
  template <typename... Args> void attribute(Args... args) {
    if constexpr (!(std::is_base_of<ansi_escape_attribute_t, Args>::value &&
                    ...))
#error "one of the ansi_escape_attribute_t required for the parameter."
      set_graphics_mode(args...);
  }

  void blink() { attribute(ansi_escape_attribute_t::blink); }
  void normal() { attribute(ansi_escape_attribute_t::normal); }
  void bold() { attribute(ansi_escape_attribute_t::bold); }
  void underscore() { attribute(ansi_escape_attribute_t::underscore); }
  void concealed() { attribute(ansi_escape_attribute_t::concealed); }
  void reverse_video() { attribute(ansi_escape_attribute_t::reverse_video); }

  // Calls the graphics functions specified by the following values. These
  // specified functions remain active until the next occurrence of this escape
  // sequence. Graphics mode changes the colors and attributes of text (such as
  // bold and underline) displayed on the screen.
  template <typename... Args> void set_graphics_mode(Args... args) {
    std::array<u_int16_t, sizeof...(args)> values = {args...};
    std::cout << "\x1b[";
    for (std::size_t idx = 0; idx < sizeof...(args); idx++) {
      std::cout << values[idx];
      if (idx < sizeof...(args) - 1)
        std::cout << ";";
    }
    std::cout << "m";
  }

  // set mode left out

  // reset mode left out
  //  	Set Keyboard Strings: left out
};
} // namespace ansi_escape_codes
