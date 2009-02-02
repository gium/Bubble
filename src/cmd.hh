#ifndef CMD_HH
# define CMD_HH

# include "interface.hh"

class Cmd : public Interface
{
public:
  ~Cmd();
  void play();
private:
  void display();
  void ask_next_move();
};

#endif
