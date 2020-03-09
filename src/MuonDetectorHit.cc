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
/// \file MuonDetectorHit.cc
/// \brief Implementation of the MuonDetectorHit class

#include "MuonDetectorHit.hh"

#include "G4VVisManager.hh"
#include "G4VisAttributes.hh"
#include "G4Circle.hh"
#include "G4Colour.hh"
#include "G4AttDefStore.hh"
#include "G4AttDef.hh"
#include "G4AttValue.hh"
#include "G4UIcommand.hh"
#include "G4UnitsTable.hh"
#include "G4SystemOfUnits.hh"
#include "G4ios.hh"

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

G4ThreadLocal G4Allocator<MuonDetectorHit> *MuonDetectorHitAllocator = 0;

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

MuonDetectorHit::MuonDetectorHit()
	: G4VHit(),
	  fLocalPos(0),
	  fWorldPos(0)
{
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

MuonDetectorHit::~MuonDetectorHit()
{
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

MuonDetectorHit::MuonDetectorHit(const MuonDetectorHit &right)
	: G4VHit()
{
	fWorldPos = right.fWorldPos;
	fLocalPos = right.fLocalPos;
}

MuonDetectorHit::MuonDetectorHit(G4int i)
	: G4VHit(),
	  fLocalPos(0),
	  fWorldPos(0),
	  copyno(i)
{
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

const MuonDetectorHit &MuonDetectorHit::operator=(const MuonDetectorHit &right)
{
	fWorldPos = right.fWorldPos;
	fLocalPos = right.fLocalPos;
	return *this;
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

int MuonDetectorHit::operator==(const MuonDetectorHit & /*right*/) const
{
	return 0;
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

void MuonDetectorHit::Draw()
{
	G4VVisManager *pVVisManager = G4VVisManager::GetConcreteInstance();
	if (pVVisManager)
	{
		G4Circle circle(fWorldPos);
		circle.SetScreenSize(2);
		circle.SetFillStyle(G4Circle::filled);
		G4Colour colour(1., 1., 0.);
		G4VisAttributes attribs(colour);
		circle.SetVisAttributes(attribs);
		pVVisManager->Draw(circle);
	}
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

const std::map<G4String, G4AttDef> *MuonDetectorHit::GetAttDefs() const
{
	G4bool isNew;
	std::map<G4String, G4AttDef> *store = G4AttDefStore::GetInstance("MuonDetectorHit", isNew);

	if (isNew)
	{
		(*store)["HitType"] = G4AttDef("HitType", "Hit Type", "Physics", "", "G4String");

		(*store)["ID"] = G4AttDef("ID", "ID", "Physics", "", "G4int");

		(*store)["Pos"] = G4AttDef("Pos", "Position", "Physics", "G4BestUnit", "G4ThreeVector");
	}
	return store;
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

std::vector<G4AttValue> *MuonDetectorHit::CreateAttValues() const
{
	std::vector<G4AttValue> *values = new std::vector<G4AttValue>;

	values
		->push_back(G4AttValue("HitType", "MuonDetectorHit", ""));
	values
		->push_back(G4AttValue("Pos", G4BestUnit(fWorldPos, "Length"), ""));

	return values;
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

void MuonDetectorHit::Print()
{ 
	G4cout << "World (x,z) " << fWorldPos.x()/cm << "[cm]" << ", " << fWorldPos.z()/cm << "[cm]" << G4endl;
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......
