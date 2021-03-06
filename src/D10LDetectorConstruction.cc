#include "D10LDetectorConstruction.hh"

#include "G4RunManager.hh"
#include "G4NistManager.hh"
#include "G4Box.hh"
#include "G4Tubs.hh"
#include "G4PVPlacement.hh"
#include "G4SystemOfUnits.hh"

D10LDetectorConstruction::D10LDetectorConstruction()
: G4VUserDetectorConstruction(),
  fScoringVolume(0)
{ }

D10LDetectorConstruction::~D10LDetectorConstruction()
{ }

G4PhysicalVolume * D10LDetectorConstruction::Construct()
{

  //Get nist material manager
  G4NistManager * nist = G4NistManager::Instance();

  //chamber parameters
  G4double cham_ir = 19.84*cm, cham_or = 21*cm, cham_z = 26.6*cm;
  G4Material * cham_mat = nist->FindOrBuildMaterial("G4_STAINLESS-STEEL");

  //field ring parameters
  G4double ring_ir = 13.5*cm, ring_or = 16*cm, ring_z = 0.1*cm, ring_sep = 1*cm;
  G4Material * ring_mat = nist->FindOrBuildMaterial("G4_STAINLESS-STEEL");

  //ground plane parameters
  G4double gnd_r = ring_or;
  //readout parameters
  G4double top_cam = 14.3*cm, bot_cam = 17.6*cm;

  //check volume overlapping
  G4Bool checkOverlaps = true;

  //world
  G4double world_sizeXY = 1.2*cham_r, world_sizeZ = 1.2*cham_h;
  G4Material * world_mat = nist->FindOrBuildMaterial("G4_AIR");

  //solid: geometry
  G4Box * solidWorld = new G4Box("World", 0.5*world_sizeXY, 0.5*world_sizeXY, 0.5*world_sizeZ);
  //logical: physical properties
  G4LogicalVolume * logicWorld = new G4LogicalVolume(solidWorld, world_mat, "World");
  //physical: placement
  G4PhysicalVolume * physWorld = new G4PVPlacement(0,G4ThreeVector(),logicWorld,"World",0,false,0,checkOverlaps);


  //chamber
  G4Tubs * solidChamber = new G4Tubs("Chamber",cham_ir,cham_or,cham_z,0*deg,360*deg);
  G4LogicalVolume * logicChamber = new G4LogicalVolume(solidCham, cham_mat, "Chamber");
  G4PhysicalVolume * physChamber = new G4PVPlacement(0, //no rotation
						     G4ThreeVector(), //location (0,0,0)
						     logicChamber, //its logical volume
						     "Chamber", //its name
						     logicWorld, //its mother volume
						     false, //no boolean operation
						     0, // copy number
						     checkOverlaps); //overlap checking

  //ring
  G4Tubs * solidRing = new G4Tubs("Ring",ring_ir,ring_or,0*deg,360*deg);
  G4LogicalVolume * logicRing = new G4LogicalVolume(solidRing, ring_mat, "Ring");
  G4PhysicalVolume * physRing = new G4PVPlacement(0,G4ThreeVector(),logicRing,"Ring",logicWorld,false,0,checkOverlaps);

  return physWorld;

}
