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

#include <boost/log/trivial.hpp>
#include <boost/log/utility/setup/file.hpp>
#include <boost/program_options.hpp>

#include <cstdlib>
#include <iostream>
#include <unordered_map>

#include "game/game_state_manager.hxx"
#include "game/game_state.hxx"
#include "game/dummy_game_state.hxx"
#include "game/keyboard_layout.hxx"

namespace logging = boost::log;
namespace po = boost::program_options;

void init_logging(logging::trivial::severity_level minimum_level, std::string const & filename)
{
  logging::add_file_log(filename);

  logging::core::get()->set_filter(logging::trivial::severity >= minimum_level);
}

void validate(boost::any & value, std::vector<std::string> const & values, KeyboardLayout *, int)
{
  static std::unordered_map<std::string, KeyboardLayout> const supportedLayouts {
      {"xbox", KeyboardLayout::XBOX},
      {"snes", KeyboardLayout::SNES},
  };

  po::validators::check_first_occurrence(value);
  auto const & single = po::validators::get_single_string(values);

  auto const it = supportedLayouts.find(single);
  if (it != std::cend(supportedLayouts))
    value = it->second;
  else
    throw po::validation_error {po::validation_error::invalid_option_value}; // NOLINT(cert-err60-cpp)
}

bool handle_command_line(int argc, char ** argv)
{
  po::options_description desc {"Allowed options"};
  desc.add_options()
      ("help,h", "show this help")
      (
          "key-layout,k",
          po::value<KeyboardLayout>(&KeyboardLayout::current)->default_value(KeyboardLayout::SNES, "snes"),
          "set the keyboard layout to use (xbox,snes)"
      )
      (
          "log-level,l",
          po::value<logging::trivial::severity_level>()->default_value(logging::trivial::info, "info"),
          "set the minimum log level (trace,debug,info,warning,error,fatal)"
      )
      (
          "log-file,f",
          po::value<std::string>()->default_value("ovenbrick.log"),
          "set the path of the log file"
      );
  po::variables_map vm;
  po::store(po::parse_command_line(argc, argv, desc), vm);
  po::notify(vm);

  if (vm.count("help"))
  {
    std::cout << desc << std::endl;
    return false;
  }

  init_logging(
      vm["log-level"].as<logging::trivial::severity_level>(),
      vm["log-file"].as<std::string>()
  );

  return true;
}

/**
 * @brief Main entry point.
 * @param argc Number of command line parameters (including the program name).
 * @param argv Parameters as an array of strings.
 * @return 0 in most cases.
 */
int main(int argc, char ** argv)
{
  if (!handle_command_line(argc, argv))
    return EXIT_SUCCESS;

  BOOST_LOG_TRIVIAL(info) << "Oven Brick";
  BOOST_LOG_TRIVIAL(info) << "Using keyboard layout: " << KeyboardLayout::current.m_name;

  auto const desktopMode = sf::VideoMode::getDesktopMode();
  auto const desiredMode = sf::VideoMode {320, 240, 24};
  BOOST_LOG_TRIVIAL(info)
    << "Desktop-Resolution: "
    << desktopMode.width << "x" << desktopMode.height
    << " (" << desktopMode.bitsPerPixel << " Bits)";
  sf::Uint32 const style = (desktopMode != desiredMode)
      ? (sf::Style::Titlebar | sf::Style::Close)
      : sf::Style::Fullscreen;

  sf::Window mainWindow {desiredMode, "Oven Brick", style};
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
