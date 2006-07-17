// -*- C++ -*-
//
// Package:    KVFTest
// Class:      KVFTest
// 
/**\class KVFTest KVFTest.cc RecoVertex/KVFTest/src/KVFTest.cc

 Description: steers tracker primary vertex reconstruction and storage

 Implementation:
     <Notes on implementation>
*/
//
// Original Author:  Pascal Vanlaer
//         Created:  Tue Feb 28 11:06:34 CET 2006
// $Id: KVFTest.h,v 1.5 2006/06/06 12:03:52 vanlaer Exp $
//
//


// system include files
#include <memory>

// user include files
#include "FWCore/Framework/interface/Frameworkfwd.h"
#include "FWCore/Framework/interface/EDAnalyzer.h"

#include "FWCore/Framework/interface/Event.h"
#include "FWCore/Framework/interface/EventSetup.h"

#include "FWCore/ParameterSet/interface/ParameterSet.h"
#include "SimDataFormats/Vertex/interface/SimVertex.h"
#include "RecoVertex/KalmanVertexFit/test/SimpleVertexTree.h"
#include <TFile.h>

  /**
   * This is a very simple test analyzer mean to test the KalmanVertexFitter
   */

class KVFTest : public edm::EDAnalyzer {
public:
  explicit KVFTest(const edm::ParameterSet&);
  ~KVFTest();
  
  virtual void analyze(const edm::Event&, const edm::EventSetup&);

  virtual void beginJob(edm::EventSetup const&);
  virtual void endJob();

  std::string trackLabel() const;
  
private:

  SimVertex getSimVertex(const edm::Event& iEvent) const;

  edm::ParameterSet theConfig;
  SimpleVertexTree *tree;
  TFile*  rootFile_;

  std::string outputFile_; // output file
};
