#include "world.h"


using namespace std;

World* World::worldInstance = NULL;

World::World() : width(300), height(300), depth(1),
	frameTime(10), backgroundColor(255, 255, 255), activeTurtle(0) {
	origin[0] = gridPoint(150, 150, 0);
	origin[1] = gridPoint(450, 150, 0);
}

void World::createMap() {
	map = new voxel**[width];
	for(int i = 0; i < width; i++) {
		map[i] = new voxel*[height];	
		for(int j = 0; j < height; j++) {
			map[i][j] = new voxel[depth];
		}
	}
}

World::~World() {
	for(int i = 0; i < width; i++) {
		for(int j = 0; j < height; j++) {
			delete[] map[i][j];
		}
		delete[] map[i];
	}
	delete[] map;
}

World* World::instance() {
	if(worldInstance == NULL) {
		worldInstance = new World();
	}
	return worldInstance;
}

void World::setup(char *filename) {
	fstream setupFile;
	setupFile.open(filename, fstream::in);
	if(!setupFile.good()) {
		cout << "Corrupted config file." << endl;
		exit(1);
	}
	
	string tmpString;
	
	setupFile >> tmpString;
	if(tmpString == "TRANSFORM") {
		mode = TRANSFORM;
	} else if (tmpString == "ESCAPE") {
		mode = ESCAPE;
	} else {
		mode = NORMAL;
	}
	
	setupFile >> width >> height >> depth;
	setupFile >> origin[0].x >> origin[0].y >> origin[0].z;
	origin[1] = origin[0];
	origin[1].x += width;
	setupFile >> frameTime;
	cout << width << " " << height << " " << depth << " " << endl;
//	cout << origin.x << " " << origin.y << " " << origin.z;
	cout << frameTime << endl;
	setupFile.close();
	cout << "World setup complete" << endl;
}

int World::getWidth() {
	return width;
}

int World::getHeight() {
	return height;
}

int World::getDepth() {
	return depth;
}

int World::getFrameTime() {
	return frameTime;
}

Mode World::getMode() {
	return mode;
}


gridPoint World::getOrigin(int id) {
	return origin[id];
}

ofColor World::getBackgroundColor() {
	return backgroundColor;
}

//voxel* World::getVoxel(gridPoint gp) {
//	return &map[gp.x + origin.x][gp.y + origin.y][gp.z + origin.z];
//}

vector<segment> World::getTrace(int id) {
	return trace[id];
}

int World::getLeft() {
	return -origin[0].x;
}

int World::getRight() {
	return -origin[0].x + width;
}

int World::getBottom() {
	return -origin[0].y;
}

int World::getTop() {
	return -origin[0].y + height;
}

bool World::outside(gridPoint p) {
	return p.y < getBottom() || p.y >= getTop() || p.x < getLeft() || p.x >= getRight();
}

void World::updateTurtle(int id, pair<point, vect> coords) {
	if(turtle[id].isPenDown && !(coords.first == turtle[id].position)) {
		trace[id].push_back(segment(turtle[id].position, coords.first, turtle[id].penColor));
		cout << trace[id].size() << endl;
	}
	turtle[id].position = coords.first;
	turtle[id].direction = coords.second;
	//cout << turtle[id].position.x << " " << turtle[id].position.y << " " << turtle[id].direction.x << " " << turtle[id].direction.y << endl;
}

void World::toggleTurtle() {
	activeTurtle = !activeTurtle; // 0 = !1, 1 = !0
}

void World::rotate(long double angle) {// in degrees
	updateTurtle(activeTurtle, make_pair(turtle[activeTurtle].position, turtle[activeTurtle].direction.rotated(angle)));	
	if(mode == TRANSFORM) {
		updateTurtle(!activeTurtle, 
			trans[activeTurtle].setCoords(make_pair(turtle[activeTurtle].position, turtle[activeTurtle].direction)));	
	}
}


void World::forward(long double distance) {

	vect displacement = turtle[activeTurtle].direction * distance;
	point newPosition = turtle[activeTurtle].position.translated(displacement);
    point newPosition2 = trans[activeTurtle].setCoords(make_pair(newPosition, vect())).first;
//    cout << "Turtle position: " << newPosition.x << " " << newPosition.y << endl;
    
	if(outside(newPosition) && outside(newPosition2)) { // It is enough when one turtle is inside viewport
		return;
		// This can be done in a better way better, e. g. by finding the intersection of the (position, newPosition) segment with the world borders
		// The problem is that I don't know what those borders shall eventually look like (it may be even a 3D mesh), so for now I leave this solution	
	}
	
	point currentPosition = turtle[activeTurtle].position; 
	point begPosition = currentPosition;
	
	while(dist(begPosition, currentPosition) <= // Note: dist is a function, while distance is a parameter
		   	dist(begPosition, newPosition)) {
		currentPosition = currentPosition.translated(turtle[activeTurtle].direction);
//		gridPoint visitedVoxel = currentPosition; 
//		if(!outside(visitedVoxel)) getVoxel(visitedVoxel)->visit(false); // We draw trace with segments, not with voxels
		updateTurtle(activeTurtle, make_pair(currentPosition, turtle[activeTurtle].direction));
		if(mode == TRANSFORM) updateTurtle(!activeTurtle, 
			trans[activeTurtle].setCoords(make_pair(turtle[activeTurtle].position, turtle[activeTurtle].direction)));
	}	

	updateTurtle(activeTurtle, make_pair(currentPosition, turtle[activeTurtle].direction));
	if(mode == TRANSFORM) updateTurtle(!activeTurtle, 
			trans[activeTurtle].setCoords(make_pair(turtle[activeTurtle].position, turtle[activeTurtle].direction)));
}

vector<point> World::getTurtleShape(int id) {
	vector<point> res(3);
	res[0] = turtle[id].position.translated(turtle[id].direction * 30.0);
	res[1] = turtle[id].position.translated(turtle[id].direction.rotated(90.0) * 10.0);
	res[2] = turtle[id].position.translated(turtle[id].direction.rotated(-90.0) * 10.0);	
	for (int i = 0; i < 3; i++) {
		res[i] = res[i].translated((vect)(point)origin[id]);
	}

	return res;
}

void World::setMobius(comp a, comp b, comp c, comp d) {
	if(a * d - b * c == 0) {
		cout << "a * d - b * c == 0" << endl;
		return;
	}
	if(outside(turtle[activeTurtle].position)) {
		cout << "Active turtle outside viewport" << endl;
		return;
	}
	trans[activeTurtle].setValues(a, b, c, d);
	trans[!activeTurtle].setValues(-d, b, c, -a);
	trace[!activeTurtle].clear();
	for(vector<segment>::iterator it = trace[activeTurtle].begin(); it < trace[activeTurtle].end(); it++) {
		trace[!activeTurtle].push_back(segment(trans[activeTurtle].setPos(it->a), 
				trans[activeTurtle].setPos(it->b)));
	}
}

bool World::crop(segment& seg) {
	if(outside(seg.a) && outside(seg.b)) return false;
	if(!outside(seg.a) && !outside(seg.b)) return true;
	if(!outside(seg.a)) {
		point tmp = seg.a;
		seg.a = seg.b;
		seg.b = tmp;
	}
	point corners[4];
	corners[0] = point(getTop(), getRight());
	corners[1] = point(getBottom(), getRight());
	corners[2] = point(getBottom(), getLeft());
	corners[3] = point(getTop(), getLeft());
	
	for(int i = 0; i < 4; i++) {
		point tmp;
		if (intersect(seg, segment(corners[i], corners[(i + 1) % 4]), tmp)) {
			seg.a = tmp;
		}
	}
}