package ija.room;

import ija.common.Environment;
import ija.tool.common.Position;
import ija.common.Robot;
import ija.common.Obstacle;
import ija.tool.common.ToolRobot;

import java.util.HashSet;
import java.util.Set;
import java.util.ArrayList;
import java.util.List;

public class Room implements Environment {
    private int width;
    private int height;
    private Set<Obstacle> obstacles;
    private Set<Robot> robots;
    public  List<Robot> autoRobots;

    private Room(int width, int height) {
        this.width = width;
        this.height = height;
        this.obstacles = new HashSet<>();
        this.robots = new HashSet<>();
        this.autoRobots = new ArrayList<>();
    }

	/**
		* Creates room
		* @param width Room's width
		* @param height Room's height
		* @return Room
		*/
	public static Room create(int width, int height) {
        if (width <= 0 || height <= 0) {
            throw new IllegalArgumentException("Room dimensions must be greater than zero.");
        }
        return new Room(width, height);
    }

	/**
		* Creates obstacle at selected position
		* @param x Column
		* @param y Row
		* @return Result of creation
		*/
	@Override
    public boolean createObstacleAt(int x, int y) {
        if (x >= 0 && x < width && y >= 0 && y <= height) {
            Position obstaclePosition = new Position(x, y);
            Obstacle obstacle = new Obstacle(this,obstaclePosition);
			for(Obstacle obs : obstacles){
				if(obs.getPosition().equals(obstaclePosition))
				{
					return false;
				}
			}
			return obstacles.add(obstacle);
        }
      return false;
    }

	/**
		* Checks if there is obstacle near selected position
		* @param p Selected position
		* @param angle Robot's angle
		* @return True, if there is an obstacle
		*/
	@Override
    public boolean obstacleAt(Position p,int angle) {
        for (Obstacle obstacle : obstacles) {
            if (!obstacle.getPosition().validPosition(p,angle)) {
                return true;
            }
        }
        return false;
    }


	/**
		* Add robot to the list
		* @param robot Robot to be added
		* @return Result of adding robot to the list
		*/
	@Override
    public boolean addRobot(Robot robot) {
        if (robot != null && !this.robots.contains(robot) && !obstacleAt(robot.getPosition(),999) && containsPosition(robot.getPosition())) {
            return this.robots.add(robot);
        }
        return false;
    }


	/**
		* Checks if the position is within room's parameters
		* @param pos Position
		* @return Result of validation
		*/
	@Override
    public boolean containsPosition(Position pos) {
        return pos.getRow() >= 25 && pos.getRow() < height - 70 && pos.getCol() >= 25 && pos.getCol() < width - 25;
    }

	/**
		* Checks if there is a robot near specified position
		* @param pos Specified position
		* @param sender Robot that checks this position
		* @return Result of validation
		*/
	@Override
    public boolean robotAt(Position pos, ToolRobot sender) {
        final double radius = 25;
        for (Robot robot : this.robots) {
            if (robot != sender && robot.getPosition().isNear(pos, radius)) {
                return true;
            }
        }
        return false;
    }

	/**
		* Returns room's height
		* @return Room's height
		*/
	@Override
    public int rows(){
        return this.height;
    }

	/**
		* Return room's width
		* @return Room's width
		*/
	@Override
    public int cols(){
        return this.width;
    }

	/**
		* Return List of robots
		* @return List of robots
		*/
	@Override
    public List<ToolRobot> robots() {
        List<ToolRobot> robotList = new ArrayList<>();
        robotList.addAll(this.robots);
        return robotList;
    }

				/**
					* Return set of robots
				* @return Set of Robots
				 */
    @Override
    public Set<Robot> getList() {
        return this.robots;
    }

	/**
		* Returns obstacle
		* @return Concrete obstacle
		*/
	@Override
	public Set<Obstacle> getObstacles() {
        return this.obstacles;
    }

}






