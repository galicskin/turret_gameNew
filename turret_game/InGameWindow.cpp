#include "InGameWindow.h"
const double PI = 3.14159265358979323;

double Projection(Vector a, Vector b)
{
	return ((a * b) / b.scalar());
}

Vector Rotate(double theta) 
{
	Vector V(0, -1);
	double temp = V.x;
	V.x = V.x * cosf(theta) - V.y * sinf(theta);
	V.y = V.x * sinf(theta) + V.y * cosf(theta);
	return V;
}


void Enemy_Missile::down()
{
	double dx = getPos().x + getVelocity().x;
	double dy = getPos().y + getVelocity().y;
	setPos(dx,dy);
}



bool Enemy_Missile::is_collide(Game_Object_Manager& OB)
{
	Vector arrowpoint(Pos.x + Width/2, Pos.y + Height/2);

	

	switch (OB.getWhat())
	{
	case shape::CannonBall :
	{
		Vector ball_center(OB.getPos().x+OB.get_Width()/2,OB.getPos().y+OB.get_Height()/2);
		
		if ((Pos.x < OB.getPos().x) && (Pos.x + Width > OB.getPos().x))
		{
			if ((Pos.y < OB.getPos().y) && (Pos.y + Height > OB.getPos().y))
			{
				return true;
			}
			return false;
		}
		return false;
		break;
	}
	case Wall:
	{
		if (OB.getPos().x + 81 > Pos.x+ Width / 2 && OB.getPos().x < Pos.x+ Width / 2)
		{
			return (OB.getPos().y  < Pos.y+Height);
		}
		else
		{
			return false;
		}
		break;
	}
	default :
	{
		return false;
		break;
	}
	}

	
}

void Enemy_Missile::Draw(HDC hdc, Image &pImg)
{

	Graphics graphics(hdc);
	ImageAttributes imgAttr;// ���İ� ����
	int w = pImg.GetWidth();
	int h = pImg.GetHeight();
	imgAttr.SetColorKey(Color(245, 0, 245), Color(255, 10, 255));
	graphics.DrawImage(&pImg, Rect(Pos.x, Pos.y, w/5, h/5), 0, 0, w, h, UnitPixel, &imgAttr);

	set_Width(w / 5);
	set_Height(h / 5);

}



Friend_Missile::Friend_Missile(Vector Pos)
{
	this->Pos.x = Pos.x;
	this->Pos.y = Pos.y;
	What = shape::CannonBall;
}

void Friend_Missile::Draw(HDC hdc, Image &pImg)
{
	Graphics graphics(hdc);
	ImageAttributes imgAttr;// ���İ� ����
	int w = pImg.GetWidth();
	int h = pImg.GetHeight();
	imgAttr.SetColorKey(Color(245, 0, 245), Color(255, 10, 255));
	graphics.DrawImage(&pImg, Rect(Pos.x, Pos.y, w/25, h/25), 0, 0, w, h, UnitPixel, &imgAttr);


	set_Width(w / 25);
	set_Height(h / 25);

}


void LifeBlock::Lifedown()
{
	if (Life > 0)
	{
		Life--;
	}
	else
	{
		destroy = true;
	}
}

void LifeBlock::Draw(HDC hdc, Image &pImg)
{
	Graphics graphics(hdc);


	REAL transparency = 0.3f*Life;
	ImageAttributes imgAttr;// ���İ� ����
	ColorMatrix colorMatrix =
	{
			1.0f,0.0f,0.0f,0.0f,0.0f,   // r
			0.0f,1.0f,0.0f,0.0f,0.0f,   // g
			0.0f,0.0f,1.0f,0.0f,0.0f,   // b
			0.0f,0.0f,0.0f,transparency,0.0f,  //��������� ���İ�: 4,4 �κ�
			0.0f,0.0f,0.0f,0.0f,1.0f, //��������� ���� ������ 4�κ��� ������
	};
	imgAttr.SetColorMatrix(&colorMatrix);

	int w = pImg.GetWidth();
	int h = pImg.GetHeight();
	imgAttr.SetColorKey(Color(245, 0, 245), Color(255, 10, 255));
	graphics.DrawImage(&pImg, Rect(Pos.x, Pos.y, w/4, h/8), 0, 0, w, h, UnitPixel, &imgAttr);

	set_Width(w / 4);
	set_Height(h / 8);

}

void cannon::Shot(Game_Object_Manager& F,int rot, double R)
{

	double rot_rad = rot * (PI / 300);

	if (rot_rad < 0)
	{
		rot_rad += 2 * PI;
	}

	if (rot_rad > 2 * PI)
	{
		rot_rad-= 2 * PI;
	}

	Rotate(rot_rad);
	
	Vector V= R * Rotate(rot_rad);
	F.setVelocity(V.x, V.y);



}

void cannon::Draw(HDC hdc, Image &img, RECT Clientrc,int rot)
{
	Graphics graphics(hdc);
	ImageAttributes imgAttr;// ���İ� ����
	int w = img.GetWidth()/10;
	int h = img.GetHeight()/10;
	imgAttr.SetColorKey(Color(245, 0, 245), Color(255, 10, 255));
	double cannon_locationx = (Clientrc.right - Clientrc.left)/2 ; 
	double cannon_locationy = Clientrc.bottom;

	Image* pImg = nullptr;
	pImg = &img;

	int xPos = (Clientrc.right - Clientrc.left) / 2;
	int yPos = Clientrc.bottom-100;




	if (pImg)//��𼱰� null���� �޾Ƽ� ������⶧���� �̷��� null�� üũ�ؼ� �׷���
	{
		// << : Rotate image
		Gdiplus::Matrix mat;
		


		mat.RotateAt((rot % 360), Gdiplus::PointF(xPos + (float)(w / 2), yPos + (float)(h / 2)));
		graphics.SetTransform(&mat);
		graphics.DrawImage(pImg, xPos, yPos, w, h);


		mat.Reset();
		graphics.SetTransform(&mat); // ������ �����ָ� �� �ڿ� �׷����°͵��� ���� ȸ���ϰԵ�.
	}


	graphics.DrawImage(&img, Rect(cannon_locationx, cannon_locationy, w, h), 0, 0, w, h, UnitPixel, &imgAttr);
	//delete pImg;
	
}

bool Game_Object_Manager::inWindow(RECT Clientrc)
{
	return (Clientrc.right - Width > Pos.x) && (Clientrc.left < Pos.x) && (Clientrc.top < Pos.y) && (Clientrc.bottom - Height > Pos.y);

}
