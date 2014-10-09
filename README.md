ECS 175 Project 0

Welcome to ECS 175 Introduction to Graphics. In this course you will learn about the fundamentals of the Graphics Pipeline.

But, first we need to begin with the basics and build up our projects step by step. In order to do so we need to prepare and set up all the infrastructure needed to be able to compile and execute within the proper programming environment.

This special project is designed to get you ready for the programming assignments to follow. It is highly recommended that you properly set up your work environment and learn the following concepts to be able to hit the ground running on Project 1.

Throughout this course we will use several core libraries:

* Qt5: https://qt-project.org/
* OpenGL: http://www.opengl.org/

Part 1: Download the Project0 

* Execute: git clone git@bitbucket.org:ecs175/project0.git
* or git clone https://<username>@bitbucket.org/ecs175/project0.git

Part 2: Compile the p0.pro Qt project file using qmake.

* First ensure you have the proper qmake with: qmake --version (should print Qt Version 5.x.x)

Now create a directory called p0-build and cd into it and run.

* qmake ../project0/p0.pro
* run application (for help add -h): 

  1. ./p0 -d simple
  2. ./p0 -d signals_and_slots
  3. ./p0 -d uidemo
  4. ./p0 -d opengl

Assignment: Extend OpenGL demo:

* Create user interface to modify OpenGL background & triangle color
* Print (x,y) location when right mouse button is clicked (look at QMessageBox)
* Finally, create a simple functioning calculator demo application using QtDesigner


Recommended Reading (Optional):

* Learn about Version Control Systems (Git,SVN,Mercurial)
* Learn about Doxygen
* Learn about gDebugger