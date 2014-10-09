#include <QApplication>
#include <QPushButton>
#include <QMessageBox>
#include <QLabel>

#include <vector>
#include <string>
#include <iostream>

#include "demo.h"

#include <getopt.h>

/// select which demo to run
std::string runDemo = "";
QString demoList = "[simple|signals_and_slots|opengl|uidemo|qtscript|scene]";

/*!
 * \brief print usage
 */
void usage() {

std::cout << "usage: \n"
          << "-h: print help\n"
          << "-d <string>: " << demoList.toStdString() << "\n"
          << std::endl;
}

/*!
 * Simple function to store command line arguments
 */
void parseArgs(int argc, char* argv[]) {

    int ch;
    while ((ch = getopt(argc, argv, "hd:")) != -1)   {
        switch (ch) {
            case 'h':
                usage();
                exit(0);
                break;
            case 'd':
                runDemo = std::string(optarg);
                std::cout << "setting runDemo = " << runDemo << std::endl;
            default:
                break;
        };
    }
}

/*!
 * \brief Main Qt application entry point for Project 0
 * \param argc - Argument count
 * \param argv - Argument array
 * \return QApplication exit status
 */
int main(int argc, char* argv[]) {

  // create main QApplication before any other user
  // interface widget
 
  parseArgs(argc, argv);

  QApplication app(argc, argv);


  Demo demo;
  QLabel* label = new QLabel("Rerun: " + demoList);
  label->setAlignment(Qt::AlignCenter);
  demo.setCentralWidget(label);
  demo.resize(400,400);

  if(runDemo == "simple") {
    demo.simple();
  } 
  else if(runDemo == "signals_and_slots") {
    demo.signalsAndSlots(); 
  } 
  else if(runDemo == "uidemo") {
    demo.uidemo();
  }
  else if(runDemo == "opengl") {
    demo.opengl();
  } else if(runDemo == "qtscript") {
#if defined(ENABLE_QTSCRIPT)
    demo.qtscript();
#endif
  } else if(runDemo == "scene") {
      demo.scenedemo();
  }

  demo.show();

  return app.exec();
}
