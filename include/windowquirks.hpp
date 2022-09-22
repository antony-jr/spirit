#ifndef WINDOW_QUIRKS_HPP_INCLUDED
#define WINDOW_QUIRKS_HPP_INCLUDED
#include <iostream>
#include <cstdlib>

class WindowQuirks {
   public:

      WindowQuirks();
      ~WindowQuirks();

   public:
      int yoffset();
      int xoffset();

   private:
      int n_Y = 0,
	  n_X = 0;
};

#endif
