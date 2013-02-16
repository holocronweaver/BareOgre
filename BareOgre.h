#include <OgreRoot.h>
#include <OgreWindowEventUtilities.h>

#include <OISEvents.h>
#include <OISInputManager.h>
#include <OISKeyboard.h>
#include <OISMouse.h>

class BasicTutorial06 : public Ogre::WindowEventListener, 
  public Ogre::FrameListener
{
 public:
  BasicTutorial06(void);
  virtual ~BasicTutorial06(void);
  bool go(void);
 protected:
  virtual bool frameRenderingQueued(const Ogre::FrameEvent& evt);

  virtual void windowResized(Ogre::RenderWindow* rw);
  virtual void windowClosed(Ogre::RenderWindow* rw);
 private:
  Ogre::String mPluginsCfg;
  Ogre::String mResourcesCfg;
  Ogre::Root* mRoot;
  Ogre::RenderWindow* mWindow;
  Ogre::SceneManager* mSceneMgr;
  Ogre::Camera* mCamera;

  OIS::InputManager* mInputManager;
  OIS::Mouse* mMouse;
  OIS::Keyboard* mKeyboard;
};
