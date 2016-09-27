#ifndef __MNA_H__
#define __MNA_H__

#include <vector>
#include <string>
#include <utility>
#include <map>
#include <iostream>
#include <fstream>

namespace mna
{
  class MNAFormulation; /* declaration for the friend function
			   declaration inside MNAFormulation */
}
typedef mna::MNAFormulation MNAType;

//! unique global interface to access the MNA matrix
MNAType &MNA();

namespace mna
{
//! label of an MNA matrix row
class Label
{
public:
  enum LabelType {NODE, BRANCH}; //!< all types of labels
  //! if label_type=NODE,
  //!   the label stands for a node equation (row) or a voltage variable (column);
  //! else
  //!   the label stands for a branch equation (row) or a current variable (column).
  Label(const std::string &label_name, LabelType label_type = NODE) :
    _label(label_name), _label_type(label_type) {}
  Label(const Label &label) //!< copy constructor
    : _label(label._label), _label_type(label._label_type) {}
  Label &operator=(const Label &l1); //!< work with the copy constructor
  ~Label() {} //!< destructor
  const std::string& label() const { return _label; } //!< return the label name
  //!< return 1 if this is a node label; return 0 otherwise
  bool is_node_label() const { return _label_type==NODE; }
  //!< provide an operator for sorting purpose
  friend bool operator<(const Label &l1, const Label &l2);
private:
  std::string _label;
  LabelType _label_type;
};

//! MNA formulation of a circuit: MNA * X = RHS
//! where X is the node voltage and branch current vector
class MNAFormulation
{
public:
  enum OutputFormat {LATEX/*, CVS, XML*/}; //!< supported output formats
  enum TermOperatorType
    //! '+', '-', "1/", "-1/", "s*", "-s*", "1/(s*)", "-1/(s*)"
    {POS, NEG, INV, NEG_INV, DIF, NEG_DIF, INV_DIF, NEG_INV_DIF};
  //! a term of a symblic polynomial
  typedef std::pair<TermOperatorType, std::string> TermType;
private:
  struct ElementType
    {
      std::vector<TermType> symbolic; //!< Symbolic part
      int constant; //!< constant part
      ElementType() : constant(0) {}
      ~ElementType() { symbolic.clear(); constant=0; }
      //! append a symblic term to the element
      void append(const TermType &term) { symbolic.push_back(term); }
      //! append a constant integer to the element
      void append(const int &c) { constant += c; }
    }; //!< element of the MNA matrix
  typedef std::map<Label, ElementType> RHSType;
  typedef std::map<Label, RHSType> MNAMatrixType;
  typedef std::vector<Label> MNAIndexType;
  typedef std::map<Label, bool> MNAIndexBufType;
private:
  RHSType _rhs; //!< Encourage vector
  MNAMatrixType _mna; //!< Modified nodal admittance matrix: a 2D sparse matrix
  MNAIndexType _idx; //!< MNA index vector
  MNAIndexBufType _idx_buf; //!< MNA index buffer
public:
  ~MNAFormulation();
  //! fill a symbolic term into the MNA matrix
  void fill_in(const TermType &term, const Label &row_label, const Label &col_label);
  //! fill a constant into the MNA matrix
  void fill_in(const int constant, const Label &row_label, const Label &col_label);
  //! fill a symblic term into the RHS vector
  void fill_in(const TermType &term, const Label &row_label);
  //! fill a constant into the RHS vector
  void fill_in(const int constant, const Label &row_label);
  void setup_index(); //!< set up the MNA index vector and sort it
  //! print the MNA equation by default on the screen
  void print(OutputFormat outputfmt = LATEX,
	     std::ostream &ostrm = std::cout) const;
  //! print the MNA equation to a file or
  //  on the screen if file_name is empty or cannot be opened
  void print(const std::string &ofile_name,
	     OutputFormat outputfmt = LATEX) const;
  friend ::MNAType &::MNA(); //!< global function to access the MNA singleton
private:
  MNAFormulation(); //!< make MNA formulation a singleton
  //! print an element of the MNA matrix
  void print_element_latex(const ElementType &e, std::ostream &ostrm) const;
  //! print the MNA equation in LaTeX format
  void print_latex(std::ostream &ostrm) const;
  //! print the MNA matrix in LaTeX format
  void print_mna_latex(std::ostream &ostrm) const;
  //! print the unknown vector
  void print_unknow_latex(std::ostream &ostrm) const;
  //! print the RHS vector, i.e. the encourage vector
  void print_rhs_latex(std::ostream &ostrm) const;
};

// public types
typedef Label MNALabelType;
typedef MNAFormulation::TermType MNATermType;
} // namespace mna

#endif // __MNA_H__
