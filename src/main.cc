#include "board.hh"
#include "ai.hh"
#include "cmd.hh"
#include "gui.hh"
#include <iostream>

int main(int argc, char *argv[])
{
  Board b(11, 11);

  if (argc == 2)
    b = Board(std::string(argv[1]));

  Board bb = b;
  Gui g;
  Ai a;

  b.register_interface(g);
  g.play();

  bb.register_interface(a);
  a.play();

  return 0;
}
