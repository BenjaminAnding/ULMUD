#include <sstream>

// make a string on-the-fly
#define MAKE_STRING(msg) \
(((std::ostringstream&) (std::ostringstream() << std::boolalpha << msg)).str())

// see Josuttis p12 and Meyers p38
struct DeleteObject
  {
  template <typename T>
  void operator() (const T* ptr) const { delete ptr; }
  };
  
// similar concept for maps
struct DeleteMapObject
  {
  template <typename T>
  void operator() (const T item) const { delete item.second; }
  };

// load a set of flags from a single line in the input stream "f"
template <typename T>
void LoadSet (std::istream & f, T & s)
{
  s.clear ();
  std::string sLine;
  getline (f, sLine);   // get one line
  std::istringstream is (sLine); // convert back to stream
  std::string flag;    // each flag name
  while (!is.eof ())    // read that stream
    {
    is >> flag;   // read flag
    s.insert (flag);  // and insert it
    } // end of getting each item  
} // end of LoadSet
