#ifndef __RNUMGEN_H__
#define __RNUMGEN_H__

#include <stdlib.h>
#include <vector>

class rnumgen {
  public:
    rnumgen( int, std::vector<int> & );

    int rand() const;

  private:
    std::vector<float> F;
};

#endif
