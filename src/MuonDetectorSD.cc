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
// $Id: MuonDetectorSD.cc 76474 2013-11-11 10:36:34Z gcosmo $
//
/// \file MuonDetectorSD.cc
/// \brief Implementation of the MuonDetector class

#include "MuonDetectorSD.hh"
#include "MuonDetectorHit.hh"
#include "G4HCofThisEvent.hh"
#include "G4TouchableHistory.hh"
#include "G4Track.hh"
#include "G4Step.hh"
#include "G4SDManager.hh"
#include "G4ios.hh"
#include "G4MuonMinus.hh"

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

MuonDetectorSD::MuonDetectorSD(G4String name)
	: G4VSensitiveDetector(name), fHitsCollection(0), fHCID(-1)
{
	collectionName.insert("MuonDetectorColl");
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

MuonDetectorSD::~MuonDetectorSD()
{
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

void MuonDetectorSD::Initialize(G4HCofThisEvent *hce)
{
	fHitsCollection = new MuonDetectorHitsCollection(SensitiveDetectorName, collectionName[0]);
	if (fHCID < 0)
	{
		fHCID = G4SDManager::GetSDMpointer()->GetCollectionID(fHitsCollection);
	}
	hce->AddHitsCollection(fHCID, fHitsCollection);
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

G4bool MuonDetectorSD::ProcessHits(G4Step *step, G4TouchableHistory *)
{
	if (G4MuonMinus::MuonMinusDefinition() == step->GetTrack()->GetDefinition())
	{	
		G4String volume_name 
			= step->GetPreStepPoint()->GetPhysicalVolume()->GetName();
	
		G4StepPoint *preStepPoint = step->GetPreStepPoint();
		G4TouchableHistory *touchable = (G4TouchableHistory *)(step->GetPreStepPoint()->GetTouchable());
			G4VPhysicalVolume *motherPhysical = touchable->GetVolume(1); 
			G4int copyNo = motherPhysical->GetCopyNo();
		
		MuonDetectorHit *hit = new MuonDetectorHit(copyNo);
		G4ThreeVector worldPos, localPos;

		if (preStepPoint->GetStepStatus() == fGeomBoundary)
		{
			worldPos = preStepPoint->GetPosition();

			
			localPos 
				= touchable->GetHistory()
				-> GetTopTransform().TransformPoint(worldPos);
			hit->SetWorldPos(worldPos);
			hit->SetLocalPos(localPos);
			fHitsCollection->insert(hit);
		} 
	}

	return true;
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

void MuonDetectorSD::EndOfEvent(G4HCofThisEvent*)
{	
	if (verboseLevel > 1)
	{
		G4int nofHits = fHitsCollection->entries();
	G4cout << G4endl
		<< "Hits Collection ID[" << fHCID <<"]: in this event there are " << nofHits 
		<< " hits in the tracker chambers: " << G4endl;
	for ( G4int i=0; i<nofHits; i++ ) (*fHitsCollection)[i]->Print();
	}
}
	

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......
