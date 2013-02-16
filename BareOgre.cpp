#include "BareOgre.h"

#include <OgreCamera.h>
#include <OgreConfigFile.h>
#include <OgreEntity.h>
#include <OgreException.h>
#include <OgreSceneManager.h>
#include <OgreRenderWindow.h>
#include <OgreViewport.h>
#include <OgreWindowEventUtilities.h>

BareOgre::BareOgre(void) 
  : mRoot(0),
    mResourcesCfg(Ogre::StringUtil::BLANK),
    mPluginsCfg(Ogre::StringUtil::BLANK),
    mShutdown(false)
{
}

BareOgre::~BareOgre(void) 
{
  // Remove ourself as a window listener.
  Ogre::WindowEventUtilities::removeWindowEventListener(mWindow, this);
  windowClosed(mWindow);

  delete mRoot;
}

bool BareOgre::go(void) 
{
  mLog = new Ogre::LogManager;
  mLog->createLog("Ogre.log");

#ifdef _DEBUG
  mResourcesCfg = "resources_d.cfg";
  mPluginsCfg = "plugins_d.cfg";
#else
  mResourcesCfg = "resources.cfg";
  mPluginsCfg = "plugins.cfg";
#endif

  mRoot = new Ogre::Root(mPluginsCfg);

  // Load resource paths from resources config file.
  setupResources();

  // Configure dialog.
  if (!( mRoot->restoreConfig() || mRoot->showConfigDialog() )) {
    return false;
  }

  // Create RenderWindow.
  mWindow = mRoot->initialise(true, "OgreVox Render Window");

  // Set default mipmap level.
  Ogre::TextureManager::getSingleton().setDefaultNumMipmaps(5);
  // Initialize all resource groups.
  Ogre::ResourceGroupManager::getSingleton().initialiseAllResourceGroups();

  // Create scene.
  createScene();

  // Initialize input. (OIS)
  initInput();

  // Set initial mouse clipping size.
  windowResized(mWindow);

  // Register as a Window listener.
  Ogre::WindowEventUtilities::addWindowEventListener(mWindow, this);

  // Render loop.
  mRoot->addFrameListener(this);
  mRoot->startRendering();

  return true;
}

void BareOgre::createScene()
{
  // Create SceneManager.
  mSceneMgr = mRoot->createSceneManager("DefaultSceneManager");

  // Create a camera.
  mCamera = mSceneMgr->createCamera("PlayerCam");
  mCamera->setPosition(Ogre::Vector3(0,0,80));
  mCamera->lookAt(Ogre::Vector3(0,0,-3));
  mCamera->setNearClipDistance(5);
  mCamNode = mSceneMgr->getRootSceneNode()->createChildSceneNode("CamNode");
  mCamNode->attachObject(mCamera);

  // Create one viewport, entire window.
  Ogre::Viewport* vp = mWindow->addViewport(mCamera);
  vp->setBackgroundColour(Ogre::ColourValue(0,0,0));

  // Set camera aspect ratio to match viewport.
  mCamera->setAspectRatio(
    Ogre::Real(vp->getActualWidth() / Ogre::Real(vp->getActualHeight())));

  // Add lighting.
  //mSceneMgr->setAmbientLight(Ogre::ColourValue(0.5,0.5,0.5));
  mSceneMgr->setAmbientLight(Ogre::ColourValue(1.0,1.0,1.0));


  Ogre::Light* l = mSceneMgr->createLight("MainLight");
  l->setPosition(20,80,50);

  // Add meshes.
  Ogre::Entity* ogreHead = mSceneMgr->createEntity("Head", "ogrehead.mesh");
  Ogre::SceneNode* headNode =
    mSceneMgr->getRootSceneNode()->createChildSceneNode();
  headNode->attachObject(ogreHead);

  // Set player control parameters.
  mRotate = 0.13;
  mMove = 250;
  mDirection = Ogre::Vector3::ZERO;
}

void BareOgre::initInput() 
{
  mLog->logMessage("*** Initializing OIS ***");
  OIS::ParamList pl;
  size_t windowHnd = 0;
  std::ostringstream windowHndStr;

  mWindow->getCustomAttribute("WINDOW", &windowHnd);
  windowHndStr << windowHnd;
  pl.insert(std::make_pair(std::string("WINDOW"), windowHndStr.str()));

  mInputManager = OIS::InputManager::createInputSystem( pl );
  //TODO switch to buffered input
  mKeyboard = static_cast<OIS::Keyboard*>(
    mInputManager->createInputObject( OIS::OISKeyboard, true ));
  mMouse = static_cast<OIS::Mouse*>(
    mInputManager->createInputObject( OIS::OISMouse, true ));

  mKeyboard->setEventCallback(this);
  mMouse->setEventCallback(this);
}

void BareOgre::setupResources() 
{
  Ogre::ConfigFile cf;
  cf.load(mResourcesCfg);

  //TODO convert to C++11
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
}

bool BareOgre::frameRenderingQueued(const Ogre::FrameEvent& evt)
{
  if (mWindow->isClosed()) return false;
  if (mShutdown) return false;
 
  // Need to capture/update each device.
  mKeyboard->capture();
  mMouse->capture();
 
  mCamNode->translate(mDirection * evt.timeSinceLastFrame,
                      Ogre::Node::TS_LOCAL);
 
  return true;
}

// Adjust mouse clipping area.
void BareOgre::windowResized(Ogre::RenderWindow* rw)
{
  unsigned int width, height, depth;
  int left, top;
  rw->getMetrics(width, height, depth, left, top);
 
  const OIS::MouseState &ms = mMouse->getMouseState();
  ms.width = width;
  ms.height = height;
}
 
// Unattach OIS before window shutdown (very important under Linux).
void BareOgre::windowClosed(Ogre::RenderWindow* rw)
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
