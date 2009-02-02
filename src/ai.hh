#ifndef AI_HH
# define AI_HH

#include "interface.hh"
#include <list>

class Ai : public Interface
{
public:
  ~Ai();
  void play();
private:
  void display();
  void ask_next_move();
  int here_to_play(unsigned int& x, unsigned int& y, Board* board, int prof);
  std::list<std::pair<int, int> > list_;
};

#endif
