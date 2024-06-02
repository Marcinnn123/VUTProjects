package ija.tool.common;

/**
 * Represents a 2D position with column and row
 */
public class Position {
    private final int col;
    private final int row;

	/**
     * Constructs a Position object
     * @param col Column coordinate
     * @param row Row coordinate
     */
    public Position(int col, int row) {
								this.col = col;
								this.row = row;

    }

	/**
     * Returns the row
     * @return Row coordinate
     */
    public int getRow() {
        return this.row;
    }

	/**
     * Returns the column 
     * @return Column coordinate
     */
    public int getCol() {
        return this.col;
    }

	/**
     * Checks if this position is equal to another object
     * @param o The object to compare with
     * @return True if the objects are the same or represent the same position
     */
    @Override
    public boolean equals(Object o) {
        if (this == o) return true;
        if (o == null || getClass() != o.getClass()) return false;
        Position position = (Position) o;
        return this.row == position.row && this.col == position.col;
    }

	/**
     * Calculates whether another position is within a certain radius
     * @param other The position to compare to
     * @param radius The radius to consider
     * @return True if the distance is less than or equal to the specified radius
     */
    public boolean isNear(Position other, double radius) {
        double dx = this.col - other.col;
        double dy = this.row - other.row;
        return Math.sqrt(dx * dx + dy * dy) <= radius + radius; // Check if the distance is less than or equal to the sum of the radii
    }
    
	/**
     * Determines if a given position is valid
     * @param other The position to validate
     * @param angle The angle specifying direction
     * @return True if the position is valid
     */
	public boolean validPosition(Position other,int angle) {
					// Distance between obstacle and robot
		int dx = this.col - other.col;
		int dy = this.row - other.row;


		// Check new position according to robot's angle
		switch (angle){
			case 0:
				if(this.col < other.col) {
					if (Math.abs(dx) > 75) {
						return true;
					}	else {
						return this.row < other.row - 75 || this.row > other.row -25;
					}
				}

				if(this.col > other.col){
					if(Math.abs(dx) > 25){
						return true;
					}
					else {
						return this.row < other.row - 75 || this.row > other.row -25;
					}
				}

			case 90:
				if(this.row < other.row) {
					if (Math.abs(dy) > 75) {
						return true;
					}	else {
						return this.col > other.col + 25 || this.col < other.col -25;
					}
				}

				if(this.row > other.row){
					if(Math.abs(dy) > 25){
						return true;
					}
					else {
						return this.col > other.col + 25 || this.col < other.col -25;
					}
				}
				break;
			case 180:
				if(this.col < other.col) {
					if (Math.abs(dx) > 75) {
						return true;
					}	else {
						return this.row > other.row + 25 || this.row < other.row -25;
					}
				}

				if(this.col > other.col){
					if(Math.abs(dx) > 25){
						return true;
					}
					else {
						return this.row > other.row + 25 || this.row < other.row -25;
					}
				}
			case 270:
				if(this.row < other.row) {
					if (Math.abs(dy) > 75) {
						return true;
					}	else {
						return this.col < other.col - 75 || this.col > other.col + 25;
					}
				}

				if(this.row > other.row){
					if(Math.abs(dy) > 25){
						return true;
					}
					else {
						return this.col < other.col - 75 || this.col > other.col + 25;
					}
				}
				return false;
		}
		int distanceSquared = dx * dx + dy * dy;
		int maxDistanceSquared = 35 * 35;

		int dx2 = this.col - other.col;
		int dy2 = this.row + 50 - other.row;
		int distanceSquared2 = dx2 * dx2 + dy2 * dy2;

		int dx3 = this.col + 50 - other.col;
		int dy3 = this.row - other.row;
		int distanceSquared3 = dx3 * dx3 + dy3 * dy3;

		int dx4 = this.col + 50 - other.col;
		int dy4 = this.row + 50 - other.row;
		int distanceSquared4 = dx4 * dx4 + dy4 * dy4;

		int min =  Math.min(Math.min(distanceSquared,Math.min(distanceSquared2,distanceSquared3)),distanceSquared4);
		return min > maxDistanceSquared;
	}

	/**
     * Returns a hash code value for this position
     * @return A hash code value for this object
     */
    @Override
    public int hashCode() {
        return 31 * row + col;
    }

	/**
     * Returns a string representation of the position
     * @return String in format "[col, row]"
     */
    @Override
    public String toString() {
        return "[" + col + ", " + row + "]";
    }

}
