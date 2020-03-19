# Word-Shooter

![Image description](https://i.imgur.com/ONvi0hN.png)



2D game like bubble shooter. The only difference is when a word is created, it vanishes and you get the score.

This is compatible with visual studio 2013 on windows. If you want to run it on ubuntu or any other visual studio version there might be some changes to do before running it, and some of them are listed below:


1. Simply run the terminal and go to directory which contains the file downloaded file “install libraries.sh”. 

      ```bash install-libraries.sh```

2. Run the command

3. Provide the password and wait for the libraries to be installed. If you get an error that libglew1.6-dev
cannot be found, try installing an older version, such as libglew1.5-dev by issuing following on command
line
      ```sudo-apt-get install libglew1.5-dev```
      
4. If you have any other flavour of Linux. You can follow similar procedure to install “OpenGL” libraries.


## Drawing Board and Shapes
  ###### Canvas
  For defining the canvas size you will be calling the function “SetCanvas” (see below) and providing two parameters to set the   drawing-world width and height in pixels.
  
      void SetCanvasSize(int width, int height)
      
  ###### Drawing Primitives
    //Draws a specific alphabet at given position coordinate
    //sx = position of x-axis from left-bottom
    //sy = position of y-axis from left-bottom
    //awidth= width of displayed alphabet in pixels
    //aheight= height of displayed alphabet pixels. ```
    
    void DrawAlphabet(const Alphabets &cname, int sx, int sy, int cwidth=60,int cheight=60)
    
    // Function draws a string at given x,y coordinates
    void DrawString(int x, int y, int width, int height, const string&score,float*color)
    
    
    void DrawShooter(int sx, int sy, int cwidth = 60, int cheight = 60)


There is a list of 140 colors (see file util.h) which can be used for drawing strings of different
colors. Note that each color is combinations of three individual components red, green and blue and each
color is stored as a separate row in the two dimensional array.


  ######  Interaction with the Game
For the interaction with the game you will be using your mouse. Graphics library will call corresponding registered functions whenever mouse button is clicked. Below function is called whenever either mouse button is pressed or released.

      void MouseClicked(int button, int state, int x, int y);



## Game Rules

-The first two rows of the board are randomly selected alphabets.
- An alphabet will be present on the top of shot gun drawn at the bottom. This alphabet will get shot whenever user clicks on some area inside the board.
- An array named as dictionary is provided that contains 370099 words. Once the alphabet hits the user specified position to form a word that matches some word of dictionary it bursts the word and increases score.
- Burst all the alphabets that forms the word. Make sure to consider the maximum length of the word in order to score more. For example if on finding the word take, before bursting do check that either the next alphabet in the row is making it taken or not.
- Whenever the alphabet bursts, all the slots gets empty.
- One point is awarded on bursting of the one alphabet.
- The game will be played for only 2 min. After that, the alphabets will be disappeared from the screen and user will be shown with game over status.




