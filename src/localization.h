/*
 *                This source code is part of
 *
 *                     E  R  K  A  L  E
 *                             -
 *                       DFT from Hel
 *
 * Written by Susi Lehtola, 2010-2011
 * Copyright (c) 2010-2011, Susi Lehtola
 *
 * This program is free software; you can redistribute it and/or
 * modify it under the terms of the GNU General Public License
 * as published by the Free Software Foundation; either version 2
 * of the License, or (at your option) any later version.
 */



#ifndef ERKALE_LOCALIZATION
#define ERKALE_LOCALIZATION

#include "basis.h"
#include "scf.h"
#include "unitary.h"
#include "hirshfeld.h"
#include "badergrid.h"

/// Localization methods
enum locmet {
  /// Boys
  BOYS,
  /// Boys, penalty 2
  BOYS_2,
  /// Boys, penalty 3
  BOYS_3,
  /// Boys, penalty 4
  BOYS_4,
  /// Fourth moment
  FM_1,
  /// Fourth moment, penalty 2
  FM_2,
  /// Fourth moment, penalty 3
  FM_3,
  /// Fourth moment, penalty 4
  FM_4,
  /// Pipek-Mezey, Mulliken charge, p=1.5
  PIPEK_MULLIKENH,
  /// Pipek-Mezey, Mulliken charge, p=2
  PIPEK_MULLIKEN2,
  /// Pipek-Mezey, Mulliken charge, p=4
  PIPEK_MULLIKEN4,
  /// Pipek-Mezey, Löwdin charge, p=1.5
  PIPEK_LOWDINH,
  /// Pipek-Mezey, Löwdin charge, p=2
  PIPEK_LOWDIN2,
  /// Pipek-Mezey, Löwdin charge, p=4
  PIPEK_LOWDIN4,
  /// Pipek-Mezey, Bader charge, p=1.5
  PIPEK_BADERH,
  /// Pipek-Mezey, Bader charge, p=2
  PIPEK_BADER2,
  /// Pipek-Mezey, Bader charge, p=4
  PIPEK_BADER4,
  /// Pipek-Mezey, Becke charge, p=1.5
  PIPEK_BECKEH,
  /// Pipek-Mezey, Becke charge, p=2
  PIPEK_BECKE2,
  /// Pipek-Mezey, Becke charge, p=4
  PIPEK_BECKE4,
  /// Pipek-Mezey, Hirshfeld charge, p=1.5
  PIPEK_HIRSHFELDH,
  /// Pipek-Mezey, Hirshfeld charge, p=2
  PIPEK_HIRSHFELD2,
  /// Pipek-Mezey, Hirshfeld charge, p=4
  PIPEK_HIRSHFELD4,
  /// Pipek-Mezey, iterative Hirshfeld charge, p=1.5
  PIPEK_ITERHIRSHH,
  /// Pipek-Mezey, iterative Hirshfeld charge, p=2
  PIPEK_ITERHIRSH2,
  /// Pipek-Mezey, iterative Hirshfeld charge, p=4
  PIPEK_ITERHIRSH4,
  /// Pipek-Mezey, intrinsic atomic orbital charge, p=1.5
  PIPEK_IAOH,
  /// Pipek-Mezey, intrinsic atomic orbital charge, p=2
  PIPEK_IAO2,
  /// Pipek-Mezey, intrinsic atomic orbital charge, p=4
  PIPEK_IAO4,
  /// Pipek-Mezey, Stockholder charge, p=1.5
  PIPEK_STOCKHOLDERH,
  /// Pipek-Mezey, Stockholder charge, p=2
  PIPEK_STOCKHOLDER2,
  /// Pipek-Mezey, Stockholder charge, p=4
  PIPEK_STOCKHOLDER4,
  /// Pipek-Mezey, Voronoi charge, p=1.5
  PIPEK_VORONOIH,
  /// Pipek-Mezey, Voronoi charge, p=2
  PIPEK_VORONOI2,
  /// Pipek-Mezey, Voronoi charge, p=4
  PIPEK_VORONOI4,
  /// Edmiston-Ruedenberg
  EDMISTON
};

/// Parse method
enum locmet parse_locmet(const std::string & met);

/// Charge method methods
enum chgmet {
  /// Mulliken charge
  MULLIKEN,
  /// Löwdin charge
  LOWDIN,
  /// Bader charge
  BADER,
  /// Becke charge
  BECKE,
  /// Hirshfeld charge
  HIRSHFELD,
  /// iterative Hirshfeld charge
  ITERHIRSH,
  /// intrinsic atomic orbital charge
  IAO,
  /// Stockholder charge
  STOCKHOLDER,
  /// Voronoi charge
  VORONOI
};

/// Boys localization
class Boys : public UnitaryFunction {
  /// Penalty
  int n;

  /// R^2 matrix
  arma::mat rsq;
  /// r_x matrix
  arma::mat rx;
  /// r_y matrix
  arma::mat ry;
  /// r_z matrix
  arma::mat rz;

 public:
  /// Constructor. n gives the penalty power to use
  Boys(const BasisSet & basis, const arma::mat & C, int n,  bool verbose=true, bool delocalize=false);
  /// Destructor
  ~Boys();
  /// Copy
  Boys * copy() const;

  /// Reset penalty
  void set_n(int n);

  /// Evaluate cost function
  double cost_func(const arma::cx_mat & W);
  /// Evaluate derivative of cost function
  arma::cx_mat cost_der(const arma::cx_mat & W);
  /// Evaluate cost function and its derivative
  void cost_func_der(const arma::cx_mat & W, double & f, arma::cx_mat & der);
};

/// Fourth moment localization
class FMLoc : public UnitaryFunction {
  /// Penalty
  int n;

  /// r^4 contributions
  arma::mat rfour;
  /// rr^2 matrices
  std::vector<arma::mat> rrsq;
  /// rr matrices
  std::vector< std::vector<arma::mat> > rr;
  /// and the r^2 matrix
  arma::mat rsq;
  /// r matrices
  std::vector<arma::mat> rmat;

 public:
  /// Constructor. n gives the penalty power to use
  FMLoc(const BasisSet & basis, const arma::mat & C, int n, bool verbose=true, bool delocalize=false);
  /// Destructor
  ~FMLoc();
  /// Copy
  FMLoc * copy() const;

  /// Reset penalty
  void set_n(int n);

  /// Evaluate cost function
  double cost_func(const arma::cx_mat & W);
  /// Evaluate derivative of cost function
  arma::cx_mat cost_der(const arma::cx_mat & W);
  /// Evaluate cost function and its derivative
  void cost_func_der(const arma::cx_mat & W, double & f, arma::cx_mat & der);
};

/// Some of the Pipek-Mezey routines store atomic/regional overlap
/// matrices on disk to avoid recomputing them at every iteration. The
/// format used for the storage is
#define PIPEK_FILEMODE arma::arma_binary

/// Pipek-Mezey localization
class Pipek : public UnitaryFunction {
  // The localization is based on partial charches on atoms, so the
  // memory requirement would be Nat * Nocc^2, limiting the routine to
  // small systems. Instead, we calculate everything on-the-fly, so
  // the memory requirement is only Nocc^2

  /// Method
  enum chgmet chg;

  /// Number of centers
  size_t N;

  /// Penalty exponent, p=2 for conventional Pipek-Mezey
  double p;

  /// Get the charge matrix for the i:th region
  arma::mat get_charge(size_t i);

 public:
  /// Constructor
  Pipek(enum chgmet chg, const BasisSet & basis, const arma::mat & C, const arma::mat & P, double p=2.0, bool verbose=true, bool delocalize=false);
  /// Destructor
  ~Pipek();
  /// Copy
  Pipek * copy() const;

  /// Clean up files on disk
  void cleanup_disk();

  /// Evaluate cost function
  double cost_func(const arma::cx_mat & W);
  /// Evaluate derivative of cost function
  arma::cx_mat cost_der(const arma::cx_mat & W);
  /// Evaluate cost function and its derivative
  void cost_func_der(const arma::cx_mat & W, double & f, arma::cx_mat & der);
};

/// Edmiston-Ruedenberg localization
class Edmiston : public UnitaryFunction {
  /// Density fitting object
  DensityFit dfit;
  /// ERI Cholesky
  ERIchol chol;

  /// Use Cholesky?
  bool use_chol;

  /// Orbitals
  arma::mat C;
  /// Orbital Coulomb matrices
  std::vector<arma::mat> Jorb;

 public:
  /// Constructor: density fitting
  Edmiston(const BasisSet & basis, const BasisSet & fitbas, const arma::mat & C, bool delocalize=false);
  /// Constructor: Cholesky
  Edmiston(const BasisSet & basis, const arma::mat & C, bool delocalize=false, double thr=1e-5);
  /// Destructor
  ~Edmiston();
  /// Copy
  Edmiston * copy() const;
  /// Set transformation matrix
  void setW(const arma::cx_mat & W);

  /// Evaluate cost function
  double cost_func(const arma::cx_mat & W);
  /// Evaluate derivative of cost function
  arma::cx_mat cost_der(const arma::cx_mat & W);
  /// Evaluate cost function and its derivative
  void cost_func_der(const arma::cx_mat & W, double & f, arma::cx_mat & der);
};

/// Analyze orbitals if they really are complex
void analyze_orbitals(const BasisSet & basis, const arma::cx_mat & C);

/// Analyze orbital if it really is complex
double analyze_orbital(const arma::mat & S, const arma::cx_vec & C);

/// Orbital localization. Density matrix is only used for construction of Bader grid (if applicable)
void orbital_localization(enum locmet method, const BasisSet & basis, const arma::mat & C, const arma::mat & P, double & measure, arma::cx_mat & U, bool verbose=true, bool real=true, int maxiter=50000, double Gthr=1e-6, double Fthr=1e-7, enum unitmethod met=POLY_DF, enum unitacc acc=CGPR, bool delocalize=false, std::string logfile="", bool debug=false);

#endif
