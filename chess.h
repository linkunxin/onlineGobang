#pragma once
//color -1 ����;1 ��ɫ;0 ��ɫ
//nextIntColor 1%2����1��Ϊ��ɫ;2%2����0��Ϊ��ɫ
class Chess :public CObject {
	DECLARE_SERIAL(Chess);//���л��Ĺ��ܣ��ܼ��غ�������̱������������ڱ�����Ϸ
public:
	int h = 0, v = 0;
	int color;
	Chess();//��ʼ�������������ɫ
	Chess(int h, int v, int color);//���Ӹ�ֵ
	Chess(const Chess &);//���ӵĸ���
	virtual void Serialize(CArchive& ar);//���л����棬������Ϸ

};
