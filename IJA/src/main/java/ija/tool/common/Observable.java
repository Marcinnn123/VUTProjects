package ija.tool.common;


public interface Observable {

	void addObserver(Observer o);

	void notifyObservers();

	public interface Observer {
	
		void update(Observable o);
	}
}
