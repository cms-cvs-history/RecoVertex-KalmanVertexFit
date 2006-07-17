#include "RecoVertex/KalmanVertexFit/test/VertexFitterResult.h"
#include "CommonTools/Statistics/interface/ChiSquaredProbability.h"

// #include "Vertex/VertexAssociation/interface/VertexAssociationToolsFactory.h"
// #include "TrackerReco/TkTrackAssociation/interface/TrackAssociator.h"
// #include "CommonReco/PatternTools/interface/TransverseImpactPointExtrapolator.h"
// #include "TrackerReco/TkEvent/interface/TkSimTrack.h"

VertexFitterResult::VertexFitterResult(const int maxTracks)
{
  theMaxTracks = maxTracks;
  for ( int i=0; i<5; i++ ) {
    if ( maxTracks>0 ) {
      simPars[i] = new float[maxTracks];
      recPars[i] = new float[maxTracks];
      refPars[i] = new float[maxTracks];
      recErrs[i] = new float[maxTracks];
      refErrs[i] = new float[maxTracks];
    }
    else {
      simPars[i] = 0;
      recPars[i] = 0;
      refPars[i] = 0;
      recErrs[i] = 0;
      refErrs[i] = 0;
    }
  }
  trackWeight = new float[maxTracks];
  simIndex = new int[maxTracks];
  recIndex = new int[maxTracks];
  numberOfRecTracks=theMaxTracks;
  numberOfSimTracks=theMaxTracks;
  reset();
}

VertexFitterResult::~VertexFitterResult()
{
    //
    // delete arrays
    //
    for ( int i=0; i<5; i++ ) {
      delete [] simPars[i];
      delete [] recPars[i];
      delete [] refPars[i];
      delete [] recErrs[i];
      delete [] refErrs[i];
    }
    delete trackWeight;
    delete simIndex;
    delete recIndex;
}

void VertexFitterResult::fill(const TransientVertex & recVertex,
  			const SimVertex * simv, const float &time) 
{
//   if (recVertex.isValid()) {
//     recPos[0] = recVertex.position().x();
//     recPos[1] = recVertex.position().y();
//     recPos[2] = recVertex.position().z();
// 
//     recErr[0] = sqrt(recVertex.positionError().cxx());
//     recErr[1] = sqrt(recVertex.positionError().cyy());
//     recErr[2] = sqrt(recVertex.positionError().czz());
// 
//     chi[0] = recVertex.totalChiSquared();
//     chi[1] = recVertex.degreesOfFreedom();
//     chi[2] = ChiSquaredProbability(recVertex.totalChiSquared(), 
// 					   recVertex.degreesOfFreedom());
//     tracks[1] = recVertex.originalTracks().size();
//     vertex = 2;
//     fitTime = time;
//   }
// 
//   if (simv!=0) {
//     simPos[0] = simv->position().x();
//     simPos[1] = simv->position().y();
//     simPos[2] = simv->position().z();
// 
//     vertex += 1;
// 
//   }
//   
  TTrackCont recTrackV;
  if (recVertex.isValid()) recTrackV = recVertex.originalTracks();
  fill(recVertex, recTrackV, simv, time);
}

void VertexFitterResult::fill(const TransientVertex & recVertex, 
			const TTrackCont & recTrackV,
			const SimVertex * simv, const float &time)
{

//   SimTrkCont simTrackV;
//   vector<const RecVertex *> rvs;
//   vector<const TkSimVertex *> svs; 
//   RecTrackCont refTracks;

  if (recVertex.isValid()) {
    recPos[0] = recVertex.position().x();
    recPos[1] = recVertex.position().y();
    recPos[2] = recVertex.position().z();

    recErr[0] = sqrt(recVertex.positionError().cxx());
    recErr[1] = sqrt(recVertex.positionError().cyy());
    recErr[2] = sqrt(recVertex.positionError().czz());

    chi[0] = recVertex.totalChiSquared();
    chi[1] = recVertex.degreesOfFreedom();
    chi[2] = ChiSquaredProbability(recVertex.totalChiSquared(), 
					   recVertex.degreesOfFreedom());
    vertex = 2;
    fitTime = time;
    tracks[1] = recVertex.originalTracks().size();
//     rvs.push_back(&recVertex);
//     refTracks = recVertex.refittedTracks();
    
  }

  if (simv!=0) {
    simPos[0] = simv->position().x();
    simPos[1] = simv->position().y();
    simPos[2] = simv->position().z();

//     simTrackV = simv->tkSimTracks();
//     svs.push_back(simv);
    vertex += 1;

//     for(SimTrkCont::const_iterator simTrack =simTrackV.begin();
//                  (simTrack != simTrackV.end() && (numberOfSimTracks<theMaxTracks));
// 		 simTrack++) {
//       
//       TrajectoryStateOnSurface simip = (**simTrack).impactPointStateOnSurface();
//       fillParameters(simip, simPars, numberOfSimTracks);
//       simIndex[numberOfSimTracks] = -1;
//       ++numberOfSimTracks;
//    }
//     tracks[0] = simTrackV.size();
  }


//   // now store all the recTrack...  
//   VertexAssociationToolsFactory vtxAssocFact( simTrackV, svs, recTrackV, rvs);
//   TrackAssociator * recTa = const_cast < TrackAssociator *> (vtxAssocFact.trackAssociator());
//   if ((simv!=0) && (recVertex.isValid())) tracks[2] = numberOfSharedTracks(recVertex, *simv, *recTa);
// 
//   for(RecTrackCont::const_iterator recTrack =recTrackV.begin();
//                (recTrack != recTrackV.end() 
// 		&& (numberOfRecTracks<theMaxTracks));
// 	       recTrack++) {
//     //    cout << "Input; 1/Pt " << 1./(*recTrack).momentumAtVertex().transverse() << endl;
// 
//     //looking for sim tracks corresponding to our reconstructed tracks:  
//     vector<const TkSimTrack *> simFound;
//     if (simv!=0) simFound = recTa->simTracks(*recTrack);
//     TrajectoryStateOnSurface recip;
//     if(simFound.size() != 0) {
//       //OK, it was associated, so get the state on the same surface as the 'SimState'
//       recip = recTrack->stateOnSurface(simFound[0]->impactPointStateOnSurface().surface());
//       SimTrkCont::const_iterator simTrackI = 
// 	find(simTrackV.begin(), simTrackV.end(), simFound[0]);
//       int simTrackIndex = simTrackI-simTrackV.begin();
//       if (simTrackIndex<numberOfSimTracks) {
//         simIndex[numberOfRecTracks] = simTrackIndex;
//         recIndex[simTrackIndex] = numberOfRecTracks;
// 	//	cout << "Assoc; 1/Pt " << 1./(*recTrack).momentumAtVertex().transverse() << endl;
//       } else {
// 	simIndex[numberOfRecTracks] = -1;
//       }
//     } else {
//       simIndex[numberOfRecTracks] = -1;
//       if (simv!=0) {
//         // Not associated, but we have a SimVertex, so get the state at the PCA
//         recip = TransverseImpactPointExtrapolator().extrapolate(
//      		recTrack->innermostState(), simv->position());
//       } else {
//         // Not associated, and no SimVertex, so get innermostState...
//         recip = recTrack->innermostState();
//       }
//     }
// 
//     fillParameters(recip, recPars, numberOfRecTracks);
//     fillErrors(recip, recErrs, numberOfRecTracks);
//     trackWeight[numberOfRecTracks] = recVertex.trackWeight(*recTrack);
// 
//     if ((recVertex.isValid())&&(recVertex.hasRefittedTracks())) {
//       //looking for corresponding refitted tracks:
//       TrajectoryStateOnSurface refip;
//       RecTrackCont::iterator refTrackI = 
//       		find_if(refTracks.begin(), refTracks.end(), RecTrackMatch(*recTrack));
//       if (refTrackI!=refTracks.end()) {
//         // If it was not found, it would mean that it was not used in the fit,
// 	// or with a low weight such that the track was then discarded.
// 	if(simFound.size() != 0) {
// 	  refip = refTrackI->stateOnSurface(simFound[0]->impactPointStateOnSurface().surface());
// 	} else {
//           refip = refTrackI->innermostState();
// 	}
// 
// 	fillParameters(refip, refPars, numberOfRecTracks);
// 	fillErrors(refip, refErrs, numberOfRecTracks);
//       }
//     }
// 
//     ++numberOfRecTracks;
//   }
  //  if (vtxAssocFact != 0) delete vtxAssocFact;
  
}

// void VertexFitterResult::fillParameters (TrajectoryStateOnSurface& ip,
// 	float* params[5], int trackNumber)
// {
//   if ( ip.isValid() ) {
//     params[0][trackNumber] = 1/ip.globalMomentum().perp();
//     params[1][trackNumber] = ip.globalMomentum().theta();
//     params[2][trackNumber] = ip.globalMomentum().phi(); 
//     params[3][trackNumber] = ip.localPosition().x(); 
//     params[4][trackNumber] = ip.localPosition().y(); 
//   }
// }
// 
// void VertexFitterResult::fillErrors (TrajectoryStateOnSurface& ip,
// 	float* errors[5], int trackNumber)
// {
//   if ( ip.isValid() ) {
//     float SinTheta=sin(ip.globalMomentum().theta());
//     float CosTheta=cos(ip.globalMomentum().theta());
//     float ptRec=ip.globalMomentum().perp();
//     float InvSinTheta=1/SinTheta;
//     float InvpErr=ip.curvilinearError().matrix()(1,1);
//     float thetaErr=ip.curvilinearError().matrix()(2,2);
//     float corr=ip.curvilinearError().matrix()(1,2);
//     float InvptErr = 
//       InvSinTheta*InvSinTheta*(InvpErr+(pow(CosTheta,2)/pow(ptRec,2))*thetaErr-2*(CosTheta/ptRec)*corr);
// 
//     errors[0][trackNumber] = sqrt(InvptErr);
//     errors[1][trackNumber] = sqrt(ip.curvilinearError().matrix()(2,2));
//     errors[2][trackNumber] = sqrt(ip.curvilinearError().matrix()(3,3));
//     errors[3][trackNumber] = sqrt(ip.localError().matrix()(4,4)); 
//     errors[4][trackNumber] = sqrt(ip.localError().matrix()(5,5)); 
// 
//   }
// }

void VertexFitterResult::reset()
{
  for ( int i=0; i<3; ++i ) {
    simPos[i] = 0.;
    recPos[i] = 0.;
    recErr[i] = 0.;
    chi[i] = 0.;
    tracks[i] = 0;
  }
  vertex =0;
  fitTime = 0;

  for ( int j=0; j<numberOfRecTracks; ++j ) {
    for ( int i=0; i<5; ++i ) {
       recPars[i][j] = 0;
       refPars[i][j] = 0;
       recErrs[i][j] = 0;
       refErrs[i][j] = 0;
    }
    trackWeight[j] = 0;
    simIndex[j] = -1;
  }
  for ( int j=0; j<numberOfSimTracks; ++j ) {
    for ( int i=0; i<5; ++i ) {
       simPars[i][j] = 0;
    }
    recIndex[j] = -1;
  }

  numberOfRecTracks=0;
  numberOfSimTracks=0;
}
