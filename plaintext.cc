"""
 *****************************************************************************
   FILE: Lab 5      

   AUTHOR: Wlajimir Alexis      

   ASSIGNMENT: ConnectFour.py

   DATE: Novenmber 05, 2010       

   DESCRIPTION: This is a program that runs the user through a game of \
                Connect Four.

 *****************************************************************************
"""
from cs1graphics import *

class BoardSlot:

    """ Representing a board slot """

    def __init__(self, upperleft, slotWidth, rows, cols, label):

        """ Creates the initial board and Labels """

        # Initializing player
        self._player = "Empty"

        # Unpacks all of the list of lists sent by make board
        self._slot, self._boardCordinates, self._boardImages = \
                    makeBoard(upperleft, slotWidth, rows, cols)

        # Finds p1 and p2 of buttons on the board
        p1 = []
        p2 = []
        for i in self._boardCordinates:
            p1.append(Point(i[0].getX(), i[0].getY() - 40))
            p2.append(Point(i[0].getX() + slotWidth, i[0].getY()))
    
        # sets self._label equal to a list of labels
        self._label = makeButton(p1, p2, label)


    def getGraphics(self):
        """ This returns the board and buttons to the main to be added to\
            Canvas """

        return self._boardImages, self._label
    

    def getWinner(self):
        """ Finding the winner """

        # Calls a function to find winner then returns to the main function
        
        self._player = winner(self._slot)
        return self._player
    

    def setPlayer(self, player, columnPress):
        """ Changes player, only if not changed before """

        row = -1
        self._player = player

        # Returns done is board is full
        count = 0
        for num in range(len(self._slot)):
            if self._slot[num][0] != "Empty":
                count += 1

        if count == len(self._slot):
            return "done"

        # Returns none if board slot is full
        if self._slot[columnPress][0] != "Empty":
            return "none"
           
        
        # Find the column number that is pressed and returns to the user
        for i in self._slot[columnPress]:
            if i == "Empty":
                row = row + 1
        # Sets the "Empty player" equal to self._player  
        self._slot[columnPress][row] = self._player

        # Returns the row 
        return row
    

    def addPeices(self, columnPress, row, slotWidth):

        """ This creates peice for the board """

        # Finds the cordinates of a particular slot
        cord = self._boardCordinates[columnPress][row]

        # Finding the ul and lr of a bounding box
        p1 = Point(cord.getX(), cord.getY())
        p2 = Point(cord.getX() + slotWidth, cord.getY() + slotWidth)
        
        # Finding the radius of a circle
        radius = (p2.getX() - p1.getX()) / 2

        # Finding the midPoints of the bounding box
        midPointX = (p2.getX() + p1.getX()) / 2
        midPointY = (p2.getY() + p1.getY()) / 2

        # Creating the peice and sending it to the main function
        circ = Circle( radius, Point(midPointX, midPointY))
        circ.setFillColor(self._player)
        circ.setDepth(1)
        circ.setBorderWidth(2)
        return circ
    

    def buttonClick(self, win):
        """ Checks if a button is clicked """

        # Calls a function none or the column that was pressed
        columnPress = getButtonPress(self._label, win)

        # returns the column pressed back to the main function
        return columnPress

    def getPlayer(self):
        """ Return the player for this slot """

        return self._player
    

def makeBoard(upperleft, slotWidth, rows, cols):
 
    """ Yes this functions returns a board and all of its charactereistics in\
        lists of lists"""

    # List of players
    playerList = []
    # List of slot cordinates
    cordinatesList = []
    # List of image layers
    imageList = []

    # Creating the layer of a board
    for num in range(cols):
        # Creating random list to be appended to the important list 
        someList = []
        someList2 = []
        someList3 = []
        for num1 in range(rows):
            xx = upperleft.getX() + (num * slotWidth)
            yy = upperleft.getY() + (num1 * slotWidth)
            ul = Point(xx, yy)
            lr = Point(xx + slotWidth, yy + slotWidth)
            # Creating a layer of the board
            slot = Layer()
            rect = Rectangle(slotWidth, slotWidth, \
                                  Point(((ul.getX() + lr.getX()) / 2), \
                                        ((ul.getY() + lr.getY()) / 2)))
            rect.setFillColor("yellow")
            slot.add(rect)
            circ = Circle(slotWidth / 2, Point(((ul.getX() + lr.getX()) / 2), \
                                           ((ul.getY() + lr.getY()) / 2)))
            slot.add(circ)
            circ.setDepth(4)
            circ.setFillColor("white")
            # Appending data to empty list
            someList.append("Empty")
            someList2.append(ul)
            someList3.append(slot)
        # Creating three lists of lists to be returned to class
        playerList.append(someList)
        cordinatesList.append(someList2)
        imageList.append(someList3)
    return playerList, cordinatesList, imageList


def makeButton(p1, p2, label): 
    """ Creates and returns a layer of graphics objects """

    # Empty
    labelList = []
   
    # Creates a layer of all the buttons
    for i in range(len(p1)):
        button = Layer()
        rect = Rectangle(p2[i].getX() - p1[i].getX(), \
                         p2[i].getY() - p1[i].getY(), \
                         Point((p2[i].getX() + p1[i].getX())/ 2.0, \
                               (p2[i].getY() + p1[i].getY())/ 2.0))
        rect.setFillColor("yellow")
        rect.setDepth(10)
        button.add(rect)

        label1 = Text(label, 18)
        label1.moveTo((p2[i].getX() + p1[i].getX())/ 2.0, \
                     (p2[i].getY() + p1[i].getY())/ 2.0)
        label1.setDepth(1)
        button.add(label1)
        
        button.adjustReference(p1[i].getX(), p1[i].getY())

        labelList.append(button)
    # Returns a list of layer of buttons    
    return labelList


def getButtonPress(buttons, win):
    """ Returns the index of the button in the buttons list in """
 
    columnPress = -1
    ev = win.wait().getMouseLocation()
    for columns in buttons:
        ul = columns.getReferencePoint()
        columnPress = columnPress + 1
        if ev.getX() > ul.getX() and ev.getX() < (ul.getX() + 70) and \
           ev.getY() > ul.getY() and ev.getY() < (ul.getY() + 45):
            return columnPress
    return "none"


def winner(boardSlots):

    """ This functions checks every possible win solution and returns none\
        if there is not a winner and the color if so."""

    # Checking all the horizontal possibilities
    didWin = winHorizontal(boardSlots)

    # If horizontal has winner return
    if didWin != "none":
        return didWin
    
    # Checking all the vertical possibilities
    didWin = winVertical(boardSlots)
    
    # If horizontal has winner return
    if didWin != "none":
        return didWin
    
    # Checking all the diagnal possibilities
    didWin = winDiagnal(boardSlots)

    # return either none or diagnal's winner
    return didWin


def winHorizontal(boardSlots):
    """ Finds if someone wins horizontally """

    # Find Horizontal for Red
    for i in range(len(boardSlots[0])):
        count = 0
        for player3 in boardSlots:
            if player3[i] == "red":
                count += 1
                if count == 4:
                    return "red"
            else:
                count = 0

    # Find Horizontal for Black
    for i in range(len(boardSlots[0])):
        count = 0
        for player4 in boardSlots:
            if player4[i] == 'black':
                count += 1
                if count == 4:
                    return 'black'
                
            else:
                count = 0
    # return no winner
    return "none"


def winVertical(boardSlots):
    """ Find if someone wins vertically """

    # Find Vertical for Black
    for i in range(len(boardSlots)):
        count = 0
        for player2 in boardSlots[i]:
            if player2 == "black":
                count += 1
                if count == 4:
                    return "black"
            else:
                count = 0

    # Find Vertical for Red
    for i in range(len(boardSlots)):
        count = 0
        for player1 in boardSlots[i]:
            if player1 == "red":
                count += 1
                if count == 4:
                    return "red"
            else:
                count = 0
    # return no winner
    return "none"


def winDiagnal(boardSlots):
    """ Finds if someone wins horizontally """

    # Find diagnal coming from the left to right
    didWin = winDiagRight(boardSlots)
    # There is a winner return
    if didWin != "none":
        return didWin
    
    # Find diagnal coming from the right to left
    didWin = winDiagLeft(boardSlots)
    # If there is a winner return
    if didWin != "none":
        return didWin

    # return whether or not there is a winner
    return didWin


def winDiagRight(boardSlots):
    """ Find the winners going to the right """

    # Empty List
    ranges = []
    # Finding the range of possibilities of diagnal wins for a board
    for i in range(len(boardSlots)):
        if i >= 4:
            ranges.append(i)

    # Find the diagnal wins for red then for black
    for iii in ranges:
        num = -1
        count = 0
        for _ in range(iii):
            num += 1
            if boardSlots[num][len(boardSlots) - iii + num] == "red":
                count += 1
                if count == 4:
                    return "red"
            else:
                count = 0
  
    for iii in ranges:
        num = -1
        count = 0
        for _ in range(iii):
            num += 1
            if boardSlots[-num][len(boardSlots) - iii + num] == "black":
                count += 1
                if count == 4:
                    return "black"
            else:
                count = 0
                
    didWin = winDiagRight2(boardSlots)

    # Returning whether or not there is a winner
    return didWin


def winDiagRight2(boardSlots):
    """ Finding the rest of the right diagnal possibilites """
    
    count = 0
    for i in range(len(boardSlots)):
        for ii in range(len(boardSlots) - i):
            if boardSlots[ii][ii] == "red":
                count += 1
                if count == 4:
                    return "red"
            else: count = 0

    count = 0
    for i in range(len(boardSlots)):
        for ii in range(len(boardSlots) - i):
            if boardSlots[ii][ii] == "black":
                count += 1
                if count == 4:
                    return "black"
            else: count = 0

    return "none"


def winDiagLeft(boardSlots):
    """ Finding if there is a winner from right to left """
    ranges = []

    # Finding the range of possibilities of diagnal wins for a board
    for i in range(len(boardSlots)):
        if i >= 4:
            ranges.append(i)

    for iii in ranges:
        num = -1
        count = 0
        for _ in range(iii):
            num += 1
            if boardSlots[-num][len(boardSlots) - iii + num] == "black":
                count += 1
                if count == 4:
                    return "black"
            else:
                count = 0

    for iii in ranges:
        num = -1
        count = 0
        for _ in range(iii):
            num += 1
            if boardSlots[-num][len(boardSlots) - iii + num] == "red":
                count += 1
                if count == 4:
                    return "red"
            else:
                count = 0

    didWin = winDiagLeft2(boardSlots)

    return didWin
           
   
def winDiagLeft2(boardSlots):
    """ Finding the rest of the left diagnal possibilites """

    count = 0
    for i in range(-len(boardSlots)):
        for ii in range(len(boardSlots) - i):
            if boardSlots[-ii][-ii] == "red":
                count += 1
                if count == 4:
                    return "red"
            else: count = 0

    count = 0
    for i in range(len(boardSlots)):
        for ii in range(len(boardSlots) - i):
            if boardSlots[-ii][-ii] == "black":
                count += 1
                if count == 4:
                    return "black"
            else: count = 0

    return "none"

def addGraphics(win, boardGraphic, labelList):
    """ This funtion adds the graphics to the win """

    # adds the board to the Canvas
    for i in range(len(boardGraphic)):
        for pic in boardGraphic[i]:
            win.add(pic)
            
    # Adds the buttons to the Canvas
    for button in labelList:
        win.add(button)
        
def main1():
    """ This functions runs a ConnectFour Game """

    # Finding the user inputs
    upperLeftX = int(raw_input("What is the x of the upperLeft of the board: "\
                               ))
    upperLeftY = int(raw_input("What is the y of the upperLeft of the board: "\
                               ))
    upperLeft = Point(upperLeftX, upperLeftY + 40)
    rows = int(raw_input("How many rows: "))
    cols = int(raw_input("How many columns: "))
    slotWidth = 70
    label = "Click Me"

    # Setting the Canvas to match the size of the board the the upper Left
    win = Canvas((cols * slotWidth + upperLeft.getX()), \
                 (rows * slotWidth + upperLeft.getY()), \
                 "white", "Connect Four")
    win.setAutoRefresh(False)

    # initializes the board
    board = BoardSlot(upperLeft, slotWidth, rows, cols, label)

    # Gets the graphics for the board
    boardGraphic, labelList = board.getGraphics()

    # Adds the board to the Canvas
    addGraphics(win, boardGraphic, labelList)
    # Allows user to see Graphics
    win.refresh()
    
    print "It is red's turn"

    # Creating list to keep up with the player for loop
    play = ["red", "black"]
    playCount = 0
    index = 0
 
    done = False
    while not done:
        # See if a player has won. Returns player there is a winner.
        player = board.getWinner()
        if  player == "none":
            # Get the column the user has clicked. 
            columnPress = board.buttonClick(win)
            # If a column is clicked find the row and set the player
            if columnPress != "none":
                row = board.setPlayer(play[index], columnPress)
                # If the slot is full tell the user
                if row == "none":
                    print "This column is full, pick another"
                # If the board is full end the loop
                elif row == "done":
                    print "This board is full"
                    done = True
                # If row is giving add the peice to the Canvas
                else:
                    peices = board.addPeices(columnPress, row, slotWidth)
                    win.add(peices)
                    playCount += 1
                    index1 = playCount % 2
                    index = index1
                    print "It is", play[index] + "'s turn"
                    win.refresh()                        
        else:
            done = True
            print "The winner is", player + "!!!"

    
    done = False
    while not done:
        if (win.wait().getDescription()) == "mouse click": 
            done = True
    win.close()

       
if __name__ == "__main__":
    main1()





    
