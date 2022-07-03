#pragma once
#include <cmath>
#include "framework.h"
#include <objidl.h>
#include <gdiplus.h>
#include <vector>

#pragma comment(lib,"Gdiplus.lib")
using namespace Gdiplus;


enum shape { Nothing = 0, Arrow, CannonBall, Cannon, Wall };




struct Vector
{
	double x, y;

	Vector operator+(const Vector& G)const  //더하기
	{
		return Vector(G.x + x, G.y + y);
	}
	Vector operator-(const Vector& G)const //빼기
	{
		return Vector(x - G.x, y - G.y);
	}

	Vector operator*(double G) const //스칼라 곱하기
	{
		return Vector(x * G, y * G);
	}
	friend Vector operator*(const double T,const Vector& G)
	{
		return Vector(G.x * T, G.y * T);
	}

	double operator*(const Vector& G) const //내적
	{
		return (G.x * x + G.y * y);
	}


	double scalar()
	{
		return sqrt(pow(x, 2) + pow(y, 2));
	}


	Vector() : x(0), y(0) {}

	Vector(double cx, double cy)
		: x(cx), y(cy) {}

	~Vector() {}
};


class Game_Object_Manager
{
protected:
	Vector Pos;
	Vector Velocity;
	double Width;
	double Height;
	int What;
	bool destroy;
public:
	Game_Object_Manager() { Pos={ 0, 0 }, Velocity = { 0, 0 }, Width = 0, Height = 0, What = shape::Nothing,destroy=false;}
	Game_Object_Manager(double x, double y, double xV, double yV) :Game_Object_Manager() { Pos.x = x, Pos.y = y, Velocity.x = xV, Velocity.y = yV ; }
	virtual ~Game_Object_Manager() {};
	
	Vector getPos() const { return Pos; }
	void setPos(double x,double y) { Pos.x = x,Pos.y=y; }
	Vector getVelocity() const { return Velocity; }
	void setVelocity(double x, double y) { Velocity.x = x, Velocity.y = y;}


	double get_Width()const { return Width; }
	void set_Width(double W) { Width = W; }
	double get_Height() const { return Height; }
	void set_Height(double H) { Height = H; }
	int getWhat()const { return What; }

	void MOVE() { setPos(getPos().x + getVelocity().x, getPos().y + getVelocity().y); }

	double get_VelocitySize()const { return sqrt(pow(Velocity.x, 2) + pow(Velocity.y, 2)); } ;
	virtual bool is_collide(Game_Object_Manager& Ob) { return false; };
	//virtual void collide( Game_Object_Manager& Ob) {};
	virtual void Draw(HDC hdc, Image &pImg) {};
	virtual void Lifedown() {} ;
	void setDestroy(bool T) { destroy = T; }
	bool getDestroy() { return destroy; }

	bool inWindow(RECT Clientrc);


};

class Enemy_Missile : public Game_Object_Manager
{
private:
	int damage;
	
public:
	Enemy_Missile() { damage = 1, What = shape::Arrow, destroy = false; }
	virtual ~Enemy_Missile() {}

	
	void down();
	
	bool is_collide(Game_Object_Manager& OB); //충돌여부
	void Draw(HDC hdc, Image &pImg);
};

class Friend_Missile :public Game_Object_Manager
{
private:
	
public:
	Friend_Missile() { What = shape::CannonBall, destroy=false; }
	Friend_Missile(Vector Pos) ;
	virtual ~Friend_Missile() {}

	void Draw(HDC hdc, Image &pImg);

};

class LifeBlock :public Game_Object_Manager
{
private:
	int Life;
	
public:
	LifeBlock() { Life = 3, What = shape::Wall,destroy=false; }
	

	//void settingWall(int num, Image& pImg, std::vector<Game_Object_Manager*>& CannonBall_Wall);
	virtual ~LifeBlock() {}

	void Lifedown();

	void Draw(HDC hdc, Image &pImg);

};

class cannon
{
private:
	Vector Cannon_Port;
public:
	cannon() { Cannon_Port = { 0,0 }; }
	cannon(Vector t) { Cannon_Port = t; }
	~cannon() {}

	void Shot(Game_Object_Manager& F,int rot,double R);
	void Draw(HDC hdc, Image &img,RECT Clientrc, int rot);
};




