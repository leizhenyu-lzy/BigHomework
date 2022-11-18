#include "Car.h"

Car::Car(CarType type, CarColor color, CarPosition position, CarSize size) :
	m_Type(type), m_Color(color), m_Position(position), m_Size(size), m_Iter(0)
{}

Car::Car(CarType type, CarColor color, std::vector<CarPosition> positions, CarSize size):
	m_Type(type), m_Color(color), m_Positions(positions), m_Size(size), m_Iter(0)
{}

void Car::UpdatePosition()
{
	m_Iter++;
	if (m_Iter > m_Positions.size() - 1)
	{
		m_Iter--;
	}
}
