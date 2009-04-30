#ifndef CMDOPTION
#define CMDOPTION

template <typename T>
class CmdOption
{
public:

  typedef T value_type;

  CmdOption(const T& _val) : val_(_val), valid_(true) { }
  CmdOption() : valid_(false), enabled_(false) { }

  // has been set and has a value
  bool is_valid(void) const   { return valid_;   }
  bool has_value(void) const  { return is_valid(); }

  // has been set and may have an value (check with is_valid())
  bool is_enabled() const { return enabled_; }

  void enable() { enabled_ = true; }

  CmdOption& operator = ( const T& _val ) 
  { 
    val_ = _val;
    valid_=true; 
    enable();
    return *this; 
  }

  operator T  () { return val_; }
  //  operator const T& () const { return val_; }

  operator T* () { return is_valid() ? &val_ : NULL; }

private:

  T    val_;
  bool valid_;
  bool enabled_;

private: // non-copyable
  
  CmdOption(const CmdOption&);
  CmdOption& operator = ( const CmdOption& );

};

template < typename T >
std::ostream& operator << ( std::ostream& _os, CmdOption<T>& _opt )
{
  _os << (T&)_opt;
  return _os;
}

#endif
