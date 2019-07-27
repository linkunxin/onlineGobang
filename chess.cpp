#include "stdafx.h"
#include "Chess.h"
IMPLEMENT_SERIAL(Chess, CObject, 1);

Chess::Chess(int _h, int _v, int c) {
	h = _h;
	v = _v;
	color = c;
}
Chess::Chess(const Chess &a)
{
	h = a.h;
	v = a.v;
	color = a.color;
}
void Chess::Serialize(CArchive & ar)
{
	CObject::Serialize(ar);
	if (ar.IsStoring())
	{
		// TODO: �ڴ���Ӵ洢����
		ar << h << v << color;
	}
	else
	{
		// TODO: �ڴ���Ӽ��ش���
		ar >> h >> v >> color;
	}
}

Chess::Chess() {
	h = -1;
	v = -1;
	color = -1;
}