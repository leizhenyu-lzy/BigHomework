#pragma once
#include "Car.h"
#include "vector"
#include "iostream"

class Gridmap {
public:
	Gridmap(int width, int height);
	~Gridmap();

	void SetHeight(int x, int y, int height);
	int GetHeight(int x, int y);

	int GetHeight();
	int GetWidth();

	void AddCar(Car& car);
	std::vector<Car> GetCars();

	void UpdateCarsPosition();
	
// for lazy
public: 
	// grid map size
	int m_Height, m_Width;
	// grid map data
	int **m_Data;
	// Car data
	std::vector<Car> m_Cars;
};
