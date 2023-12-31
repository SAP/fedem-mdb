// SPDX-FileCopyrightText: 2023 SAP SE
//
// SPDX-License-Identifier: Apache-2.0
//
// This file is part of FEDEM - https://openfedem.org
////////////////////////////////////////////////////////////////////////////////

#ifndef FM_STRAIN_ROSETTE_H
#define FM_STRAIN_ROSETTE_H

#include "vpmDB/FmIsPlottedBase.H"
#include "vpmDB/FmPart.H"
#include "FFaLib/FFaContainers/FFaReference.H"
#include "FFaLib/FFaAlgebra/FFaMat34.H"


class FmStrainRosette : public FmIsPlottedBase
{
  Fmd_DB_HEADER_INIT();

public:
  FmStrainRosette();

  virtual const char* getUITypeName() const { return "Strain rosette"; }

  virtual void getEntities(std::vector<FmSensorChoice>& choicesToFill, int);
  virtual void getDofs(std::vector<FmSensorChoice>& choicesToFill);

  FaMat34 getSymbolPosMx(bool& matrixIsOk);
  FaMat34 getGlobSymbolPosMx(bool& matrixIsOk);

  double getElmWidth() const;
  FaVec3 getCalculationPoint() const;
  void   setGlobalAngleOriginVector(const FaVec3& dir);

  bool setTopology(FmPart* part, int n1, int n2, int n3, int n4);
  bool setNode(int ID, int idx);

  BoolVec syncWithFEModel();
  static void syncStrainRosettes(FmPart* part = NULL);

  void flipFaceNormal();

  virtual int printSolverEntry(FILE* fd);
  bool writeToRosetteInputFile(std::ostream& os);

  static bool writeSolverFile(const std::string& fName,
                              const FmPart* part = NULL);
  static bool writeRosetteInputFile(const std::string& fName,
                                    const FmPart* part = NULL);
  static bool createRosettesFromOldFile(const std::string& fName,
                                        bool defaultResetStartStrainValue);

  FFaField<FFaReferenceBase*> rosetteLinkField;
  FFaReference<FmPart>        rosetteLink;

  enum RosetteType {
    SINGLE_GAGE,
    DOUBLE_GAGE_90,
    TRIPLE_GAGE_60,
    TRIPLE_GAGE_45
  };

  FFaEnumMapping(RosetteType){
    FFaEnumEntry(SINGLE_GAGE,    "SINGLE_GAGE");
    FFaEnumEntry(DOUBLE_GAGE_90, "DOUBLE_GAGE_90");
    FFaEnumEntry(TRIPLE_GAGE_60, "TRIPLE_GAGE_60");
    FFaEnumEntry(TRIPLE_GAGE_45, "TRIPLE_GAGE_45");
    FFaEnumEntryEnd;
  };

  FFaField<RosetteTypeEnum> rosetteType;

  static const Strings& getRosetteUINames();

  FFaField<int> numNodes;

  FFaField<FaVec3> nodePos1;
  FFaField<FaVec3> nodePos2;
  FFaField<FaVec3> nodePos3;
  FFaField<FaVec3> nodePos4;

  FFaField<int> node1;
  FFaField<int> node2;
  FFaField<int> node3;
  FFaField<int> node4;

  double getZPos() const { return useFEThickness.getValue() ? 0.5*FEThickness.getValue() : zPos.getValue(); }

  FFaField<bool>   useFEThickness;
  FFaField<double> zPos;
  FFaField<double> FEThickness;

  enum AngleOriginType {
    LINK_X,
    LINK_Y,
    LINK_VECTOR
  };

  FFaEnumMapping(AngleOriginType){
    FFaEnumEntry(LINK_X,     "LINK_X");
    FFaEnumEntry(LINK_Y,     "LINK_Y");
    FFaEnumEntry(LINK_VECTOR,"LINK_VECTOR");
    FFaEnumEntryEnd;
  };

  FFaField<AngleOriginTypeEnum> angleOrigin;

  FFaField<double> angle;
  FFaField<FaVec3> angleOriginVector;

  double getEMod() const { return useFEMaterial.getValue() ? EModFE.getValue() : EMod.getValue(); }
  double getNu() const   { return useFEMaterial.getValue() ? nuFE.getValue() : nu.getValue(); }

  FFaField<bool>   useFEMaterial;
  FFaField<double> EMod;
  FFaField<double> EModFE;
  FFaField<double> nu;
  FFaField<double> nuFE;

  FFaField<bool> removeStartStrains;

  // Normal stuff:

  static bool readAndConnect(std::istream& is, std::ostream& os = std::cout);
  virtual std::ostream& writeFMF(std::ostream& os = std::cout);
  virtual bool clone(FmBase* obj, int depth);

protected:
  virtual ~FmStrainRosette();

  FmStrainRosette(const FmStrainRosette&) = delete;
  FmStrainRosette& operator=(const FmStrainRosette&) = delete;

  virtual bool cloneLocal(FmBase* obj, int depth);

private:
  FaVec3* tmpZDirection; //!< For reading old gage input files
};

#endif
