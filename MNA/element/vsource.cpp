#include "element.h"
#include "mna.h"
#include <cassert>

using namespace mna;

void VSource::stamp(MNAType &mna) const
{
  assert(Element::node_list().size()==2); // a voltage source is a 2-pin instance
  assert(node(0)!=node(1)); // voltage source cannot be a self-loop
  mna.fill_in(MNATermType(MNAType::POS, Element::name()),
	      MNALabelType(Element::name(), MNALabelType::BRANCH));
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
