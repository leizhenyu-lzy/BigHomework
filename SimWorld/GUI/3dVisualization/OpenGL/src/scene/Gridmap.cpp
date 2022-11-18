#include  "Gridmap.h"
#include "Car.h"
#include "vector"

Gridmap::Gridmap(int height, int width)
{
	this->m_Height = height;
	this->m_Width = width;
	this->m_Data = new int* [height];
	for (int i = 0; i < height; i++)
	{
		this->m_Data[i] = new int[width];
		for (int j = 0; j < width; j++)
		{
			this->m_Data[i][j] = 0;
		}
	}
}

Gridmap::~Gridmap()
{
	for (int i = 0; i < this->m_Height; i++)
	{
		delete[] this->m_Data[i];
	}
	delete[] this->m_Data;
}


void Gridmap::SetHeight(int x,int y,int height)
{
	m_Data[x][y] = height;
}

int Gridmap::GetHeight(int x, int y)
{
	return m_Data[x][y];
}

void Gridmap::AddCar(Car &car)
{
	m_Cars.push_back(car);
}

std::vector<Car> Gridmap::GetCars()
{
	return m_Cars;
}

int Gridmap::GetHeight()
{
	return m_Height;
}

int Gridmap::GetWidth()
{
	return m_Width;
}

void Gridmap::UpdateCarsPosition()
{
	for (auto &car : m_Cars)
	{
		car.UpdatePosition();
	}
}