.. raw:: latex

    \clearpage
  	\pagenumbering{arabic}
    \setcounter{page}{1}
    \setcounter{chapter}{0}
    \chapter{Introduction}

.. _introduction: 

INTRODUCTTION
==============

EPANET :cite:p:`Rossman2000,Rossman2020` is a widely used program for modeling the
hydraulic and water quality behavior of drinking water distribution
systems. Its water quality component is limited to tracking the
transport and fate of just a single chemical species, such as fluoride
used in a tracer study or free chlorine used in a disinfectant decay
study. And the longitudinal dispersion process, which can play an important role in affecting the water qualities
at the dead ends of a water distribution system, is not modeled in EPANET.  
This manual describes an extension to the original EPANET that
allows it to model the advection, dispersion and reaction of any system of multiple, interacting chemical species.
This capability has been incorporated into both a stand-alone executable
program as well as a toolkit library of functions that programmers can
use to build custom applications. This set of software tools is referred
to as EPANET-MSX, where MSX stands for Multi-Species Extension. 

Many water quality problems in distribution systems can only be analyzed
by using a multi-species approach. Consider the following descriptive
examples:

- Free chlorine disinfectant is lost in bulk solution due mainly to oxidation-reduction
  reactions involving HOCl and OCl\ :sup:`-` and natural organic matter (NOM). The NOM itself
  is a heterogeneous mixture of organic compounds (e.g., humic and fulvic acids) of varying 
  chemical characteristics. Current single-species models, however, must model free chlorine loss under
  the assumption that all other reactants are in excess and thus their concentrations
  can be considered constant. This limitation is responsible for the
  widespread observation that the water-specific decay rate constant
  of the common first-order model is not a constant at all, but
  rather varies significantly with chlorine dose (a clear indication
  of model structure error). The formation of regulated chlorination
  by-products, which result from free chlorine and NOM interactions,
  presents yet another set of reaction mechanisms involving multiple interacting species.

- Mono-, di-, and tri-chloramine result from interactions between free chlorine species and ammonia, and are increasingly used as 
  residual disinfectants. These chloramines also interact with NOM, though the reactions are slower than those for free chlorine. Thus
  chloramine decay in distribution systems involves multiple interacting chemical species, which a single-species model is forced to simplify as a quasi-first order reaction. Further, ammonia may be produced by auto-decomposition of chloramines, which is of 
  significant practical importance for understanding nitrification episodes in distribution systems and storage tanks. Nitrification models may need to consider attached-growth nitrifying biofilms, suspended nitrifying biomass, and the
  electron donor (ammonia), electron acceptor (oxygen), and carbon source that supports microbial growth.

- For the relatively common situation where more than one water source supplies a distribution system, current models are not able to represent meaningful differences in source water quality, as they relate to water quality evolution in the distribution system. Modelers
  must try to compensate for this limitation by assigning bulk decay rate coefficients to specific pipes, according to which source supplies them. Such an approach has obvious deficiencies when attempting to model distribution system zones where sources blend together, and 
  these zones are sometimes the focus of water quality issues.

None of these examples can be accurately modeled by using the
single-species capabilities of the current EPANET program. This
shortcoming provides the motivation to extend EPANET so that it can
model reaction systems of any level of complexity.

Another feature in the the current version of the EPANET-MSX is the modeling option to include dispersion process in the water quality modeling analysis of water distribution system. EPANET models the transport of a single species by solving one-dimensional
advection-reaction (AR) equation; while the EPAENT-MSX solves a set of one-dimensional advection-dispersion-reaction (ADR) equations to analyze the water quality problems that involve mutiple interacting species.     

The following sections of this manual describe the conceptual framework
used by EPANET-MSX to model multiple reacting species within a
distribution system, provide instructions on how to use both the command
line and toolkit versions of EPANET-MSX, give a complete description of
the format of an MSX input file, and describe several example
applications in detail. The appendices describe each function in the
EPANET-MSX toolkit, the format of its binary output file, and the
meaning of its error codes.