#include "db.h"
#include "mna.h"
#include "element.h"
#include <memory>
using namespace std;

namespace database
{

Database::Database()
{
#ifndef NDEBUG
  cout << "Construct Database" << endl;
#endif // NDEBUG
}

Database::~Database()
{
#ifndef NDEBUG
  cout << "Destruct Database" << endl;
#endif // NDEBUG
  for (DatabaseType::iterator it=_data.begin(); it!=_data.end(); ++it)
    {
      delete it->second;
      it->second = 0;
    }
  _data.clear();
}

void Database::appendElement(const std::string &element_name,
			     Element *ptr_element_data)
{
  DatabaseType::iterator it = _data.find(element_name);
  if (it!=_data.end()) // redefined element found
    cerr << "Warning: " << element_name
      << " is redefined and skipped!" << endl;
  else
    _data[element_name] = ptr_element_data;
}

//! build up the MNA matrix for the whole circuit
void Database::stamp(MNAType &mna) const
{
  for (DatabaseType::const_iterator
       it=_data.begin(); it!=_data.end(); ++it)
      it->second->stamp(mna); // stamp every element to the MNA
  mna.setup_index();
}

//! Look up element data via an element name. Return 0 if no such an element.
Element *Database::data(const std::string &element_name)
{
  DatabaseType::iterator it = _data.find(element_name);
  if (it!=_data.end())
    return it->second;
  else
    return 0;
}
} // namespace database

//! global function to access the Database singleton
DatabaseType &DB()
{
  static auto_ptr<DatabaseType> db(new DatabaseType);
  return *db;
}
