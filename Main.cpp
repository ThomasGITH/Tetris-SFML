#pragma comment(linker, "/SUBSYSTEM:windows /ENTRY:mainCRTStartup") //prevents the console from opening
#include<SFML\Graphics.hpp>
#include <SFML\Audio.hpp>

#define PI 3.14159265359

typedef uint8_t byte;

byte grid[20][12];
byte colliders[20][12];

struct Vec2
{
	byte y, x;
	Vec2(){}
	Vec2(byte dy, byte dx) : x(dx), y(dy){}
};

struct Piece
{
	Vec2 a, b, c, d;
	Piece(){}
	Piece(byte ax, byte ay, byte bx, byte by, byte cx, byte cy, byte dx, byte dy)
		: a(Vec2(ax,ay)), b(Vec2(bx, by)), c(Vec2(cx, cy)), d(Vec2(dx, dy)) {}
	Piece(Vec2 da, Vec2 db, Vec2 dc, Vec2 dd)
		: a(da), b(db), c(dc), d(dd){}
};

enum PIECE_TYPE
{
	S, Z, L, J, SQR, I, T
};

Piece CreatePiece(PIECE_TYPE type)
{
	switch (type)
	{
	case S : return Piece(Vec2(1, 5), Vec2(1, 4), Vec2(0, 4), Vec2(2, 5));
		break;
	case Z: return Piece(Vec2(1, 4), Vec2(1, 5), Vec2(0, 5), Vec2(2, 4));
		break;
	case L: return Piece(Vec2(1, 5), Vec2(1, 4), Vec2(1, 6), Vec2(0, 6));
		break;
	case J: return Piece(Vec2(1, 5), Vec2(1, 4), Vec2(1, 6), Vec2(0, 4));
		break;
	case SQR: return Piece(Vec2(1, 5), Vec2(1, 4), Vec2(0, 5), Vec2(0, 4));
		break;
	case I: return Piece(Vec2(1, 5), Vec2(1, 4), Vec2(1, 6), Vec2(1, 7));
		break;
	case T: return Piece(Vec2(1, 5), Vec2(1, 4), Vec2(1, 6), Vec2(0, 5));
		break;
	default:
		break;
	}
}

void rotate(Piece& piece)
{
	float angle = 90 * (PI / 180);

	float b_offset_x = piece.b.x - piece.a.x;
	float c_offset_x = piece.c.x - piece.a.x;
	float d_offset_x = piece.d.x - piece.a.x;

	float b_offset_y = piece.b.y - piece.a.y;
	float c_offset_y = piece.c.y - piece.a.y;
	float d_offset_y = piece.d.y - piece.a.y;

	float pbx = piece.a.x + (b_offset_x * cosf(angle) - b_offset_y * sinf(angle));
	float pby = piece.a.y + (b_offset_x * sinf(angle) + b_offset_y * cosf(angle));

	float pcx = piece.a.x + (c_offset_x * cosf(angle) - c_offset_y * sinf(angle));
	float pcy = piece.a.y + (c_offset_x * sinf(angle) + c_offset_y * cosf(angle));

	float pdx = piece.a.x + (d_offset_x * cosf(angle) - d_offset_y * sinf(angle));
	float pdy = piece.a.y + (d_offset_x * sinf(angle) + d_offset_y * cosf(angle));

	//Prevents rotating into objects and walls
	if (colliders[(byte)pby][(byte)pbx] != 2 && 
		colliders[(byte)pcy][(byte)pcx] != 2 &&
		colliders[(byte)pdy][(byte)pdx] != 2 &&
		piece.a.x != 1 && piece.a.y != 1)
	{
		piece.b.x = pbx;
		piece.b.y = pby;

		piece.c.x = pcx;
		piece.c.y = pcy;

		piece.d.x = pdx;
		piece.d.y = pdy;
	}
}

int main()
{
	//16x16 tiles
	sf::Texture tile_tex;
	tile_tex.loadFromFile("images/tetris_tile.png");

	sf::Sprite tile(tile_tex);
	tile.setScale(2.83, 2.83);
	
	float tile_size = tile_tex.getSize().x * tile.getScale().x;
	size_t width = tile_size * 12, height = tile_size * 20;

	//Music
	sf::Music track;
	track.openFromFile("sounds/tetris_theme.wav");
	track.play();

	//Sound effects
	sf::SoundBuffer buffer;
	buffer.loadFromFile("sounds/fall.wav");
	sf::Sound fall;
	fall.setBuffer(buffer);

	sf::SoundBuffer bufferTwo;
	bufferTwo.loadFromFile("sounds/line.wav");
	sf::Sound line;
	line.setBuffer(bufferTwo);

	sf::SoundBuffer bufferThree;
	bufferThree.loadFromFile("sounds/tetris.wav");
	sf::Sound tetris;
	tetris.setBuffer(bufferThree);

	//Setup score counter
	sf::Text score;
	sf::Font font;
	font.loadFromFile("Forwa_font.TTF");
	score.setFont(font);
	score.setCharacterSize(15);
	score.setFillColor(sf::Color::Blue);
	score.setPosition(10,25);
	sf::Vector2<float> score_scale(1.5f,1.5f);
	score.setScale(score_scale);
	score.setString("Lines: 0");

	//Setup window & create first piece
	sf::RenderWindow window(sf::VideoMode(width, height), "Tetris", sf::Style::Titlebar | sf::Style::Close);
	window.setKeyRepeatEnabled(true);
	window.setFramerateLimit(60);
	
	Piece piece = CreatePiece(static_cast<PIECE_TYPE>((rand() % 7)));

	unsigned int timer = 0, gamespeed = 10, scoreCounter = 0;

	while (window.isOpen())
	{
		srand(time(NULL));

		sf::Event event;
		while (window.pollEvent(event))
		{
			if (event.type == sf::Event::Closed || sf::Keyboard::isKeyPressed(sf::Keyboard::Key::Escape))
			{
				window.close();
			}
			if (event.type == sf::Event::KeyPressed)
			{
				//Movement
				if (event.key.code == sf::Keyboard::Up)
				{
					rotate(piece);
				}
				else if (event.key.code == sf::Keyboard::Left &&
					piece.a.x != 0 && piece.b.x != 0 && piece.c.x != 0 && piece.d.x != 0 &&
					(colliders[piece.a.y][piece.a.x - 1]) != 2 && (colliders[piece.b.y][piece.b.x - 1]) != 2 &&
					(colliders[piece.c.y][piece.c.x - 1]) != 2 && (colliders[piece.d.y][piece.d.x - 1]) != 2)
				{
					piece.a.x--;
					piece.b.x--;
					piece.c.x--;
					piece.d.x--;
				}
				else if (event.key.code == sf::Keyboard::Right &&
					piece.a.x != 11 && piece.b.x != 11 && piece.c.x != 11 && piece.d.x != 11 &&
					(colliders[piece.a.y][piece.a.x + 1]) != 2 && (colliders[piece.b.y][piece.b.x + 1]) != 2 &&
					(colliders[piece.c.y][piece.c.x + 1]) != 2 && (colliders[piece.d.y][piece.d.x + 1]) != 2)
				{
					piece.a.x++;
					piece.b.x++;
					piece.c.x++;
					piece.d.x++;
				}
			}
			if (event.type == sf::Event::KeyReleased)
			{
				gamespeed = 10;
			}
		}

		if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::Down))
		{
			gamespeed = 1;
		}

		window.clear(sf::Color(0, 220, 255, 1));


		//Draw the current falling piece
		sf::Sprite piece_tile = tile;

		piece_tile.setPosition(tile_size * piece.a.x, tile_size * piece.a.y);
		window.draw(piece_tile);

		piece_tile.setPosition(tile_size * piece.b.x, tile_size * piece.b.y);
		window.draw(piece_tile);

		piece_tile.setPosition(tile_size * piece.c.x, tile_size * piece.c.y);
		window.draw(piece_tile);

		piece_tile.setPosition(tile_size * piece.d.x, tile_size * piece.d.y);
		window.draw(piece_tile);

		//Refresh the grid-array
		for (size_t i = 0; i < 20; i++)
		{
			for (size_t j = 0; j < 12; j++)
			{
				if (colliders[i][j] == 2)
				{
					grid[i][j] = 2;
				}
				else
				{
					grid[i][j] = 0;
				}
			}
		}
		
		//Clock
		if (timer > gamespeed)
		{
			//Collission checks
			if (grid[piece.a.y + 1][piece.a.x] == 2 ||
				grid[piece.b.y + 1][piece.b.x] == 2 ||
				grid[piece.c.y + 1][piece.c.x] == 2 ||
				grid[piece.d.y + 1][piece.d.x] == 2 ||
				piece.a.y == 19 || piece.b.y == 19 || piece.c.y == 19 || piece.d.y == 19
				)
			{
				fall.play();

				grid[piece.a.y][piece.a.x] = 2;
				grid[piece.b.y][piece.b.x] = 2;
				grid[piece.c.y][piece.c.x] = 2;
				grid[piece.d.y][piece.d.x] = 2;

				colliders[piece.a.y][piece.a.x] = 2;
				colliders[piece.b.y][piece.b.x] = 2;
				colliders[piece.c.y][piece.c.x] = 2;
				colliders[piece.d.y][piece.d.x] = 2;

				piece = CreatePiece(static_cast<PIECE_TYPE>((rand() % 7)));
			}
			else
			{
				grid[piece.a.y + 1][piece.a.x] = 1;
				grid[piece.b.y + 1][piece.b.x] = 1;
				grid[piece.c.y + 1][piece.c.x] = 1;
				grid[piece.d.y + 1][piece.d.x] = 1;

				piece.a.y++;
				piece.b.y++;
				piece.c.y++;
				piece.d.y++;
			}

			//Check if the player has a line or 'tetris'
			byte tetris_row = 0;
			for (size_t i = 0; i < 20; i++)
			{
				byte blocks_in_a_row = 0;
				for (size_t j = 0; j < 12; j++)
				{
					if (colliders[i][j] == 2)
					{
						blocks_in_a_row++;
					}
				}
				if (blocks_in_a_row == 12)
				{
					tetris_row++;
					if (tetris_row >= 4)
					{
						tetris.play();
					}
					else
					{
						line.play();
					}
					for (size_t k = i; k > 0; k--)
					{
						for (size_t l = 0; l < 12; l++)
						{
							colliders[k][l] = colliders[k - 1][l];
						}
					}
					scoreCounter++;
					char temp[256];
					sprintf_s(temp, "Lines: %i", scoreCounter);
					score.setString(temp);
				}
			}

			//If game over, then close application
			for (size_t i = 0; i < 12; i++)
			{
				if (colliders[0][i] == 2)
				{
					window.close();
				}
			}

			timer = 0;

			//Prints out the grid in the console (if enabled) for testing purposes
			/*
			for (size_t i = 0; i < 20; i++)
			{
				for (size_t j = 0; j < 12; j++)
				{
					printf("%i", grid[i][j]);
				}
				printf("\n");
			}
			*/
		}
		else
		{
			timer++;
		}

		//Draws all the tiles
		for (size_t i = 0; i < 20; i++)
		{
			for (size_t j = 0; j < 12; j++)
			{
				if (grid[i][j] == 1)
				{
					sf::Sprite t = tile;
					t.setPosition(tile_size * j, tile_size * i);
					window.draw(t);
				}
				if (colliders[i][j] == 2)
				{
					sf::Sprite t = tile;
					t.setPosition(tile_size * j, tile_size * i);
					window.draw(t);
				}
			}
		}

		//Draw the score and finally update the window
		window.draw(score);
		window.display();
	}

	return 0;
}
