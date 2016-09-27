#include "element.h"
#include "mna.h"
#include <cassert>

using namespace mna;

void ISource::stamp(MNAType &mna) const
{
  assert(Element::node_list().size()==2); // a current source is a 2-pin instance
  // a current source can be a self-loop
  if (node(0)!="0") // ignore GND node when filling the NMA matrix
    mna.fill_in(MNATermType(MNAType::NEG, Element::name()),
		MNALabelType(node(0)));
  if (node(1)!="0") // ignore GND node when filling the NMA matrix
    mna.fill_in(MNATermType(MNAType::POS, Element::name()),
		MNALabelType(node(1)));
}
