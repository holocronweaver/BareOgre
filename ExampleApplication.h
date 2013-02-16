#ifndef __ExampleApplication_h_
#define __ExampleApplication_h_

#include "BaseApplication.h"

class ExampleApplication : public BaseApplication
{
 public:
  ExampleApplication(void);
  virtual ~ExampleApplication(void);

 protected:
  virtual void createScene(void);
};

#endif // #ifndef __ExampleApplication_h_
