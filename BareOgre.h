#ifndef __BareOgre_h_
#define __BareOgre_h_

#include "OgreRoot.h"
// #include "OgreWindowEventUtilities.h"

#include "OgreCamera.h"
#include "OgreEntity.h"
#include "OgreLogManager.h"
#include "OgreViewport.h"
#include "OgreSceneManager.h"
#include "OgreRenderWindow.h"
#include "OgreOptionsFile.h"

#include "OgreFrameListener.h"

#include "SDL.h"

#define SDL_MAIN_HANDLED

class BareOgre: public Ogre::FrameListener // public Ogre::WindowEventListener
{
public:
    BareOgre();
    virtual ~BareOgre();
    virtual bool go();

protected:
    virtual void createScene();
    virtual void initInput();
    // virtual void initVideo();
    virtual void setupResources();

    virtual bool frameRenderingQueued(const Ogre::FrameEvent& evt);

    // Ogre::WindowEventListener
    // virtual void windowResized(Ogre::RenderWindow* rw);
    // virtual void windowClosed(Ogre::RenderWindow* rw);

    // // OIS::KeyListener
    // virtual bool keyPressed(const OIS::KeyEvent &arg);
    // virtual bool keyReleased(const OIS::KeyEvent &arg);
    // // OIS::MouseListener
    // virtual bool mouseMoved(const OIS::MouseEvent &arg);
    // virtual bool mousePressed(const OIS::MouseEvent &arg,
    //                            OIS::MouseButtonID id);
    // virtual bool mouseReleased(const OIS::MouseEvent &arg,
    //                             OIS::MouseButtonID id);

// private:
    Ogre::String mPluginsCfg;
    Ogre::String mResourcesCfg;
    Ogre::Root* mRoot;
    Ogre::LogManager* mLog;
    Ogre::RenderWindow* mWindow;
    Ogre::SceneManager* mSceneMgr;
    bool mShutdown;

    Ogre::Camera* mCamera;
    Ogre::SceneNode* mCamNode;
    Ogre::Real mRotationSpeed;
    Ogre::Vector3 mVelocity;
    Ogre::Vector3 mVelocityDirection;
    Ogre::Real mMaxSpeed;
    Ogre::Real mSlowMaxSpeed;
    Ogre::Vector3 mAcceleration;
    Ogre::Vector3 mAccelerationDirection;
    Ogre::Real mAccelerationRate;
    bool mSlowMove;
    bool mMovingForward;
    bool mMovingBack;
    bool mMovingUp;
    bool mMovingDown;
    bool mMovingRight;
    bool mMovingLeft;

    // SDL input
    SDL_Window* mSDLWindow;
    virtual void processInput();
    virtual void keyPressed(const SDL_KeyboardEvent& event);
    virtual void keyReleased(const SDL_KeyboardEvent& event);
    virtual void mouseMoved(const SDL_MouseMotionEvent& event);
    virtual void mousePressed(const SDL_MouseButtonEvent& event);
    virtual void mouseReleased(const SDL_MouseButtonEvent& event);
    virtual void mouseWheelRotated(const SDL_MouseWheelEvent& event);
};

#endif
