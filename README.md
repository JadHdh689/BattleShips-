•	Difficulty chosen is based on whether or not misses appear on the grid.

There are 4 grids: 
public grid for player 1
public grid for bot 
private grid for player 1
private grid for bot

Additional grids:
SmokeScreenP1 --> for player
SmokeScreenP2 --> for bot
SunkShipsGrid --> the bot uses it to sink ships that have been hit but not fully sunk.



initialize all the grids and place the ships 
For the player:  place all ships and do moves based on what the player chooses.



For bot:

•	First, the bot places the ships at random validly without any issues.


•	When choosing the move that the bot chooses based on the following steps:

1. The algorithm first gives priority to Torpedo. 
If a torpedo can be done, the bot must choose it. 

2. Then the priority goes to Artillery. If an Artillery can be done, then the bot chooses it. 

3. Then, if the bot enters a hit phase (in which the bot has sunk a part of a ship) the bot chooses fire to continue the phase. 

4. Then, the bot has a 50% chance of choosing fire, 25% chance of choosing smoke, and 25% chance of choosing radar. The smoke and radar have 25% chance each if the bot still can do any of them. Else, the bot chooses fire.


•	For the moves Fire, Artillery, and Radar, the bot utilizes two strategies to choose which coordinates to fire at:

1. The first strategy is called Parity, and it is used at the beginning of the game until the player’s carrier (5 cells long) is sunk. Parity selects 20 specific cells such that firing at all these cells guarantees a hit on the carrier. This works because the carrier takes up 5 consecutive cells. After the carrier is sunk, the program switches to the other strategy which is called Probability.


2. In the probability strategy, the bot will fire at the cell that has the highest probability of the entire player grid. Probabilities of each cell are calculated based on how many different permutations of ships that would occupy this cell, so the more ships you can place, the higher the probability is. 


•	The bot calculates coordinates for the next fire differently when it reaches a hit phase.
•	If the bot finds a ship that has not been sunk but has parts that have been sunk, the bot enters a hit phase on a cell that has been sunk, which helps the bot sink the ship.


•	Whenever the bot hits a ship successfully for the first time, it enters a phase where it would focus on destroying the ship that it just hit. 

In the phase, the bot will fire at the 4 adjacent cells of the last hit, hitting in order from highest probability to lowest, until it gets another successful hit. Then it would keep going in that direction as it keeps destroying parts of the ship until it can’t anymore (out of bounds) or it hits water, which is in when it goes back to the original hit cell (the first hit) and begins the cycle again by choosing another adjacent cell. This phase stops when the ship has been sunk. Note that when a cell is fired on, it will always have a probability of 0, meaning we cannot revisit an adjacent cell that has been fired on.


•	For both these strategies, if there had been a recent Radar used successfully (ships found successfully),
the bot would fire at the 4 cells, from highest probability to lowest, until it gets a hit. After it gets a hit, it enters the regular hit phase to make sure it sinks the ship. 




•	 For the move Smoke Screen, the bot will place the smoke screen over the first unsmoked cell it finds that has a standing ship.


•	 For the move Torpedo, the bot will calculate the sum of probabilities of the cells in each row and column and fire at the one with the highest probability. 






