#include "ai.hh"
#include <vector>
#include <iostream>
#include "board.hh"

Ai::~Ai()
{
}

void
Ai::play()
{
  while (!board_->is_finished())
  {
    display();
    ask_next_move();
    std::cout << "Score : " << board_->score_get() << std::endl;
  }
  display();

  std::cout << "Final :" << std::endl;

  std::list<std::pair<int, int> >::const_iterator it;

  for (it = list_.begin(); it != list_.end(); ++it)
    std::cout << "(" << (*it).first << ", " << (*it).second << ") ; ";
  std::cout << std::endl;
}

// Private part

void
Ai::display()
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
Ai::ask_next_move()
{
  unsigned int x;
  unsigned int y;
  Board b = *board_;
  here_to_play(x, y, &b, 4);
  board_->pick(x, y);
  board_->pick(x, y);
  list_.push_back(std::make_pair(x, y));
  std::cout << "Nouvelle position (" << x << ", " << y << ")" << std::endl;
}

int
Ai::here_to_play(unsigned int& x, unsigned int& y, Board* board, int prof)
{
  int max = -1;
  unsigned int xx, yy;
  int score = 0;
  int score_max = 0;

  if (prof == 0)
    return 0;

  x = 0;
  y = 0;

  std::vector<std::vector<bool> > adj(board->height_,
				      std::vector<bool>(board->width_, false));

  for (unsigned int i = 0; i < board->height_; ++i)
    for (unsigned int j = 0; j < board->width_; ++j)
    {
      if (adj[i][j])
	continue;

      if (board->map_[i][j] == 0)
      {
	adj[i][j] = true;
	continue;
      }

      int nb = 0;
      board->select_all(j, i, nb, board->map_[i][j]->color_get(), adj, false);
      if (nb > 1)
      {
	Board b = *board;
	b.pick(j, i);
	b.pick(j, i);
	int tmp = here_to_play(xx, yy, &b, prof - 1);
	if (tmp > max)
	{
	  x = j;
	  y = i;
	  max = tmp;
	  score_max = nb;
	}
	score += b.score_calc(nb);
      }
    }

  return score + score_max;
}
