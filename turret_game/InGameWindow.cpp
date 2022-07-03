#include "InGameWindow.h"
const double PI = 3.14159265358979323;

double Projection(Vector a, Vector b)
{
	return ((a * b) / b.scalar());
}
void Rotate(Vector &a, double theta) 
{
	
	double temp = a.x;
	a.x = a.x * cosf(theta) - a.y * sinf(theta);
	a.y = a.x * sinf(theta) + a.y * cosf(theta);
}


void Enemy_Missile::down()
{
	double dx = getPos().x + getVelocity().x;
	double dy = getPos().y + getVelocity().y;
	setPos(dx,dy);
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
	graphics.DrawImage(&pImg, Rect(Pos.x, Pos.y, w/15, h/15), 0, 0, w, h, UnitPixel, &imgAttr);
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
	graphics.DrawImage(&pImg, Rect(Pos.x, Pos.y, w/4, h/8), 0, 0, w, h, UnitPixel, &imgAttr);
}

void cannon::Shot(Game_Object_Manager& F,int rot)
{

	double rot_rad = rot * (PI / 180);

	if (rot_rad < 0)
	{
		rot_rad += 2 * PI;
	}

	if (rot_rad > 2 * PI)
	{
		rot_rad-= 2 * PI;
	}

	Rotate(this->Cannon_Port, rot_rad);
	

	F.setVelocity(Cannon_Port.x, Cannon_Port.y);



}

void cannon::Draw(HDC hdc, Image &img, RECT Clientrc,int rot)
{
	Graphics graphics(hdc);
	ImageAttributes imgAttr;// 알파값 관련
	int w = img.GetWidth()/10;
	int h = img.GetHeight()/10;
	imgAttr.SetColorKey(Color(245, 0, 245), Color(255, 10, 255));
	double cannon_locationx = (Clientrc.right - Clientrc.left)/2 ; 
	double cannon_locationy = Clientrc.bottom;

	Image* pImg = nullptr;
	pImg = &img;

	int xPos = (Clientrc.right - Clientrc.left) / 2;
	int yPos = Clientrc.bottom-100;

	if (pImg)//어디선가 null값을 받아서 실행됬기때문에 이렇게 null을 체크해서 그려줌
	{
		// << : Rotate image
		Gdiplus::Matrix mat;
		


		mat.RotateAt((rot % 360), Gdiplus::PointF(xPos + (float)(w / 2), yPos + (float)(h / 2)));
		graphics.SetTransform(&mat);
		graphics.DrawImage(pImg, xPos, yPos, w, h);


		mat.Reset();
		graphics.SetTransform(&mat); // 리셋을 안해주면 그 뒤에 그려지는것들이 같이 회전하게됨.
	}


	graphics.DrawImage(&img, Rect(cannon_locationx, cannon_locationy, w, h), 0, 0, w, h, UnitPixel, &imgAttr);
	//delete pImg;
	
}


