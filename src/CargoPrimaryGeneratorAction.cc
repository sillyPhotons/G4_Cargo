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
// $Id: CargoPrimaryGeneratorAction.cc 94307 2015-11-11 13:42:46Z gcosmo $
//
/// \file CargoPrimaryGeneratorAction.cc
/// \brief Implementation of the CargoPrimaryGeneratorAction class

#include "CargoPrimaryGeneratorAction.hh"

#include "G4LogicalVolumeStore.hh"
#include "G4LogicalVolume.hh"
#include "G4Box.hh"
#include "G4RunManager.hh"
#include "G4ParticleGun.hh"
#include "G4ParticleTable.hh"
#include "G4ParticleDefinition.hh"
#include "G4SystemOfUnits.hh"
#include "Randomize.hh"

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

CargoPrimaryGeneratorAction::CargoPrimaryGeneratorAction()
	: G4VUserPrimaryGeneratorAction(),
	  fParticleGun(0),
	  field_x(10. * m),
	  field_y(3. * m),
	  field_z(3. * m),
	  gun_num(1),
	  fMessenger(0)
{
	DefineCommands();

	G4int n_particle = 1;
	fParticleGun = new G4ParticleGun(n_particle);
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

CargoPrimaryGeneratorAction::~CargoPrimaryGeneratorAction()
{
	delete fParticleGun;
	delete fMessenger;
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

void CargoPrimaryGeneratorAction::GeneratePrimaries(G4Event *anEvent)
{
	switch (gun_num)
	{
	case 1:
		gGun();
		break;
	case 2:
		nGun();
		break;
	case 3:
		mGun();
		break;
	default:
		gGun();
		break;
	}

	fParticleGun->GeneratePrimaryVertex(anEvent);
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

void CargoPrimaryGeneratorAction::DefineCommands()
{
	fMessenger = new G4GenericMessenger(this,
										"/Cargo/gun/",
										"Primary Generator Control");

	G4GenericMessenger::Command &chooseGun 
		= fMessenger->DeclareProperty("chooseGun",
									  gun_num,
								"Choose Gun: 1: Gamma, 2: Neutron, 3: Muon");

	chooseGun.SetParameterName("num", true);
	chooseGun.SetRange("num>=1 && num <=3");
	chooseGun.SetDefaultValue("1");
}

void CargoPrimaryGeneratorAction::gGun()
{
	G4ParticleTable *particleTable = G4ParticleTable::GetParticleTable();
	G4ParticleDefinition *particle = particleTable->FindParticle("gamma");

	fParticleGun->SetParticleDefinition(particle);
	fParticleGun->SetParticleMomentumDirection(G4ThreeVector(0., 0., -1.));
	fParticleGun->SetParticleEnergy(5. * MeV);

	G4double particle_x = field_x * G4UniformRand() - 0.5 * field_x,
			 particle_y = field_y * G4UniformRand() - 0.5 * field_y,
			 particle_z = field_z;

	G4ThreeVector particle_pos = G4ThreeVector(particle_x, particle_y, particle_z);
	fParticleGun->SetParticlePosition(particle_pos);
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

void CargoPrimaryGeneratorAction::nGun()
{
	G4ParticleTable *particleTable = G4ParticleTable::GetParticleTable();
	G4ParticleDefinition *particle = particleTable->FindParticle("neutron");

	fParticleGun->SetParticleDefinition(particle);
	fParticleGun->SetParticleMomentumDirection(G4ThreeVector(0., 0., -1.));
	fParticleGun->SetParticleEnergy(15. * MeV);

	G4double particle_x = field_x * G4UniformRand() - 0.5 * field_x,
			 particle_y = field_y * G4UniformRand() - 0.5 * field_y,
			 particle_z = field_z;

	G4ThreeVector particle_pos = G4ThreeVector(particle_x, particle_y, particle_z);
	fParticleGun->SetParticlePosition(particle_pos);
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

void CargoPrimaryGeneratorAction::mGun()
{
	G4ParticleTable *particleTable = G4ParticleTable::GetParticleTable();
	G4String particleName;
	G4ParticleDefinition *particle = particleTable->FindParticle(particleName = "mu-");

	fParticleGun->SetParticleDefinition(particle);
	fParticleGun->SetParticleMomentumDirection(G4ThreeVector(0., -1, 0.));
	fParticleGun->SetParticleEnergy(4. * GeV);

	G4double particle_x = field_x * G4UniformRand() - 0.5 * field_x,
			 particle_y = field_y,
			 particle_z = field_z * G4UniformRand() - 0.5 * field_z;

	G4ThreeVector particle_pos = G4ThreeVector(particle_x, particle_y, particle_z);
	fParticleGun->SetParticlePosition(particle_pos);
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......