// SPDX-FileCopyrightText: 2023 SAP SE
//
// SPDX-License-Identifier: Apache-2.0
//
// This file is part of FEDEM - https://openfedem.org
////////////////////////////////////////////////////////////////////////////////

#include "vpmDB/FmSeaState.H"
#include "vpmDB/FmDB.H"
#include "FFaLib/FFaString/FFaParse.H"
#include "FFaLib/FFaString/FFaStringExt.H"
#ifdef USE_INVENTOR
#include "vpmDisplay/FdSeaState.H"
#endif


/**********************************************************************
 *
 * Class constructor and destructor
 *
 **********************************************************************/

Fmd_DB_SOURCE_INIT(FcSEA_STATE, FmSeaState, FmIsRenderedBase);


FmSeaState::FmSeaState()
{
  Fmd_CONSTRUCTOR_INIT(FmSeaState);

  FFA_FIELD_INIT(waterDensity,1000.0,"WATER_DENSITY");
  FFA_FIELD_INIT(meanSeaLevel,0.0,"MEAN_SEA_LEVEL");
  FFA_FIELD_INIT(waveDir,FaVec3(1.0,0.0,0.0),"WAVE_DIRECTION");
  FFA_FIELD_INIT(xLength,15.0,"VISU_LENGTH_X");
  FFA_FIELD_INIT(yLength,15.0,"VISU_LENGTH_Y");
  FFA_FIELD_INIT(x,0.0,"VISU_X");
  FFA_FIELD_INIT(y,0.0,"VISU_Y");
  FFA_FIELD_INIT(seaDepth,0.0,"SEA_DEPTH");

  FFA_FIELD_INIT(myRGBColor, FmColor(0.0f,0.3f,0.8f), "COLOR");
  FFA_FIELD_INIT(myTransparency, 0.65, "TRANSPARENCY");
  FFA_FIELD_INIT(myQuantization, 10, "VISU_QUANTIZATION");
  FFA_FIELD_INIT(showGrid, 1, "VISU_SHOW_GRID");
  FFA_FIELD_INIT(showSolid, 1, "VISU_SHOW_SOLID");

  FFA_FIELD_INIT(growthDensity  ,0.0,"MARINE_GROWTH_DENSITY");
  FFA_FIELD_INIT(growthThickness,0.0,"MARINE_GROWTH_THICKNESS");
  FFA_FIELD_INIT(growthLimit,std::make_pair(0.0,0.0),"MARINE_GROWTH_LIMIT");

  FFA_REFERENCE_FIELD_INIT(waveFunctionField,waveFunction,"WAVE_FUNCTION");
  FFA_REFERENCE_FIELD_INIT(currFunctionField,currFunction,"CURR_FUNCTION");
  FFA_REFERENCE_FIELD_INIT(currentDirField,currentDir,"CURR_DIRECTION");
  FFA_REFERENCE_FIELD_INIT(currScaleField,currScale,"CURR_SCALE");
  FFA_REFERENCE_FIELD_INIT(hdfScaleField,hdfScale,"HYDRODYN_FORCE_SCALE");

  currScale.setPrintIfZero(false);
  hdfScale.setPrintIfZero(false);

#ifdef USE_INVENTOR
  itsDisplayPt = new FdSeaState(this);
#endif
}


FmSeaState::~FmSeaState()
{
  this->disconnect();
}


/**********************************************************************
 *
 * other class methods
 *
 **********************************************************************/

bool FmSeaState::clone(FmBase* obj, int depth)
{
  return cloneInherited(obj, depth);
}


bool FmSeaState::cloneLocal(FmBase* obj, int)
{
  return obj->isOfType(FmSeaState::getClassTypeID());
}


/***********************************************************************
*
* Input and output from stream.
*
************************************************************************/

std::ostream& FmSeaState::writeFMF(std::ostream& os)
{
  os <<"SEA_STATE\n{\n";
  this->writeFields(os);
  os <<"}\n\n";
  return os;
}


bool FmSeaState::readAndConnect(std::istream& is, std::ostream&)
{
  FmSeaState* obj = new FmSeaState();

  while (is.good())
  {
    std::stringstream activeStatement;
    char keyWord[BUFSIZ];
    if (FaParse::parseFMFASCII(keyWord, is, activeStatement, '=', ';'))
      parentParse(keyWord, activeStatement, obj);
  }

  return obj->cloneOrConnect();
}


bool FmSeaState::setRGBColor(const FmColor& col)
{
  if (col == myRGBColor.getValue()) return false;

  myRGBColor.setValue(col);
  this->draw();

  return true;
}


bool FmSeaState::setTransparency(double var)
{
  if (var == myTransparency.getValue()) return false;

  myTransparency.setValue(var);
  this->draw();

  return true;
}


bool FmSeaState::setX(double var)
{
  if (var == x.getValue()) return false;

  x.setValue(var);
  this->draw();

  return true;
}


bool FmSeaState::setY(double var)
{
  if (var == y.getValue()) return false;

  y.setValue(var);
  this->draw();

  return true;
}


bool FmSeaState::setQuantization(double var)
{
  if (var == myQuantization.getValue()) return false;

  myQuantization.setValue(var);
  this->draw();

  return true;
}


bool FmSeaState::setShowGrid(bool var)
{
  if (var == showGrid.getValue()) return false;

  showGrid.setValue(var);
  this->draw();

  return true;
}


bool FmSeaState::setShowSolid(bool var)
{
  if (var == showSolid.getValue()) return false;

  showSolid.setValue(var);
  this->draw();

  return true;
}


bool FmSeaState::setWaterDensity(double rho)
{
  if (rho == waterDensity.getValue()) return false;

  waterDensity.setValue(rho);

  return true;
}


bool FmSeaState::setMeanSeaLevel(double z0)
{
  if (z0 == meanSeaLevel.getValue()) return false;

  meanSeaLevel.setValue(z0);
  this->draw();

  return true;
}


bool FmSeaState::setWaveDir(const FaVec3& dir)
{
  if (dir == waveDir.getValue()) return false;

  waveDir.setValue(dir);
  this->draw();

  return true;
}


bool FmSeaState::setSeaDepth(double z)
{
  if (z == seaDepth.getValue()) return false;

  seaDepth.setValue(z);
  this->draw();

  return true;
}


bool FmSeaState::setLengthX(double x)
{
  if (x == xLength.getValue()) return false;

  xLength.setValue(x);
  this->draw();

  return true;
}


bool FmSeaState::setLengthY(double y)
{
  if (y == yLength.getValue()) return false;

  yLength.setValue(y);
  this->draw();

  return true;
}


void FmSeaState::onEventSwitched(const FmSimulationEvent*)
{
  this->draw();
}


const FaMat34& FmSeaState::getLocalCS() const
{
  if (FmDB::useSeaCS())
    myCS = FmDB::getSeaCS();

  return myCS;
}


const FaVec3& FmSeaState::getGrav() const
{
  return FmDB::getGrav();
}
