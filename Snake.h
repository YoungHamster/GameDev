#pragma once
struct SnakePart : PhysicalObject {
	int dir;
	int prevDir;
	AABB prevBorders;
};

// snake directions
#define UP 1
#define DOWN 2
#define LEFT 3
#define RIGHT 4

// synchronization between threads
bool volatile keeprendering = true;

class Snake
{
private:
	std::vector<SnakePart> snake;
public:
	int InitSnake(int lenght)
	{
		if (lenght < 3) return 1;
		if (lenght > 100) return 2;
		for (int i = 0; i < lenght; i++)
		{
			SnakePart block;
			block.borders.min.x = 12 + (i + 1);
			block.borders.min.y = 15;
			block.borders.max.x = 12 + (i + 2);
			block.borders.max.y = 16;

			if (i == 0) {
				block.type = SNAKE_HEAD_LEFT;
			}
			else if (i == lenght - 1) {
				block.type = SNAKE_TAIL_LEFT;
			}
			else {
				block.type = SNAKE_BODY;
			}

			block.dir = LEFT;
			snake.push_back(block);
		}
		return 0;
	}

	void moveSnakeBlock(SnakePart* s)
	{
		s->prevBorders = s->borders;
		s->prevDir = s->dir;
		switch (s->dir)
		{
		case UP:
			s->borders.min.y = s->borders.max.y;
			s->borders.max.y = s->borders.max.y + 1;
			break;
		case DOWN:
			s->borders.max.y = s->borders.min.y;
			s->borders.min.y = s->borders.min.y - 1;
			break;
		case LEFT:
			s->borders.max.x = s->borders.min.x;
			s->borders.min.x = s->borders.min.x - 1;
			break;
		case RIGHT:
			s->borders.min.x = s->borders.max.x;
			s->borders.max.x = s->borders.max.x + 1;
			break;
		}
	}

	// moves all snake
	void move(int direction)
	{
		keeprendering = false;
		for (int i = 0; i < snake.size(); i++)
		{
			if (i != 0) {
				snake[i].prevBorders = snake[i].borders;
				snake[i].borders = snake[i - 1].prevBorders;
				snake[i].prevDir = snake[i].dir;
				snake[i].dir = snake[i - 1].prevDir;
				if (i == snake.size() - 1)
				{
					switch (snake[i].dir)
					{
					case UP: snake[i].type = SNAKE_TAIL_UP; break;
					case LEFT: snake[i].type = SNAKE_TAIL_LEFT; break;
					case DOWN: snake[i].type = SNAKE_TAIL_DOWN; break;
					case RIGHT: snake[i].type = SNAKE_TAIL_RIGHT; break;
					}
				}
			}
			else { 
				snake[i].dir = direction;
				snake[i].prevDir = snake[i].dir;
				switch (direction)
				{
				case UP: snake[i].type = SNAKE_HEAD_UP; break;
				case LEFT: snake[i].type = SNAKE_HEAD_LEFT; break;
				case DOWN: snake[i].type = SNAKE_HEAD_DOWN; break;
				case RIGHT: snake[i].type = SNAKE_HEAD_RIGHT; break;
				}
				moveSnakeBlock(&snake[i]);
			}
		}
		keeprendering = true;
	}

	bool checkCollision(AABB a, int i)
	{
		if (AABBvsAABB(a, snake[i].borders)) return true;
		return false;
	}

	bool checkCollisionSnakeHeadAndBody(int i)
	{
		if (AABBvsAABB(snake[0].borders, snake[i].borders)) return true;
		return false;
	}

	void addlenght()
	{
		keeprendering = false;
		int x = snake.size() - 1;
		SnakePart block;
		block.borders = snake[x].prevBorders;
		block.dir = snake[x].dir;
		block.type = snake[x].type;
		snake[x].type = SNAKE_BODY;

		snake.push_back(block);
		keeprendering = true;
	}

	int snakelenght()
	{
		return snake.size();
	}

	PhysicalObject getPhysicalObject(int i)
	{
		PhysicalObject obj;
		obj.borders = snake[i].borders;
		obj.type = snake[i].type;
		return obj;
	}

	AABB getAABB(int i)
	{
		AABB a;
		a = snake[i].borders;
		return a;
	}
};