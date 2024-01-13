# VUT FIT 2023 - ITU
# Minesweeper
# Marcin Sochacki
# xsocha03
#
import random
import os
import pygame


class Square:
    def __init__(self, x, y, image, type, shown=False, flagged=False):
        # Convert grid coordinates to pixel coordinates
        self.x, self.y = x * SQUARE_SIZE, y * SQUARE_SIZE

        # Initialize attributes
        self.image = image
        self.type = type
        self.shown = shown
        self.flagged = flagged

    def draw(self, table_surface):
        # Draw the square based on its state
        if not self.flagged and self.shown:
            # If not flagged and shown, draw the image
            table_surface.blit(self.image, (self.x, self.y))
        elif self.flagged and not self.shown:
            # If flagged and not shown, draw the flag image
            table_surface.blit(square_flag, (self.x, self.y))
        elif not self.shown:
            # If not shown and not flagged, draw the unknown square image
            table_surface.blit(square_unknown, (self.x, self.y))

    def __repr__(self):
        # Return a string representation of the square's type
        return self.type



class Table:
    def __init__(self, amount_mines):
        self.table_surface = pygame.Surface((WIDTH, HEIGHT))
        self.table_list = [[Square(col, row, square_empty, "NONE") for row in range(ROWS)] for col in range(COLS)]
        self.processed = []
        self.mines_placed = False
        self.generate_mines(amount_mines)
        self.generate_numbers()

    def generate_mines(self, num_mines):
        if not self.mines_placed:
            # Get a list of empty cells
            empty_cells = [(x, y) for x in range(ROWS) for y in range(COLS) if self.table_list[x][y].type == "NONE"]
            # Choose mine locations randomly from the empty cells
            mine_locations = random.sample(empty_cells, min(num_mines, len(empty_cells)))

            # Place mines on the selected locations
            for x, y in mine_locations:
                self.table_list[x][y].image = square_mine
                self.table_list[x][y].type = "MINE"

            self.mines_placed = True


    def generate_numbers(self):
        for x in range(ROWS):
            for y in range(COLS):
                cell = self.table_list[x][y]

                if cell.type != "MINE":
                    # Count surrounding mines for each non-mine cell
                    total_mines = self.surrounding_control(x, y)

                    if total_mines > 0:
                        # Set the image and type for clue cells
                        cell.image = square_numbers[total_mines - 1]
                        cell.type = "CLUE"


    @staticmethod
    def is_inside(x, y):
        return 0 <= x < ROWS and 0 <= y < COLS

    def surrounding_control(self, x, y):
        total_mines = 0
        # Iterate through the neighboring cells
        for x_offset in range(-1, 2):
            for y_offset in range(-1, 2):
                neighbour_x = x + x_offset
                neighbour_y = y + y_offset
                # Check if the neighbor is inside the game table and contains a mine
                if self.is_inside(neighbour_x, neighbour_y) and self.table_list[neighbour_x][neighbour_y].type == "MINE":
                    total_mines += 1

        return total_mines

    def draw(self, screen):
        # Draw the board
        for row in self.table_list:
            for tile in row:
                tile.draw(self.table_surface)
        screen.blit(self.table_surface, (0, 0))

    def process_cell(self, x, y):
        if (x, y) in self.processed:
        # Cell has already been processed
            return True

        self.processed.append((x, y))
        cell = self.table_list[x][y]
        cell.shown = True

        if cell.type == "MINE":
            # The cell is a mine, set the image and return False
            cell.image = square_exploded
            return False
        elif cell.type == "CLUE":
            # The cell is a clue, return True
            return True

        # Process neighboring cells recursively
        for row in range(max(0, x-1), min(ROWS-1, x+1) + 1):
            for col in range(max(0, y-1), min(COLS-1, y+1) + 1):
                self.process_cell(row, col)

        return True

# Game stats
SQUARE_SIZE = 32
NAVBAR_HEIGHT = 2 * SQUARE_SIZE
ROWS = 15
COLS = 15
WIDTH = SQUARE_SIZE * ROWS
HEIGHT = (SQUARE_SIZE * COLS) + NAVBAR_HEIGHT
TITLE = "Minesweeper"

square_numbers = []
for i in range(1, 9):
    square_numbers.append(pygame.transform.scale(pygame.image.load(os.path.join("assets", f"Square_{i}.png")), (SQUARE_SIZE, SQUARE_SIZE)))

square_empty = pygame.transform.scale(pygame.image.load(os.path.join("assets", "Square_None.png")), (SQUARE_SIZE, SQUARE_SIZE))
square_exploded = pygame.transform.scale(pygame.image.load(os.path.join("assets", "Square_Exploded.png")), (SQUARE_SIZE, SQUARE_SIZE))
square_flag = pygame.transform.scale(pygame.image.load(os.path.join("assets", "Square_Flag.png")), (SQUARE_SIZE, SQUARE_SIZE))
square_mine = pygame.transform.scale(pygame.image.load(os.path.join("assets", "Square_Mine.png")), (SQUARE_SIZE, SQUARE_SIZE))
square_unknown = pygame.transform.scale(pygame.image.load(os.path.join("assets", "Square_Not_Shown.png")), (SQUARE_SIZE, SQUARE_SIZE))
square_not_mine = pygame.transform.scale(pygame.image.load(os.path.join("assets", "Square_Not_Mine.png")), (SQUARE_SIZE, SQUARE_SIZE))