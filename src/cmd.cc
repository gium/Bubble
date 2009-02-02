#include "cmd.hh"
#include <vector>
#include <iostream>
#include "board.hh"

Cmd::~Cmd()
{
}

void
Cmd::play()
{
  while (!board_->is_finished())
  {
    display();
    ask_next_move();
    std::cout << "Score : " << board_->score_get() << std::endl;
  }
}

// Private part

void
Cmd::display()
{
  Board::board_t::iterator it;
  std::vector<Piece*>::iterator itp;

  Board::board_t& map = board_->map_get();

  for (it = map.begin(); it != map.end(); ++it)
  {
    for (itp = it->begin(); itp != it->end(); ++itp)
    {
      if (*itp != 0)
      {
	switch ((*itp)->color_get())
	{
	  case RED: std::cout << "[41m"; break;
	  case BLUE: std::cout << "[44m"; break;
	  case GREEN: std::cout << "[42m"; break;
	  case VIOLET: std::cout << "[45m"; break;
	  case YELLOW: std::cout << "[43m"; break;
	}
	std::cout << ((*itp)->is_selected() ? "." : " ") << "[40m";
      }
      else
	std::cout << " ";
    }
    std::cout << std::endl;
  }
}

void
Cmd::ask_next_move()
{
  std::cout << "Nouvelle position" << std::endl;
  unsigned int x;
  unsigned int y;
  std::cin >> x >> y;
  board_->pick(x, y);
}
