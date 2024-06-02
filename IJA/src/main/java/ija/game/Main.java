package ija.game;

import ija.tool.EnvPresenter;
import javafx.application.Application;

import java.util.*;
import java.io.BufferedReader;
import java.io.FileReader;
import java.io.IOException;

/**
 * Main class to launch the application
 */
public class Main {

    /**
     * Main method to launch the graphical user interface
     * @param args Command line arguments
     */
    public static void main(String... args) {
        Application.launch(EnvPresenter.class); // Start the JavaFX application
    }

    /**
     * Nested class for loading parameters from a file
     */
    public static class ParametersLoader {
        private final String filePath;

        /**
         * Constructor for ParametersLoader
         * @param filePath Path to the configuration file
         */
        public ParametersLoader(String filePath) {
            this.filePath = filePath;
        }

        /**
         * Loads parameters from a file into a nested map structure
         * @return Map of parameters grouped by sectio
         * @throws IOException if the file cannot be read
         */
        public Map<String, Map<String, Integer>> loadParameters() throws IOException {
            Map<String, Map<String, Integer>> parameters = new HashMap<>();
            String currentSection = null;

            try (BufferedReader reader = new BufferedReader(new FileReader(this.filePath))) {
                String line;
                while ((line = reader.readLine()) != null) {
                    line = line.trim();
                    if (line.startsWith("[")) { // Start of a new section
                        currentSection = line.substring(1, line.length() - 1);
                        parameters.put(currentSection, new HashMap<>());
                    } else if (currentSection != null && !line.isEmpty()) {
                        String[] parts = line.split("=");
                        String key = parts[0].trim();
                        Integer value = Integer.valueOf(parts[1].trim());
                        parameters.get(currentSection).put(key, value);
                    }
                }
            }
            return parameters;
        }
    }
}

