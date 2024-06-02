package ija.common;
import ija.tool.common.Position;
import ija.tool.common.ToolEnvironment;
import ija.tool.common.ToolRobot;

public interface Environment extends ToolEnvironment{
    boolean addRobot(Robot robot);
    boolean createObstacleAt(int col, int row);

    boolean obstacleAt(Position p,int angle);
    boolean robotAt(Position p, ToolRobot sender);
    boolean containsPosition(Position pos);
}
