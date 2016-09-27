#include "mna.h"
#include <memory>
#include <algorithm>
#include <cassert>

using namespace std;

namespace mna
{
//! default comparison function for map< , , class Compare = std::less<T>>
//! Definition: node labels are always less than branch labels
bool operator<(const Label &l1, const Label &l2)
{
  if (l1.is_node_label() && !l2.is_node_label())
    return 1; // node label < branch label
  else if (!l1.is_node_label() && l2.is_node_label())
    return 0; // branch label !< node label
  else // l1 and l2 have the same label type
    return l1._label < l2._label;
}

//!< work with the copy constructor
Label &Label::operator=(const Label &label)
{
  if (this==&label)
    return *this; // self assignment
  _label = label._label;
  _label_type = label._label_type;
  return *this;
}

MNAFormulation::MNAFormulation()
{
#ifndef NDEBUG
  cout << "Create MNA" << endl;
#endif // NDEBUG
}

MNAFormulation::~MNAFormulation()
{
#ifndef NDEBUG
  cout << "Destroy MNA" << endl;
#endif // NDEBUG
}

//! fill a term into the MNA matrix as part of a matrix element
void MNAFormulation::fill_in(const TermType &term,
			     const Label &row_label, const Label &col_label)
{
  MNA()._mna[row_label][col_label].append(term);
  _idx_buf[row_label] = true; // add a label to the MNA index buffer
  _idx_buf[col_label] = true; // add a label to the MNA index buffer
}

//! fill a constant into the MNA matrix as part of a matrix element
void MNAFormulation::fill_in(const int constant,
			     const Label &row_label, const Label &col_label)
{
  MNA()._mna[row_label][col_label].append(constant);
  _idx_buf[row_label] = true; // add a label to the MNA index buffer
  _idx_buf[col_label] = true; // add a label to the MNA index buffer
}

//! fill a symblic term into the RHS vector
void MNAFormulation::fill_in(const TermType &term, const Label &row_label)
{
  MNA()._rhs[row_label].append(term);
  _idx_buf[row_label] = true; // add a label to the MNA index buffer
}

//! fill a constant into the RHS vector
void MNAFormulation::fill_in(const int constant, const Label &row_label)
{
  MNA()._rhs[row_label].append(constant);
  _idx_buf[row_label] = true; // add a label to the MNA index buffer
}

//! set up the MNA index vector and sort it
void MNAFormulation::setup_index()
{
  assert(_idx.empty());
  for (MNAIndexBufType::const_iterator it=_idx_buf.begin(); it!=_idx_buf.end(); ++it)
    _idx.push_back(it->first);
  sort(_idx.begin(), _idx.end());
}

//! print a single symbolic term in LaTeX format
static void _print_symbolic_term_latex(const MNATermType &term,
				       ostream &ostrm,
				       bool is_first_term = 0)
{
  switch (term.first)
    { // print '+' if this is the 1st symbolic term and it is positive
    case MNAFormulation::POS:
      ostrm << (is_first_term ? ' ' : '+') << term.second;
    break;
    case MNAFormulation::NEG:
      ostrm << '-' << term.second;
    break;
    case MNAFormulation::INV:
      ostrm << (is_first_term ? ' ' : '+') << term.second << "^{-1}";
    break;
    case MNAFormulation::NEG_INV:
      ostrm << '-' << term.second << "^{-1}";
    break;
    case MNAFormulation::DIF:
      ostrm << (is_first_term ? ' ' : '+') << "s*"
	<< term.second;
    break;
    case MNAFormulation::NEG_DIF:
      ostrm << '-' << "s*"
	<< term.second;
    break;
    case MNAFormulation::INV_DIF:
      ostrm << (is_first_term ? ' ' : '+') << "s^{-1}*"
	<< term.second << "^{-1}";
    break;
    case MNAFormulation::NEG_INV_DIF:
      ostrm << '-' << "s^{-1}*"
	<< term.second << "^{-1}";
    break;
    default:
      assert(!"Unknown TermOperatorType found!");
    }
}

//! print an element of the MNA matrix
void MNAFormulation::print_element_latex(const ElementType &e,
					 ostream &ostrm) const
{
  assert(e.constant!=0 || !e.symbolic.empty());
  if (!e.symbolic.empty()) // print the symbolic terms
    {
      // print the 1st symbolic term
      _print_symbolic_term_latex(e.symbolic.at(0), ostrm, 1);
      // print the rest symbolic terms
      for (vector<TermType>::const_iterator
	   it=e.symbolic.begin()+1; it!=e.symbolic.end(); ++it)
	_print_symbolic_term_latex(*it, ostrm);
    }
  // print the constant term
  if (e.constant != 0) // do not print 0 element
    {
      if (e.symbolic.empty()) // the constant is the unique term
	ostrm << e.constant;
      else // print '+' for positive numbers
	ostrm << (e.constant>0 ? '+' : ' ') << e.constant;
    }
  ostrm << flush;
}

//! print the head of a LaTeX file
inline void _print_latex_head(ostream &ostrm)
{
  ostrm
    << "\\documentclass{article}" << '\n'
    << "\\usepackage[landscape]{geometry}" << '\n'
    << "\\pagestyle{empty}" << '\n'
    << "\\setlength{\\oddsidemargin}{0cm}" << '\n'
    << "\\setlength{\\parindent}{0cm}" << '\n'
    << "\\begin{document}" << '\n'
    << "\\[" << endl;
}

//! print the tail of a LaTeX file
inline void _print_latex_tail(ostream &ostrm)
{
  ostrm << "\\]" << '\n' << "\\end{document}" << endl;
}

//! print the MNA equation in LaTeX format: (MNA)*(X)=(RHS)
void MNAFormulation::print_latex(ostream &ostrm) const
{
  _print_latex_head(ostrm); //! print the head of a LaTeX file
  print_mna_latex(ostrm);// print the MNA matrix
  print_unknow_latex(ostrm); // print the unknown vector
  ostrm << "=";
  print_rhs_latex(ostrm); // print the encourage vector
  _print_latex_tail(ostrm); //! print the tail of a LaTeX file
}

//! print the MNA equation in LaTeX format
void MNAFormulation::print_mna_latex(ostream &ostrm) const
{
  ostrm << "\\left( \\begin{array}{"
    << string(_idx.size(), 'c') << "}" << '\n'; // set matrix style
  assert(_idx_buf.size()==_idx.size());
  for (MNAIndexType::const_iterator
       ridxit=_idx.begin(); ridxit!=_idx.end(); ++ridxit)
    {
      MNAMatrixType::const_iterator rit=_mna.find(*ridxit); // find the row
      if (rit==_mna.end()) // no such a row
	for (MNAIndexType::const_iterator
	     cidxit=_idx.begin(); cidxit!=_idx.end(); ++cidxit)
	  {
	    ostrm << "0"; // empty element
	    ostrm << (((cidxit+1)!=_idx.end()) ? " & " : ""); // insert an element delimit
	  }
      else
	for (MNAIndexType::const_iterator
	     cidxit=_idx.begin(); cidxit!=_idx.end(); ++cidxit)
	  {
	    RHSType::const_iterator cit=rit->second.find(*cidxit);
	    if (cit!=rit->second.end()) // element found
	      print_element_latex(cit->second, ostrm);
	    else
	      ostrm << "0"; // empty element
	    ostrm << (((cidxit+1)!=_idx.end()) ? " & " : ""); // insert an element delimit
	  }
      ostrm << (((ridxit+1)!=_idx.end()) ? " \\\\" : "") << '\n'; // the last row doesn't need "\\"
    }
  ostrm << "\\end{array} \\right)" << endl; // matrix end
}

//! print the unknown vector: X
void MNAFormulation::print_unknow_latex(std::ostream &ostrm) const
{
  ostrm << "\\left( \\begin{array}{" << 'c' << "}" << '\n'; // set vector style
  assert(!_idx.empty());
  for (MNAIndexType::const_iterator it=_idx.begin(); it!=_idx.end(); ++it)
    {
      assert(!it->label().empty());
      if (it->is_node_label())
	ostrm << "V_{" << it->label() << "}";
      else // this is a branch label
	ostrm << "I_{(" << it->label() << ")}";
      ostrm << (((it+1)!=_idx.end()) ? " \\\\" : "") << '\n'; // the last row doesn't need "\\"
    }
  ostrm << "\\end{array} \\right)" << endl; // matrix end
}

//! print the RHS vector, i.e. the encourage vector
void MNAFormulation::print_rhs_latex(std::ostream &ostrm) const
{
  ostrm << "\\left( \\begin{array}{" << 'c' << "}" << '\n'; // set vector style
  assert(_rhs.size()<=_idx.size());
  for (MNAIndexType::const_iterator
       ridxit=_idx.begin(); ridxit!=_idx.end(); ++ridxit)
    {
      RHSType::const_iterator rit=_rhs.find(*ridxit);
      if (rit!=_rhs.end()) // element found
	print_element_latex(rit->second, ostrm);
      else
	ostrm << "0"; // empty element
      ostrm << (((ridxit+1)!=_idx.end()) ? " \\\\" : "") << '\n'; // the last row doesn't need "\\"
    }
  ostrm << "\\end{array} \\right)" << endl; // matrix end
}

//! print the MNA equation
void MNAFormulation::print(OutputFormat outputfmt, ostream &ostrm) const
{
  assert(!_idx.empty()); // _idx must be set up once and only once for printing
  switch (outputfmt)
    {
    case LATEX:
      print_latex(ostrm);
    break;
    default:
      assert((!"unsupported output format found!"));
    }
}

//! print the MNA equation to a file or on the screen if file_name is empty
void MNAFormulation::print(const std::string &ofile_name,
			   OutputFormat outputfmt) const
{
  assert(!_idx.empty()); // _idx must be set up once and only once for printing
  // select the output stream
  if (ofile_name.empty())
    {
      print(outputfmt); // print on the screen
    }
  else
    {
      ofstream ofstrm(ofile_name.c_str());
      if (ofstrm.good())
	{
	  print(outputfmt, ofstrm); // print to a file
	}
      else
	{
	  ofstrm.close(); // don't forget to close the stream
	  print(outputfmt); // print on the screen if failed to open the output file
	}
      ofstrm.close(); // don't forget to close the stream
    }
}
} // namespace mna

//! global function to access the MNA singleton
MNAType &MNA()
{
  static auto_ptr<MNAType> mna(new MNAType);
  return *mna;
}
