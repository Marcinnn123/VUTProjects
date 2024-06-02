package ija.tool.common;


public interface ToolRobot extends Observable{
	int angle();
	Position getPosition();

	Position getPrevPosition();
	void turn(int num);

	boolean move();
}
