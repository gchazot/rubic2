/** \file main.cpp
  * \brief Le programme principal de calcul
  * C'est ici que se trouve le code qui lance le calcul d'après le fichier de
  * configuration donné en paramètre.
  */
  
// En-têtes de rubicalc
#include "globals.h"
#include "fonctions.h"

// En-têtes de rubicalc
#include "logutil.h"

// En-têtes standards
#include <cstdlib>
#include <getopt.h>
#include <iostream>
using namespace std;
#include <cmath>

// En-têtes Qt
#include <qstring.h>

/// Fonction de description du fonctionnement du programme
void usage(void)
{
	cout << "Rubicalc -- Copyright LamCoS 2006" << endl;
	cout << " -------------------------------" << endl;
	cout << "Utilisation : rubicalc [options] {-f | --file} fichier_commande" << endl;
	cout << "Options possibles:" << endl;
	cout << "\t-f fichier ou --file fichier : Spécifie le fichier de commande à utiliser." << endl;
	cout << "\t-l fichier ou --log-to-file fichier : Placer les logs globaux dans le fichier spécifié." << endl;
	cout << "\t-s ou --log-to-stdout : Placer les logs globaux dans la sortie standard." << endl;
	cout << "\t-e ou --log-to-stderr : Placer les logs globaux dans la sortie d'erreur." << endl;
	cout << "\t-h ou --help : Affiche ce message." << endl;
	cout << "\t-n ou --log-level niveau : sélectionne le niveau de log (1,2,3,4,5,10)." << endl;
	cout << endl;
	quitter(0);
}

/// Fonction principale du programme de calcul
/** Cette fonction prépare le lancement du calcul et lance le calcul proprement
  * dit.
  *
  * Elle commence par initialiser le système de log vers la sortie d'erreur
  * standard. Puis elle lit les paramètres de la ligne de commande et elle
  * charge en fonction le fichier de commande. Si cela a été spécifié dans
  * le fichier de paramètres ou sur la ligne de commande, le système de log est
  * modifié.
  *
  * Finalement le calcul est lancé.
  *
  * La fonction renvoie 0 si tout s'est déroulé correctement. Sinon elle
  * renvoie un code d'erreur.
  *
  * Organisation des codes d'erreur:
  * # 10 -> 19 : erreurs d'initialisation du programme
  * # 20 -> 29 : erreurs dans les options de ligne de commande
  * # 50 -> 69 : erreurs dans les paramètres du fichier de commande
  * # 70 -> 89 : erreurs de calcul
  * 
  */
int main(int argc, char** argv)
{
// Initialisation du système de log global
	// Le log s'effectue sur la sortie d'erreur
	
// 	stdlog = new LogUtil(LOG_TO_STDERR, LOGUTIL_INFO);
	stdlog = new LogUtil(LOG_TO_STDERR, LOGUTIL_DEBUG);
	
	if( (stdlog == NULL) || (!stdlog->isReady()) )
	{
		cerr << "Impossible d'initialiser le système de log !!" << endl;
		cerr << "Fin du programme." << endl << flush;
		quitter(10);
	}
	
	stdlog->message(LOGUTIL_INFO, "Système de log global initialisé.");
	
// Création des paramètres
	params = new RubicParams;
	if(params == NULL)
	{
		stdlog->message(LOGUTIL_CRITICAL, "Impossible de créer l'objet params.");
		quitter(11);
	}
	
	// Analyse des paramètres de la ligne de commande
	// -f nomfichier ou --file nomfichier
	//	permet de spécifier le nom du fichier de commande
	// -l nomfichier ou --log-to-file nomfichier
	//	permet de spécifier le nom du fichier de log global
	// -s ou --log-to-stdout
	//	permet d'activer le log global vers la sortie standard
	// -e ou --log-to-stderr
	//	permet d'activer le log global vers la sortie d'erreur
	// -h ou --help
	//	affiche le fonctionnement de la commande
	
	/// Paramètres courts gérés
	static char stropts[] = "f:l:sehn:";
	/// Paramètres longs gérés
	static struct option lopts[] = {
		{ "file", 1, 0, 'f' },		// --file
		{ "log-to-file", 1, 0, 'l' },	// --log-to-file
		{ "log-to-stdout", 0, 0, 's' },	// --log-to-stdout
		{ "log-to-stderr", 0, 0, 'e' },	// --log-to-stderr
		{ "help", 0, 0, 'h' },		// --help
		{ "log-level", 1, 0, 'n'}	// --log-level
	};
	/// La sortie choisie pour le log du calcul
	QString calc_log;
	/// indice de lopts
	int cmdx = 0;
	/// caractère du paramètre actuel
	int optch;
	/// Pour vérifier que le fichier de commande a été spécifié
	bool opt_cmdfile = false;
	
// Traitement de tous les paramètres de la ligne de comande
	while( (optch = getopt_long(argc, argv, stropts, lopts, &cmdx)) != EOF )
		switch(optch)
		{
		case 0: // Option déjà traitée
			stdlog->DEBUGMESS("On ne devrait jamais arriver ici.");
			break;
		case 'h': //-h ou --help
			stdlog->DEBUGMESS("Demande de message d'aide.");
			usage();
			break;
		case 'f': // -f ou --file
			stdlog->DEBUGMESS("Fichier de commande spécifié.");
			params->setCmdFileName(optarg);
			stdlog->DEBUGMESS(optarg);
			stdlog->DEBUGMESS(params->getCmdFileName());
			if(params->readCommandFile())
			{
				opt_cmdfile = true;
				stdlog->DEBUGMESS("Fichier de commande chargé.");
				break;
			}
			else
			{
				stdlog->message(LOGUTIL_CRITICAL, "Impossible de lire le fichier de commande.");
				quitter(20);
			}
		case 'l': // -l ou --log-to-file
			stdlog->DEBUGMESS("Demande de sortie log vers un fichier.");
			delete stdlog;
			stdlog = NULL;
			stdlog = new LogUtil(optarg, LOGUTIL_INFO);
			if( (stdlog == NULL) || (!stdlog->isReady()) )
			{
				cerr << "Impossible de réinitialiser le système de log !!" << endl;
				cerr << "Fin du programme" << endl << flush;
				quitter(12);
			}
			break;
		case 's': // -s ou --log-to-stdout
			stdlog->DEBUGMESS("Demande de sortie log vers la sortie standard.");
			delete stdlog;
			stdlog = NULL;
			stdlog = new LogUtil(LOG_TO_STDOUT, LOGUTIL_INFO);
			if( (stdlog == NULL) || (!stdlog->isReady()) )
			{
				cerr << "Impossible de réinitialiser le système de log !!" << endl;
				cerr << "Fin du programme" << endl << flush;
				quitter(13);
			}
			break;
		case 'e': // -e ou --log-to-stderr
			stdlog->DEBUGMESS("Demande de sortie log vers la sortie d'erreur.");
			// Rien à faire (comportement par défaut)
			break;
		case 'n':
			{
				int valeur;
				bool reussi;
				valeur = QString(optarg).toInt(&reussi);
				if( reussi && (valeur > 0) && ((valeur < 6) || (valeur == 10)) )
				{
					stdlog->setLogLevel(valeur);
					stdlog->DEBUGMESS("Niveau de log changé.");
				}
				else
				{
					stdlog->message(LOGUTIL_CRITICAL, "Niveau de log invalide");
					usage();
				}
			}
			break;
		default: // Option inconnue
			cout << "Option inconnue : " << (char)optch << endl;
			cout << "---" << endl;
			usage();
		}
// Fin du traitement des paramètres
	
// Traitement des arguments sur la ligne de commande
	if (optind < argc)
	{
		QString argus("La commande ne prend pas d'argument : \"");
		while (optind < argc)
		{
			argus += "  ";
			argus += argv[optind++];
		}
		argus += "\"  invalides!";
		stdlog->message( LOGUTIL_CRITICAL, argus );
		usage();
	}
	
	stdlog->message(LOGUTIL_INFO, "Paramètres de ligne de commande analysés.");

	// On vérifie que des paramètres ont bien été chargés
	if(! opt_cmdfile)
	{
		stdlog->message(LOGUTIL_CRITICAL, "Vous devez obligatoirement spécifier un fichier de commande.");
		usage();
	}
	
// Création du système de log des calculs
	calclog = new LogUtil(params->getLogFileName(), LOGUTIL_INFO);
	if( (calclog == NULL) || (!calclog->isReady()) )
	{
		stdlog->message(LOGUTIL_CRITICAL, "Impossible de charger le système de log du calcul.");
		quitter(14);
	}
	
	stdlog->message(LOGUTIL_INFO, "Système de log du calcul chargé.");
	
// Chargement de l'image initiale
	refFiles = new RubicFiles;
	refFiles->setVgiFileName(params->getVgiRefFileName());
	stdlog->DEBUGMESS(QString("Image Initiale : ") + params->getVgiRefFileName());
	if(! refFiles->setRawFileNameFromVgiFileName() )
	{
		stdlog->message(LOGUTIL_CRITICAL, "Impossible de créer le nom du fichier raw pour l'image initiale.");
		quitter(21);
	}
	
	if(! refFiles->checkFilesRead() )
	{
		stdlog->message(LOGUTIL_CRITICAL, "Impossible de lire les fichiers de l'image initiale.");
		quitter(22);
	}
	
	if(! refFiles->readSizeFromVgi() )
	{
		stdlog->message(LOGUTIL_CRITICAL, "Impossible de lire les dimensions de l'image initiale.");
		quitter(23);
	}
	
	if(! refFiles->readRawFile( &refImg ) )
	{
		stdlog->message(LOGUTIL_CRITICAL, "Erreur de lecture dans les fichiers de l'image initiale.");
		quitter(24);
	}

// 	#define TAILLEIM 50
// 	refImg = new RubicImage(TAILLEIM, TAILLEIM, TAILLEIM);
// 	for(long i = 0; i < TAILLEIM, i++)
// 		for(long j = 0; j < TAILLEIM, j++)
// 			for(long k = 0; k < TAILLEIM, k++)
// 			{
// 				unsigned char val = 


// Chargement de l'image déformée
	defFiles = new RubicFiles;
	defFiles->setVgiFileName(params->getVgiDefFileName());
	stdlog->DEBUGMESS(QString("ImageDeformée : ") + params->getVgiDefFileName());
	if(! defFiles->setRawFileNameFromVgiFileName() )
	{
		stdlog->message(LOGUTIL_CRITICAL, "Impossible de créer le nom du fichier raw pour l'image déformée.");
		quitter(25);
	}
	
	if(! defFiles->checkFilesRead() )
	{
		stdlog->message(LOGUTIL_CRITICAL, "Impossible de lire les fichiers de l'image déformée.");
		quitter(26);
	}
	
	if(! defFiles->readSizeFromVgi() )
	{
		stdlog->message(LOGUTIL_CRITICAL, "Impossible de lire les dimensions de l'image déformée.");
		quitter(27);
	}
	
	if(! defFiles->readRawFile( &defImg ) )
	{
		stdlog->message(LOGUTIL_CRITICAL, "Erreur de lecture dans les fichiers de l'image déformée.");
		quitter(28);
	}

	stdlog->message(LOGUTIL_INFO, "Fichiers image chargés.");
	
// Chargement des paramètres de calcul
	
	Lcur = params->getVoxernSize();
	Iv = params->getStartPointRef();
	// On vérifie que Lcur est pair, sinon on prend Lcur-1
	if( (Lcur % 2) != 0 )
	{
		Lcur -= 1;
		stdlog->message(LOGUTIL_WARNING, "Lcur doit être pair?? utilisation de la valeur " + QString::number(Lcur));
	}
	stdlog->message(LOGUTIL_DEBUG, "Taille d'un voxern : " + QString::number(Lcur));
	stdlog->message(LOGUTIL_DEBUG, "Iv : " + Iv.toString());
	
	// Chargement de U3 depuis le fichier puis mise dans U24
	DVect U3 = (DVect)( params->getStartPointDef() - params->getStartPointRef() );
	for(short i = 0; i < 3; i++)
		U24[i] = U3[i];
	
	NGMin = params->getNGMini();
	NGMax = params->getNGMaxi();
	defImg->setNGMin(NGMin);
	refImg->setNGMin(NGMin);
	defImg->setNGMax(NGMax);
	refImg->setNGMax(NGMax);
	stdlog->message(LOGUTIL_DEBUG, "Niveau de gris mini : " + QString::number(NGMin));
	stdlog->message(LOGUTIL_DEBUG, "Niveau de gris maxi : " + QString::number(NGMax));
	
	MaxThreads = params->getMaxThreads();
	stdlog->message(LOGUTIL_DEBUG, "Nombre de threads max : " + QString::number(MaxThreads));
	
	InterpolCubiq = params->getInterpolCubiq();
	stdlog->message(LOGUTIL_DEBUG, QString("Interpolation par splines cubiques : ") + (InterpolCubiq?"true":"false"));
	
	minZone = params->getMinCoord();
	maxZone = params->getMaxCoord();
	stdlog->message(LOGUTIL_DEBUG, "Mini zone : " + minZone.toString());
	stdlog->message(LOGUTIL_DEBUG, "Maxi zone : " + maxZone.toString());
	
	// On vérifie que la zone de calcul est sensée
	if( (minZone.getMin() < 0) || ((maxZone - minZone).getMin() < Lcur) 
	 || (maxZone.getX() > defImg->getXSize()) || (maxZone.getY() > defImg->getYSize())
	 || (maxZone.getZ() > defImg->getZSize()) )
	{
		stdlog->message(LOGUTIL_CRITICAL, "Erreur de zone de calcul");
		stdlog->message(LOGUTIL_DEBUG, minZone.toString());
		stdlog->message(LOGUTIL_DEBUG, maxZone.toString());
		stdlog->message(LOGUTIL_DEBUG, (maxZone-minZone).toString());
		quitter(50);
	}
	
	// On vérifie que les marges sur les bords de l'image sont suffisantes
	if( (minZone.getMin() < Lcur) || ((defImg->getSize()-maxZone).getMin() < Lcur) )
	{
		stdlog->message(LOGUTIL_CRITICAL, "Marges au bord de l'image insuffisantes");
		stdlog->message(LOGUTIL_DEBUG, minZone.toString());
		stdlog->message(LOGUTIL_DEBUG, maxZone.toString());
		stdlog->message(LOGUTIL_DEBUG, defImg->getSize().toString());
		quitter(51);
	}
	
	// On reprend le chargement des paramètres
	pasScanning = params->getPasScanning();
	tailleZoneScanning = params->getTailleZoneScanning();
	tailleZoneScanning = nearbyint(tailleZoneScanning / (2.0 * pasScanning)) * 2.0 * pasScanning;
	stdlog->message(LOGUTIL_DEBUG, "Pas du scanning : " + QString::number(pasScanning));
	stdlog->message(LOGUTIL_DEBUG, "Taille de la zone de scanning : " + QString::number(tailleZoneScanning));
	
	precGradConj = params->getPrecGradConj();
	precNewtonGradConj = params->getPrecNewtonGradConj();
	pasNewtonGradConj = params->getPasNewtonGradConj();
	stdlog->message(LOGUTIL_DEBUG, "Précision du gradient conjugué : " + QString::number(precGradConj));
	stdlog->message(LOGUTIL_DEBUG, "Précision du Newton du gradient conjugué : " + QString::number(precNewtonGradConj));
	stdlog->message(LOGUTIL_DEBUG, "Pas du Newton du gradient conjugué : " + QString::number(pasNewtonGradConj));
	
	maxitGC = params->getMaxitGradConj();
	maxitNewtonGC = params->getMaxitNewtonGradConj();
	stdlog->message(LOGUTIL_DEBUG, "Nb max d'itérations du gradient conjugué : " + QString::number(maxitGC));
	stdlog->message(LOGUTIL_DEBUG, "Nb max d'itérations du Newton du gradient conjugué : " + QString::number(maxitNewtonGC));
	
	ASCIIFileName = params->getAsciiOutputFileName();
	INPFileName = params->getInpOutputFileName();
	stdlog->message(LOGUTIL_DEBUG, "Nom du fichier de sortie ASCII : " + ASCIIFileName);
	stdlog->message(LOGUTIL_DEBUG, "Nom du fichier de sortie INP : " + INPFileName);
	if( (ASCIIFileName == "") || (INPFileName == "") )
	{
		stdlog->message(LOGUTIL_CRITICAL, "Vous devez préciser les noms des deux fichiers de sortie.");
		quitter(52);
	}
	
// Fin du chargement des paramètres
	stdlog->message(LOGUTIL_INFO, "Paramètres de calcul chargés.");
	
// Calcul du nombre de voxerns à calculer dans chcaque dimension.
	LVect reste = (maxZone - minZone) % Lcur;
	LVect tailleCalcul = (maxZone - minZone) - reste;
	nbVoxerns = tailleCalcul / Lcur;
	stdlog->message(LOGUTIL_INFO, "Dimensions des résultat : " + nbVoxerns.toString());
	
	// Calcul des coordonnées initiales en voxern
	Ivoxern = (Iv - minZone) / Lcur;
	stdlog->message(LOGUTIL_DEBUG, "Ivoxern : " + Ivoxern.toString());
	
	// On vérifie que Ivoxern est dans la zone de calcul
	if( (Ivoxern.getMin() < 0) || (Ivoxern.getX() >= nbVoxerns.getX())
	 || (Ivoxern.getY() >= nbVoxerns.getY()) || (Ivoxern.getZ() >= nbVoxerns.getZ()) )
	{
		stdlog->DEBUGMESS("Point de départ Ivoxern : " + Ivoxern.toString());
		stdlog->message(LOGUTIL_CRITICAL, "Le point de départ n'est pas dans la zone de calcul");
		quitter(15);
	}
	
// Création du stockage des résultats de calcul
 	// Allocation mémoire
 	resultats = new RubicResults(nbVoxerns);
	if(resultats == NULL)
	{
		stdlog->message(LOGUTIL_CRITICAL, "Impossible de créer l'objet résultats");
		quitter(16);
	}
	// Assignation des coordonnées
	LVect pos;
	for(pos[0] = 0; pos[0] < nbVoxerns[0]; pos[0]++)
		for(pos[1] = 0; pos[1] < nbVoxerns[1]; pos[1]++)
			for(pos[2] = 0; pos[2] < nbVoxerns[2]; pos[2]++)
				(*resultats)[pos]->Coord = minZone + Lcur * pos;
		
	stdlog->message(LOGUTIL_INFO, "Objet de stockage des résultats chargé");
	
	
// Lancement du calcul
	// Scanning initial
	if( ! executeScanning() )
	{
		stdlog->message(LOGUTIL_CRITICAL, "Erreur dans le scanning initial");
		quitter(70);
	}
	stdlog->message(LOGUTIL_INFO, "Scanning initial terminé.");
	
	// Propagation du calcul (la boucle principale du programme)
	if( ! executePropagation() )
	{
		stdlog->message(LOGUTIL_CRITICAL, "Erreur dans le calcul principal");
		quitter(71);
	}
	
	// Stockage des résultats
	if( ! resultats->writeToASCII(ASCIIFileName) )
	{
		stdlog->message(LOGUTIL_CRITICAL, "Erreur dans l'écriture du fichier ASCII");
	}
	
	if(! resultats->writeToINP(INPFileName, Lcur) )
	{
		stdlog->message(LOGUTIL_CRITICAL, "Erreur dans l'écriture du fichier INP Abaqus");
	}
	
	quitter(0);
}
