#include "element.h"
#include "mna.h"
#include <cassert>

using namespace mna;

void Capacitor::stamp(MNAType &mna) const
{
  assert(Element::node_list().size()==2); // a capacitor is a 2-pin instance
  // a capacitor can be a self-loop and it doesn't appear in the MNA matrix
  if (node(0)!="0") // ignore GND node when filling the NMA matrix
    {
      mna.fill_in(MNATermType(MNAType::DIF, Element::name()),
		  MNALabelType(node(0)),
		  MNALabelType(node(0)));
    }
  if (node(1)!="0") // ignore GND node when filling the NMA matrix
    {
      mna.fill_in(MNATermType(MNAType::DIF, Element::name()),
		  MNALabelType(node(1)),
		  MNALabelType(node(1)));
    }
  if (node(0)!="0" && node(1)!="0")
    {
      mna.fill_in(MNATermType(MNAType::NEG_DIF, Element::name()),
		  MNALabelType(node(0)),
		  MNALabelType(node(1)));
      mna.fill_in(MNATermType(MNAType::NEG_DIF, Element::name()),
		  MNALabelType(node(1)),
		  MNALabelType(node(0)));
    }
}
