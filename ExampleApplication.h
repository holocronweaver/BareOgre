#ifndef __ExampleApplication_h_
#define __ExampleApplication_h_

#include "BareOgre.h"

class ExampleApplication : public BareOgre
{
 public:
  ExampleApplication(void);
  virtual ~ExampleApplication(void);

 protected:
  virtual void createScene(void);
};

#endif // #ifndef __ExampleApplication_h_
