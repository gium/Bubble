#ifndef BOARD_HH
# define BOARD_HH

# include "piece.hh"
# include <vector>
# include <ios>

class Interface;

class Board
{
  friend class Ai;
  friend class Gui;
public:
  Board(unsigned int width, unsigned int height);
  Board(const Board& b);
  Board(const std::string& filename);
  ~Board();

  Board operator=(const Board& b);

  void pick(unsigned int x, unsigned int y);
  unsigned int score_get() const;
  void register_interface(Interface& inter);
  bool is_finished();
  typedef std::vector<std::vector<Piece*> > board_t;
  board_t& map_get ();

private:
  unsigned int width_;
  unsigned int height_;
  unsigned int score_;
  Piece* selected_;
  board_t map_;

  unsigned int score_tmp_;

  bool is_selected(unsigned int x, unsigned int y);

  void deselect_all(unsigned int x, unsigned int y);
  void select_all(unsigned int x, unsigned int y, int& nb_color, e_color color,
		  std::vector<std::vector<bool> >& mark_mat, bool sel = true);
  void destroy_selected(unsigned int x, unsigned int y);
  void gravity();
  void gravity_hor();
  int score_calc(int n);
};

#include "interface.hh"

#endif
