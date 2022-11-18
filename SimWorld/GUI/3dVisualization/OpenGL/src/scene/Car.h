#pragma once

#include "vector"

class Car {
public:
	
	enum class CarType {
		CAR,
		BUS,
		TRUCK
	};
	
	enum class CarColor {
		RED,
		BLUE,
		GREEN
	};
	
	struct CarPosition {
		float x, y;
		float theta;
	};

	struct CarSize {
		float scale_x;
		float scale_y;
		float scale_z;
	};

	Car(CarType type, CarColor color, CarPosition position, CarSize size);
	Car(CarType type, CarColor color, std::vector<CarPosition> position, CarSize size);
	~Car() {};
	
	// realtime rendering
	void UpdatePosition(CarPosition position);

	// rendering at one time
	void UpdatePosition();

// should be private, but be lazy 
public:
	CarType m_Type;
	CarColor m_Color;
	CarPosition m_Position;
	std::vector<CarPosition> m_Positions;
	CarSize m_Size;
	// iter the position
	int m_Iter;
};