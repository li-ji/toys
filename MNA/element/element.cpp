#include "element.h"
#include <iostream>
#include <cassert>
using namespace std;

Element::Element()
{}

Element::~Element()
{}

  void
Element::setName(const NodeType &name)
{
  _name = name;
}

  void
Element::appendNode(const NodeType &node)
{
  _node_list.push_back(node);
}

  void
Element::setType(const ElementType &type)
{
  _type = type;
}

  void
Element::print() const
{
#ifndef NDEBUG
  cout << "{" << endl; // block begins
  cout << "  Name: " << _name << endl; // print the element name and type
  // print the node list
  cout << "  Node: ";
  for (NodeListType::const_iterator cit = _node_list.begin();
       cit != _node_list.end(); ++cit)
    cout << *cit << " ";
  cout << endl;
  // print the element type
  cout << "  Type: " << flush;
  switch (_type)
    {
    case R: cout << "Resistor" << endl; break;
    case C: cout << "Capacitor" << endl; break;
    case L: cout << "Inductor" << endl; break;
    case V: cout << "Independent DC voltage source" << endl; break;
    case I: cout << "Independent DC current source" << endl; break;
    case VCVS: cout << "VCVS" << endl; break;
    case CCCS: cout << "CCCS" << endl; break;
    case VCCS: cout << "VCCS" << endl; break;
    case CCVS: cout << "CCVS" << endl; break;
    default: assert(0);
    }
  cout << "}" << endl; // block ends
#endif // NDEBUG
}

//! return the node name
//! node(0) or node('+'): n+
//! node(1) or node('-'): n-
const Element::NodeType &Element::node_2pin(size_t i) const
{
  assert(Element::node_list().size()==2); // this is a 2-pin element
  switch (i)
    {
    case 0: case '+': return Element::node_list().at(0);
    case 1: case '-': return Element::node_list().at(1);
    default: break;
    }
  cerr << "Warning: node index '" << i
    << "' is too large and will be replaced by %2!" << endl;
  return Element::node_list().at(i%2);
}

//! return the node name
//! node(0) or node('+'): n+
//! node(1) or node('-'): n-
//! node(2)             : in+
//! node(3)             : in-
const Element::NodeType &Element::node_4pin(size_t i) const
{
  assert(Element::node_list().size()==4); // this is a 4-pin element
  switch (i)
    {
    case 0: case '+': return Element::node_list().at(0);
    case 1: case '-': return Element::node_list().at(1);
    case 2: case 3: return Element::node_list().at(i);
    default: break;
    }
  cerr << "Warning: node index '" << i
    << "' is too large and will be replaced by %4!" << endl;
  return Element::node_list().at(i%4);
}
