#ifdef STANDALONE

#include "autopilot.h"
#include <string>
#include <sys/inotify.h>
#include <iostream>
#include <fstream>
#include <unistd.h>
#include <thread>
#include <cstring>

using namespace std;

#define EVENT_SIZE  ( sizeof (struct inotify_event) )
#define BUF_LEN     ( 1024 * ( EVENT_SIZE + 16 ) )

void standaloneRunner() {
  while (true) {
    autopilotThreadRunner();
  }
}

int folderMonitoring(string monitorFolder) 
{
  char buffer[BUF_LEN]; 
  int fd = inotify_init();
  if ( fd < 0 ) {
    perror( "inotify_init" );
  }
  int wd = inotify_add_watch( fd, monitorFolder.c_str() , IN_CREATE);
    
  while (1) {
    int i = 0;
    memset(buffer, 0, BUF_LEN);
    
    int length = read( fd, buffer, BUF_LEN );
    getInterpolatedData(getCurrentTime());

    if ( length < 0 ) {
      perror( "read" );
    }  
 
    while ( i < length ) {
      struct inotify_event *event = ( struct inotify_event * ) &buffer[ i ];
      if ( event->len ) {
        string dataFileName(event->name, 30);
        dataFileName = dataFileName.substr(0, dataFileName.find(".")).append(".txt");
        dataFileName.insert(0,monitorFolder);
        
        ofstream dataFile(dataFileName, ofstream::out);
        dataFile << text_file_string_buffer;
        dataFile.close();

      }
      i += EVENT_SIZE + event->len;
    } 
  }
}

int main(int argc, char *argv[]) {
  cout << "Program Started" << endl;
  
  std::string monitorFolder;
  if (argc > 1)
  {
    monitorFolder = argv[1];
  }

  initAutopilotDataReading();

  std::thread t1(standaloneRunner);
  cout << "Started Autopilot Parsing Thread" << endl;
  t1.detach();
  while (waitForDataReady() == 1);

  cout << monitorFolder << endl;
  folderMonitoring(monitorFolder);
  
  return 0;
}

#endif