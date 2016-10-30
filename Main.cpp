
# include <Siv3D.hpp>
# include <HamFramework.hpp>


struct CommonData
{
	double counter = 0.0;
};


using MyApp = SceneManager<String, CommonData>;


class Title : public MyApp::Scene
{
	Font font1;
	Font font2;
	Font font3;
	Font font4;

public:

	void init()override
	{
		font1 = Font(25);
		font2 = Font(10);
		font3 = Font(30);
		font4 = Font(20);
	}

	void update()override
	{

		if (Input::KeyS.clicked)
		{
			++m_data->counter;
			changeScene(L"Game1");
		}

		if (Input::KeyR.clicked)
		{
			changeScene(L"Rule");
		}

		if (Input::KeyQ.clicked)
		{
			System::Exit();
		}

	}

	void draw()const override
	{
		font1(L"ブロックくずし").draw(200, 80,Palette::Brown);
		font2(L" いっきゅう　にゅうこん").draw(220,135,Palette::Lightcoral);
		font3(L"一球入魂").draw(220, 150, Palette::Lightcoral);
		font4(L"Sボタン：ゲームスタート").draw(130,300);
		font4(L"Rボタン：ルール説明").draw(130,350);
		font4(L"Qボタン：ゲームをやめる").draw(130, 400);
	}
};


class Game1 : public MyApp::Scene
{
	Font font;
	Circle ball,item;
	Array<Rect> blocks;
	Rect player;
	Point pos;
	Vec2 itemspeed;
	Vec2 ballspeed;
	Sound sound;
	Size blocksize;
	double speed;
	int score,itemdrop,itemcount,evolcount;
	Texture texture;
	Effect effect;

	struct Evol : IEffect
	{
		Evol(Rect player,Point pos,Circle item)
		{
			player = Rect(120, 10);
			player.setCenter(pos.x, pos.y);
			player.draw(Palette::Lightcyan);

			item.setPos(pos.x, pos.y + 5);
			item.draw(Palette::Black);
		}

		bool update(double t)override
		{
			if (t >= 1.5)
			{
				return false;
			}
			return true;
		}
	};

public:
		
	void init()
	{
		sound = Sound(Wave(0.1s, [](double t) {return Fraction(t * 440)*0.5 - 0.25; }));
		blocksize = Size(40, 20);
		speed = double(5.0 + m_data->counter);

		pos = Point(100, 420);
		ball = Circle(320, 380, 8);
		item = Circle(295,-10,8);
		itemspeed = Vec2(0, -2.0);
		ballspeed = Vec2(0, -speed);
		player = Rect(80, 10);
		score = int(0);
		itemdrop = int(0);
		itemcount = int(0);
		evolcount = int(0);
		texture = Texture(L"block.png");
		effect = Effect();

		for (auto p : step({ Window::Width() / blocksize.x,5 }))
		{
			blocks.emplace_back((p*blocksize).moveBy(0, 60), blocksize);
		}

	
		
	}

	void update()override
	{
		ball.moveBy(ballspeed);
		player.setCenter(pos.x,pos.y);

		if (Input::KeyRight.pressed)
		{
			(pos.x) += 12;
		}

		if (Input::KeyLeft.pressed)
		{
			(pos.x) -= 12;
		}

		for (auto it = blocks.begin(); it != blocks.end(); ++it)
		{
			if (it->intersects(ball))
			{
				(it->bottom.intersects(ball) || it->top.intersects(ball) ? ballspeed.y : ballspeed.y) *= -1;

				blocks.erase(it);

				score = score + 1;

				itemdrop = Random(1,10);

				if (itemcount == 0 && itemdrop == 1)
				{
					item.draw(Palette::Lightcyan);
					itemcount = 1;
				}

				sound.playMulti();

				break;
			}
		}
		for (auto const & block : blocks)
		{
			block(texture).draw();
		}

		if (score == 80)
		{
			++m_data->counter;
			changeScene(L"Game2");
		}

		if (itemcount == 1)
		{
			item.moveBy(-itemspeed);
		}

		if (ball.y < 0 && ballspeed.y < 0)
		{
			ballspeed.y *= -1;
		}

		if ((ball.x < 0 && ballspeed.x < 0) || (Window::Width() < ball.x && ballspeed.x > 0))
		{
			ballspeed.x *= -1;
		}

		if (ballspeed.y > 0 && player.intersects(ball))
		{
			ballspeed = Vec2((ball.x - player.center.x) / 8, -ballspeed.y).setLength(speed);
		}

		if (ball.y > Window::Height() && ballspeed.y > 0)
		{
			changeScene(L"Result");
		}

		if (player.intersects(item))
		{
			evolcount = 1;
		}

		if (evolcount == 1)
		{
			itemcount = 0;
			effect.add<Evol>(player, pos, item);
			player.draw(Palette::Lightcyan);
			item.draw(Palette::Black);
			effect.update();
		}

		if (item.y > Window::Height() && itemspeed.y > 0)
		{
			evolcount = 0;
			itemcount = 0;
		}

		if (player.center.x < 0)
		{
			(pos.x) += 12;
		}

		if ((Window::Width()) < player.center.x)
		{
			(pos.x) -= 12;
		}

		
		ball.draw();
		player.draw();
	}

	void draw()const override
	{
		item.draw(Palette::Lightcyan);
		ball.draw(Palette::Lightgreen);
		player.draw(Palette::Lightyellow);

		if (evolcount == 1)
		{
			player.draw(Palette::Lightcyan);
			item.draw(Palette::Black);
		}

	}
};

class Game2 : public MyApp::Scene
{
	Font font;
	Circle ball, item;
	Array<Rect> blocks;
	Rect player;
	Point pos;
	Vec2 itemspeed;
	Vec2 ballspeed;
	Sound sound;
	Size blocksize;
	double speed;
	int score, itemdrop, itemcount, evolcount;
	Texture texture;
	Effect effect;

	struct Evol : IEffect
	{
		Evol(Rect player, Point pos, Circle item)
		{
			player = Rect(120, 10);
			player.setCenter(pos.x, pos.y);
			player.draw(Palette::Lightcyan);

			item.setPos(pos.x, pos.y + 5);
			item.draw(Palette::Black);
		}

		bool update(double t)override
		{
			if (t >= 1.5)
			{
				return false;
			}
			return true;
		}
	};

public:

	void init()
	{
		sound = Sound(Wave(0.1s, [](double t) {return Fraction(t * 440)*0.5 - 0.25; }));
		blocksize = Size(40, 20);
		speed = double(5.0 + m_data->counter);

		pos = Point(100, 420);
		ball = Circle(320, 380, 8);
		item = Circle(295, -10, 8);
		itemspeed = Vec2(0, -2.0);
		ballspeed = Vec2(0, -speed);
		player = Rect(80, 10);
		score = int(0);
		itemdrop = int(0);
		itemcount = int(0);
		evolcount = int(0);
		texture = Texture(L"block.png");
		effect = Effect();

		for (auto p : step({ Window::Width() / blocksize.x,5 }))
		{
			blocks.emplace_back((p*blocksize).moveBy(0, 60), blocksize);
		}



	}

	void update()override
	{
		ball.moveBy(ballspeed);
		player.setCenter(pos.x, pos.y);

		if (Input::KeyRight.pressed)
		{
			(pos.x) += 12;
		}

		if (Input::KeyLeft.pressed)
		{
			(pos.x) -= 12;
		}

		for (auto it = blocks.begin(); it != blocks.end(); ++it)
		{
			if (it->intersects(ball))
			{
				(it->bottom.intersects(ball) || it->top.intersects(ball) ? ballspeed.y : ballspeed.y) *= -1;

				blocks.erase(it);

				score = score + 1;

				itemdrop = Random(1, 10);

				if (itemcount == 0 && itemdrop == 1)
				{
					item.draw(Palette::Lightcyan);
					itemcount = 1;
				}

				sound.playMulti();

				break;
			}
		}
		for (auto const & block : blocks)
		{
			block(texture).draw();
		}

		if (score == 80)
		{
			++m_data->counter;
			changeScene(L"Game1");
		}

		if (itemcount == 1)
		{
			item.moveBy(-itemspeed);
		}

		if (ball.y < 0 && ballspeed.y < 0)
		{
			ballspeed.y *= -1;
		}

		if ((ball.x < 0 && ballspeed.x < 0) || (Window::Width() < ball.x && ballspeed.x > 0))
		{
			ballspeed.x *= -1;
		}

		if (ballspeed.y > 0 && player.intersects(ball))
		{
			ballspeed = Vec2((ball.x - player.center.x) / 8, -ballspeed.y).setLength(speed);
		}

		if (ball.y > Window::Height() && ballspeed.y > 0)
		{
			changeScene(L"Result");
		}

		if (player.intersects(item))
		{
			evolcount = 1;
		}

		if (evolcount == 1)
		{
			itemcount = 0;
			effect.add<Evol>(player, pos, item);
			player.draw(Palette::Lightcyan);
			item.draw(Palette::Black);
			effect.update();
		}

		if (item.y > Window::Height() && itemspeed.y > 0)
		{
			evolcount = 0;
			itemcount = 0;
		}

		if (player.center.x < 0)
		{
			(pos.x) += 12;
		}

		if ((Window::Width()) < player.center.x)
		{
			(pos.x) -= 12;
		}


		ball.draw();
		player.draw();
	}

	void draw()const override
	{
		item.draw(Palette::Lightcyan);
		ball.draw(Palette::Lightgreen);
		player.draw(Palette::Lightyellow);

		if (evolcount == 1)
		{
			player.draw(Palette::Lightcyan);
			item.draw(Palette::Black);
		}

	}
};

class Rule : public MyApp::Scene
{

	Font font1;
	Font font2;

public:

	void init()override
	{
		font1 = Font(25);
		font2 = Font(15);
	}

	void update()override
	{
		if (Input::KeyR.clicked)
		{
			changeScene(L"Title");
		}
	}

	void draw()const override
	{
		font1(L"ルール説明").draw(215,50,Palette::Lightgreen);
		font2(L"プレイヤー(棒)を十字ボタン(←と→)で\n左右に動かして、動いているボールをはね返して、\nひたすらブロックをこわしていくゲームです\n途中、確率で振ってくる玉にさわると、\n棒が１．５倍にのびるよ\n一つの面をクリアすると、次の面にいきます\n次の面ではボールの速さが一つ前の面よりも速くなります\nまた、１回ミスしたらそこでゲームオーバーです").draw(40,150);
		font2(L"Rボタン：タイトルに戻る").draw(150,400);
	}
};



class Result : public MyApp::Scene
{
	Font font1;
	Font font2;

public:

	void init()override
	{
		font1 = Font(30);
		font2 = Font(20);
	}

	void update()override
	{
		m_data->counter = 0.0;

		if (Input::KeyT.clicked)
		{
			changeScene(L"Title");
		}
	}

	void draw()const override
	{
		font1(L"残念！ゲームオーバー！").draw(50, 150);
		font1(L"遊んでくれてありがとう！").draw(50, 200);
		font2(L"Tボタン：タイトルに戻る").draw(120, 350);
	}
};



void Main()
{
	MyApp manager;

	manager.add<Title>(L"Title");
	manager.add<Game1>(L"Game1");
	manager.add<Game2>(L"Game2");
	manager.add<Rule>(L"Rule");
	manager.add<Result>(L"Result");
	
	while (System::Update())
	{
		manager.updateAndDraw();

	}
}
