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
// $Id: CargoDetectorConstruction.hh 69565 2013-05-08 12:35:31Z gcosmo $
//
/// \file CargoDetectorConstruction.hh
/// \brief Definition of the CargoDetectorConstruction class

#ifndef CargoDetectorConstruction_h
#define CargoDetectorConstruction_h 1

#include "G4VUserDetectorConstruction.hh"
#include "globals.hh"
#include "G4GenericMessenger.hh"

class G4VPhysicalVolume;
class G4LogicalVolume;

/// Detector construction class to define materials and geometry.

class CargoDetectorConstruction : public G4VUserDetectorConstruction
{
public:
	CargoDetectorConstruction();
	virtual ~CargoDetectorConstruction();
	virtual void ConstructSDandField();
	virtual G4VPhysicalVolume *Construct();

	void DefineCommands();
	void ConstructMaterials();

private:
	G4bool checkOverlaps;
	G4GenericMessenger *fMessenger;
	G4int gnDet_num;

	G4LogicalVolume *flogicalMuonDetBot;
	G4LogicalVolume *flogicalMuonDetTop;

};

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

#endif
