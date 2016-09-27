#ifndef __DB_H__
#define __DB_H__

#include <string>
#include <map>

class Element; // forward declaration

//! Declare the MNA matrix type
namespace mna
{
  class MNAFormulation; /* declaration for the friend function
			   declaration inside MNAFormulation */
}
typedef mna::MNAFormulation MNAType;

namespace database
{
  class Database; // forward declaration for the friend function
}
typedef database::Database DatabaseType;
//! unique global interface to access the MNA matrix
DatabaseType &DB();

namespace database
{
//! A structure to hold all (element_name, element_data) pairs
class Database
{
  typedef std::map<std::string, Element *> DatabaseType;
  DatabaseType _data;
  Database(); //!< Constructor: the class can only be a singleton
public:
  ~Database(); //!< Destructor
  //! make a map from element names to their data and manage the memory
  void appendElement(const std::string &element_name, Element *ptr_element_data);
  //! build up the MNA matrix for the whole circuit
  void stamp(MNAType &mna) const;
  //! Look up element data via an element name. Return 0 if no such an element.
  Element *data(const std::string &element_name);
  //! global function to access the Database singleton
  friend ::DatabaseType &::DB();
};
} // namespace database

#endif // __DB_H__
