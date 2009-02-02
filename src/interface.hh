#ifndef INTERFACE_HH
# define INTERFACE_HH

# include "board.hh"

class Interface
{
public:
  virtual ~Interface();
  void register_board(Board* board);
  virtual void play() = 0;
protected:
  Board* board_;
};

#endif
