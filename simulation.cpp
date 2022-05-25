// Copyright 2020 Joren Brunekreef and Andrzej Görlich
// Copyright 2020 Joren Brunekreef and Andrzej Görlich
#include "simulation.hpp"
#include <vector>

std::default_random_engine Simulation::rng(0);  // TODO(JorenB): seed properly
int Simulation::targetVolume = 0;
double Simulation::lambda = 0;
int Simulation::seed = 0;
std::string Simulation::geomOut = "";
double Simulation::epsilon = 0.02;
std::vector<Observable*> Simulation::observables;

std::array<int, 2> Simulation::moveFreqs = {1, 1};

void Simulation::start(int measurements, double lambda_, int targetVolume_, int seed_, std::string geomOut_) {
	targetVolume = targetVolume_;
	lambda = lambda_;

	for (auto o : observables) {
		o->clear();
	}

	seed = seed_;

	rng.seed(seed);

	geomOut = geomOut_;
	// tune();

	if (!Universe::imported) {
		grow();
		thermalize();
	}

	for (int i = 1; i <= measurements; i++) {
		sweep(); // hace un barrido por cada iteracion (basicamente hace una cantidad "measurements " de simulaicones)
		printf("m %d\n", i);
		if (i % 50 == 0) Universe::exportGeometry(geomOut); // solo imprime cada 50 mediciones, por que ?????
		fflush(stdout); // esto me parece que es para limpiar el buffer (intermediaro entre el programa y el dispositivo de salida de datos), pero no estoy seguro de que funcione con todos los compiladores, y no entiendo por que lo hace
	}
}

int Simulation::attemptMove() {
	std::array<int, 2> cumFreqs = {0, 0};
	int freqTotal = 0;
	int prevCumFreq = 0;
	for (auto i = 0u; i < moveFreqs.size(); i++) {
		freqTotal += moveFreqs[i];
		cumFreqs[i] = prevCumFreq + moveFreqs[i];
		prevCumFreq = cumFreqs[i];
	}

	std::uniform_int_distribution<> moveGen(0, freqTotal-1);
	std::uniform_int_distribution<> binGen(0, 1);

	int move = moveGen(rng);

	if (move < cumFreqs[0]) {
		if (binGen(rng) == 0) {
			if (moveAdd()) return 1;
		} else {
			if (moveDelete()) return 2;
		}
	} else if (cumFreqs[0] <= move) {
		if (moveFlip()) return 3;
	}

	return 0;
}

void Simulation::sweep() {
	std::uniform_int_distribution<> uniform_int(0, 3); // distribucion uniforme entera entre 0 y 3

	std::array<int, 4> moves = {0, 0, 0, 0}; //cuenta cada una de las 4 movidas posibles

	if (targetVolume > 0) {
		for (int i = 0; i < 1000 * targetVolume; i++) { // hace 1000*targetvolume intentos de movida
			moves[attemptMove()]++;
		} // hace esto como para arrancar, luego hace mas movidas solo para llegar al volumen Target Volume

		do {
			attemptMove();
		} while (Triangle::size() != targetVolume); // solo termina de hacer movidas cuando el volumen es exactamente el volumen que se desea (target volume)
	} else {
		for (int i = 0; i < 1000 * 50000; i++) { // en el caso de que el target volume esté mal puesto, sea negativo, entonces le manda 50000*1000 movidas
			moves[attemptMove()]++;
		}
	}

	prepare(); // actualiza toda la data del universo antes de hacer una medición
	for (auto o : observables) {
		o->measure(); // mide cada uno de los observables
	}
}


bool Simulation::moveAdd() {
	double n0 = Vertex::size();
	// double n2 = Triangle::size();
	double n0_four = Universe::verticesFour.size();

	// double edS = exp(- 2 * lambda);
	// double rg = n2 / (n2 + 2.0);
	// double ar = edS * rg;

	double ar = n0 / (n0_four + 1.0) * exp(- 2 * lambda);
	if (targetVolume > 0) {
		double expesp = exp(2 * epsilon);
		ar *= Triangle::size() < targetVolume ? expesp : 1 / expesp; // si se supera el volumen esperado, se hace menos probalbe que se agregue un nuevo vertice
	}

	Triangle::Label t = Universe::trianglesAll.pick();

	if (Universe::sphere) {
		if (t->time == 0) return false;
	}

	if (ar < 1.0) {
		std::uniform_real_distribution<> uniform(0.0, 1.0);
		double r = uniform(rng);
		if (r > ar) return false;
	}

	Universe::insertVertex(t);
	return true;
}

bool Simulation::moveDelete() {
	if (Universe::verticesFour.size() == 0) return false;

	double n0 = Vertex::size();
	// double n2 = Triangle::size();
	double n0_four = Universe::verticesFour.size();

	// double edS = exp(2 * lambda);
	// double rg = n2 / (n2 - 2.0);
	// double ar = edS * rg;

	double ar = n0_four / (n0 - 1.0) * exp(2 * lambda); // notar el cambio de signo en la exponencial al scar un vértice
	if (targetVolume > 0) {
		double expesp = exp(2 * epsilon);
		ar *= Triangle::size() < targetVolume ? 1 / expesp : expesp; // Si el volmen es mayor que el volumen el el target, hace ams probable que eliminen un vèrtice
	}

	if (ar < 1.0) {
		std::uniform_real_distribution<> uniform(0.0, 1.0);
		double r = uniform(rng);
		if (r > ar) return false;
	}

	Vertex::Label v = Universe::verticesFour.pick();
	// auto t = Universe::trianglesAll.pick();
	// auto v = t->getVertexLeft();
	// if (v->nUp + v->nDown != 4) return false;

	if (Universe::sliceSizes[v->time] < 4) return false;  // reject moves that shrink slices below size 3

	Universe::removeVertex(v);

	return true;
}

bool Simulation::moveFlip() {
	if (Universe::trianglesFlip.size() == 0) return false;

	auto t = Universe::trianglesFlip.pick(); // auto es una fincion para declarar una variable que adivina el tipo cuando se le hace una asignacion

	int wa = Universe::trianglesFlip.size();
	int wb = wa;
	if (t->type == t->getTriangleLeft()->type) {
		wb++;
	} else {
		wb--;
	}

	if (t->getTriangleRight()->type == t->getTriangleRight()->getTriangleRight()->type) {
	   	wb++;
	} else {
		wb--;
	}

	double ar = 1.0*wa/wb; // creo que el 1.0 es una forma de castear a double el cociente de enteros

	if (ar < 1.0) {
		std::uniform_real_distribution<> uniform(0.0, 1.0);
		double r = uniform(rng);
		if (r > ar) return false;
	}

	Universe::flipLink(t);

	return true;
}

void Simulation::prepare() {
	Universe::updateVertexData();
	Universe::updateTriangleData();
	Universe::updateLinkData();
}

#if 0  // tuning isn't used in the current setup
void Simulation::tune() {
	printf("start tune..\n");
	fflush(stdout);
	std::vector<int> volumes;
	epsilon = 0.02;

	bool done = false;
	int tuneSteps = 50;
	for (int k = 0; k < tuneSteps && !done; k++) {
		for (int i = 0; i < targetVolume; i++) {
			for (int j = 0; j < 100; j++) attemptMove();

			volumes.push_back(Triangle::size());
		}

		double avg = 0.0;
		for (auto v : volumes) avg += static_cast<double>(v);
		avg /= volumes.size();

		double sd = 0.0;
		for (auto v : volumes) sd += (static_cast<double>(v) - avg)*(static_cast<double>(v) - avg);
		sd /= volumes.size();

		if ((targetVolume - avg)*(targetVolume - avg) < 2*sd) {
			epsilon *= 0.7;
			if (epsilon < 0.02) {
				epsilon = 0.02;
				lambda -= 0.003 * (targetVolume - avg) / sqrt(sd);
			}
		} else if ((targetVolume - avg)*(targetVolume - avg) > 8*sd) {
			epsilon *= 1.2;
			if (epsilon > 5.0) epsilon = 5.0;
		} else if ((targetVolume - avg)*(targetVolume - avg) < 0.04*targetVolume*targetVolume) {
			lambda += 0.6*(avg - targetVolume)/abs((avg-targetVolume)) * epsilon;
		}
		volumes.clear();
		if (k >= tuneSteps && abs(avg-targetVolume) < 0.1*targetVolume && epsilon < 0.021) done = true;

		printf("step %d - epsilon: %f, lambda: %f, avg: %f, sd: %f\n", k, epsilon, lambda, avg, sd);
	}
}
#endif

void Simulation::grow() { // hace movimidas hasta que el sistema crece al Target volume o mas
	int growSteps = 0;
	printf("growing");
	do {
		for (int i = 0; i < targetVolume; i++) attemptMove(); // hace tantas movidas como el Target Volume para que crezca
		printf("."); // esto significa que el sistema esta creciendo, en cada uno de los pasos
		fflush(stdout);
		growSteps++;
	} while (Triangle::size() < targetVolume);
	printf("\n");
	printf("grown in %d sweeps\n", growSteps);
}

void Simulation::thermalize() {
	int thermSteps = 0;
	printf("thermalizing");
	fflush(stdout);
	do {
		if (targetVolume > 0) {
			for (int i = 0; i < 1000 * targetVolume; i++) attemptMove();
		} else {
			for (int i = 0; i < 100 * 50000; i++) attemptMove();
		}
		printf(".");
		fflush(stdout);

		prepare();

		if (thermSteps % 50 == 0) Universe::exportGeometry(geomOut); // exporta geometría cada 50 pasos de termalización

		for (auto o : observables) {
			if (o->name == "maxcnum" || o->name == "volume_profile") o->measure();
		}

		thermSteps++;
	} while (thermSteps < 300); // las primeras 301 lineas (multiplicadas por las lineas que de cada medicion) en el archivo de un observable son datos de termailización
	//} while (thermSteps < 10);
	printf("\n");
	printf("thermalized for %d sweeps\n", thermSteps);
}
