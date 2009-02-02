#include "board.hh"
#include <cstdlib>
#include <cassert>
#include <time.h>
#include <iostream>
#include <fstream>

Board::Board(unsigned int width, unsigned int height)
  : width_(width), height_(height), score_(0), selected_(0),
    map_(height, std::vector<Piece*>(width, 0))
{
  srand(time(NULL));

  score_tmp_ = 0;

  for (unsigned int i = 0; i < height; ++i)
    for (unsigned int j = 0; j < width; ++j)
      map_[i][j] = new Piece(static_cast<e_color>(rand() % 5), j, i);
}

Board::Board(const Board& b)
{
  width_ = b.width_;
  height_ = b.height_;
  score_ = b.score_;
  selected_ = b.selected_;
  map_ = b.map_;
  score_tmp_ = b.score_tmp_;

  for (unsigned i = 0; i < height_; ++i)
    for (unsigned j = 0; j < height_; ++j)
    {
      if (map_[i][j] != 0)
	map_[i][j] = new Piece(*map_[i][j]);
    }
}

Board::Board(const std::string& filename)
{
  std::ifstream stream(filename.c_str());

  if (!stream.is_open ())
    throw std::string("Unable to open the file");

  stream >> width_;
  stream >> height_;

  map_ = board_t(height_, std::vector<Piece*>(width_, 0));
  score_ = 0;
  score_tmp_ = 0;

  char c;
  for (unsigned int i = 0; i < height_; ++i)
    for (unsigned int j = 0; j < width_; ++j)
    {
      do
      {
	if (stream.eof())
	  throw std::string("Unexpected EOF");
	stream.get(c);
      }
      while (c != 'V' && c != 'B' && c != 'G' && c != 'Y' && c != 'R');

      switch (c)
      {
	case 'V' : map_[i][j] = new Piece(VIOLET, j, i); break;
	case 'B' : map_[i][j] = new Piece(BLUE, j, i); break;
	case 'G' : map_[i][j] = new Piece(GREEN, j, i); break;
	case 'Y' : map_[i][j] = new Piece(YELLOW, j, i); break;
	case 'R' : map_[i][j] = new Piece(RED, j, i); break;
      }
    }
}

Board Board::operator=(const Board& b)
{
  if (&b == this)
    return *this;

  width_ = b.width_;
  height_ = b.height_;
  score_ = b.score_;
  selected_ = b.selected_;
  map_ = b.map_;
  score_tmp_ = b.score_tmp_;

  for (unsigned i = 0; i < height_; ++i)
    for (unsigned j = 0; j < height_; ++j)
      map_[i][j] = new Piece(*map_[i][j]);

  return *this;
}

Board::~Board()
{
  for (unsigned int i = 0; i < height_; ++i)
    for (unsigned int j = 0; j < width_; ++j)
      delete map_[i][j];
}

void
Board::pick(unsigned int x, unsigned int y)
{
  assert(x < width_ && y < height_);

  if (map_[y][x] == 0)
    return;

  if (map_[y][x]->is_selected())
  {
    destroy_selected(x, y);
    gravity();
    gravity_hor();
    score_ += score_tmp_;
    score_tmp_ = 0;
  }
  else
  {
//     if (selected_ != 0)
//       deselect_all(selected_->x_get(), selected_->y_get());
    deselect_all(0, 0);
    std::vector<std::vector<bool> > adj(height_, std::vector<bool>(width_, false));
    int nb = 0;
    select_all(x, y, nb, map_[y][x]->color_get(), adj);
    if (nb > 1)
    {
      selected_ = map_[y][x];
      score_tmp_ = score_calc(nb);
    }
  }
}

unsigned int
Board::score_get() const
{
  return score_;
}

void
Board::register_interface(Interface& inter)
{
  inter.register_board(this);
}

Board::board_t&
Board::map_get()
{
  return map_;
}

bool
Board::is_finished()
{
  std::vector<std::vector<bool> > adj(height_, std::vector<bool>(width_, false));

  for (unsigned int i = 0; i < height_; ++i)
    for (unsigned int j = 0; j < width_; ++j)
    {
      if (adj[i][j])
	continue;

      if (map_[i][j] == 0)
      {
	adj[i][j] = true;
	continue;
      }

      int nb = 0;
      select_all(j, i, nb, map_[i][j]->color_get(), adj, false);
      if (nb > 1)
	return false;
    }

  return true;
}

// Private

void
Board::deselect_all(unsigned int x, unsigned int y)
{
  y = y;
  x = x;

  for (unsigned int i = 0; i < height_; ++i)
    for (unsigned int j = 0; j < width_; ++j)
      if (map_[i][j] != 0 && map_[i][j]->is_selected())
	map_[i][j]->select_set(false);
}

void
Board::select_all(unsigned int x, unsigned int y, int& nb_color, e_color color,
		  std::vector<std::vector<bool> >& mark_mat, bool sel)
{
  if (map_[y][x] != 0 && (mark_mat[y][x] == false) && (*map_[y][x] == color))
  {
    mark_mat[y][x] = true;
    ++nb_color;
  }
  else
    return;

  if (x != 0)
    select_all(x - 1, y, nb_color, color, mark_mat, sel);
  if (x < width_ - 1)
    select_all(x + 1, y, nb_color, color, mark_mat, sel);
  if (y < height_ - 1)
    select_all(x, y + 1, nb_color, color, mark_mat, sel);
  if (y != 0)
    select_all(x, y - 1, nb_color, color, mark_mat, sel);

  if (sel && nb_color > 1)
    map_[y][x]->select_set(true);
}

void
Board::destroy_selected(unsigned int x, unsigned int y)
{
  if (map_[y][x] != 0 && map_[y][x]->is_selected())
    map_[y][x] = 0;
  else
    return;

  if (x != 0)
    destroy_selected(x - 1, y);
  if (x < width_ - 1)
    destroy_selected(x + 1, y);
  if (y < height_ - 1)
    destroy_selected(x, y + 1);
  if (y != 0)
    destroy_selected(x, y - 1);
}

void
Board::gravity()
{
  for (unsigned int i = 0; i < width_; ++i)
    for (unsigned int j = 0, min = height_ - 1; j < height_; ++j)
    {
      if (map_[height_ - j - 1][i] != 0)
      {
	if (min != height_ - j - 1)
	{
	  map_[min][i] = map_[height_ - j - 1][i];
	  map_[height_ - j - 1][i] = 0;
	}

	--min;
      }
    }
}

void
Board::gravity_hor()
{
  unsigned int j;
  for (unsigned int i = 0; i < width_; ++i)
  {
    for (j = 0; j < height_; ++j)
      if (map_[j][width_ - i - 1] != 0)
	break;

    if (j == height_)		// Decalage
    {
      for (unsigned int k = width_ - i; k < width_; ++k)
	for (unsigned int j = 0; j < height_; ++j)
	{
	  map_[j][k - 1] = map_[j][k];
	  map_[j][k] = 0;
	}
    }
  }
}

int
Board::score_calc(int n)
{
  int score = 0;
  int incr = 0;

  for (int i = 0; i < n; ++i)
  {
    score += incr;
    incr += 2;
  }
  return score;
}
