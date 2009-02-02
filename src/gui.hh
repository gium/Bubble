#ifndef GUI_HH
# define GUI_HH

#include "interface.hh"
#include <vector>

class Gui : public Interface
{
public:
  ~Gui();
  void play();
private:
  void display();
  bool ask_next_move();
  bool sdl_init(size_t	height,
		size_t	width,
		int	fullscreen);

  int load_text(std::string name);
  void bitmap_output(int x, int y, const char *string, void *font);
  std::vector<int> text_;
};

#endif
