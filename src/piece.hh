#ifndef PIECE_HH
# define PIECE_HH

enum e_color
  {
    RED,
    BLUE,
    GREEN,
    VIOLET,
    YELLOW
  };

class Piece
{
public:
  Piece (e_color color, unsigned int x, unsigned int y);
  Piece(const Piece &p);
  Piece& operator=(const Piece &p);
  bool is_selected() const;
  void select_set(bool sel);
  e_color color_get() const;
  bool operator==(e_color color) const;
  bool operator==(Piece *piece) const;
  unsigned int x_get() const;
  unsigned int y_get() const;
private:
  e_color color_;
  unsigned int x_;
  unsigned int y_;
  bool selected_;
};

#endif
