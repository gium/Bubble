#include "piece.hh"

Piece::Piece (e_color color, unsigned int x, unsigned int y)
  : color_(color), x_(x), y_(y), selected_(false)
{
}

Piece::Piece(const Piece &p)
{
  color_ = p.color_;
  x_ = p.x_;
  y_ = p.y_;
  selected_ = p.selected_;
}

Piece& Piece::operator=(const Piece &p)
{
  color_ = p.color_;
  x_ = p.x_;
  y_ = p.y_;
  selected_ = p.selected_;

  return *this;
}

bool
Piece::is_selected() const
{
  return selected_;
}

e_color
Piece::color_get() const
{
  return color_;
}

bool
Piece::operator==(e_color color) const
{
  return color_ == color;
}

bool
Piece::operator==(Piece *piece) const
{
  if (piece == 0)
    return false;

  return color_ == piece->color_get();
}

void
Piece::select_set(bool sel)
{
  selected_ = sel;
}

unsigned int
Piece::x_get() const
{
  return x_;
}

unsigned int
Piece::y_get() const
{
  return y_;
}
