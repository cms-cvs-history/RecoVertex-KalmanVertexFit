#include "RecoVertex/KalmanVertexFit/test/KVFTrackUpdate.h"

#include "DataFormats/VertexReco/interface/Vertex.h"
#include "DataFormats/VertexReco/interface/VertexFwd.h"
#include "DataFormats/TrackReco/interface/Track.h"
#include "DataFormats/TrackReco/interface/TrackFwd.h"
#include "DataFormats/Common/interface/EDProduct.h"
#include "DataFormats/Common/interface/Handle.h"
#include "FWCore/Framework/interface/MakerMacros.h"
#include "FWCore/MessageLogger/interface/MessageLogger.h"
#include "FWCore/Framework/interface/ESHandle.h"

#include "TrackingTools/TransientTrack/interface/TransientTrackBuilder.h"
#include "TrackingTools/Records/interface/TransientTrackRecord.h"
#include "TrackingTools/TransientTrack/interface/TransientTrack.h"
#include "RecoVertex/VertexPrimitives/interface/TransientVertex.h"
#include "RecoVertex/VertexPrimitives/interface/ConvertError.h"
#include "RecoVertex/KalmanVertexFit/interface/KalmanVertexFitter.h"
#include "SimTracker/Records/interface/TrackAssociatorRecord.h"

#include <iostream>

using namespace reco;
using namespace edm;
using namespace std;

KVFTrackUpdate::KVFTrackUpdate(const edm::ParameterSet& iConfig)
{
  trackLabel_ = iConfig.getParameter<std::string>("TrackLabel");
}


KVFTrackUpdate::~KVFTrackUpdate() {
}

void KVFTrackUpdate::beginJob(edm::EventSetup const& setup){
}


void KVFTrackUpdate::endJob() {
}

//
// member functions
//

void
KVFTrackUpdate::analyze(const edm::Event& iEvent, const edm::EventSetup& iSetup)
{



  try {
    edm::LogInfo("RecoVertex/KVFTrackUpdate") 
      << "Reconstructing event number: " << iEvent.id() << "\n";
    
    // get RECO tracks from the event
    // `tks` can be used as a ptr to a reco::TrackCollection
    edm::Handle<reco::TrackCollection> tks;
    iEvent.getByLabel(trackLabel_, tks);

    edm::LogInfo("RecoVertex/KVFTrackUpdate") 
      << "Found: " << (*tks).size() << " reconstructed tracks" << "\n";
    cout << "got " << (*tks).size() << " tracks " << endl;

    // Transform Track to TransientTrack

    //get the builder:
    edm::ESHandle<TransientTrackBuilder> theB;
    iSetup.get<TransientTrackRecord>().get("TransientTrackBuilder",theB);
    //do the conversion:
    vector<TransientTrack> t_tks = (*theB).build(tks);

    edm::LogInfo("RecoVertex/KVFTrackUpdate") 
      << "Found: " << t_tks.size() << " reconstructed tracks" << "\n";
    
    GlobalPoint glbPos(0.,0.,0.);

    AlgebraicSymMatrix mat(3,0);
    mat[0][0] = (20.e-04)*(20.e-04);
    mat[1][1] = (20.e-04)*(20.e-04);
    mat[2][2] = (5.3)*(5.3);
    GlobalError glbErrPos(mat);
    vector<TransientTrack> singeTrackV(1, t_tks.front());

    // Call the KalmanVertexFitter if more than 1 track
    if (t_tks.size() >= 1) {
      //      KalmanVertexFitter kvf(kvfPSet);
      KalmanVertexFitter kvf(true);
      CachingVertex<5> tv = kvf.vertex(singeTrackV, glbPos, glbErrPos);
      FreeTrajectoryState fts = tv.tracks().front()->refittedState()->freeTrajectoryState();

      std::cout << "Position: " << fts.position() << "\n";
      std::cout << "Momentum: " << fts.momentum() << "\n";

    }
    
  }

  catch (std::exception & err) {
    edm::LogInfo("RecoVertex/KVFTrackUpdate") 
      << "Exception during event number: " << iEvent.id() 
      << "\n" << err.what() << "\n";
  }

}

