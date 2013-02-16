#include "BasicTutorial06.h"

#include <OgreCamera.h>
#include <OgreConfigFile.h>
#include <OgreEntity.h>
#include <OgreException.h>
#include <OgreSceneManager.h>
#include <OgreRenderWindow.h>
#include <OgreViewport.h>
#include <OgreWindowEventUtilities.h>

BasicTutorial06::BasicTutorial06(void) 
  : mRoot(0),
    mResourcesCfg(Ogre::StringUtil::BLANK),
    mPluginsCfg(Ogre::StringUtil::BLANK)
{
}

BasicTutorial06::~BasicTutorial06(void) 
{
  //Remove ourself as a Window listener
  Ogre::WindowEventUtilities::removeWindowEventListener(mWindow, this);
  windowClosed(mWindow);

  delete mRoot;
}

bool BasicTutorial06::go(void) 
{
#ifdef _DEBUG
  mResourcesCfg = "resources_d.cfg";
  mPluginsCfg = "plugins_d.cfg";
#else
  mResourcesCfg = "resources.cfg";
  mPluginsCfg = "plugins.cfg";
#endif
  mRoot = new Ogre::Root(mPluginsCfg);
  
  // Load resource paths from resources config file.
  Ogre::ConfigFile cf;
  cf.load(mResourcesCfg);

  Ogre::ConfigFile::SectionIterator seci = cf.getSectionIterator();
  Ogre::String secName, typeName, archName;
  while (seci.hasMoreElements()) {
    secName = seci.peekNextKey();
    Ogre::ConfigFile::SettingsMultiMap *settings = seci.getNext();
    Ogre::ConfigFile::SettingsMultiMap::iterator i;
    for (i = settings->begin(); i != settings->end(); i++) {
      typeName = i->first;
      archName = i->second;
      Ogre::ResourceGroupManager::getSingleton().addResourceLocation(
          archName, typeName, secName
      );
    }
  }

  // Configure dialog.
  if (!( mRoot->restoreConfig() || mRoot->showConfigDialog() )) {
    return false;
  }

  // Create RenderWindow.
  mWindow = mRoot->initialise(true, "BasicTutorial6 Render Window");

  // Set default mipmap level.
  Ogre::TextureManager::getSingleton().setDefaultNumMipmaps(5);
  // Initialize all resource groups.
  Ogre::ResourceGroupManager::getSingleton().initialiseAllResourceGroups();

  // Create SceneManager.
  mSceneMgr = mRoot->createSceneManager("DefaultSceneManager");

  // Create a camera.
  mCamera = mSceneMgr->createCamera("PlayerCam");
  mCamera->setPosition(Ogre::Vector3(0,0,80));
  mCamera->lookAt(Ogre::Vector3(0,0,-300));
  mCamera->setNearClipDistance(5);

  // Create one viewport, entire window.
  Ogre::Viewport* vp = mWindow->addViewport(mCamera);
  vp->setBackgroundColour(Ogre::ColourValue(0,0,0));

  // Set camera aspect ratio to match viewport.
  mCamera->setAspectRatio(
      Ogre::Real(vp->getActualWidth() / Ogre::Real(vp->getActualHeight())));

  // Create a scene.
  //TODO move to createScene function
  Ogre::Entity* ogreHead = mSceneMgr->createEntity("Head", "ogrehead.mesh");
  Ogre::SceneNode* headNode = 
    mSceneMgr->getRootSceneNode()->createChildSceneNode();
  headNode->attachObject(ogreHead);
  
  mSceneMgr->setAmbientLight(Ogre::ColourValue(0.5,0.5,0.5));

  Ogre::Light* l = mSceneMgr->createLight("MainLight");
  l->setPosition(20,80,50);

  // Initialize OIS for user input.
  Ogre::LogManager::getSingletonPtr()->logMessage("*** Initializing OIS ***");
  OIS::ParamList pl;
  size_t windowHnd = 0;
  std::ostringstream windowHndStr;
 
  mWindow->getCustomAttribute("WINDOW", &windowHnd);
  windowHndStr << windowHnd;
  pl.insert(std::make_pair(std::string("WINDOW"), windowHndStr.str()));
 
  mInputManager = OIS::InputManager::createInputSystem( pl );
  //TODO switch to buffered input
  mKeyboard = static_cast<OIS::Keyboard*>(
               mInputManager->createInputObject( OIS::OISKeyboard, false ));
  mMouse = static_cast<OIS::Mouse*>(
               mInputManager->createInputObject( OIS::OISMouse, false ));

  // Set initial mouse clipping size.
  windowResized(mWindow);
 
  // Register as a Window listener.
  Ogre::WindowEventUtilities::addWindowEventListener(mWindow, this);

  // Render loop.
  mRoot->addFrameListener(this);

  mRoot->startRendering();

  return true;
}

bool BasicTutorial06::frameRenderingQueued(const Ogre::FrameEvent& evt)
{
  if (mWindow->isClosed()) {
    return false;
  }
 
  // Need to capture/update each device.
  mKeyboard->capture();
  mMouse->capture();
 
  if (mKeyboard->isKeyDown(OIS::KC_ESCAPE)) {
    return false;
  }
 
  return true;
}

// Adjust mouse clipping area.
void BasicTutorial06::windowResized(Ogre::RenderWindow* rw)
{
  unsigned int width, height, depth;
  int left, top;
  rw->getMetrics(width, height, depth, left, top);
 
  const OIS::MouseState &ms = mMouse->getMouseState();
  ms.width = width;
  ms.height = height;
}
 
// Unattach OIS before window shutdown (very important under Linux).
void BasicTutorial06::windowClosed(Ogre::RenderWindow* rw)
{
  // Only close for window that created OIS (the main window in these demos).
  if (rw == mWindow) {
    if (mInputManager) {
      mInputManager->destroyInputObject( mMouse );
      mInputManager->destroyInputObject( mKeyboard );
      
      OIS::InputManager::destroyInputSystem(mInputManager);
      mInputManager = 0;
    }
  }
}


//==============================================================================
// Cross-platform boilerplate main()
//==============================================================================
#if OGRE_PLATFORM == OGRE_PLATFORM_WIN32
#define WIN32_LEAN_AND_MEAN
#include "windows.h"
#endif

#ifdef __cplusplus
extern "C" {
#endif

#if OGRE_PLATFORM == OGRE_PLATFORM_WIN32
INT WINAPI WinMain( HINSTANCE hInst, HINSTANCE, LPSTR strCmdLine, INT )
#else
int main(int argc, char *argv[]) 
#endif
{
  BasicTutorial06 app;

  try {
    app.go();
  } catch( Ogre::Exception& e ) {
#if OGRE_PLATFORM == OGRE_PLATFORM_WIN32
    MessageBox( NULL, e.getFullDescription().c_str(), 
                "An exception has occured!", 
                MB_OK | MB_ICONERROR | MB_TASKMODAL);
#else
    std::cerr << "An exception has occured: " <<
      e.getFullDescription().c_str() << std::endl;
#endif
  }

  return 0;
}

#ifdef __cplusplus
}
#endif
