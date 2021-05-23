#ifndef LOGGER_H_
#define LOGGER_H_

#include <fstream>
#include <iostream>
#include <sstream>
#include <string>

class NulStreambuf : public std::streambuf {
    char dummyBuffer[64];

  protected:
    virtual int overflow (int c) {
        setp (dummyBuffer, dummyBuffer + sizeof (dummyBuffer));
        return (c == traits_type::eof ()) ? '\0' : c;
    }
};

class NulOStream : private NulStreambuf, public std::ostream {
  public:
    NulOStream () : std::ostream (this) {}
    const NulStreambuf* rdbuf () const { return this; }
};

class Logger : public std::ostream {
  protected:
    bool          isDummy;
    NulOStream    dump;
    int           logLevel;
    bool          isFile;
    bool          isTime;
    std::ofstream file;
    std::string   errorStr = "[ERROR]:";
    std::string   infoStr  = "[INFO]:";
    std::string   debugStr = "[DEBUG]:";
    std::ostream& getTime (std::ostream& strm);

  public:
    Logger (int logLevel, bool time = false);
    Logger (int logLevel, std::string path, bool time = false);
    bool             dummy ();
    std::ostream&    err ();
    std::ostream&    inf ();
    std::ostream&    dbg ();
    static const int NONE  = 0;
    static const int ERROR = 1;
    static const int INFO  = 2;
    static const int DEBUG = 3;
    ~Logger ();
};

class DummyLogger : public Logger {
  public:
    DummyLogger ();
};

#endif
