# include <iostream>
# include <fstream>
# include <conio.h>
using namespace std;

struct Game
{
    char **maze;
    int rows,cols;
    int current_x,current_y;
    int steps;
};

int bestSteps[2]={9999,9999};

void printMazeInstructions()
 {
    cout << "==============================\n";
    cout << "        MAZE GAME RULES       \n";
    cout << "==============================\n\n";

    cout << "Welcome to the Maze Game!\n\n";
    cout << "Your goal:\n";
    cout << " - Navigate through the maze and reach the exit.\n\n";

    cout << "Controls:\n";
    cout << " - Use W : Move Up\n";
    cout << " - Use S : Move Down\n";
    cout << " - Use A : Move Left\n";
    cout << " - Use D : Move Right\n\n";

    cout << "Rules:\n";
    cout << " - You cannot walk through walls (# symbols).\n";
    cout << " - Follow pathways to reach the exit (usually marked as 'E').\n";
    cout << " - Your player is represented by '@'.\n\n";

    cout << "Tips:\n";
    cout << " - Look for open paths; avoid dead ends and traps.\n";
    cout << " - Be patient and plan your moves.\n\n";

    cout << "Good luck, and enjoy the maze!\n";
    cout << "==============================\n";
}


void loadScores()
{
    ifstream fin;
    fin.open("scores.txt");

    if (!fin.is_open()) {
        cout << "File does not exist. Default scores will be used.\n";
        return;
    }


    int i = 0;
    while (!fin.eof() && i < 2)
    {
        fin >> bestSteps[i];
        if (fin.fail())
         break;
        i++;
    }

    fin.close();
}

void saveScores()
{
    ofstream fout;
    fout.open("scores.txt");

    if (!fout.is_open()) {
        cout << "Error saving scores!\n";
        return;
    }

    fout << bestSteps[0] << endl;
    fout << bestSteps[1] << endl;

    fout.close();
}

void drawMaze(Game *g,int level)
{
    system("cls");
    cout<<"LEVEL "<<level<<" Steps: "<<g->steps<<"\n";
    cout<<"Best Steps for level "<<level<<": ";

    if (bestSteps[level-1]==9999)
        cout<<"No record\n\n";
    else
        cout<<bestSteps[level-1]<<" steps\n\n";

    for (int r=0;r < g->rows;r++)
    {
        for (int c=0;c < g->cols;c++)
        {
            if(r==g->current_x && c==g->current_y)
                cout<<'@';
            else
                cout<<g->maze[r][c];
        }
        cout<<"\n";
    }
}

bool movePlayer(Game *g,int new_x,int new_y,int level)
{
    char cell=g->maze[new_x][new_y];
    if(cell=='#')
        return false;
    if(cell=='T')
    {
        g->steps++;
        g->current_x = new_x;
        g->current_y = new_y;

        drawMaze(g,level);
        cout<<"\nYOU STEPPED ON A TRAP! GAME OVER\n";
        return true;
    }
    if(cell=='E')
    {
        g->steps++;
        g->current_x = new_x;
        g->current_y = new_y;

        drawMaze(g,level);
        cout<<"\nCONGRATULATIONS! YOU ESCAPED THE MAZE \n";
        if(g->steps < bestSteps[level-1])
            bestSteps[level-1]=g->steps;

        saveScores();
        return true;
    }
    g->steps++;
    g->current_x = new_x;
    g->current_y = new_y;
    return false;
}

void playLevel(const char *layout[],int r,int c,int level)
{
    Game g;
    g.rows=r;
    g.cols=c;
    g.maze=new char*[r];

    for(int i=0;i<r;i++)
        g.maze[i]=new char[c];

    for(int i=0;i<r;i++)
    {
       for(int j=0;j<c;j++)
       {
           char ch=layout[i][j];
           g.maze[i][j]=ch;
           if(ch=='S')
           {
                g.current_x=i;
                g.current_y=j;
                g.maze[i][j]=' ';
           }
       }
    }
    g.steps=0;
    drawMaze(&g,level);
    while(true)
    {
        char ch=_getch();
        ch=tolower(ch);

        int new_x = g.current_x, new_y = g.current_y;
        if(ch=='w')
            new_x--;
        else if(ch=='s')
            new_x++;
        else if(ch=='a')
            new_y--;
        else if(ch=='d')
            new_y++;
        else
            continue;

        if(new_x<0 || new_y<0 || new_x >= g.rows || new_y >= g.cols)
            continue;

        bool end=movePlayer(&g,new_x,new_y,level);
        if(end)
        {
            cout<<"Press any key...";
            _getch();
            break;
        }
        drawMaze(&g,level);
    }
    for(int i=0;i<r;i++)
    {
        delete[] g.maze[i];
    }
    delete[] g.maze;

}

int main()
{
    loadScores();
    const char *level1[] ={
        "####################",
        "#S     #       T   E",
        "# #### #   T     ###",
        "#      #          ##",
        "#### #   #######  ##",
        "#      T        T  #",
        "###########    #####",
        "#       T         ##",
        "#    #######       #",
        "####################"
    };

   const char *level2[] ={
        "##########################",
        "#S     #      T      #  E#",
        "# ###  #######  ## ###   #",
        "#   T      #     T       #",
        "##### #### # ########### #",
        "#   T             T      #",
        "# ###########  ###########",
        "#     T     #      T     #",
        "#########   #######    ###",
        "##########################"
    };
    while(true)
    {
        system("cls");
        cout<<"======MAZE GAME======\n";
        cout<<"1. Instructions\n";
        cout<<"2. Play Level 1\n";
        cout<<"3. Play Level 2\n";
        cout<<"4. View Scores\n";
        cout<<"5. Exit\n";
        cout<<"Choose: ";

        char op=_getch();
        cout<<op<<"\n";

        if(op=='1')
        {
            system("cls");
            printMazeInstructions();
            system("pause");
        }
        else if (op=='2')
        {
            playLevel(level1,10,20,1);

        }
        else if(op=='3')
        {
            playLevel(level2,10,26,2);

        }
        else if(op=='4')
        {
            system("cls");
            cout<<"==== BEST SCORES ====\n";
            cout<<"Level 1: ";
            if(bestSteps[0]==9999)
                cout<<"No record\n";
            else
                cout<<bestSteps[0]<<" steps\n";

            cout<<"Level 2: ";
            if(bestSteps[1]==9999)
                cout<<"No record\n";
            else
                cout<<bestSteps[1]<<" steps\n";

            cout<<"\nPress any key...";
            _getch();
        }

        else if(op=='5')
            break;
        else
        {
            cout<<"Invalid!\n";
            _getch();
        }
    }
    return 0;

}
