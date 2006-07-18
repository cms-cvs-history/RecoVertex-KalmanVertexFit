#include "RecoVertex/KalmanVertexFit/test/KVFTest.h"

#include "DataFormats/VertexReco/interface/Vertex.h"
#include "DataFormats/VertexReco/interface/VertexFwd.h"
#include "DataFormats/TrackReco/interface/Track.h"
#include "DataFormats/TrackReco/interface/TrackFwd.h"
#include "DataFormats/Common/interface/EDProduct.h"
#include "FWCore/Framework/interface/Handle.h"
#include "FWCore/Framework/interface/MakerMacros.h"
#include "FWCore/MessageLogger/interface/MessageLogger.h"

#include "TrackingTools/TransientTrack/interface/TransientTrack.h"
#include "RecoVertex/VertexPrimitives/interface/TransientVertex.h"
#include "RecoVertex/VertexPrimitives/interface/ConvertError.h"
#include "RecoVertex/KalmanVertexFit/interface/KalmanVertexFitter.h"
#include "SimDataFormats/Vertex/interface/SimVertexContainer.h"

#include <iostream>

using namespace reco;
using namespace edm;
using namespace std;

KVFTest::KVFTest(const edm::ParameterSet& iConfig)
  : theConfig(iConfig)
{
  trackLabel_ = iConfig.getParameter<std::string>("TrackLabel");
  outputFile_ = iConfig.getUntrackedParameter<std::string>("outputFile");
  kvfPSet = iConfig.getParameter<edm::ParameterSet>("KVFParameters");
  rootFile_ = TFile::Open(outputFile_.c_str(),"RECREATE"); 
  edm::LogInfo("RecoVertex/KVFTest") 
    << "Initializing KVF TEST analyser  - Output file: " << outputFile_ <<"\n";
}


KVFTest::~KVFTest() {
  delete rootFile_;
}

void KVFTest::beginJob(edm::EventSetup const&){
  tree = new SimpleVertexTree("VertexFitter");
}


void KVFTest::endJob() {
  delete tree;
}

//
// member functions
//

void
KVFTest::analyze(const edm::Event& iEvent, const edm::EventSetup& iSetup)
{



  try {
    edm::LogInfo("RecoVertex/KVFTest") 
      << "Reconstructing event number: " << iEvent.id() << "\n";
    
    // get RECO tracks from the event
    // `tks` can be used as a ptr to a reco::TrackCollection
    edm::Handle<reco::TrackCollection> tks;
    iEvent.getByLabel(trackLabel_, tks);

    edm::LogInfo("RecoVertex/KVFTest") 
      << "Found: " << (*tks).size() << " reconstructed tracks" << "\n";
    cout << "got " << (*tks).size() << " tracks " << endl;

    // Convert Track vector to TransientTrack vector
    vector<TransientTrack> t_tks;
    for (unsigned int i = 0; i < (*tks).size() ; i++) {
      // Create a TrackRef
      TrackRef trkRef(tks, i);
      // Conversion to TransientTrack (implicit conversion using the TrackRef constructor)
      t_tks.push_back(trkRef);
    }

    edm::LogInfo("RecoVertex/KVFTest") 
      << "Found: " << t_tks.size() << " reconstructed tracks" << "\n";
    
    // Call the KalmanVertexFitter if more than 1 track
    if (t_tks.size() > 1) {
      KalmanVertexFitter kvf(kvfPSet);
      TransientVertex tv = kvf.vertex(t_tks);

      std::cout << "Position: " << Vertex::Point(tv.position()) << "\n";

      // For the analysis: compare to your SimVertex
      SimVertex sv = getSimVertex(iEvent);
      tree->fill(tv, &sv);
    }
    
  }

  catch (std::exception & err) {
    edm::LogInfo("RecoVertex/KVFTest") 
      << "Exception during event number: " << iEvent.id() 
      << "\n" << err.what() << "\n";
  }

}


//Returns the first vertex in the list.

SimVertex KVFTest::getSimVertex(const edm::Event& iEvent) const
{
   // get the simulated vertices
   Handle<edm::SimVertexContainer> simVtcs;
   iEvent.getByLabel("SimG4Object", simVtcs);
//    std::cout << "SimVertex " << simVtcs->size() << std::endl;
//    for(edm::SimVertexContainer::const_iterator v=simVtcs->begin();
//        v!=simVtcs->end(); ++v){
//      std::cout << "simvtx "
// 	       << v->position().x() << " "
// 	       << v->position().y() << " "
// 	       << v->position().z() << " "
// 	       << v->parentIndex() << " "
// 	       << v->noParent() << " "
//               << std::endl;
//    }
   return *(simVtcs->begin());
}
