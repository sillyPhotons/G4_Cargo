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
// $Id: CargoDetectorConstruction.cc 94307 2015-11-11 13:42:46Z gcosmo $
//
/// \file CargoDetectorConstruction.cc
/// \brief Implementation of the CargoDetectorConstruction class

#include "CargoDetectorConstruction.hh"

#include "G4RunManager.hh"
#include "G4NistManager.hh"
#include "G4Box.hh"
#include "G4Cons.hh"
#include "G4Orb.hh"
#include "G4Sphere.hh"
#include "G4Trd.hh"
#include "G4Tubs.hh"
#include "G4LogicalVolume.hh"
#include "G4PVPlacement.hh"
#include "G4AssemblyVolume.hh"
#include "G4SystemOfUnits.hh"
#include "G4SDManager.hh"
#include "G4VSensitiveDetector.hh"
#include "G4GenericMessenger.hh"
#include "MuonDetectorSD.hh"

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

CargoDetectorConstruction::CargoDetectorConstruction()
	: G4VUserDetectorConstruction(),
	  checkOverlaps(false),
	  fMessenger(0),
	  gnDet_num(0),
	  flogicalMuonDetBot(0),
	  flogicalMuonDetTop(0)
{
	DefineCommands();
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

CargoDetectorConstruction::~CargoDetectorConstruction()
{
	delete fMessenger;
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

void CargoDetectorConstruction::ConstructSDandField()
{
	G4SDManager *SDman = G4SDManager::GetSDMpointer();

	G4VSensitiveDetector *muonDetTop = new MuonDetectorSD("/muonDetTop");
	SDman->AddNewDetector(muonDetTop);
	flogicalMuonDetTop->SetSensitiveDetector(muonDetTop);

	G4VSensitiveDetector *muonDetBot = new MuonDetectorSD("/muonDetBot");
	SDman->AddNewDetector(muonDetBot);
	flogicalMuonDetBot->SetSensitiveDetector(muonDetBot);
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

G4VPhysicalVolume *CargoDetectorConstruction::Construct()
{
	ConstructMaterials();

	G4Material *world_mat = G4Material::GetMaterial("G4_AIR");
	G4Material *container_mat = G4Material::GetMaterial("G4_Al");

	// material for gamma or neutron detection
	G4Material *gnDet_mat;
	switch (gnDet_num)
	{
	case 0:
		gnDet_mat = G4Material::GetMaterial("G4_CESIUM_IODIDE");
		break;
	case 1:
		gnDet_mat = G4Material::GetMaterial("G4_PLASTIC_SC_VINYLTOLUENE");
		break;
	default:
		gnDet_mat = G4Material::GetMaterial("G4_CESIUM_IODIDE");
		break;
	}

	G4Material *mDet_mat = G4Material::GetMaterial("G4_CESIUM_IODIDE");
	G4Material *gold_mat = G4Material::GetMaterial("G4_Au");
	G4Material *barrel_mat = G4Material::GetMaterial("wood");
	G4Material *barrelContent_mat = G4Material::GetMaterial("G4_WATER");
	G4Material *man_mat = G4Material::GetMaterial("G4_MUSCLE_SKELETAL_ICRP");
	G4Material *bomb_mat = G4Material::GetMaterial("G4_U");
	G4Material *lead_mat = G4Material::GetMaterial("G4_Pb");

	const G4double world_hx = 10. * m,
				   world_hy = 3. * m,
				   world_hz = 3. * m;

	G4Box *solidWorld = new G4Box("World", world_hx, world_hy, world_hz);

	G4LogicalVolume *logicalWorld = new G4LogicalVolume(solidWorld,
														world_mat,
														"World");

	G4VPhysicalVolume *physWorld = new G4PVPlacement(0,
													 G4ThreeVector(),
													 logicalWorld,
													 "World",
													 0,
													 false,
													 0,
													 checkOverlaps);

	G4double container_hx = 5. * m,
			 container_hy = 1.5 * m,
			 container_hz = 1.5 * m;

	G4Box *solidContainer = new G4Box("solidContainer",
									  container_hx,
									  container_hy,
									  container_hz);

	G4LogicalVolume *logicalContainer = new G4LogicalVolume(solidContainer,
															container_mat,
															"logicalContainer");

	new G4PVPlacement(0,
					  G4ThreeVector(),
					  logicalContainer,
					  "Container",
					  logicalWorld,
					  false,
					  0,
					  checkOverlaps);

	const G4double air_hx = container_hx - 5. * mm,
				   air_hy = container_hy - 5. * mm,
				   air_hz = container_hz - 5. * mm;

	G4Box *solidAir = new G4Box("solidAir", air_hx, air_hy, air_hz);

	G4LogicalVolume *logicalAir = new G4LogicalVolume(solidAir,
													  world_mat,
													  "logicalAir");

	new G4PVPlacement(0,
					  G4ThreeVector(),
					  logicalAir,
					  "AirInContainer",
					  logicalContainer,
					  false,
					  0,
					  checkOverlaps);

	const G4double gammaNeutron_hx = 5.0 * m,
				   gammaNeutron_hy = 1.5 * m,
				   gammaNeutron_hz = 5.0 * cm;

	G4Box *solidGammaNeutronDet = new G4Box("solidGammaNeutronDet",
											gammaNeutron_hx,
											gammaNeutron_hy,
											gammaNeutron_hz);

	G4LogicalVolume *logicalGammaNeutronDet = new G4LogicalVolume(solidGammaNeutronDet,
																  lead_mat,
																  "logicalGammaNeutronDet");

	G4ThreeVector gammaNeutronDetPos = G4ThreeVector(0, 0, -1 * (container_hz + 10.0 * cm));

	new G4PVPlacement(0,
					  gammaNeutronDetPos,
					  logicalGammaNeutronDet,
					  "GammaNeutronDetector",
					  logicalWorld,
					  false,
					  0,
					  checkOverlaps);

	const G4double muon_hx = 5.0 * m,
				   muon_hy = 5.0 * cm,
				   muon_hz = 1.5 * m;

	G4Box *solidMuonDet = new G4Box("solidMuonDet",
									muon_hx,
									muon_hy,
									muon_hz);

	flogicalMuonDetBot = new G4LogicalVolume(solidMuonDet,
											 mDet_mat,
											 "logicalMuonDetBot");
	flogicalMuonDetTop = new G4LogicalVolume(solidMuonDet,
											 mDet_mat,
											 "logicalMuonDetTot");

	G4ThreeVector muonDetPos1 = G4ThreeVector(0, -1 * (container_hy + 10.0 * cm), 0);

	new G4PVPlacement(0,
					  muonDetPos1,
					  flogicalMuonDetBot,
					  "MuonDetectorBot",
					  logicalWorld,
					  false,
					  0,
					  checkOverlaps);

	G4ThreeVector muonDetPos2 = G4ThreeVector(0, (container_hy + 10.0 * cm), 0);

	new G4PVPlacement(0,
					  muonDetPos2,
					  flogicalMuonDetTop,
					  "MuonDetectorTop",
					  logicalWorld,
					  false,
					  1,
					  checkOverlaps);

	G4Trd *solidGold = new G4Trd("solidGold",
								 125.0 * mm,
								 100.0 * mm,
								 225.0 * mm,
								 200.0 * mm,
								 50.0 * mm);

	G4LogicalVolume *logicalGold = new G4LogicalVolume(solidGold,
													   gold_mat,
													   "logicalGold");

	G4RotationMatrix *gold_rot = new G4RotationMatrix();
	gold_rot->rotateX(90 * deg);

	G4int gold_count = 0;
	for (int k = 0; k < 5; k += 1)
	{
		for (int j = 0; j < 15; j += 1)
		{
			for (int i = 0; i < 8; i += 1)
			{
				G4double gold_xpos = -4.5 * m + i * (250 * mm),
						 gold_ypos = -1.5 * m + 55.0 * mm + j * (110 * mm),
						 gold_zpos = 1.0 * m - k * (500 * mm);

				new G4PVPlacement(gold_rot,
								  G4ThreeVector(gold_xpos, gold_ypos, gold_zpos),
								  logicalGold,
								  "gold",
								  logicalAir,
								  false,
								  gold_count,
								  checkOverlaps);

				gold_count += 1;
			}
		}
	}

	G4double barrel_outer_radius = 0.5 * m,
			 barrel_half_length = 0.5 * m;

	G4Tubs *solidBarrel = new G4Tubs("solidBarrel",
									 0.,
									 barrel_outer_radius,
									 barrel_half_length,
									 0 * deg,
									 360 * deg);

	G4LogicalVolume *logicalBarrel = new G4LogicalVolume(solidBarrel,
														 barrel_mat,
														 "logicalBarrel");

	G4Tubs *solidBarrelContent = new G4Tubs("solidBarrelContent",
											barrel_outer_radius - 5 * cm,
											0.5 * m,
											barrel_half_length - 2.5 * cm,
											0 * deg,
											360 * deg);

	G4LogicalVolume *logicalBarrelContent = new G4LogicalVolume(solidBarrelContent,
																barrelContent_mat,
																"logicalBarrelContent");

	new G4PVPlacement(0,
					  G4ThreeVector(0, 0, 2.5 * cm),
					  logicalBarrelContent,
					  "physicalBarrelContent",
					  logicalBarrel,
					  0,
					  checkOverlaps);

	G4RotationMatrix *barrrel_rot = new G4RotationMatrix();
	barrrel_rot->rotateX(90 * deg);
	new G4PVPlacement(barrrel_rot,
					  G4ThreeVector(-1.5 * m, -1 * m, 0.7 * m),
					  logicalBarrel,
					  "physicalBarrel1",
					  logicalAir,
					  0,
					  checkOverlaps);
	new G4PVPlacement(barrrel_rot,
					  G4ThreeVector(-0.7 * m, -1 * m, -0.7 * m),
					  logicalBarrel,
					  "physicalBarrel2",
					  logicalAir,
					  0,
					  checkOverlaps);

	G4double manAir_hx = 0.4 * m,
			 manAir_hy = 1. * m,
			 manAir_hz = 0.4 * m;

	G4Box *solidManAir = new G4Box("solidManAir", manAir_hx, manAir_hy, manAir_hz);

	G4LogicalVolume *logicalManAir = new G4LogicalVolume(solidManAir,
														 world_mat,
														 "logicalManAir");

	new G4PVPlacement(0,
					  G4ThreeVector(1 * m, -0.5 * m, 1. * m),
					  logicalManAir,
					  "physicalManAir",
					  logicalAir,
					  0,
					  checkOverlaps);

	G4RotationMatrix *man_rot = new G4RotationMatrix();
	man_rot->rotateY(30 * deg);
	new G4PVPlacement(man_rot,
					  G4ThreeVector(4 * m, -0.5 * m, 0. * m),
					  logicalManAir,
					  "physicalManAir",
					  logicalAir,
					  0,
					  checkOverlaps);

	G4RotationMatrix *man_rot2 = new G4RotationMatrix();
	man_rot2->rotateY(60 * deg);
	new G4PVPlacement(man_rot2,
					  G4ThreeVector(3 * m, -0.5 * m, -1. * m),
					  logicalManAir,
					  "physicalManAir",
					  logicalAir,
					  0,
					  checkOverlaps);

	G4Box *solidManLeg = new G4Box("solidManLeg", 0.1 * m, 0.5 * m, 0.1 * m);

	G4LogicalVolume *logicalManLeg = new G4LogicalVolume(solidManLeg,
														 man_mat,
														 "logicalManLeg");

	new G4PVPlacement(0, G4ThreeVector(-0.14 * m, -0.5 * m, 0. * m),
					  logicalManLeg, "physicalManLegLeft", logicalManAir,
					  false, 0, checkOverlaps);
	new G4PVPlacement(0, G4ThreeVector(0.14 * m, -0.5 * m, 0. * m),
					  logicalManLeg, "physicalManLegRight", logicalManAir,
					  false, 0, checkOverlaps);

	G4Box *solidManBody = new G4Box("solidManBody", 0.25 * m, 0.27 * m, 0.25 * m);

	G4LogicalVolume *logicalManBody = new G4LogicalVolume(solidManBody,
														  man_mat,
														  "logicalManBody");

	new G4PVPlacement(0, G4ThreeVector(0, 0.27 * m, 0),
					  logicalManBody, "physicalManBody", logicalManAir,
					  false, 0, checkOverlaps);

	G4Orb *solidManHead = new G4Orb("solidManHead", 0.2 * m);

	G4LogicalVolume *logicalManHead = new G4LogicalVolume(solidManHead,
														  man_mat,
														  "logicalManHead");

	new G4PVPlacement(0, G4ThreeVector(0, 0.74 * m, 0),
					  logicalManHead, "physicalManHead", logicalManAir,
					  false, 0, checkOverlaps);

	G4Box *solidManArm = new G4Box("solidManArm", 0.07 * m, 0.3 * m, 0.1 * m);

	G4LogicalVolume *logicalManArm = new G4LogicalVolume(solidManArm,
														 man_mat,
														 "logicalManArm");

	new G4PVPlacement(0, G4ThreeVector(-0.32 * m, 0.22 * m, 0),
					  logicalManArm, "physicalManArm1", logicalManAir,
					  false, 0, checkOverlaps);

	new G4PVPlacement(0, G4ThreeVector(0.32 * m, 0.22 * m, 0),
					  logicalManArm, "physicalManArm2", logicalManAir,
					  false, 0, checkOverlaps);

	G4double shield_hx = 40. * cm,
			 shield_hy = 40. * cm,
			 shield_hz = 40. * cm;

	G4Box *solidShield = new G4Box("solidShield", shield_hx, shield_hy, shield_hz);

	G4LogicalVolume *logicalShield = new G4LogicalVolume(solidShield,
														 lead_mat,
														 "logicalShield");

	new G4PVPlacement(0,
					  G4ThreeVector(2.0*m, -(air_hy - shield_hy), 0.*m),
					  logicalShield, "physicalShield", logicalAir,
					  false, 0, checkOverlaps);

	G4Box *solidShieldAir 
		= new G4Box("solidShieldAir", shield_hx - 0.5*cm, shield_hz - 0.5*cm, shield_hz - 0.5*cm);
	
	G4LogicalVolume *logicalShieldAir
		= new G4LogicalVolume(solidShieldAir,
							  world_mat,
							  "solidShieldAir");

	new G4PVPlacement(0,
					  G4ThreeVector(),
					  logicalShieldAir, "physicalShieldAir", logicalShield,
					  false, 0, checkOverlaps);

	G4Orb *solidBomb = new G4Orb("solidBomb", shield_hx - 2.*cm);
	
	G4LogicalVolume *logicalBomb
		= new G4LogicalVolume(solidBomb,
							  bomb_mat,
							  "logicalBomb");

	new G4PVPlacement(0,
					  G4ThreeVector(),
					  logicalBomb, "physicalBomb", logicalShieldAir,
					  false, 0, checkOverlaps);

	return physWorld;
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

void CargoDetectorConstruction::ConstructMaterials()
{
	G4String name, symbol;
	G4double density;
	G4int ncomp, z;

	G4NistManager *nistMan = G4NistManager::Instance();

	density = 1.01 * g / mole;
	G4Element *elH = new G4Element(name = "Hydrogen", symbol = "H", z = 1., density);
	density = 12.01 * g / mole;
	G4Element *elC = new G4Element(name = "Carbon", symbol = "C", z = 6., density);
	density = 16.00 * g / mole;
	G4Element *elO = new G4Element(name = "Oxygen", symbol = "O", z = 8., density);

	// Container Material
	nistMan->FindOrBuildMaterial("G4_AIR");
	nistMan->FindOrBuildMaterial("G4_Al");

	nistMan->FindOrBuildMaterial("G4_WATER");
	nistMan->FindOrBuildMaterial("G4_Au");
	nistMan->FindOrBuildMaterial("G4_MUSCLE_SKELETAL_ICRP");
	nistMan->FindOrBuildMaterial("G4_Pb");
	nistMan->FindOrBuildMaterial("G4_U");

	G4Material *wood = new G4Material(name = "wood", density = 0.9 * g / cm3, ncomp = 3);
	wood->AddElement(elH, 4);
	wood->AddElement(elO, 1);
	wood->AddElement(elC, 2);

	// Detector Material
	nistMan->FindOrBuildMaterial("G4_CESIUM_IODIDE");
	nistMan->FindOrBuildMaterial("G4_PLASTIC_SC_VINYLTOLUENE");

	G4cout << G4endl << "The materials defined are : " << G4endl << G4endl;
	G4cout << *(G4Material::GetMaterialTable()) << G4endl;
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

void CargoDetectorConstruction::DefineCommands()
{
	fMessenger = new G4GenericMessenger(this,
										"/Cargo/Gamma_Neutron/",
										"Gamma/Neutron Detector Material Control");

	G4GenericMessenger::Command &chooseGun 
		= fMessenger->DeclareProperty("chooseMat",
									  gnDet_num,
									  "Choose particle for detection: 0: Gamma, 1: Neutron");

	chooseGun.SetParameterName("num", true);
	chooseGun.SetRange("num>=0 && num <=1");
	chooseGun.SetDefaultValue("0");
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......