// Snake Game

#include <iostream>
#include <stdlib.h>
#include <time.h>
#include <conio.h>

using namespace std;

#define rows 10
#define cols 15

struct Node
{
    int x, y;
    char value;
    Node *next;
} *head, *tail;

struct Food
{
    int x, y;
    char value = 'F';
} *food;

struct Snake
{
    Node * head = NULL;
    int length = 0;
};

Node * NewNode(int x, int y)
{
    Node *newNode = new Node();
    newNode->x = x;
    newNode->y = y;
    newNode->value = '#';
    newNode->next = NULL;
    return newNode;
}

void insert(Node **head_ref, Node *temp)
{
    Node *last = *head_ref;
    Node *newNode = NewNode(temp->x, temp->y);
    if (*head_ref == NULL)
    {
        *head_ref = newNode;
        return;
    }
    while (last->next != NULL)
    {
        last = last->next;
    }
    last->next = newNode;
    return;
}

bool checkExist(Snake *snake, int x, int y)
{
    Node **temp = &snake[x].head;
    Node *last = *temp;
    int count = snake[x].length;
    while (count--)
    {
        if (last->y == y)
        {
            return true;
        }
        last = last->next;
    }
    return false;
}

Food *createFood(Snake *snake)
{
    Food *newFood = new Food();
    do
    {
        srand(time(0));
        newFood->x = rand() % (rows - 1);
        newFood->y = rand() % (cols - 1);
    }
    while (checkExist(snake, newFood->x, newFood->y));
    return newFood;
}

char ** createBoard()
{
    char **board = new char*[rows];
    for (int i = 0; i < rows; i++)
    {
        board[i] = new char[cols];
        for (int j = 0; j < cols; j++)
        {
            board[i][j] = '_';
        }
    }
    return board;
}

void displayList(Node *head)
{
    while (head != NULL)
    {
        cout << head->x << " " << head->y << " " << head->value << endl;
        head = head->next;
    }
    cout << endl;
}

void displayBoard(char **board)
{
    for (int i = 0; i < rows; i++)
    {
        for (int j = 0; j < cols; j++)
        {
            cout << board[i][j] << '|';
        }
        cout << endl;
    }
}

void displaySnake(Snake *snake)
{
    cout << "Snake: \n";
    for (int i = 0; i < rows; i++)
    {
        cout << i << ": ";
        Node *temp = snake[i].head;
        while (temp != NULL)
        {
            cout << temp->x << " " << temp->y << " " << temp->value << "-->";
            temp = temp->next;
        }
        cout << endl;
    }
}

void saveNode(Snake *snake, Node *temp)
{
    // cout << temp->x << " " << temp->y << endl;
    if (checkExist(snake, temp->x, temp->y))
    {
        return;
    }

    // cout << "SaveNode:Before:Add:\n";
    // displayList(snake[temp->x].head);
    // cout << endl;

    insert(&snake[temp->x].head, temp);

    // cout << "SaveNode:After:Add:\n";
    // displayList(snake[temp->x].head);
    // cout << endl;

    snake[temp->x].length++;
    return;
}

void saveSnake(Snake *snake, Node **head_ref)
{
    Node *temp = *head_ref;
    while(temp != NULL)
    {
        saveNode(snake, temp);
        temp = temp->next;
    }
}

void deleteNode(Snake *snake, Node *current)
{
    Node **temp = &snake[current->x].head;
    Node *last = *temp;
    Node *prev = *temp;
    int count = snake[current->x].length;
    if (last->y == current->y)
    {
        Node *ptr = last;
        snake[current->x].head = ptr->next;
        // cout << "prev: " << prev->x << " " << prev->y << "\n";
        free(ptr);
        snake[current->x].length--;
        return;
    }
    last = last->next;
    count--;
    while(count--)
    {
        // cout << "prev: " << prev->x << " " << prev->y << "\n";
        // cout << "last: " << last->x << " " << last->y << "\n";
        if (last->y == current->y)
        {
            Node *ptr = last;
            // cout << "ptr: " << ptr->x << " " << ptr->y << "\n";
            prev->next = ptr->next;
            // cout << "prev: " << prev->x << " " << prev->y << "\n";
            free(ptr);
            snake[current->x].length--;
            return;
        }
        prev = last;
        last = last->next;
    }
}

char createSnake(char flag, Snake *snake)
{
    head = NewNode(4, 5);
    head->value = 'H';
    Node *newNode = new Node();
    newNode = NewNode(4, 6);
    head->next = newNode;
    tail = NewNode(4, 7);
    tail->value = 'T';
    newNode->next = tail;
    flag = 'a';
    saveSnake(snake, &head);
    return flag;
}

void projectFood(char **board, Food *food)
{
    board[food->x][food->y] = food->value;
}

void clearBoard(char **board)
{
    for (int i = 0; i < rows; i++)
    {
        for (int j = 0; j < cols; j++)
        {
            board[i][j] = '_';
        }
    }
}

void projectSnake(char **board, Node **head_ref)
{
    Node *temp = *head_ref;
    while(temp != NULL)
    {
        board[temp->x][temp->y] = temp->value;
        temp = temp->next;
    }
}

Node * updateSnake(Node *temp)
{
    Node *last;
    // Node *current = temp;
    if (temp->next == NULL)
    {
        // return current->next;
        last = NewNode(temp->x, temp->y);
        return last;
    }
    last = updateSnake(temp->next);
    temp->next->x = temp->x;
    temp->next->y = temp->y;
    return last;
}

bool checkCollision(Snake *snake, Node *head)
{
    if (checkExist(snake, head->x, head->y))
    {
        cout << "Game Ended!!!!!!\n";
        return true;
    }
    return false;

}

void moveSnake(Snake *snake, Node **head_ref, char **board, Food *food, char flag)
{
    Node *temp = *head_ref;
    char choice;
    bool createfood;
    cout << "Enter:- w: forward, s: backward, a: left, d: right\n";
    while (1)
    {
        
        if (kbhit())
        {
            createfood = false;
            
            choice = (char)getch();
            Node *last;
            if (choice == 'X' || choice == 'x')
            {
                break;
            }
            switch (choice)
            {
                case 'W':
                case 'w':
                    // if (temp->x - temp->next->x > 0)
                    if (flag == 's')
                    {
                        cout << "Not Allowed\n";
                        continue;
                    }
                    else
                    {
                        Node *prev = temp;
                        last = updateSnake(prev);
                        if (prev->x == 0)
                        {
                            prev->x = rows - 1;
                        }
                        else
                        {
                            prev->x = prev->x - 1;
                        }
                        flag = 'w';
                    }
                    // displayList(temp);
                    break;
                case 'S':
                case 's':
                    // if (temp->next->x - temp->x > 0)
                    if (flag == 'w')
                    {
                        cout << "Not Allowed\n";
                        continue;
                    }
                    else
                    {
                        Node *prev = temp;
                        last = updateSnake(prev);
                        // prev->x = prev->x + 1;
                        if (prev->x == rows - 1)
                        {
                            prev->x = 0;
                        }
                        else
                        {
                            prev->x = prev->x + 1;
                        }
                        flag = 's';
                    }
                    // displayList(temp);
                    break;
                case 'A':
                case 'a':
                    // if (temp->y - temp->next->y > 0)
                    if (flag == 'd')
                    {
                        cout << "Not Allowed\n";
                        continue;
                    }
                    else
                    {
                        Node *prev = temp;
                        last = updateSnake(prev);
                        if (prev->y == 0)
                        {
                            prev->y = cols - 1;
                        }
                        else
                        {
                            prev->y = prev->y - 1;
                        }
                        flag = 'a';
                    }
                    // displayList(temp);
                    break;
                case 'D':
                case 'd':
                    // if (temp->next->y - temp->y > 0)
                    if (flag == 'a')
                    {
                        cout << "Not Allowed\n";
                        continue;
                    }
                    else
                    {
                        Node *prev = temp;
                        last = updateSnake(prev);
                        if (prev->y == cols - 1)
                        {
                            prev->y = 0;
                        }
                        else
                        {
                            prev->y = prev->y + 1;
                        }
                        flag = 'd';
                    }
                    // displayList(temp);
                    break;
                default:
                    cout << "Wrong choice\n";
                    cout << "Enter:- w: forward, s: backward, a: left, d: right\n";
                    continue;
                    break;
            }

            if (temp->x == food->x && temp->y == food->y)
            {
                Node *ptr = temp;
                // cout << "Entered\n";
                // cout << "last:" << last->x << " " << last->y << " " << last->value << endl;
                Node *newNode = NewNode(last->x, last->y);
                newNode->value = 'T';
                while(ptr->next != NULL)
                {
                    ptr = ptr->next;
                }
                ptr->value = '#';
                ptr->next = newNode;
                createfood = true;
            }
            else
            {
                // cout << "MoveFunc:Before:Delete:\n";
                // cout << last->x << " " << last->y << "\n";
                // cout << endl;

                deleteNode(snake, last);
            }

            // cout << endl;
            if (checkCollision(snake, temp))
            {
                return;
            }

            saveNode(snake, temp);
            if (createfood)
            {
                food = createFood(snake);
            }
            clearBoard(board);
            projectFood(board, food);
            projectSnake(board, &temp);
            displayBoard(board);
            cout << "Enter:- w: forward, s: backward, a: left, d: right\n";
            // displaySnake(snake);
            // cout << "Enter:- w: forward, s: backward, a: left, d: right\n";
            // cin >> choice;
        }
        
    }
    
    cout << "Thanks For playing!!!!\n";
}

int main()
{
    char flag;
    Snake *snake  = new Snake[rows];
    char **board = createBoard();
    // displayBoard(board);
    food = createFood(snake);
    // cout << food->x << " " << food->y << " " << food->value << endl;
    flag = createSnake(flag, snake);
    projectSnake(board, &head);
    projectFood(board, food);
    // displayList(head);
    displayBoard(board);
    // cout << "SNAKE LIST:\n";
    // displayList(snake[3].head);
    moveSnake(snake, &head, board, food, flag);
}
