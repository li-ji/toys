#include "element.h"
#include "mna.h"
#include "db.h"
#include <cassert>
#include <cstdlib> // exit()

using namespace std;
using namespace mna;

///// VCVS //////

void VCVS::stamp(MNAType &mna) const
{
  assert(Element::node_list().size()==4); // a VCVS is a 4-pin instance
  // a voltage contolling branch can be a self-loop
  assert(node(0)!=node(1)); // voltage source cannot be a self-loop
  if (node(2)!="0") // ignore GND node when filling the NMA matrix
    mna.fill_in(MNATermType(MNAType::NEG, Element::name()),
		MNALabelType(Element::name(), MNALabelType::BRANCH),
		MNALabelType(node(2)));
  if (node(3)!="0") // ignore GND node when filling the NMA matrix
    mna.fill_in(MNATermType(MNAType::POS, Element::name()),
		MNALabelType(Element::name(), MNALabelType::BRANCH),
		MNALabelType(node(3)));
  if (node(0)!="0") // ignore GND node when filling the NMA matrix
    {
      mna.fill_in(1,
		  MNALabelType(node(0)),
		  MNALabelType(Element::name(), MNALabelType::BRANCH));
      mna.fill_in(1,
		  MNALabelType(Element::name(), MNALabelType::BRANCH),
		  MNALabelType(node(0)));
    }
  if (node(1)!="0") // ignore GND node when filling the NMA matrix
    {
      mna.fill_in(-1,
		  MNALabelType(node(1)),
		  MNALabelType(Element::name(), MNALabelType::BRANCH));
      mna.fill_in(-1,
		  MNALabelType(Element::name(), MNALabelType::BRANCH),
		  MNALabelType(node(1)));
    }
}

///// CCCS //////

void CCCS::stamp(MNAType &mna) const
{
  assert(!_cc.empty());
  assert(Element::node_list().size()==2); // a CCCS is a 2-pin instance
  // a current source can be a self-loop
  if (node(0)!="0") // ignore GND node when filling the NMA matrix
    mna.fill_in(MNATermType(MNAType::POS, Element::name()),
		MNALabelType(node(0)),
		MNALabelType(_cc, MNALabelType::BRANCH));
  if (node(1)!="0") // ignore GND node when filling the NMA matrix
    mna.fill_in(MNATermType(MNAType::NEG, Element::name()),
		MNALabelType(node(1)),
		MNALabelType(_cc, MNALabelType::BRANCH));
  if (node(2)!="0") // ignore GND node when filling the NMA matrix
    {
      mna.fill_in(1,
		  MNALabelType(node(2)),
		  MNALabelType(_cc, MNALabelType::BRANCH));
      mna.fill_in(1,
		  MNALabelType(_cc, MNALabelType::BRANCH),
		  MNALabelType(node(2)));
    }
  if (node(3)!="0") // ignore GND node when filling the NMA matrix
    {
      mna.fill_in(-1,
		  MNALabelType(node(3)),
		  MNALabelType(_cc, MNALabelType::BRANCH));
      mna.fill_in(-1,
		  MNALabelType(_cc, MNALabelType::BRANCH),
		  MNALabelType(node(3)));
    }
}

//! return the node name
const Element::NodeType &CCCS::node(size_t i) const
{
  switch (i)
    {
    case 0: case '+': return Element::node_2pin(0);
    case 1: case '-': return Element::node_2pin(1);
    case 2: case 3:
		      if (!_cc_data)
			_cc_data = DB().data(_cc); // buffer the CC element
		      if (!_cc_data)
			{
			  cerr << "Error: no such a controlling element: "
			    << _cc << endl;
			  std::exit(1);
			}
		      return _cc_data->node(i-2);
    default: break;
    }
  cerr << "Warning: node index '" << i
    << "' is too large and will be replaced by %4!" << endl;
  return CCCS::node(i%4); // recursive calculation
}

///// VCCS //////

void VCCS::stamp(MNAType &mna) const
{
  assert(Element::node_list().size()==4); // a VCCS is a 4-pin instance
  // a voltage contolling branch can be a self-loop
  // a current source can be a self-loop
  if (node(0)!="0") // ignore GND node when filling the NMA matrix
    {
      if (node(2)!="0") // ignore GND node when filling the NMA matrix
	mna.fill_in(MNATermType(MNAType::POS, Element::name()),
		    MNALabelType(node(0)),
		    MNALabelType(node(2)));
      if (node(3)!="0") // ignore GND node when filling the NMA matrix
	mna.fill_in(MNATermType(MNAType::NEG, Element::name()),
		    MNALabelType(node(0)),
		    MNALabelType(node(3)));
    }
  if (node(1)!="0") // ignore GND node when filling the NMA matrix
    {
      if (node(2)!="0") // ignore GND node when filling the NMA matrix
	mna.fill_in(MNATermType(MNAType::NEG, Element::name()),
		    MNALabelType(node(1)),
		    MNALabelType(node(2)));
      if (node(3)!="0") // ignore GND node when filling the NMA matrix
	mna.fill_in(MNATermType(MNAType::POS, Element::name()),
		    MNALabelType(node(1)),
		    MNALabelType(node(3)));
    }
}

///// CCVS //////

void CCVS::stamp(MNAType &mna) const
{
  assert(!_cc.empty());
  assert(Element::node_list().size()==2); // a CCVS is a 2-pin instance
  // voltage source cannot be a self-loop
  assert(node(0)!=node(1));
  // a current contolling branch cannot be a self-loop
  assert(node(2)!=node(3));
  mna.fill_in(MNATermType(MNAType::NEG, Element::name()),
	      MNALabelType(Element::name(), MNALabelType::BRANCH),
	      MNALabelType(_cc, MNALabelType::BRANCH));
  if (node(0)!="0") // ignore GND node when filling the NMA matrix
    {
      mna.fill_in(1,
		  MNALabelType(node(0)),
		  MNALabelType(Element::name(), MNALabelType::BRANCH));
      mna.fill_in(1,
		  MNALabelType(Element::name(), MNALabelType::BRANCH),
		  MNALabelType(node(0)));
    }
  if (node(1)!="0") // ignore GND node when filling the NMA matrix
    {
      mna.fill_in(-1,
		  MNALabelType(node(1)),
		  MNALabelType(Element::name(), MNALabelType::BRANCH));
      mna.fill_in(-1,
		  MNALabelType(Element::name(), MNALabelType::BRANCH),
		  MNALabelType(node(1)));
    }
  if (node(2)!="0") // ignore GND node when filling the NMA matrix
    {
      mna.fill_in(1,
		  MNALabelType(node(2)),
		  MNALabelType(_cc, MNALabelType::BRANCH));
      mna.fill_in(1,
		  MNALabelType(_cc, MNALabelType::BRANCH),
		  MNALabelType(node(2)));
    }
  if (node(3)!="0") // ignore GND node when filling the NMA matrix
    {
      mna.fill_in(-1,
		  MNALabelType(node(3)),
		  MNALabelType(_cc, MNALabelType::BRANCH));
      mna.fill_in(-1,
		  MNALabelType(_cc, MNALabelType::BRANCH),
		  MNALabelType(node(3)));
    }
}

//! return the node name
const Element::NodeType &CCVS::node(size_t i) const
{
  switch (i)
    {
    case 0: case '+': return Element::node_2pin(0);
    case 1: case '-': return Element::node_2pin(1);
    case 2: case 3:
		      if (!_cc_data)
			_cc_data = DB().data(_cc); // buffer the CC element
		      if (!_cc_data)
			{
			  cerr << "Error: no such a controlling element: "
			    << _cc << endl;
			  std::exit(1);
			}
		      return _cc_data->node(i-2);
    default: break;
    }
  cerr << "Warning: node index '" << i
    << "' is too large and will be replaced by %4!" << endl;
  return CCVS::node(i%4); // recursive calculation
}

///// OPAMP //////

void OPAMP::stamp(MNAType &mna) const
{
  // always ignore node n2 for OPAMP because it is expected to be GND
  assert(Element::node_list().size()==4); // an OPAMP is a 4-pin instance
  assert(node(0)!=node(1)); // the output cannot be a self-loop
  if (node(2)!="0") // ignore GND node when filling the NMA matrix
    mna.fill_in(1,
		MNALabelType(Element::name(), MNALabelType::BRANCH),
		MNALabelType(node(2)));
  if (node(3)!="0") // ignore GND node when filling the NMA matrix
    mna.fill_in(-1,
		MNALabelType(Element::name(), MNALabelType::BRANCH),
		MNALabelType(node(3)));
  if (node(0)!="0") // ignore GND node when filling the NMA matrix
    mna.fill_in(1,
		MNALabelType(node(0)),
		MNALabelType(Element::name(), MNALabelType::BRANCH));
}
