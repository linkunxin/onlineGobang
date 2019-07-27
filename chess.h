#pragma once
//color -1 待定;1 黑色;0 白色
//nextIntColor 1%2等于1，为黑色;2%2等于0，为白色
class Chess :public CObject {
	DECLARE_SERIAL(Chess);//序列化的功能，能加载和载入磁盘保存起来，用于保存游戏
public:
	int h = 0, v = 0;
	int color;
	Chess();//初始化棋子坐标和颜色
	Chess(int h, int v, int color);//棋子赋值
	Chess(const Chess &);//棋子的复制
	virtual void Serialize(CArchive& ar);//序列化储存，保存游戏

};
