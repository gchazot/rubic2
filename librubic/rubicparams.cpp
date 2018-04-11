/** \file rubicparams.cpp
  * \brief Contient les définitions des fonctions membres de RubicParams.
  */

#include "rubicparams.h"

#include "params.h"
#include "rubicparams_defaults.h"
#include "rubicparams_names.h"
#include "dvect.h"
#include "lvect.h"

#include <qstring.h>
#include <qfileinfo.h>

/**********************************************************************
****                 Construction et Destruction                   ****
**********************************************************************/
//Constructeur par défaut
RubicParams::RubicParams(void):
	mCmdFileName(""), mVgiRefFileName(""), mVgiDefFileName(""),
	mAsciiOutputFileName(""), mInpOutputFileName(""), mLogOutputFileName(""),
	
	mVoxernSize(RPD_VOXERNSIZE),
	
	mMinCoordCalc(-1,-1,-1), mMaxCoordCalc(-1,-1,-1),
	
	mStartPointRef(RPD_STARTPOINT_REF_X, RPD_STARTPOINT_REF_Y, RPD_STARTPOINT_REF_Z),
	mStartPointDef(RPD_STARTPOINT_DEF_X, RPD_STARTPOINT_DEF_Y, RPD_STARTPOINT_DEF_Z),
	
	mNGMini(RPD_NGMINI), mNGMaxi(RPD_NGMAXI),
	
	mMaxThreads(RPD_MAXTHREADS),
	
	mInterpolCubiq(RPD_INTERPOLCUBIQ),
	
	mTailleZoneScanning(RPD_TAILLEZONNESCANNING),
	
	mPasScanning(RPD_PASSCANNING),
	mPasNewtonGradConj(RPD_PASNEWTONGRADCONJ),
	
	mPrecScanning(RPD_PRECSCANNING), mPrecGradConj(RPD_PRECGRADCONJ),
	mPrecNewtonGradConj(RPD_PRECNEWTONGRADCONJ)
{
}

//Destructeur
RubicParams::~RubicParams()
{
	// Rien à détruire pour l'instant
}
	
/**********************************************************************
****             Fonctions d'accès aux paramètres                  ****
**********************************************************************/
/*         Fichiers d'entrée-sortie            **/
// Récupère le nom du fichier de commande
QString RubicParams::getCmdFileName(void) const
{
	return mCmdFileName;
}

// Modifie le nom du fichier de commande
void RubicParams::setCmdFileName(const QString& newName)
{
	QFileInfo info(newName);
	mCmdFileName = info.absFilePath();
}

// Récupère le nom du fichier vgi de l'image de référence
QString RubicParams::getVgiRefFileName(void) const
{
	return mVgiRefFileName;
}

// Modifie le nom du fichier vgi de l'image de référence
void RubicParams::setVgiRefFileName(QString& newName)
{
	mVgiRefFileName = newName;
}


// Récupère le nom du fichier vgi de l'image déformée
QString RubicParams::getVgiDefFileName(void) const
{
	return mVgiDefFileName;
}

// Modifie le nom du fichier vgi de l'image déformée
void RubicParams::setVgiDefFileName(QString& newName)
{
	mVgiDefFileName = newName;
}

// Récupère le nom du fichier de sortie des résultats (ASCII)
QString RubicParams::getAsciiOutputFileName(void) const
{
	return mAsciiOutputFileName;
}

// Modifie le nom du fichier de sortie des résultats (ASCII)
void RubicParams::setAsciiOutputFileName(QString& newName)
{
	mAsciiOutputFileName = newName;
}

// Récupère le nom du fichier de sortie des résultats (INP Abaqus)
QString RubicParams::getInpOutputFileName(void) const
{
	return mInpOutputFileName;
}

// Modifie le nom du fichier de sortie des résultats (INP Abaqus)
void RubicParams::setInpOutputFileName(QString& newName)
{
	mInpOutputFileName = newName;
}

// Récupère le nom du fichier de log du programme
QString RubicParams::getLogFileName(void) const
{
	return mLogOutputFileName;
}

// Modifie le nom du fichier de log du programme
void RubicParams::setLogFilName(QString& newName)
{
	mLogOutputFileName = newName;
}

/*          Paramètres de calcul              **/
// Récupère la taille d'un voxern
long RubicParams::getVoxernSize(void) const
{
	return mVoxernSize;
}

// Modifie la taille du voxern
void RubicParams::setVoxernSize(long newSize)
{
	mVoxernSize = newSize;
}

// Récupère les coordonnées de la zone dans laquelle effectuer le calcul
LVect RubicParams::getMinCoord(void) const
{
	return mMinCoordCalc;
}

LVect RubicParams::getMaxCoord(void) const
{
	return mMaxCoordCalc;
}

// Modifie les coordonnées de la sone dans laquelle effectuer le cacul
void RubicParams::setMinCoord(LVect& newCoord)
{
	mMinCoordCalc = newCoord;
}

void RubicParams::setMaxCoord(LVect& newCoord)
{
	mMaxCoordCalc = newCoord;
}

// Récupère les coordonnées du point de départ du calcul dans l'image
// de référence
LVect RubicParams::getStartPointRef(void) const
{
	return mStartPointRef;
}
// Modifie les coordonnées du point de départ du calcul dans l'image
// de référence
void RubicParams::setStartPointRef(LVect newCoord)
{
	mStartPointRef = newCoord;
}

// Récupère les coordonnées du point de départ du calcul dans l'image
// déformée
LVect RubicParams::getStartPointDef(void) const
{
	return mStartPointDef;
}

// Modifie les coordonnées du point de départ du calcul dans l'image
// déformée
void RubicParams::setStartPointDef(LVect newCoord)
{
	mStartPointDef = newCoord;
}


// Récupère le niveau de gris minimal à prendre en compte dans les
// calculs
unsigned char RubicParams::getNGMini(void) const
{
	return mNGMini;
}

// Modifie le niveau de gris minimal à prendre en compte dans les
// calculs
void RubicParams::setNGMini(unsigned char newMin)
{
	mNGMini = newMin;
}

// Récupère le niveau de gris maximal à prendre en compte dans les
// calculs
unsigned char RubicParams::getNGMaxi(void) const
{
	return mNGMaxi;
}

// Modifie le niveau de gris maximal à prendre en compte dans les
// calculs
void RubicParams::setNGMaxi(unsigned char newMax)
{
	mNGMaxi = newMax;
}

// Récupère le nombre maximal de threads executables en même temps
int RubicParams::getMaxThreads(void) const
{
	return mMaxThreads;
}

// Nombre maximal de threads executables en même temps
void RubicParams::setMaxThreads(int newMax)
{
	mMaxThreads = newMax;
}

// Récupère : Utilisation de l'interpolation par splines cubiques?
bool RubicParams::getInterpolCubiq(void) const
{
	return mInterpolCubiq;
}

// Modifie : Utilisation de l'interpolation par splines cubiques?
void RubicParams::setInterpoleCubiq(bool newVal)
{
	mInterpolCubiq = newVal;
}

/*     Paramètres des algorithmes de calcul    **/
// Récupère la taille de la zone de recherche en scanning pour le
// premier voxern
double RubicParams::getTailleZoneScanning(void) const
{
	return mTailleZoneScanning;
}

// Modifie la taille de la zone de recherche en scanning pour le
// premier voxern
void RubicParams::setTailleZoneScanning(double newSize)
{
	mTailleZoneScanning = newSize;
}

// Récupère le pas de calcul du scanning initial
double RubicParams::getPasScanning(void) const
{
	return mPasScanning;
}
// Modifie le pas de calcul du scanning initial
void RubicParams::setPasScanning(double newVal)
{
	mPasScanning = newVal;
}

// Récupère le pas de calcul du Newton dans le gradient conjuqué
double RubicParams::getPasNewtonGradConj(void) const
{
	return mPasNewtonGradConj;
}

// Modifie le pas de calcul du Newton dans le gradient conjuqué
void RubicParams::setPasNewtonGradConj(double newVal)
{
	mPasNewtonGradConj = newVal;
}

// Récupère la précision désirée pour la convergence du scanning initial
double RubicParams::getPrecScanning(void) const
{
	return mPrecScanning;
}

// Modifie la précision désirée pour la convergence du scanning initial
void RubicParams::setPrecScanning(double newVal)
{
	mPrecScanning = newVal;
}

// Récupère la précision désirée pour la convergence du gradient conjugué
double RubicParams::getPrecGradConj(void) const
{
	return mPrecGradConj;
}

// Modifie la précision désirée pour la convergence du gradient conjugué
void RubicParams::setPrecGradConj(double newVal)
{
	mPrecGradConj = newVal;
}

// Récupère la précision désirée pour la convergence du Newton dans le
// gradient conjugué
double RubicParams::getPrecNewtonGradConj(void) const
{
	return mPrecNewtonGradConj;
}

// Modifie la précision désirée pour la convergence du Newton dans le
// gradient conjugué
void RubicParams::setPrecNewtonGradConj(double newVal)
{
	mPrecNewtonGradConj = newVal;
}

// Récupère le nombre maximum d'itérations pour la convergence du gradient conjugué
long RubicParams::getMaxitGradConj(void) const
{
	return mMaxitGradConj;
}

// Récupère le nombre maximum d'itérations pour la convergence du gradient conjugué
void RubicParams::setMaxitGradConj(const long newVal )
{
	mMaxitGradConj = newVal;
}

// Récupère le nombre maximum d'itérations pour la convergence du Newton du gradient conjugué
long RubicParams::getMaxitNewtonGradConj(void) const
{
	return mMaxitNewtonGradConj;
}

// Récupère le nombre maximum d'itérations pour la convergence du Newton du gradient conjugué
void RubicParams::setMaxitNewtonGradConj(const long newVal )
{
	mMaxitNewtonGradConj = newVal;
}

/**********************************************************************
****  Fonctions de lecture/écriture des paramètres dans un fichier ****
**********************************************************************/
// Lit les paramètres depuis le fichier de commande et renvoie true si
// l'opération réussit. Renvoie false si une erreur se produit durant
// la lecture du fichier des paramètres.
bool RubicParams::readCommandFile(void)
{
	long tmplong;
// 	short tmpshort;
	int tmpint;
	double tmpdouble;
	bool tmpbool;
	DVect tmpdvect;
	LVect tmplvect;
	
	Params parametres;
	
	// Lecture du fichier
	if( ! parametres.readFromFile(mCmdFileName) )
		return false;
	
	// Récupération du nom du fichier vgi de l'image de référence
	if( ! parametres.getString(RPN_GRP_IOFILES, RPN_IOFILES_VGIREF, &mVgiRefFileName) )
		mVgiRefFileName = "";
	// Récupération du nom du fichier vgi de l'image déformes
	if( ! parametres.getString(RPN_GRP_IOFILES, RPN_IOFILES_VGIDEF, &mVgiDefFileName) )
		mVgiDefFileName = "";
	// Récupération du nom du fichier de sortie ASCII
	if( ! parametres.getString(RPN_GRP_IOFILES, RPN_IOFILES_ASCIIOUT, &mAsciiOutputFileName) )
		mAsciiOutputFileName = "";
	// Récupération du nom du fichier de sortie INP
	if( ! parametres.getString(RPN_GRP_IOFILES, RPN_IOFILES_INPOUT, &mInpOutputFileName) )
		mInpOutputFileName = "";
	// Récupération du nom du fichier de sortie des Logs
	if( ! parametres.getString(RPN_GRP_IOFILES, RPN_IOFILES_LOGOUT, &mLogOutputFileName) )
		mLogOutputFileName = "";

	// Récupération de la taille du voxern
	if( ! parametres.getLong(RPN_GRP_CALCPARAMS, RPN_CALCPARAMS_VOXSIZE, &mVoxernSize) )
		mVoxernSize = RPD_VOXERNSIZE;
	// Récupération des  coordonnées mini de la zone dans laquelle
	// effectuer le calcul
	if( ! parametres.getLVect(RPN_GRP_CALCPARAMS, RPN_CALCPARAMS_ZONEMIN, &tmplvect) )
		mMinCoordCalc = LVect(0,0,0);
	else
		mMinCoordCalc = tmplvect;
		
	if( ! parametres.getLVect(RPN_GRP_CALCPARAMS, RPN_CALCPARAMS_ZONEMAX, &tmplvect) )
		mMaxCoordCalc = LVect(0,0,0);
	else
		mMaxCoordCalc = tmplvect;
		
	// Récupération des coordonnées du point de départ dans l'image de référence
	if( ! parametres.getLVect(RPN_GRP_CALCPARAMS, RPN_CALCPARAMS_STARTREF, &tmplvect) )
		mStartPointRef = RPD_STARTPOINT_REF;
	else
		mStartPointRef = tmplvect;
		
	// Récupération des coordonnées du point de départ dans l'image déformée
	if( ! parametres.getLVect(RPN_GRP_CALCPARAMS, RPN_CALCPARAMS_STARTDEF, &tmplvect) )
		mStartPointDef = RPD_STARTPOINT_DEF;
	else
		mStartPointDef = tmplvect;
		
	// Récupération du niveau de gris mini à prendre en compte pour le calcul
	if( ! parametres.getInt(RPN_GRP_CALCPARAMS, RPN_CALCPARAMS_NG_MIN, &tmpint) )
		mNGMini = RPD_NGMINI;
	else
		mNGMini = (unsigned char) tmpint;
	
	// Récupération du niveau de gris maxi à prendre en compte pour le calcul
	if( ! parametres.getInt(RPN_GRP_CALCPARAMS, RPN_CALCPARAMS_NG_MAX, &tmpint) )
		mNGMaxi = RPD_NGMAXI;
	else
		mNGMaxi = (unsigned char) tmpint;
		
	// Récupération du nombre maximal de threads executables en même temps
	if( ! parametres.getLong(RPN_GRP_CALCPARAMS, RPN_CALCPARAMS_MAXTHREADS, &tmplong) )
		mMaxThreads = RPD_MAXTHREADS;
	else
		mMaxThreads = tmplong;
		
	// Récupération de : Utilisation de l'interpolation par splines cubiques?
	if( ! parametres.getBool(RPN_GRP_CALCPARAMS, RPN_CALCPARAMS_INTERPOLCUBIQ, &tmpbool) )
		mInterpolCubiq = RPD_INTERPOLCUBIQ;
	else
		mInterpolCubiq = tmpbool;
			
	// Récupération de la taille de la zone de recherche en scanning
	if( ! parametres.getDouble(RPN_GRP_ALGOPARAMS, RPN_ALGOPARAMS_ZONESCAN, &tmpdouble) )
		mTailleZoneScanning = RPD_TAILLEZONNESCANNING;
	else
		mTailleZoneScanning = tmpdouble;
		
	// Récupération du pas de calcul en scanning
	if( ! parametres.getDouble(RPN_GRP_ALGOPARAMS, RPN_ALGOPARAMS_PASSCAN, &tmpdouble) )
		mPasScanning = RPD_PASSCANNING;
	else
		mPasScanning = tmpdouble;

	// Récupération du pas de calcul dans le Newton du gradient conjugué
	if( ! parametres.getDouble(RPN_GRP_ALGOPARAMS, RPN_ALGOPARAMS_PASNEWTONGC, &tmpdouble) )
		mPasNewtonGradConj = RPD_PASNEWTONGRADCONJ;
	else
		mPasNewtonGradConj = tmpdouble;
	
	// Récupération de la précision désirée  pour la convergence du scanning
	if( ! parametres.getDouble(RPN_GRP_ALGOPARAMS, RPN_ALGOPARAMS_PRECSCAN, &tmpdouble) )
		mPrecScanning = RPD_PRECSCANNING;
	else
		mPrecScanning = tmpdouble;
		
	// Récupération de la précision désirée  pour la convergence du gradient conjugué
	if( ! parametres.getDouble(RPN_GRP_ALGOPARAMS, RPN_ALGOPARAMS_PRECGRADCCONJ, &tmpdouble) )
		mPrecGradConj = RPD_PRECGRADCONJ;
	else
		mPrecGradConj = tmpdouble;
		
	// Récupération de la précision désirée  pour la convergence du Newton dans le gradient
	// conjugué.
	if( ! parametres.getDouble(RPN_GRP_ALGOPARAMS, RPN_ALGOPARAMS_PRECNEWTONGC, &tmpdouble) )
		mPrecNewtonGradConj = RPD_PRECNEWTONGRADCONJ;
	else
		mPrecNewtonGradConj = tmpdouble;
	
	// Récupération du nombre maximum d'itérations pour la convergence du gradient conjugué
	if( ! parametres.getLong(RPN_GRP_ALGOPARAMS, RPN_ALGOPARAMS_MAXITGRADCONJ, &tmplong) )
		mMaxitGradConj = RPD_MAXITGRADCONJ;
	else
		mMaxitGradConj = tmplong;
		
	// Récupération du nombre maximum d'itérations pour la convergence du Newton du
	// gradient conjugué
	if( ! parametres.getLong(RPN_GRP_ALGOPARAMS, RPN_ALGOPARAMS_MAXITNEWTONGC, &tmplong) )
		mMaxitNewtonGradConj = RPD_MAXITNEWTONGC;
	else
		mMaxitNewtonGradConj = tmplong;
		
	return true;
}

// Ecrit les paramètres dans le fichier de commande et renvoie true si
// l'opération réussit. Renvoie false si une erreur se produit durant
// l'écriture des paramètres.
bool RubicParams::writeCommandFile(void) const
{
	Params parametres;
	
	// Stockage du nom du fichier vgi de l'image de référence
	parametres.insertString(RPN_GRP_IOFILES, RPN_IOFILES_VGIREF, mVgiRefFileName);
	// Stockage du nom du fichier vgi de l'image déformes
	parametres.insertString(RPN_GRP_IOFILES, RPN_IOFILES_VGIDEF, mVgiDefFileName);
	// Stockage du nom du fichier de sortie ASCII
	parametres.insertString(RPN_GRP_IOFILES, RPN_IOFILES_ASCIIOUT, mAsciiOutputFileName);
	// Stockage du nom du fichier de sortie INP
	parametres.insertString(RPN_GRP_IOFILES, RPN_IOFILES_INPOUT, mInpOutputFileName);
	// Stockage du nom du fichier de sortie des Logs
	parametres.insertString(RPN_GRP_IOFILES, RPN_IOFILES_LOGOUT, mLogOutputFileName);

	// Stockage de la taille du voxern
	parametres.insertLong(RPN_GRP_CALCPARAMS, RPN_CALCPARAMS_VOXSIZE, mVoxernSize);
	// Stockage des  coordonnées mini de la zone dans laquelle
	// effectuer le calcul
	parametres.insertLVect(RPN_GRP_CALCPARAMS, RPN_CALCPARAMS_ZONEMIN, mMinCoordCalc);
	parametres.insertLVect(RPN_GRP_CALCPARAMS, RPN_CALCPARAMS_ZONEMAX, mMaxCoordCalc);


	// Stockage des coordonnées du point de départ dans l'image de référence
	parametres.insertLVect(RPN_GRP_CALCPARAMS, RPN_CALCPARAMS_STARTREF, mStartPointRef);
	
	// Stockage des coordonnées du point de départ dans l'image déformée
	parametres.insertLVect(RPN_GRP_CALCPARAMS, RPN_CALCPARAMS_STARTDEF, mStartPointDef);
	
	// Stockage du niveau de gris mini à prendre en compte pour le calcul
	parametres.insertInt(RPN_GRP_CALCPARAMS, RPN_CALCPARAMS_NG_MIN, (int)mNGMini);
	
	// Stockage du niveau de gris maxi à prendre en compte pour le calcul
	parametres.insertInt(RPN_GRP_CALCPARAMS, RPN_CALCPARAMS_NG_MAX, (int)mNGMaxi);
	
	// Stockage du nombre maximal de threads executables en même temps
	parametres.insertInt(RPN_GRP_CALCPARAMS, RPN_CALCPARAMS_MAXTHREADS, mMaxThreads);
	
	// Récupération de : Utilisation de l'interpolation par splines cubiques?
	parametres.insertBool(RPN_GRP_CALCPARAMS, RPN_CALCPARAMS_INTERPOLCUBIQ, mInterpolCubiq);
	
	// Stockage de la taille de la zone de recherche en scanning
	parametres.insertDouble(RPN_GRP_ALGOPARAMS, RPN_ALGOPARAMS_ZONESCAN, mTailleZoneScanning);
		
	// Stockage du pas de calcul en scanning
	parametres.insertDouble(RPN_GRP_ALGOPARAMS, RPN_ALGOPARAMS_PASSCAN, mPasScanning);
	
	// Stockage du pas de calcul dans le Newton du gradient conjugué
	parametres.insertDouble(RPN_GRP_ALGOPARAMS, RPN_ALGOPARAMS_PASNEWTONGC, mPasNewtonGradConj);
	
	// Stockage de la précision désirée  pour la convergence du scanning
	parametres.insertDouble(RPN_GRP_ALGOPARAMS, RPN_ALGOPARAMS_PRECSCAN, mPrecScanning);
		
	// Stockage de la précision désirée  pour la convergence du gradient conjugué
	parametres.insertDouble(RPN_GRP_ALGOPARAMS, RPN_ALGOPARAMS_PRECGRADCCONJ, mPrecGradConj);
		
	// Stockage de la précision désirée  pour la convergence du Newton dans le gradient
	// conjugué.
	parametres.insertDouble(RPN_GRP_ALGOPARAMS, RPN_ALGOPARAMS_PRECNEWTONGC, mPrecNewtonGradConj);
	
	// Stockage du nombre maximum d'itérations pour la convergence du gradient conjugué
	parametres.insertLong(RPN_GRP_ALGOPARAMS, RPN_ALGOPARAMS_MAXITGRADCONJ, mMaxitGradConj);
	
	// Stockage du nombre maximum d'itérations pour la convergence du Newton du gradient conjugué
	parametres.insertLong(RPN_GRP_ALGOPARAMS,RPN_ALGOPARAMS_MAXITNEWTONGC, mMaxitNewtonGradConj);
	
	return parametres.writeToFile(mCmdFileName, true);
}
