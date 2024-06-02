package ija.room;

import java.util.ArrayList;
import java.util.List;

import ija.common.Environment;
import ija.tool.common.Position;
import ija.common.Robot;

public class ControlledRobot implements Robot{
    private Environment environment;
    private Position position;
	private Position prevPosition;
    private int angle;
    private static List<Observer> observers = new ArrayList<>();

    private ControlledRobot(Environment environment, Position position) {
        this.environment = environment;
        this.position = position;
		this.prevPosition = position;
        this.angle = 0;
		observers = new ArrayList<>();
  
    }

	/**
		* Creates new robot in environment
		* @param env Environment
		* @param pos Robot's position
		* @return New robot
		*/
	public static ControlledRobot create(Environment env, Position pos) {
        if (env.containsPosition(pos) && !env.obstacleAt(pos,999) && !env.robotAt(pos,null)) {
            ControlledRobot robot = new ControlledRobot(env, pos);

            if (env.addRobot(robot)) {
                return robot; 
            }
        }
        return null;
    }


	/**
		* Moves robot to the next position
		* @return result of validation next position
		*/
	@Override
    public boolean move() {
        int newX = this.position.getCol();
        int newY = this.position.getRow();

        switch (this.angle) {
			case 0:
				newY -=1;
				break;
			case 45:
				newY-=1;
				newX+=1;
				break;
			case 90:
				newX+=1;
				break;
			case 135:
				newX+=1;
				newY+=1;
				break;
			case 180:
				newY+=1;
				break;
			case 225:
				newY+=1;
				newX-=1;
				break;
			case 270:
				newX-=1;
				break;
			case 315:
				newX-=1;
				newY-=1;
				break;
            default:
                return false;
        }
    
        if (environment.containsPosition(new Position(newX, newY)) && !environment.obstacleAt(new Position(newX, newY),this.angle) && !environment.robotAt(new Position(newX, newY),this)) {
            // Updates robot's position and notifies observers about changes
												Position old = new Position(this.position.getCol(),this.position.getRow());
												this.prevPosition = old;
												this.position = new Position(newX, newY);
            this.notifyObservers();

            return true; 
        }
        return false;
    }

	/**
		* Checks if robot can move to the next position according to its angle
		* @return result of validation
		*/
	@Override
    public boolean canMove() {
        int newX = this.position.getCol();
        int newY = this.position.getRow();

								// Changes new coordinates according to robot's angle
        switch (this.angle) {
            case 0:
                newY -=1;
                break;
            case 45:
                newY-=1;
                newX+=1;
                break;
            case 90:
                newX+=1;
                break;
            case 135:
                newX+=1;
                newY+=1;
                break;
            case 180:
                newY+=1;
                break;
            case 225:
                newY+=1;
                newX-=1;
                break;
            case 270:
                newX-=1;
                break;
            case 315:
                newX-=1;
                newY-=1;
                break;
            default:
                return false; 
        }

								// Returns result of validation new position
        return environment.containsPosition(new Position(newX, newY)) && !environment.obstacleAt(new Position(newX, newY),this.angle) && !environment.robotAt(new Position(newX, newY),this);
    }


	/**
		* Turns robot and notifies observers about changes
		*/
    @Override
    public void turn(int num) {
        this.angle = (this.angle + (45*num)) % 360;
        this.prevPosition = this.position;
        this.notifyObservers();
    }

	/**
		* Turns robot and notifies observers about changes
		*/
	@Override
    public void turn() {
        this.angle = (this.angle + 90) % 360;
        this.prevPosition = this.position;
        this.notifyObservers();
    }

	/**
		* Returns robot's position
		* @return Robot's position
		*/
	@Override
    public Position getPosition() {
        return this.position;
    }

	/**
		* Returns robot's previous position
		* @return Robot's previous position
		*/
	@Override
	public Position getPrevPosition(){
		return this.prevPosition;
	}

	/**
		* Returns angle
		* @return Robot's angle
		*/
    @Override
    public int angle() {
        return this.angle;
    }

	/**
		* Add observer to the list
		* @param o Observer
		*/
	@Override
    public void addObserver(Observer o) {
        observers.add(o);
    }

	/**
		* Notifies all observers about changes
		*/
	@Override
    public void notifyObservers() {
        for (Observer observer : observers) {
            observer.update(this);
        }
    }

}


