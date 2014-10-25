#include "ExampleApplication.h"

#include <iostream>

#include <OgreCamera.h>
#include <OgreOptionsFile.h>
#include <OgreEntity.h>
#include <OgreException.h>
#include <OgreSceneManager.h>
#include <OgreRenderWindow.h>
#include <OgreViewport.h>
#include <OgreWindowEventUtilities.h>

#include <OgreSceneNode.h>

ExampleApplication::ExampleApplication()
{
}

ExampleApplication::~ExampleApplication()
{
}

//-------------------------------------------------------------------------------------
void ExampleApplication::createScene()
{
  // Create SceneManager.
  mSceneMgr = mRoot->createSceneManager("DefaultSceneManager");

  // Create a camera.
  mCamera = mSceneMgr->createCamera("PlayerCam");
  mCamera->setPosition(Ogre::Vector3(0, 0, 80));
  mCamera->lookAt(Ogre::Vector3(0, 0, -3));
  mCamera->setNearClipDistance(5);
  mCamNode = mSceneMgr->getRootSceneNode()->createChildSceneNode("CamNode");
  mCamNode->attachObject(mCamera);

  // Set control parameters.
  mRotationSpeed = 0.13;
  mMaxSpeed = 1000;
  mSlowMaxSpeed = mMaxSpeed / 15;
  mAccelerationRate = 100;

  // Create one viewport, entire window.
  Ogre::Viewport* vp = mWindow->addViewport(mCamera);
  // vp->setBackgroundColour(Ogre::ColourValue(0,0,0));

  // Set camera aspect ratio to match viewport.
  mCamera->setAspectRatio(
    Ogre::Real(vp->getActualWidth() / Ogre::Real(vp->getActualHeight())));

  // Add lighting.
  mSceneMgr->setAmbientLight(Ogre::ColourValue(0.5, 0.5, 0.5));

  Ogre::Light* l = mSceneMgr->createLight("MainLight");
  l->setPosition(20, 80, 50);

  // Add meshes.
  Ogre::Entity* ogreHead = mSceneMgr->createEntity("Head", "ogrehead.mesh");
  Ogre::SceneNode* headNode =
    mSceneMgr->getRootSceneNode()->createChildSceneNode();
  headNode->attachObject(ogreHead);  
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
    ExampleApplication app;

    try {
      app.go();
    } catch (Ogre::Exception& e) {
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
