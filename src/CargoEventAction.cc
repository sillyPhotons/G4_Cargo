//
// ********************************************************************
// * License and Disclaimer                                           *
// *                                                                  *
// * The  Geant4 software  is  copyright of the Copyright Holders  of *
// * the Geant4 Collaboration.  It is provided  under  the terms  and *
// * conditions of the Geant4 Software License,  included in the file *
// * LICENSE and available at  http://cern.ch/geant4/license .  These *
// * include a list of copyright holders.                             *
// *                                                                  *
// * Neither the authors of this software system, nor their employing *
// * institutes,nor the agencies providing financial support for this *
// * work  make  any representation or  warranty, express or implied, *
// * regarding  this  software system or assume any liability for its *
// * use.  Please see the license in the file  LICENSE  and URL above *
// * for the full disclaimer and the limitation of liability.         *
// *                                                                  *
// * This  code  implementation is the result of  the  scientific and *
// * technical work of the GEANT4 collaboration.                      *
// * By using,  copying,  modifying or  distributing the software (or *
// * any work based  on the software)  you  agree  to acknowledge its *
// * use  in  resulting  scientific  publications,  and indicate your *
// * acceptance of all terms of the Geant4 Software license.          *
// ********************************************************************
//
/// \file CargoEventAction.cc
/// \brief Implementation of the CargoEventAction class

#include "CargoEventAction.hh"
#include "MuonDetectorHit.hh"
#include "CargoAnalysis.hh"
#include "G4Event.hh"
#include "G4RunManager.hh"
#include "G4EventManager.hh"
#include "G4HCofThisEvent.hh"
#include "G4VHitsCollection.hh"
#include "G4SDManager.hh"
#include "G4SystemOfUnits.hh"
#include "G4ios.hh"

namespace
{
// Utility function which finds a hit collection with the given Id
// and print warnings if not found
G4VHitsCollection *GetHC(const G4Event *event, G4int collId)
{
	auto hce = event->GetHCofThisEvent();
	if (!hce)
	{
		G4ExceptionDescription msg;
		msg << "No hits collection of this event found." << G4endl;
		G4Exception("CargoEventAction::EndOfEventAction()",
					"CargoCode001", JustWarning, msg);
		return nullptr;
	}

	auto hc = hce->GetHC(collId);
	if (!hc)
	{
		G4ExceptionDescription msg;
		msg << "Hits collection " << collId << " of this event not found." << G4endl;
		G4Exception("CargoEventAction::EndOfEventAction()",
					"CargoCode001", JustWarning, msg);
	}
	return hc;
}

} // namespace

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

CargoEventAction::CargoEventAction()
	: G4UserEventAction(),
	  HCIDTop(-1),
	  HCIDBot(-1)
{
	G4RunManager::GetRunManager()->SetPrintProgress(1);
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

CargoEventAction::~CargoEventAction()
{
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

void CargoEventAction::BeginOfEventAction(const G4Event *)
{
	// Find hit collections and histogram Ids by names (just once)
	// and save them in the data members of this class
	if ( HCIDTop == -1 || HCIDBot == -1) 
	{
        G4SDManager* sdManager = G4SDManager::GetSDMpointer();
        HCIDTop = sdManager->GetCollectionID("muonDetTop/MuonDetectorColl");
		HCIDBot = sdManager->GetCollectionID("muonDetBot/MuonDetectorColl");
    }
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

void CargoEventAction::EndOfEventAction(const G4Event *event)
{
	// Get analysis manager
	G4AnalysisManager *analysisManager = G4AnalysisManager::Instance();

	G4HCofThisEvent* hce = event->GetHCofThisEvent();
    if (!hce) {
        G4ExceptionDescription msg;
        msg << "No hits collection of this event found.\n";
        G4Exception("Run::RecordEvent()",
                    "Code001", JustWarning, msg);
        return;
    }

    const MuonDetectorHitsCollection* HCTop =
        static_cast<const MuonDetectorHitsCollection*>(hce->GetHC(HCIDTop));
	
	const MuonDetectorHitsCollection* HCBot =
        static_cast<const MuonDetectorHitsCollection*>(hce->GetHC(HCIDBot));
   
    if (!HCBot || !HCTop)
    {

        G4ExceptionDescription msg;
        msg << "Some of hits collections of this event not found.\n";
        G4Exception("Run::RecordEvent()",
                    "Code001", JustWarning, msg);
        return;

    }

	G4int top_hits = HCTop->entries();
	G4int bot_hits = HCBot->entries();
	
	if (top_hits > 0 && bot_hits > 0)
	{
		MuonDetectorHit* firstHit = (*HCTop)[0];
		MuonDetectorHit* secondHit = (*HCBot)[0];

		G4ThreeVector worldPos_1 = firstHit->GetWorldPos();
		G4ThreeVector worldPos_2 = secondHit->GetWorldPos();
		
		G4ThreeVector ref_vec 
			= G4ThreeVector(0, 
							worldPos_2.getY(), 
							0);

		G4ThreeVector dis_vec 
			= G4ThreeVector(worldPos_2.getX() - worldPos_1.getX(), 
							worldPos_2.getY() - worldPos_1.getY(), 
							worldPos_2.getZ() - worldPos_1.getZ());

		G4double weight 
			= acos(ref_vec.dot(dis_vec) / (ref_vec.mag() * dis_vec.mag()))*(180./ M_PI);

		G4int hist_id = analysisManager->GetFirstH2Id();
		
		// G4cout << "P1: " << worldPos_1.getX() << "," << worldPos_1.getY() << "," << worldPos_1.getZ() << G4endl;

		// G4cout << "P2: " << worldPos_2.getX() << "," << worldPos_2.getY() << "," << worldPos_2.getZ() << G4endl;

		analysisManager->FillH2(hist_id, worldPos_1.getX(), worldPos_1.getZ(), weight);
	}
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......