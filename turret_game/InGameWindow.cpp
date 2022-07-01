#include "InGameWindow.h"

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
			return (projection(endpoint - OB.getPos(), L_Top) <= (OB.get_Width()/2));
		}
		else if (OB.getVelocity().x > 0)
		{
			return (projection(endpoint - OB.getPos(), R_Top) >= (OB.get_Width() / 2));
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

void Enemy_Missile::Draw(HDC hdc, Image img)
{

	Graphics graphics(hdc);
	ImageAttributes imgAttr;// 알파값 관련
	int w = img.GetWidth();
	int h = img.GetHeight();
	imgAttr.SetColorKey(Color(245, 0, 245), Color(255, 10, 255));
	graphics.DrawImage(&img, Rect(Pos.x, Pos.y, w, h), 0, 0, w, h, UnitPixel, &imgAttr);
}

void Friend_Missile::Draw(HDC hdc, Image img)
{
	Graphics graphics(hdc);
	ImageAttributes imgAttr;// 알파값 관련
	int w = img.GetWidth();
	int h = img.GetHeight();
	imgAttr.SetColorKey(Color(245, 0, 245), Color(255, 10, 255));
	graphics.DrawImage(&img, Rect(Pos.x, Pos.y, w, h), 0, 0, w, h, UnitPixel, &imgAttr);
}

void LifeBlock::Draw(HDC hdc, Image img)
{
	Graphics graphics(hdc);
	ImageAttributes imgAttr;// 알파값 관련
	int w = img.GetWidth();
	int h = img.GetHeight();
	imgAttr.SetColorKey(Color(245, 0, 245), Color(255, 10, 255));
	graphics.DrawImage(&img, Rect(Pos.x, Pos.y, w, h), 0, 0, w, h, UnitPixel, &imgAttr);
}

void Cannon::Shot(Friend_Missile& F)
{
	F.setVelocity(Cannon_Port.x, Cannon_Port.y);
}

void Cannon::Draw(HDC hdc,RECT Clientrc)
{

	Ellipse(hdc, ((Clientrc.left)/2)-20, , , , )
}
