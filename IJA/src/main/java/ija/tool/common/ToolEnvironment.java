package ija.tool.common;
import ija.common.Obstacle;
import ija.common.Robot;

import java.util.List;
import java.util.Set;

public interface ToolEnvironment {
	int rows();
	int cols();
	boolean obstacleAt(Position p,int angle);
	boolean robotAt(Position p, ToolRobot sender);
	List<ToolRobot> robots();

	Set<Robot> getList();
	Set<Obstacle> getObstacles();
}
