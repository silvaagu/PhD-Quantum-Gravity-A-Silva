// Copyright 2020 Joren Brunekreef and Andrzej G?rlich
#include "universe.hpp"

int Universe::nSlices = 0;
int Universe::totalVolume = 0;
std::vector<int> Universe::sliceSizes;
bool Universe::sphere = false;
bool Universe::imported = false;
std::default_random_engine Universe::rng(0);  // TODO(JorenB): set seed somewhere else
Bag<Triangle, Triangle::pool_size> Universe::trianglesAll(rng);
Bag<Vertex, Vertex::pool_size> Universe::verticesFour(rng);
Bag<Triangle, Triangle::pool_size> Universe::trianglesFlip(rng);

std::vector<Vertex::Label> Universe::vertices;
std::vector<Link::Label> Universe::links;
std::vector<Triangle::Label> Universe::triangles;
std::vector<std::vector<Vertex::Label>> Universe::vertexNeighbors;
std::vector<std::vector<Triangle::Label>> Universe::triangleNeighbors;
std::vector<std::vector<Link::Label>> Universe::vertexLinks;
std::vector<std::vector<Link::Label>> Universe::triangleLinks;

void Universe::create(int nSlices_,int totalVolume_) {
	nSlices = nSlices_;
	totalVolume = totalVolume_;
	initialize();
}

void Universe::initialize() {
//	int w = 800/nSlices;  // width of the initial strip. Can be adjusted for thermalization purposes - unclear what the 'optimal' value is.
//	int w = 10; // ancho inicial para pruebas peque?as
    int w = totalVolume/nSlices; // I think the ideal number to start is the target volume divided by the number of slices
	int t = nSlices;

	std::vector<Vertex::Label> initialVertices(w*t);

	for (int i = 0; i < w*t; i++) {
		auto v = Vertex::create();
		v->time = i / w;
		initialVertices[i] = v;
	}

	for (int i = 0; i < t; i++) {
		sliceSizes.push_back(w);
	}

	// create triangles
	std::vector<Triangle::Label> initialTriangles(2*w*t);
	for(int i = 0; i < t; i++) {
		for(int j = 0; j < w; j++) {
			auto tl = Triangle::create();
			tl->setVertices(
					initialVertices[i*w+j],
					initialVertices[i*w+(j+1)%w],
					initialVertices[((i+1)%t)*w+j]);
			initialTriangles[2*(i*w+j)] = tl;

			auto tr = Triangle::create();
			tr->setVertices(
					initialVertices[((i+1)%t)*w+j],
					initialVertices[((i+1)%t)*w+(j+1)%w],
					initialVertices[i*w+(j+1)%w]);
			initialTriangles[2*(i*w+j)+1] = tr;
			trianglesAll.add(tl);
			trianglesAll.add(tr);

			trianglesFlip.add(tl);
			trianglesFlip.add(tr);
		}
	}

	// set triangle connectivity
	int row = 0, column = 0;
	for(int i = 0; i < t; ++i) {
		for(int j = 0; j < w; ++j) {
			row = 2*i*w;
			column = 2*j;
			initialTriangles[row + column]->setTriangles(
					initialTriangles[row + (column-1+2*w)%(2*w)],
					initialTriangles[row + column + 1],
					initialTriangles[(row + column -2*w+1+2*t*w)%(2*t*w)]);

			initialTriangles[row + column +1]->setTriangles(
					initialTriangles[row + column],
					initialTriangles[row + (column+2)%(2*w)],
					initialTriangles[(row + column + 2*w)%(2*t*w)]);
		}
	}
}


void Universe::insertVertex(Triangle::Label t) {
	Triangle::Label tc = t->getTriangleCenter();

	Vertex::Label vr = t->getVertexRight();

	int time = t->time;

	Vertex::Label v = Vertex::create();
	v->time = time;
	verticesFour.add(v);
	sliceSizes[time] += 1;

	t->setVertexRight(v);
	tc->setVertexRight(v);

	Triangle::Label t1 = Triangle::create();  // right neighbor of t
	Triangle::Label t2 = Triangle::create();  // right neighbor of tc
	trianglesAll.add(t1);
	trianglesAll.add(t2);

	t1->setVertices(v, vr, t->getVertexCenter());
	t2->setVertices(v, vr, tc->getVertexCenter());

	t1->setTriangles(t, t->getTriangleRight(), t2);
	t2->setTriangles(tc, tc->getTriangleRight(), t1);

	if (t1->type != t1->getTriangleRight()->type) {
		trianglesFlip.remove(t);
		trianglesFlip.add(t1);
	}

	if (t2->type != t2->getTriangleRight()->type) {
		trianglesFlip.remove(tc);
		trianglesFlip.add(t2);
	}
}

void Universe::removeVertex(Vertex::Label v) {
	Triangle::Label tl = v->getTriangleLeft();
	Triangle::Label tr = v->getTriangleRight();
	Triangle::Label tlc = tl->getTriangleCenter();
	Triangle::Label trc = tr->getTriangleCenter();

	Triangle::Label trn = tr->getTriangleRight();
	Triangle::Label trcn = trc->getTriangleRight();

	tl->setTriangleRight(trn);
	tlc->setTriangleRight(trcn);

	tl->setVertexRight(tr->getVertexRight());
	tlc->setVertexRight(tr->getVertexRight());

	tr->getVertexRight()->setTriangleLeft(tl);

	sliceSizes[v->time] -= 1;

	trianglesAll.remove(tr);
	trianglesAll.remove(trc);
	if (trianglesFlip.contains(tr)) {
		trianglesFlip.remove(tr);
		trianglesFlip.add(tl);
	}
	if (trianglesFlip.contains(trc)) {
		trianglesFlip.remove(trc);
		trianglesFlip.add(tlc);
	}

	Triangle::destroy(tr);
	Triangle::destroy(trc);

	verticesFour.remove(v);
	Vertex::destroy(v);
}

void Universe::flipLink(Triangle::Label t) {
	auto tr = t->getTriangleRight();
	auto tc = t->getTriangleCenter();
	auto trc = tr->getTriangleCenter();

	if (t->isUpwards()) {
		t->getVertexLeft()->setTriangleRight(tr);
		t->getVertexRight()->setTriangleLeft(tr);
	} else if (t->isDownwards()) {
		tr->getVertexLeft()->setTriangleRight(t);
		tr->getVertexRight()->setTriangleLeft(t);
	}

	t->setTriangleCenter(trc);
	tr->setTriangleCenter(tc);

	auto vl = t->getVertexLeft();
	auto vr = t->getVertexRight();
	auto vc = t->getVertexCenter();
	auto vrr = tr->getVertexRight();

	t->setVertices(vc, vrr, vl);
	tr->setVertices(vl, vr, vrr);

	if (verticesFour.contains(vl)) verticesFour.remove(vl);
	if (isFourVertex(vr)) verticesFour.add(vr);
	if (isFourVertex(vc)) verticesFour.add(vc);
	if (verticesFour.contains(vrr)) verticesFour.remove(vrr);

	if (trianglesFlip.contains(t->getTriangleLeft()) && (t->type == t->getTriangleLeft()->type)) trianglesFlip.remove(t->getTriangleLeft());
	if (trianglesFlip.contains(tr) && (tr->type == tr->getTriangleRight()->type)) trianglesFlip.remove(tr);

	if ((!trianglesFlip.contains(t->getTriangleLeft())) && (t->type != t->getTriangleLeft()->type)) trianglesFlip.add(t->getTriangleLeft());
	if ((!trianglesFlip.contains(tr)) && (tr->type != tr->getTriangleRight()->type)) trianglesFlip.add(tr);
}

bool Universe::isFourVertex(Vertex::Label v) {
	return (v->getTriangleLeft()->getTriangleRight() == v->getTriangleRight())
				&& (v->getTriangleLeft()->getTriangleCenter()->getTriangleRight() == v->getTriangleRight()->getTriangleCenter());
}

void Universe::check() {
	for (auto t : trianglesAll) {
		assert(t->getTriangleLeft() >= 0);
		assert(t->getTriangleRight() >= 0);
		assert(t->getTriangleCenter() >= 0);

		assert(t->getVertexLeft() >= 0);
		assert(t->getVertexRight() >= 0);
		assert(t->getVertexCenter() >= 0);

		if (trianglesFlip.contains(t)) {
			assert(t->type != t->getTriangleRight()->type);
		} else if (!trianglesFlip.contains(t)) {
			assert(t->type == t->getTriangleRight()->type);
		}
	}

	for (auto t : trianglesAll) {
		if (t->isDownwards()) continue;

		auto v = t->getVertexLeft();


		int nu = 1;
		auto tl = v->getTriangleLeft();
		while (tl->getTriangleRight() != v->getTriangleRight()) {
			tl = tl->getTriangleRight();
			nu++;
		}
		nu++;

		int nd = 1;
		tl = v->getTriangleLeft()->getTriangleCenter();
		while (tl->getTriangleRight() != v->getTriangleRight()->getTriangleCenter()) {
			tl = tl->getTriangleRight();
			nd++;
		}
		nd++;

		if (nu + nd == 4) {
			assert(Universe::verticesFour.contains(v));
		} else {
			assert(!Universe::verticesFour.contains(v));
		}
	}

	for (auto v : verticesFour) {
		auto tl = v->getTriangleLeft();
		auto tr = v->getTriangleRight();

		assert(tl->getTriangleRight() == tr);
		assert(tr->getTriangleLeft() == tl);
	}
}

void Universe::updateVertexData() {
	vertices.clear();
	int max = 0;
	for (auto t : trianglesAll) {
		if (t->isUpwards()) {
			auto v = t->getVertexLeft();
			vertices.push_back(v);
			if (v > max) max = v;
		}
	}

	vertexNeighbors.clear();
	vertexNeighbors.resize(max+1);
	for (auto v : vertices) {
		if (sphere) {
			if (v->time == 0) {
				auto tl = v->getTriangleLeft();
				Triangle::Label tn = tl;
				do {
					vertexNeighbors.at(v).push_back(tn->getVertexLeft());
					tn = tn->getTriangleRight();
				} while (tn->isDownwards());
				vertexNeighbors.at(v).push_back(tn->getVertexCenter());
				vertexNeighbors.at(v).push_back(tn->getVertexRight());

				continue;
			} else if (v->time == nSlices-1) {
				auto tld = v->getTriangleLeft()->getTriangleCenter();
				auto tn = tld;
				do {
					vertexNeighbors.at(v).push_back(tn->getVertexLeft());
					tn = tn->getTriangleRight();
				} while (tn->isUpwards());
				vertexNeighbors.at(v).push_back(tn->getVertexCenter());
				vertexNeighbors.at(v).push_back(tn->getVertexRight());

				continue;
			}
		}

		auto tl = v->getTriangleLeft();
		Triangle::Label tn = tl;
		do {
			vertexNeighbors.at(v).push_back(tn->getVertexLeft());
			tn = tn->getTriangleRight();
		} while (tn->isDownwards());
		vertexNeighbors.at(v).push_back(tn->getVertexCenter());
		vertexNeighbors.at(v).push_back(tn->getVertexRight());

		tn = tn->getTriangleCenter()->getTriangleLeft();
		while (tn->isUpwards()) {
			vertexNeighbors.at(v).push_back(tn->getVertexRight());
			tn = tn->getTriangleLeft();
		}
		vertexNeighbors.at(v).push_back(tn->getVertexCenter());
	}
}

void Universe::updateLinkData() {
	for (auto l : links) {
		Link::destroy(l);
	}

	links.clear();
	int max = 0;

	vertexLinks.clear();
	for (auto i = 0u; i < vertexNeighbors.size(); i++) {
		vertexLinks.push_back({});
	}
	triangleLinks.clear();
	for (auto i = 0u; i < triangleNeighbors.size(); i++) {
		triangleLinks.push_back({-1, -1, -1});
	}

	for (auto t : trianglesAll) {
		auto ll = Link::create();  // left link
		if (t->isUpwards()) ll->setVertices(t->getVertexLeft(), t->getVertexCenter());
		else if (t->isDownwards()) ll->setVertices(t->getVertexCenter(), t->getVertexLeft());
		ll->setTriangles(t->getTriangleLeft(), t);

		vertexLinks.at(t->getVertexLeft()).push_back(ll);
		vertexLinks.at(t->getVertexCenter()).push_back(ll);

		triangleLinks.at(t).at(0) = ll;
		triangleLinks.at(t->getTriangleLeft()).at(1) = ll;
		links.push_back(ll);
		if (ll > max) max = ll;
		if (t->isUpwards()) {
			auto lh = Link::create();  // horizontal link
			lh->setVertices(t->getVertexLeft(), t->getVertexRight());
			lh->setTriangles(t, t->getTriangleCenter());

			vertexLinks.at(t->getVertexLeft()).push_back(lh);
			vertexLinks.at(t->getVertexRight()).push_back(lh);

			triangleLinks.at(t).at(2) = lh;
			triangleLinks.at(t->getTriangleCenter()).at(2) = lh;

			links.push_back(lh);
			if (lh > max) max = lh;
		}
	}

	assert(links.size() == 3*vertices.size());
}

void Universe::updateTriangleData() {
	triangles.clear();
	int max = 0;
	for (auto t : trianglesAll) {
		triangles.push_back(t);

		if (t > max) max = t;
	}

	triangleNeighbors.clear();
	triangleNeighbors.resize(max+1);
	for (auto t : trianglesAll) {
		if (sphere) {
			if (t->isUpwards() && t->time == 0) {
				triangleNeighbors.at(t) = {t->getTriangleLeft(), t->getTriangleRight()};
				continue;
			}
			if (t->isDownwards() && t->time == nSlices-1) {
				triangleNeighbors.at(t) = {t->getTriangleLeft(), t->getTriangleRight()};
				continue;
			}
		}

		triangleNeighbors.at(t) = {t->getTriangleLeft(), t->getTriangleRight(), t->getTriangleCenter()};
	}
}

void Universe::exportGeometry(std::string fname) {
	std::unordered_map<int, int> vertexMap;
	std::vector<Vertex::Label> intVMap;
	intVMap.resize(vertices.size());

	int i = 0;
	for (auto v : vertices) {
		vertexMap.insert({v, i});
		intVMap.at(i) = v;
		i++;
	}

	std::unordered_map<int, int> triangleMap;
	std::vector<Triangle::Label> intTMap;
	intTMap.resize(triangles.size()); // le da el tama?o de la cantidad de triangulos que tiene la triangulacion

	i = 0;
	for (auto t : triangles) {
		triangleMap.insert({t, i});
		intTMap.at(i) = t; // llena el mapa de triangulos, un valor del mapa es el indice, y el otro es el triangulo en si
		i++;
	}

	std::string output;

	output += std::to_string(vertices.size()); // pone la cantidad de v?rtices como el principio del string
	output += "\n"; // salto de linea

	for (int j = 0; j < intVMap.size(); j++) {
		output += std::to_string(intVMap.at(j)->time); // llena el tiempo de cada v?rtice (del 0 al verticices.size-1)
		output += "\n"; // todos separados con un salto de linea
	}

	output += std::to_string(vertices.size()); // vuelve a poner la cantidad de v?rtices
	output += "\n"; // salto de linea
	output += std::to_string(triangles.size()); // pone la cantidad de triangulos
	output += "\n"; // salto de linea

	for (int j = 0; j < intTMap.size(); j++) { // hace un for por cada uno de los triangulos en la triangulacion
		Triangle::Label t = intTMap.at(j); // pone en t un puntero a cada triangulo que est? en el mapa


        if (t->type == 0){
        output += "UP";
        output += "\n";
        }else{
        output += "DOWN";
        output += "\n";
        }

		Vertex::Label tVs [3] = {t->getVertexLeft(),t->getVertexRight(),t->getVertexCenter()}; // array que guarda los tres v?rtices de cada triangulo
		for (auto v : tVs) { // pone cada uno de los v?rtices asociados a cada triangulo se parados por un /n: izquierdo /n derecho /n central /n
			output += std::to_string(vertexMap.at(v));
			output += "\n";
		}

		Triangle::Label tNeighb [3] = {t->getTriangleLeft(),t->getTriangleRight(),t->getTriangleCenter()}; // array que guarda los triangulos de la izquierda, derecha y el centro
		for (auto t : tNeighb) {  // hace un for por cada uno de los triangulos que tiene a la izquierda, a la derecha y en el medio, e imprime cada triangulo en separado por /n: izquierdo /n derecho /n centro
			output += std::to_string(triangleMap.at(t));
			output += "\n";
		}
	}

	output += std::to_string(triangles.size()); // escribe la cantidad de triangulos que hay

	std::ofstream file; // crea un archivo solo para poder abrirlo, escribirlo, y cerrarlo
	file.open(fname, std::ios::out | std::ios::trunc);
	assert(file.is_open());

	file << output << "\n"; // imprime todo el bloque de info que escribio en output en forma de string
	file.close(); // cierra el archivo

	std::cout << fname << "\n"; // una vez que termina de exportar todo la geometr?a, escribe en donde la guard?
}
