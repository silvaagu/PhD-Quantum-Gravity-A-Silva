#include <iostream>

using namespace std;

#include "funcionsuma.hpp"
#include "pool.hpp"
#include "bag.hpp"
#include "config.hpp"
#include "vertex.hpp"
#include "triangle.hpp"
#include "link.hpp"
#include "universe.hpp"
#include "observable.hpp"
#include "simulation.hpp"
#include "observables/volume_profile.hpp"
#include "observables/hausdorff.hpp"
#include "observables/hausdorff_dual.hpp"
#include "observables/ricci.hpp"
#include "observables/ricci_dual.hpp"
#include "observables/riccih.hpp"
#include "observables/ricciv.hpp"
#include "observables/ricci_hist.hpp"
#include "observables/ricci_int.hpp"
#include "observables/ricci_dual_int.hpp"
#include "observables/maxcnum.hpp"
#include "observables/riccicorrelator.hpp"
#include "observables/IsotropyMeasureCurvature.hpp"
#include "observables/IsotropyMeasureVolume.hpp"
#include "observables/IsotropyMeasureCurvatureSLBounds.hpp"
#include "observables/IsotropyMeasureCurvatureSLBoundsDual.hpp"
#include "observables/IsotropyMeasureCurvatureDual.hpp"
#include "observables/HomogeneityMeasureCoordinationNumber.hpp"



int main(int argc, const char * argv[]) {
    cout<<"Introduce the file path of the file that determines the Configuration, after doing so, press ENTER"<<endl;
    std::string fname;
    getline(cin,fname);
//	if (argc > 1) {
//		fname = std::string(argv[1]);
//		printf("%s\n", fname.c_str());
//	}
	ConfigReader cfr;
	cfr.read(fname);



    double lambda = cfr.getDouble("lambda");
	int targetVolume = cfr.getInt("targetVolume");
	int slices = cfr.getInt("slices");
	std::string sphereString = cfr.getString("sphere");
	if (sphereString == "true") {
		Universe::sphere = true;
		printf("sphere\n");
	}

	int seed = cfr.getInt("seed");
	std::string fID = cfr.getString("fileID");
	int measurements = cfr.getInt("measurements");
	std::string geomIn = cfr.getString("geom_in");
	std::string geomOut = cfr.getString("geom_out");
	bool impGeom = false;
	if (geomIn != "0") impGeom = true;

//    if (impGeom) {
//		Universe::importGeometry(geomIn); //The definition of this function is missing, it should be inside universe.cpp
//	} else {
//		Universe::create(slices);
//	}

    Universe::create(slices); //I added this instead of the las commented block to see if it worked

	printf("initialized.\n");

//	VolumeProfile vp(fID);
//	Simulation::addObservable(vp);

//	MaxCNum maxcnum(fID);
//	Simulation::addObservable(maxcnum);

//	Hausdorff haus(fID);
//	Simulation::addObservable(haus);

//	int ricMax = ((slices-2)/6); // numero maximo para el radio de las esferas para medir la curvatura de ricci
////	int ricMax = (2);
//	std::vector<int> vric(ricMax);
//	for (int i = 0; i < ricMax; i++) {
//		vric.at(i) = i + 1;
//	}
//	Ricci ric(fID, vric);
//	Simulation::addObservable(ric);

    ////////////ACA MIDO ISOTROPÍA EN LA TRIANGULACION////////////////
//	int ricMax = 15; // numero maximo para el radio de las esferas para medir la curvatura de ricci
//	std::vector<int> vric(ricMax);
//	for (int i = 0; i < ricMax; i++) {
//		vric.at(i) = i + 1;
//	}
//	IsotropyCurvatureSLBounds isocurvSLB(fID, vric);
//	Simulation::addObservable(isocurvSLB);

//	//ahora uso lo mismo para medir los la isotropía a partir de las areas de esferas
//    IsotropyVolume isovol(fID, vric);
//	Simulation::addObservable(isovol);
    ////////////////////// FIN ISOTROPIA TRIANGULACION ////////////////////////

    ////////// ACA MIDO ISOTROPÍA PERO CON DISTANCIAMIENTO DUAL /////////////////
//    int ricMaxdual = ((slices-2)/6); // numero maximo para el radio de las esferas para medir la curvatura de ricci
////	int ricMax = (2);
//	std::vector<int> vricdual(ricMaxdual);
//	for (int i = 0; i < ricMaxdual; i++) {
//		vricdual.at(i) = i + 1;
//	}
//
//	IsotropyCurvatureDual isocurvDual(fID, vricdual);
//	Simulation::addObservable(isocurvDual);

//	IsotropyCurvatureSLBoundsDual isocurvSLBDual(fID, vric);
//	Simulation::addObservable(isocurvSLBDual);


////	int radmax=20; //for totalvolume 9000 and slices 40
////	int distmax=2*radmax+1; // for totalvolume 9000 and slices 40
//	int radmax=20;
//	int distmax=100;
//	std::vector<int> radius(radmax);
//	std::vector<int> distancepoints(distmax);
//	for (int i = 0; i < radmax; i++) {
//		radius.at(i) = i + 1;
//	}
//	for (int i = 0; i < distmax; i++) {
//		distancepoints.at(i) = i; // arranco de distancia cero
//	}
//	RicciCorrelator riccor(fID, radius,distancepoints);
//	Simulation::addObservable(riccor);

/////////////// FIN ISOTROPIA DUAL //////////////////////

/////////////////////// ACA MIDO HOMOGENEIDAD EN LA TRIANGULAICON /////////////////////

    int radmax = ((slices-2)/4); // numero maximo para el radio de las esferas para medir homogeneity
//	int ricMax = (2);
	std::vector<int> vrad(radmax);
	for (int i = 0; i < radmax; i++) {
		vrad.at(i) = i + 1;
	}

	HomogeneityCoordinationNumber HomoCoord(fID, vrad);
	Simulation::addObservable(HomoCoord);

////////////// FIN HOMOGENEIDAD EN LA TRIANGUALCION ///////////////



	//ric.clear();
	//Simulation::prepare();
	//for (int i = 0; i < 5000; i++) {
	//	ric.measure();
	//}
	//exit(0);


	//int ricDMax = 60;
	//std::vector<int> vricd(ricDMax);
	//for (int i = 0; i < ricDMax; i++) {
	//	vricd.at(i) = i + 1;
	//}
	//RicciDual ricd(fID, vricd);
	//Simulation::addObservable(ricd);

	//int richMax = 40;
	//std::vector<int> vrich(richMax);
	//for (int i = 0; i < richMax; i++) {
	//	vrich.at(i) = i + 1;
	//}
	//RicciH rich(fID, vrich);
	//Simulation::addObservable(rich);

	//int ricvMax = 40;
	//std::vector<int> vricv(ricvMax);
	//for (int i = 0; i < ricvMax; i++) {
	//	vricv.at(i) = i + 1;
	//}
	//RicciV ricv(fID, vricv);
	//Simulation::addObservable(ricv);

	//RicciHist richist(fID);
	//Simulation::addObservable(richist);

	//richist.clear();
	//Simulation::prepare();
	//for (int i = 0; i < 5000; i++) {
	//	richist.measure();
	//}
	//exit(0);

	//int riciMax = 30;
	//std::vector<int> vrici(riciMax);
	//for (int i = 0; i < riciMax; i++) {
	//	vrici.at(i) = i + 1;
	//}
	//RicciInt rici(fID, vrici);
	//Simulation::addObservable(rici);

	//rici.clear();
	//Simulation::prepare();
	//for (int i = 0; i < 1; i++) {
	//	rici.measure();
	//}
	//exit(0);

//	int ricdiMax = 40;
//	std::vector<int> vricdi(ricdiMax);
//	for (int i = 0; i < ricdiMax; i++) {
//		vricdi.at(i) = i + 1;
//	}
//	RicciDualInt ricdi(fID, vricdi);
//	Simulation::addObservable(ricdi);
//	cout<<"pase"<<endl;
//	ricdi.clear();
//	cout<<"pase"<<endl;
//	Simulation::prepare();
//	for (int i = 0; i < 3; i++) {
//		ricdi.measure();
//	}
//	exit(0);


	printf("seed: %d\n", seed);

	try {
		Simulation::start(measurements, lambda, targetVolume, seed, geomOut);
	} catch(std::exception & e) {
		std::cout << "exception: " << e.what() << std::endl;
		exit(1);
	}
	printf("end\n");
	return 0;
}
