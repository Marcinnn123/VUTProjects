# VUT FIT 2023 - ITU
# Minesweeper
# Marcin Sochacki
# xsocha03
#
import pygame
from game_logic import *

pygame.init()
pygame.font.init()

class Game:
    def __init__(self):
        self.screen = pygame.display.set_mode((WIDTH, HEIGHT))
        pygame.display.set_caption(TITLE)
        self.clock = pygame.time.Clock()
        self.play_button = pygame.Rect(WIDTH // 2 - 50, HEIGHT // 2 - 30, 100, 50)
        self.playing = False
        self.win = False
        self.game_over = False
        self.start_time = 0
        self.menu_button = pygame.Rect(10, HEIGHT - NAVBAR_HEIGHT + 10, 80, 30)
        self.play_again_button = pygame.Rect(WIDTH - 90, HEIGHT - NAVBAR_HEIGHT + 10, 80, 30)
        self.mine_button_10 = pygame.Rect(WIDTH // 2 - 180, HEIGHT // 2 , 100, 30)
        self.mine_button_20 = pygame.Rect(WIDTH // 2 - 50, HEIGHT // 2 , 100, 30)
        self.mine_button_30 = pygame.Rect(WIDTH // 2 + 80, HEIGHT // 2 , 100, 30)
        self.quit_button = pygame.Rect(WIDTH // 2 - 50, HEIGHT // 2 + 30, 100, 150)
        self.mines = 10
        
    ## View
    
    def draw_play_button(self):
        if not self.playing:
            # Draw background
            
            for row in range(0, HEIGHT, 40):
                for col in range(0, WIDTH, 40):
                    pygame.draw.rect(self.screen, (200, 200, 200) if (row // 40 + col // 40) % 2 == 0 else (220, 220, 220),
                                     (col, row, 40, 40))

            # Draw play button
            font = pygame.font.Font(None, 72)
            play_text = font.render("Play", True, (255, 255, 255))
            play_rect = play_text.get_rect(center=(WIDTH // 2, HEIGHT // 2 - 100))

            pygame.draw.rect(self.screen, (0, 255, 0), play_rect, 0)
            self.screen.blit(play_text, play_rect)
            self.play_button = play_rect

            font = pygame.font.Font(None, 40)
            # Draw mine selection buttons
            # If button is selected change its color
            mine_button_text_10 = font.render("Easy", True, (255, 255, 255))
            mine_button_text_rect_10 = mine_button_text_10.get_rect(center=(self.mine_button_10.centerx, self.mine_button_10.centery))
            if self.mines == 10:
                pygame.draw.rect(self.screen, (0, 0, 0), self.mine_button_10, 0)
            else:
                pygame.draw.rect(self.screen, (100, 100, 100), self.mine_button_10, 0)
            self.screen.blit(mine_button_text_10, mine_button_text_rect_10)
            self.mine_button_10 = mine_button_text_rect_10

            mine_button_text_20 = font.render("Medium", True, (255, 255, 255))
            mine_button_text_rect_20 = mine_button_text_20.get_rect(center=(self.mine_button_20.centerx, self.mine_button_20.centery))
            if self.mines == 20:
                pygame.draw.rect(self.screen, (0, 0, 0), self.mine_button_20, 0)
            else:
                pygame.draw.rect(self.screen, (100, 100, 100), self.mine_button_20, 0)
            self.screen.blit(mine_button_text_20, mine_button_text_rect_20)
            self.mine_button_20 = mine_button_text_rect_20

            mine_button_text_30 = font.render("Hard", True, (255, 255, 255))
            mine_button_text_rect_30 = mine_button_text_30.get_rect(center=(self.mine_button_30.centerx, self.mine_button_30.centery))
            if self.mines == 30:
                pygame.draw.rect(self.screen, (0, 0, 0), self.mine_button_30, 0)
            else:
                pygame.draw.rect(self.screen, (100, 100, 100), self.mine_button_30, 0)  
            self.screen.blit(mine_button_text_30, mine_button_text_rect_30)
            self.mine_button_30 = mine_button_text_rect_30

            font = pygame.font.Font(None, 72)
            # Draw quit button
            quit_text = font.render("Quit", True, (255, 255, 255))
            quit_rect = quit_text.get_rect(center=(self.quit_button.centerx, self.quit_button.centery))
            pygame.draw.rect(self.screen, (255, 0, 0), self.quit_button, 0)
            self.screen.blit(quit_text, quit_rect)
            self.quit_button = quit_rect
            
            # Select cursor shape
            mx, my = pygame.mouse.get_pos()
            if self.play_button.collidepoint(mx, my):
                pygame.mouse.set_cursor(*pygame.cursors.tri_left)
            elif self.mine_button_10.collidepoint(mx, my):
                pygame.mouse.set_cursor(*pygame.cursors.tri_left)
            elif self.mine_button_20.collidepoint(mx, my):
                pygame.mouse.set_cursor(*pygame.cursors.tri_left)
            elif self.mine_button_30.collidepoint(mx, my):
                pygame.mouse.set_cursor(*pygame.cursors.tri_left)
            elif self.quit_button.collidepoint(mx, my):
                pygame.mouse.set_cursor(*pygame.cursors.tri_left)
            else:
                pygame.mouse.set_cursor(*pygame.cursors.arrow)

    def draw_end_screen(self):
        font = pygame.font.Font(None, 72)
        # Display win/loose message
        if self.win:
            end_text = font.render("You won!", True, (0, 255, 0))
        else:
            end_text = font.render("You lost!", True, (255, 0, 0))
    
        # Get the size of the text
        text_width, text_height = font.size("You won!" if self.win else "You lost!")
    
        pygame.draw.rect(self.screen, (0, 0, 0), (WIDTH // 2 - text_width // 2, HEIGHT // 2 - text_height // 2, text_width, text_height))
    
        end_rect = end_text.get_rect(center=(WIDTH // 2, HEIGHT // 2))
        self.screen.blit(end_text, end_rect)
    
        pygame.display.flip()
        
    def draw_navbar(self):
        pygame.draw.rect(self.screen, (100, 100, 100), (0, HEIGHT - NAVBAR_HEIGHT, WIDTH, NAVBAR_HEIGHT))
        self.draw_time()

        # Draw "Menu" button
        font = pygame.font.Font(None, 30)
        menu_text = font.render("Menu", True, (255, 255, 255))
        menu_rect = menu_text.get_rect(center=(self.menu_button.centerx, self.menu_button.centery))
        pygame.draw.rect(self.screen, (0, 255, 0), self.menu_button, 0)
        self.screen.blit(menu_text, menu_rect)

        # Draw "Restart" button
        play_again_text = font.render("Restart", True, (255, 255, 255))
        play_again_rect = play_again_text.get_rect(center=(self.play_again_button.centerx, self.play_again_button.centery))
        pygame.draw.rect(self.screen, (0, 255, 0), self.play_again_button, 0)
        self.screen.blit(play_again_text, play_again_rect)

    def draw_time(self):
        current_time = pygame.time.get_ticks()
        elapsed_time = (current_time - self.start_time) // 1000  # Convert milliseconds to seconds

        font = pygame.font.Font(None, 36)
        time_text = font.render(f"Time: {elapsed_time}", True, (255, 255, 255))
        time_rect = time_text.get_rect(center=(WIDTH // 2, HEIGHT - NAVBAR_HEIGHT // 2))
        self.screen.blit(time_text, time_rect)
        
    def draw(self):
        self.screen.fill((40, 40, 40))
        self.table.draw(self.screen)
        self.draw_navbar() 
        pygame.display.flip()
    
    ## Controler

    def new(self, mines):
        self.table = Table(mines)
        self.start_time = pygame.time.get_ticks()

    def run(self):
        while not self.playing:
            self.events()
            self.draw_play_button()
            pygame.display.flip()

        self.playing = True
        while self.playing:
            self.events()
            self.draw()
        else:
            self.end_screen()
        return self.mines

    def check_win(self):
        # Check if all non mines are shown
        for row in self.table.table_list:
            for tile in row:
                if tile.type != "MINE" and not tile.shown:
                    return False
        return True
    
    def events(self):
        for event in pygame.event.get():
            if event.type == pygame.QUIT:
                pygame.quit()
                quit(0)
    
            if event.type == pygame.MOUSEBUTTONDOWN:
                mx, my = pygame.mouse.get_pos()
    
                if event.button == 1:
                    if not self.playing:
                        # Handle clicks when not playing
                        if self.play_button.collidepoint(mx, my):
                            self.playing = True
                            return
                        elif self.mine_button_10.collidepoint(mx, my):
                            self.mines = 10
                            self.new(self.mines)
                            return
                        elif self.mine_button_20.collidepoint(mx, my):
                            self.mines = 20
                            self.new(self.mines)
                            return
                        elif self.mine_button_30.collidepoint(mx, my):
                            self.mines = 30
                            self.new(self.mines)
                            return
                        elif self.quit_button.collidepoint(mx, my):
                            pygame.quit()
                            quit(0)
    
                    if self.playing:
                        # Handle clicks during the game
                        if 0 <= mx < COLS * SQUARE_SIZE and HEIGHT - NAVBAR_HEIGHT <= my < HEIGHT:
                            if self.menu_button.collidepoint(mx, my):
                                self.playing = False
                                self.win = False
                                self.game_over = False  
                                return # Exit the loop
                            elif self.play_again_button.collidepoint(mx, my):
                                self.playing = True
                                self.win = False
                                self.game_over = False  
                                self.new(self.mines)
                                return # Exit the loop
    
                        mx //= SQUARE_SIZE
                        my //= SQUARE_SIZE
    
                        if 0 <= mx < COLS and 0 <= my < ROWS:
                            if not self.table.table_list[mx][my].flagged and not self.table.table_list[mx][my].shown:
                                # Left-click to dig if not flagged and not shown
                                if not self.table.process_cell(mx, my):
                                    for row in self.table.table_list:
                                        for tile in row:
                                            if tile.flagged and tile.type != "MINE":
                                                tile.flagged = False
                                                tile.shown = True
                                                tile.image = square_not_mine
                                            elif tile.type == "MINE":
                                                tile.shown = True
                                    self.playing = False
                                    self.win = False
                                    self.game_over = True  # Set game over state when the player loses
    
                elif event.button == 3:
                    if self.playing:
                        # Handle right-clicks during the game
                        mx //= SQUARE_SIZE
                        my //= SQUARE_SIZE
    
                        if 0 <= mx < COLS and 0 <= my < ROWS:
                            if not self.table.table_list[mx][my].shown:
                                self.table.table_list[mx][my].flagged = not self.table.table_list[mx][my].flagged
    
                if self.check_win():
                    self.win = True
                    self.playing = False
                    self.game_over = True

    

    def end_screen(self):
        while self.game_over:
            self.draw_end_screen()
            for event in pygame.event.get():
                if event.type == pygame.QUIT:
                    pygame.quit()
                    quit(0)
    
                if event.type == pygame.MOUSEBUTTONDOWN:
                    mx, my = pygame.mouse.get_pos()
    
                    # Use the navbar buttons for the end screen actions
                    if self.menu_button.collidepoint(mx, my):
                        self.playing = False
                        self.win = False
                        self.game_over = False  # Exit the loop
                        return
                    elif self.play_again_button.collidepoint(mx, my):
                        self.playing = True
                        self.win = False
                        self.game_over = False  # Exit the loop
                        return

    

mines = 10
game = Game()
while True:
    game.new(mines)
    mines = game.run()

    