package ija.common;
import ija.tool.common.Position;
import ija.tool.common.ToolRobot;

public interface Robot extends ToolRobot{
    void turn();
    int angle();
    boolean canMove();
    boolean move();
    Position getPosition();
}

