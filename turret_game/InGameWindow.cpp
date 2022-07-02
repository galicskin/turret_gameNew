#include "InGameWindow.h"


double Projection(Vector a, Vector b)
{
	return ((a * b) / b.scalar());
}


bool Enemy_Missile::is_collide(Game_Object_Manager& OB, Image* pImg)
{
	Vector endpoint(Pos.x, Pos.y + (Height / 2));
	Vector R_Top(-1 / sqrt(2), -1 / sqrt(2));
	Vector L_Top(1 / sqrt(2), -1 / sqrt(2));

	switch (What)
	{
	case shape::CannonBall :
	{
		if (OB.getVelocity().x < 0)
		{
			return (Projection(endpoint - OB.getPos(), L_Top) <= (OB.get_Width()/2));
		}
		else if (OB.getVelocity().x > 0)
		{
			return (Projection(endpoint - OB.getPos(), R_Top) >= (OB.get_Width() / 2));
		}
		else
		{
			return (OB.getPos().y - (OB.get_Width()/2) < endpoint.y);
		}
		break;
	}
	case Wall:
	{
		return (OB.getPos().y - (OB.get_Width() / 2) < endpoint.y);
		break;
	}
	}

	
}

void Enemy_Missile::Draw(HDC hdc, Image &pImg)
{

	Graphics graphics(hdc);
	ImageAttributes imgAttr;// 알파값 관련
	int w = pImg.GetWidth();
	int h = pImg.GetHeight();
	imgAttr.SetColorKey(Color(245, 0, 245), Color(255, 10, 255));
	graphics.DrawImage(&pImg, Rect(Pos.x, Pos.y, w, h), 0, 0, w, h, UnitPixel, &imgAttr);
}

void Friend_Missile::Draw(HDC hdc, Image &pImg)
{
	Graphics graphics(hdc);
	ImageAttributes imgAttr;// 알파값 관련
	int w = pImg.GetWidth();
	int h = pImg.GetHeight();
	imgAttr.SetColorKey(Color(245, 0, 245), Color(255, 10, 255));
	graphics.DrawImage(&pImg, Rect(Pos.x, Pos.y, w, h), 0, 0, w, h, UnitPixel, &imgAttr);
}

void LifeBlock::Draw(HDC hdc, Image &pImg)
{
	Graphics graphics(hdc);
	ImageAttributes imgAttr;// 알파값 관련
	int w = pImg.GetWidth();
	int h = pImg.GetHeight();
	imgAttr.SetColorKey(Color(245, 0, 245), Color(255, 10, 255));
	graphics.DrawImage(&pImg, Rect(Pos.x, Pos.y, w, h), 0, 0, w, h, UnitPixel, &imgAttr);
}

void cannon::Shot(Friend_Missile& F)
{
	F.setVelocity(Cannon_Port.x, Cannon_Port.y);
}

void cannon::Draw(HDC hdc, Image &img, RECT Clientrc)
{
	Graphics graphics(hdc);
	ImageAttributes imgAttr;// 알파값 관련
	int w = img.GetWidth();
	int h = img.GetHeight();
	imgAttr.SetColorKey(Color(245, 0, 245), Color(255, 10, 255));
	double cannon_locationx = (Clientrc.right - Clientrc.left)/2 ; 
	double cannon_locationy = Clientrc.bottom;
	graphics.DrawImage(&img, Rect(cannon_locationx, cannon_locationy, w, h), 0, 0, w, h, UnitPixel, &imgAttr);
	
}


