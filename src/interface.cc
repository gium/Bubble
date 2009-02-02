#include "interface.hh"

Interface::~Interface()
{
}

void Interface::register_board(Board* board)
{
  board_ = board;
}
