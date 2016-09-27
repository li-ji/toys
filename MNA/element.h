#ifndef __ELEMENT_H__
#define __ELEMENT_H__
#include <string>
#include <vector>

//! Declare the MNA matrix type
namespace mna
{
  class MNAFormulation; /* declaration for the friend function
			   declaration inside MNAFormulation */
}
typedef mna::MNAFormulation MNAType;

//! Base class of all kinds of elements
class Element
{
public:
  typedef std::string NameType;
  typedef std::string NodeType;
  typedef std::vector<NodeType> NodeListType;
  enum ElementType //!< all supported element types
    {R, C, L, V, I, VCVS, CCCS, VCCS, CCVS};
private:
  ElementType _type; //!< element type
  NameType _name; //!< element name
  NodeListType _node_list; //!< element node list
public:
  Element();
  virtual ~Element() = 0; //!< make this class a pure virtual base class
  virtual void setName(const NameType &name); //!< set the element name
  virtual void appendNode(const NodeType &node); //!< add a node to the node list
  virtual void print() const; //!< print the element information
  virtual void stamp(MNAType &mna) const = 0; //!< fill in the MNA matrix
  const NameType &name() const { return _name; } //!< return the element name
  NodeListType &node_list() { return _node_list; } //!< return the node list
  const NodeListType &node_list() const { return _node_list; } //!< return the node list: const version
  //! return the node name by index
  virtual const NodeType &node(size_t i) const = 0;
protected:
  void setType(const ElementType &type); //!< set the element type
  const NodeType &node_2pin(size_t i) const; //!< return the node name for 2-pin elements
  const NodeType &node_4pin(size_t i) const; //!< return the node name for 4-pin elements
};

// Derived classes

class Resistor : public Element
{
public:
  Resistor() { setType(Element::R); }
  ~Resistor() {}
  //! return the node name
  virtual const NodeType &node(size_t i) const { return Element::node_2pin(i); }
  virtual void stamp(MNAType &mna) const; //!< fill in the MNA matrix
};

class Capacitor : public Element
{
public:
  Capacitor() { setType(Element::C); }
  ~Capacitor() {}
  //! return the node name
  virtual const NodeType &node(size_t i) const { return Element::node_2pin(i); }
  virtual void stamp(MNAType &mna) const; //!< fill in the MNA matrix
};

class Inductor : public Element
{
public:
  Inductor() { setType(Element::L); }
  ~Inductor() {}
  //! return the node name
  virtual const NodeType &node(size_t i) const { return Element::node_2pin(i); }
  virtual void stamp(MNAType &mna) const; //!< fill in the MNA matrix
};

class VSource : public Element
{
public:
  VSource() { setType(Element::V); }
  ~VSource() {}
  //! return the node name
  virtual const NodeType &node(size_t i) const { return Element::node_2pin(i); }
  virtual void stamp(MNAType &mna) const; //!< fill in the MNA matrix
};

class ISource : public Element
{
public:
  ISource() { setType(Element::I); }
  ~ISource() {}
  //! return the node name
  virtual const NodeType &node(size_t i) const { return Element::node_2pin(i); }
  virtual void stamp(MNAType &mna) const; //!< fill in the MNA matrix
};

class VCVS : public Element
{
public:
  VCVS() { setType(Element::VCVS); }
  ~VCVS() {}
  //! return the node name
  virtual const NodeType &node(size_t i) const { return Element::node_4pin(i); }
  virtual void stamp(MNAType &mna) const; //!< fill in the MNA matrix
};

class CCCS : public Element
{
  //! a volatge source name, through which controlling current flows
  std::string _cc;
  mutable Element *_cc_data; //!< data of controlling element
public:
  CCCS() : _cc_data(0) { setType(Element::CCCS); }
  ~CCCS() { _cc_data = 0; }
  virtual void stamp(MNAType &mna) const; //!< fill in the MNA matrix
  //! return the node name
  virtual const NodeType &node(size_t i) const;
  //! set the CC voltage device
  virtual void setCC(const std::string &vn1) { _cc = vn1; }
};

class VCCS : public Element
{
public:
  VCCS() { setType(Element::VCCS); }
  ~VCCS() {}
  //! return the node name
  virtual const NodeType &node(size_t i) const { return Element::node_4pin(i); }
  virtual void stamp(MNAType &mna) const; //!< fill in the MNA matrix
};

class CCVS : public Element
{
  //! a volatge source name, through which controlling current flows
  std::string _cc;
  mutable Element *_cc_data; //!< data of controlling element
public:
  CCVS() : _cc_data(0) { setType(Element::CCVS); }
  ~CCVS() { _cc_data = 0; }
  virtual void stamp(MNAType &mna) const; //!< fill in the MNA matrix
  //! return the node name
  virtual const NodeType &node(size_t i) const;
  //! set the CC voltage device
  virtual void setCC(const std::string &vn1) { _cc = vn1; }
};

//! Ideal Operational amplifier
class OPAMP : public VCVS
{
public:
  OPAMP() {}
  ~OPAMP() {}
  //! return the node name
  virtual const NodeType &node(size_t i) const { return VCVS::node(i); }
  virtual void stamp(MNAType &mna) const; //!< fill in the MNA matrix
};

#endif // __ELEMENT_H__
