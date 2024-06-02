package ija.common;
import ija.tool.common.Position;

/**
 * Class Obstacle handles obstacle location within an environment
 */
public class Obstacle {
    private Position position;

    /**
     * Constructor for Obstacle
     * @param env The environment that contains the obstacle
     * @param pos The position of the obstacle
     */
    public Obstacle(Environment env, Position pos) {
        this.position = pos;
    }

    /**
     * Gets the position of the obstacle
     * @return Position object representing the location of the obstacle
     */
    public Position getPosition() {
        return this.position;
    }

}


