#include "Asteroid.h"
#include "Asteroids.h"
#include "Animation.h"
#include "AnimationManager.h"
#include "GameUtil.h"
#include "GameWindow.h"
#include "GameWorld.h"
#include "GameDisplay.h"
#include "Spaceship.h"
#include "BoundingShape.h"
#include "BoundingSphere.h"
#include "GUILabel.h"
#include "Explosion.h"
#include "Life.h"
#include "Shield.h"
 

// PUBLIC INSTANCE CONSTRUCTORS ///////////////////////////////////////////////

/** Constructor. Takes arguments from command line, just in case. */
Asteroids::Asteroids(int argc, char *argv[])
	: GameSession(argc, argv)
{
	mLevel = 0;
	mAsteroidCount = 0;
	mStartBoolean = false;
	
}

/** Destructor. */
Asteroids::~Asteroids(void)
{
}

// PUBLIC INSTANCE METHODS ////////////////////////////////////////////////////
 shared_ptr<GUILabel> mLivesPopUpLabel;
 shared_ptr<GUILabel> mShieldPopUpLabel;
 shared_ptr<GUILabel> mShieldGUILabel;
 
/** Start an asteroids game. */
void Asteroids::Start()
{
	// Create a shared pointer for the Asteroids game object - DO NOT REMOVE
	shared_ptr<Asteroids> thisPtr = shared_ptr<Asteroids>(this);

	if (mStartBoolean == false) {
		CreateStartGUI();
		SetTimer(1000, BLINK_START_MESSAGE);
		 
		// Add this as a listener to the world and the keyboard
		mGameWindow->AddKeyboardListener(thisPtr);
		 
		mStartTrigger = true;



	}else{
		mGameNameLabel->SetVisible(false);
		mGameWindow->SetWorld(mGameWorld);
 
		 
		mGameStartScreen->~GameWorld();
		// Add a score keeper to the game world
		mGameWorld->AddListener(&mShieldKeeper);
		// Add this class as a listener of the score keeper
		mShieldKeeper.AddListener(thisPtr);

		// Add this class as a listener of the game world
		mGameWorld->AddListener(thisPtr.get());
		
		// Add a score keeper to the game world
		mGameWorld->AddListener(&mScoreKeeper);
 

		// Add this class as a listener of the score keeper
		mScoreKeeper.AddListener(thisPtr);

		// Add this class as a listener of the player
		mPlayer.AddListener(thisPtr);


	// Create an ambient light to show sprite textures
	GLfloat ambient_light[] = { 1.0f, 1.0f, 1.0f, 1.0f };
	GLfloat diffuse_light[] = { 1.0f, 1.0f, 1.0f, 1.0f };
	glLightfv(GL_LIGHT0, GL_AMBIENT, ambient_light);
	glLightfv(GL_LIGHT0, GL_DIFFUSE, diffuse_light);
	glEnable(GL_LIGHT0);

	Animation *explosion_anim = AnimationManager::GetInstance().CreateAnimationFromFile("explosion", 64, 1024, 64, 64, "explosion_fs.png");
	Animation *asteroid1_anim = AnimationManager::GetInstance().CreateAnimationFromFile("asteroid1", 128, 8192, 128, 128, "asteroid1_fs.png");
	Animation *spaceship_anim = AnimationManager::GetInstance().CreateAnimationFromFile("spaceship", 128, 128, 128, 128, "spaceship_fs.png");
	Animation *life_anim = AnimationManager::GetInstance().CreateAnimationFromFile("life", 128, 128, 128, 128, "life2_fs.png");
 	Animation* spaceship_shield_anim = AnimationManager::GetInstance().CreateAnimationFromFile("spaceship_with_shield", 128, 128, 128, 128, "shield.png");
 	Animation* shield_anim = AnimationManager::GetInstance().CreateAnimationFromFile("shield", 128, 128, 128, 128, "shieldObj.png");

	
		// Create a spaceship and add it to the world
	mGameWorld->AddObject(CreateSpaceship());
		// Create some asteroids and add them to the world
		CreateAsteroids(3);
		//CreateLife(3);
		//Create the GUI
		CreateGUI();
		CreateShieldGUI();
		//CreateShield(5);
		

		// Add a player (watcher) to the game world
		mGameWorld->AddListener(&mPlayer);

		mStartTrigger = false;
	}

	// Start the game
	GameSession::Start();
}

/** Stop the current game. */
void Asteroids::Stop()
{
	// Stop the game
	GameSession::Stop();
}

// PUBLIC INSTANCE METHODS IMPLEMENTING IKeyboardListener /////////////////////

void Asteroids::OnKeyPressed(uchar key, int x, int y)
{
	if (key >= 13 && key <= 126) {

		mGameStartLabel->SetVisible(false);
		mStartBoolean = true;
		if (mStartTrigger == true) {
			Start();

		}

		switch (key)
		{
		case ' ':
			mSpaceship->Shoot();
			break;
	
		default:
			break;
		}
	}
}

void Asteroids::OnKeyReleased(uchar key, int x, int y) {}

void Asteroids::OnSpecialKeyPressed(int key, int x, int y)
{
	switch (key)
	{
	// If up arrow key is pressed start applying forward thrust
	case GLUT_KEY_UP: mSpaceship->Thrust(10); 
		 
		break;
	// If left arrow key is pressed start rotating anti-clockwise
	case GLUT_KEY_LEFT: mSpaceship->Rotate(90);
		SetTimer(3000, HIDE_LIFE_POP_UP); 
		break;
	// If right arrow key is pressed start rotating clockwise
	case GLUT_KEY_RIGHT: mSpaceship->Rotate(-90);
		SetTimer(3000, HIDE_LIFE_POP_UP); 
		break;
	// Default case - do nothing
	default: break;
	}
}

void Asteroids::OnSpecialKeyReleased(int key, int x, int y)
{
	switch (key)
	{
	// If up arrow key is released stop applying forward thrust
	case GLUT_KEY_UP: mSpaceship->Thrust(0); break;
	// If left arrow key is released stop rotating
	case GLUT_KEY_LEFT: mSpaceship->Rotate(0); break;
	// If right arrow key is released stop rotating
	case GLUT_KEY_RIGHT: mSpaceship->Rotate(0); break;
	// Default case - do nothing
	default: break;
	} 
}


// PUBLIC INSTANCE METHODS IMPLEMENTING IGameWorldListener ////////////////////

void Asteroids::OnObjectRemoved(GameWorld* world, shared_ptr<GameObject> object)
{
	if (object->GetType() == GameObjectType("Asteroid"))
	{
		shared_ptr<GameObject> explosion = CreateExplosion();
		explosion->SetPosition(object->GetPosition());
		explosion->SetRotation(object->GetRotation());
		mGameWorld->AddObject(explosion);
		mAsteroidCount--;
		if (mAsteroidCount <= 0) 
		{ 
			SetTimer(500, START_NEXT_LEVEL); 
		}
	}if (object->GetType() == GameObjectType("Life"))
	{
		shared_ptr<GameObject> explosion = CreateExplosion();
		explosion->SetPosition(object->GetPosition());
		explosion->SetRotation(object->GetRotation());
		mGameWorld->AddObject(explosion);
		SetTimer(100, SHOW_LIFE_POP_UP);
	}if (object->GetType() == GameObjectType("Shield"))
	{
		shared_ptr<GameObject> explosion = CreateExplosion();
		explosion->SetPosition(object->GetPosition());
		explosion->SetRotation(object->GetRotation());
		mGameWorld->AddObject(explosion);

		Animation* shieldanim_ptr = AnimationManager::GetInstance().GetAnimationByName("spaceship_with_shield");
		shared_ptr<Sprite> shield_spaceship_sprite =
			make_shared<Sprite>(shieldanim_ptr->GetWidth(), shieldanim_ptr->GetHeight(), shieldanim_ptr);

		mSpaceship->SetSprite(shield_spaceship_sprite);
		mSpaceship->SetScale(0.11f);


	}
}

// PUBLIC INSTANCE METHODS IMPLEMENTING ITimerListener ////////////////////////

void Asteroids::OnTimer(int value)
{ 
	if (value == CREATE_NEW_PLAYER)
	{
		
		Animation* shieldanim_ptr = AnimationManager::GetInstance().GetAnimationByName("spaceship");
		shared_ptr<Sprite> shield_spaceship_sprite =
			make_shared<Sprite>(shieldanim_ptr->GetWidth(), shieldanim_ptr->GetHeight(), shieldanim_ptr);
		mSpaceship->SetScale(0.1f);
		mSpaceship->SetSprite(shield_spaceship_sprite);
		mSpaceship->Reset();
		mGameWorld->AddObject(mSpaceship);
	}

	if (value == START_NEXT_LEVEL)
	{
		mLevel++;
		switch (mLevel) {

		case 1:
			CreateAsteroids(5);
			CreateLife(1);
			break;
		 
		case 2:
			CreateAsteroids(8);
			CreateShield(1);
			CreateLife(2);
			break;
		case 3:
			CreateAsteroids(8);
			CreateShield(2);
			CreateLife(2);
			break;
		default:
			CreateAsteroids(8);
			CreateShield(2);
			CreateLife(2);
			
		 }
		
		
	}

	if (value == SHOW_GAME_OVER)
	{
		mGameOverLabel->SetVisible(true);
	}
	if (value == BLINK_START_MESSAGE)
	{
		
		if (!mStartBoolean) {
			if (mGameStartLabel->GetVisible())
			{
				 
				mGameStartLabel->SetVisible(false);
			}
			else {
				mGameNameLabel->SetVisible(true);
				mGameStartLabel->SetVisible(true);

			}
			SetTimer(500, BLINK_START_MESSAGE);
		}
		else {
			mGameNameLabel->SetVisible(false);
			mGameStartLabel->SetVisible(false);
			
		}
		 
	} 
	if (value == SHOW_LIFE_POP_UP)
	{
		mLivesPopUpLabel->SetVisible(true);
	}
	if (value == HIDE_LIFE_POP_UP)
	{
		mLivesPopUpLabel->SetVisible(false);
	}

}

// PROTECTED INSTANCE METHODS /////////////////////////////////////////////////
shared_ptr<GameObject> Asteroids::CreateSpaceship()
{
	// Create a raw pointer to a spaceship that can be converted to
	// shared_ptrs of different types because GameWorld implements IRefCount
	mSpaceship = make_shared<Spaceship>();
	mSpaceship->SetBoundingShape(make_shared<BoundingSphere>(mSpaceship->GetThisPtr(), 4.0f));
	shared_ptr<Shape> bullet_shape = make_shared<Shape>("bullet.shape");
	mSpaceship->SetBulletShape(bullet_shape);
	Animation *anim_ptr = AnimationManager::GetInstance().GetAnimationByName("spaceship");
	shared_ptr<Sprite> spaceship_sprite =
		make_shared<Sprite>(anim_ptr->GetWidth(), anim_ptr->GetHeight(), anim_ptr);
	mSpaceship->SetSprite(spaceship_sprite);
	mSpaceship->SetScale(0.1f);
	// Reset spaceship back to centre of the world
	mSpaceship->Reset();
	// Return the spaceship so it can be added to the world
	return mSpaceship;

}





void Asteroids::CreateShield(const uint num_shield)
{
	for (uint i = 0; i < num_shield; i++) {
	 



		Animation* anim_ptr = AnimationManager::GetInstance().GetAnimationByName("shield");
		shared_ptr<Sprite> shield_sprite =
			make_shared<Sprite>(anim_ptr->GetWidth(), anim_ptr->GetHeight(), anim_ptr);


		shared_ptr<GameObject> shield = make_shared<Shield>();
		shield->SetBoundingShape(make_shared<BoundingSphere>(shield->GetThisPtr(),7.9f));
		shield->SetScale(0.15f);
		shield->SetSprite(shield_sprite);;
		mGameWorld->AddObject(shield);




	}
}





void Asteroids::CreateAsteroids(const uint num_asteroids)
{
	mAsteroidCount = num_asteroids;
	for (uint i = 0; i < num_asteroids; i++)
	{
		Animation *anim_ptr = AnimationManager::GetInstance().GetAnimationByName("asteroid1");
		shared_ptr<Sprite> asteroid_sprite
			= make_shared<Sprite>(anim_ptr->GetWidth(), anim_ptr->GetHeight(), anim_ptr);
		asteroid_sprite->SetLoopAnimation(true);
		shared_ptr<GameObject> asteroid = make_shared<Asteroid>();
		asteroid->SetBoundingShape(make_shared<BoundingSphere>(asteroid->GetThisPtr(), 10.0f));
		asteroid->SetSprite(asteroid_sprite);
		asteroid->SetScale(0.2f);
		mGameWorld->AddObject(asteroid);
	}
}


void Asteroids::CreateStartGUI()
{
	// Create a new GUILabel and wrap it up in a shared_ptr
	mGameStartLabel = shared_ptr<GUILabel>(new GUILabel("PRESS ANY KEY TO START THE GAME!"));
	
	// Set the horizontal alignment of the label to GUI_HALIGN_CENTER
	mGameStartLabel->SetHorizontalAlignment(GUIComponent::GUI_HALIGN_CENTER);
	// Set the vertical alignment of the label to GUI_VALIGN_MIDDLE
	mGameStartLabel->SetVerticalAlignment(GUIComponent::GUI_VALIGN_MIDDLE);

	mGameStartLabel->SetColor(GLVector3f(0.0f, 1.0f,2));

 
	mGameStartLabel->SetBorder(GLVector2i(10, 10));

	// Set the visibility of the label to false (hidden)
	//mGameStartLabel->SetVisible(true);
	// Add the GUILabel to the GUIContainer  
	shared_ptr<GUIComponent> game_start_component
		= static_pointer_cast<GUIComponent>(mGameStartLabel);
	mGameDisplay->GetContainer()->AddComponent(game_start_component, GLVector2f(0.5f, 0.5f));

	//Write the name of the game at the TOP of the screen.

	 // Add a (transparent) border around the edge of the game display
 	// Create a new GUILabel and wrap it up in a shared_ptr
	mGameNameLabel = make_shared<GUILabel>("--------- THE ASTEROIDS GAME ---------");
	// Set the vertical alignment of the label to GUI_VALIGN_TOP
	mGameNameLabel->SetVerticalAlignment(GUIComponent::GUI_VALIGN_TOP);

	// Add the GUILabel to the GUIComponent  
	shared_ptr<GUIComponent> name_component
		= static_pointer_cast<GUIComponent>(mGameNameLabel);
	mGameNameLabel->SetColor(GLVector3f(1.0f, 0.0f, 0));

	mGameDisplay->GetContainer()->AddComponent(name_component, GLVector2f(0.08f, 0.92));

	mGameNameLabel->SetVisible(true);
	 

	 
}



void Asteroids::CreateGUI()
{
	// Add a (transparent) border around the edge of the game display
	mGameDisplay->GetContainer()->SetBorder(GLVector2i(10, 10));
	// Create a new GUILabel and wrap it up in a shared_ptr
	mScoreLabel = make_shared<GUILabel>("Score: 0");
	// Set the vertical alignment of the label to GUI_VALIGN_TOP
	mScoreLabel->SetVerticalAlignment(GUIComponent::GUI_VALIGN_TOP);
	mScoreLabel->SetColor(GLVector3f(0.0f, 1.0f, 0.0f));

	// Add the GUILabel to the GUIComponent  
	shared_ptr<GUIComponent> score_component
		= static_pointer_cast<GUIComponent>(mScoreLabel);
	mGameDisplay->GetContainer()->AddComponent(score_component, GLVector2f(0.0f, 1.0f));
 

	// Create a new GUILabel and wrap it up in a shared_ptr
	mLivesLabel = make_shared<GUILabel>("Lives: 3");

	mLivesLabel->SetColor(GLVector3f(0.0f, 1.0f, 0.0f));


	// Set the vertical alignment of the label to GUI_VALIGN_BOTTOM
	mLivesLabel->SetVerticalAlignment(GUIComponent::GUI_VALIGN_BOTTOM);
	// Add the GUILabel to the GUIComponent  
	shared_ptr<GUIComponent> lives_component = static_pointer_cast<GUIComponent>(mLivesLabel);
	mGameDisplay->GetContainer()->AddComponent(lives_component, GLVector2f(0.0f, 0.0f));

	// Create a new GUILabel and wrap it up in a shared_ptr
	mGameOverLabel = shared_ptr<GUILabel>(new GUILabel("GAME OVER"));
	// Set the horizontal alignment of the label to GUI_HALIGN_CENTER
	mGameOverLabel->SetHorizontalAlignment(GUIComponent::GUI_HALIGN_CENTER);
	// Set the vertical alignment of the label to GUI_VALIGN_MIDDLE
	mGameOverLabel->SetVerticalAlignment(GUIComponent::GUI_VALIGN_MIDDLE);
	// Set the visibility of the label to false (hidden)
	mGameOverLabel->SetVisible(false);

	mGameOverLabel->SetColor(GLVector3f(1.0f, 0.0f, 0));

	// Add the GUILabel to the GUIContainer  
	shared_ptr<GUIComponent> game_over_component
		= static_pointer_cast<GUIComponent>(mGameOverLabel);
	mGameDisplay->GetContainer()->AddComponent(game_over_component, GLVector2f(0.5f, 0.5f));

	 
	CreateLifePopUpMessage();
}

void Asteroids::CreateLifePopUpMessage()
{
	// Create a new GUILabel and wrap it up in a shared_ptr
	mLivesPopUpLabel = shared_ptr<GUILabel>(new GUILabel("LIFE POWER UP!!! +1"));
	// Set the horizontal alignment of the label to GUI_HALIGN_CENTER
	mLivesPopUpLabel->SetHorizontalAlignment(GUIComponent::GUI_HALIGN_CENTER);
	// Set the vertical alignment of the label to GUI_VALIGN_MIDDLE
	mLivesPopUpLabel->SetVerticalAlignment(GUIComponent::GUI_VALIGN_MIDDLE);
	mLivesPopUpLabel->SetColor(GLVector3f(0.0f, 1.0f, 0.0f));

	// Set the visibility of the label to false (hidden)
	mLivesPopUpLabel->SetVisible(false);
	// Add the GUILabel to the GUIContainer  
	shared_ptr<GUIComponent> lifes_pop_up_component
		= static_pointer_cast<GUIComponent>(mLivesPopUpLabel);
	mGameDisplay->GetContainer()->AddComponent(lifes_pop_up_component, GLVector2f(0.5f, 0.5f));
}
 


void Asteroids::OnScoreChanged(int score)
{
	// Format the score message using an string-based stream
	std::ostringstream msg_stream;
	msg_stream << "Score: " << score;
	// Get the score message as a string
	std::string score_msg = msg_stream.str();
	mScoreLabel->SetText(score_msg);
}
 

void Asteroids::OnPlayerKilled(int lives_left)
{
	shared_ptr<GameObject> explosion = CreateExplosion();
	explosion->SetPosition(mSpaceship->GetPosition());
	explosion->SetRotation(mSpaceship->GetRotation());
	mGameWorld->AddObject(explosion);

	// Format the lives left message using an string-based stream
	std::ostringstream msg_stream;
	msg_stream << "Lives: " << lives_left;
	// Get the lives left message as a string
	std::string lives_msg = msg_stream.str();
	mLivesLabel->SetText(lives_msg);

	if (lives_left > 0) 
	{ 
		SetTimer(1000, CREATE_NEW_PLAYER); 
	}
	else
	{
		SetTimer(500, SHOW_GAME_OVER);
	}
}


void Asteroids::OnPlayerLifeKilled(int lives_left)
{


	// Format the lives left message using an string-based stream
	std::ostringstream msg_stream;
	msg_stream << "Lives: " << lives_left;
	// Get the lives left message as a string
	std::string lives_msg = msg_stream.str();
	mLivesLabel->SetText(lives_msg);


}

 

shared_ptr<GameObject> Asteroids::CreateExplosion()
{
	Animation *anim_ptr = AnimationManager::GetInstance().GetAnimationByName("explosion");
	shared_ptr<Sprite> explosion_sprite =
		make_shared<Sprite>(anim_ptr->GetWidth(), anim_ptr->GetHeight(), anim_ptr);
	explosion_sprite->SetLoopAnimation(false);
	shared_ptr<GameObject> explosion = make_shared<Explosion>();
	explosion->SetSprite(explosion_sprite);
	explosion->Reset();
	return explosion;
}




void Asteroids::CreateLife(const uint num_life)
{
	for (uint i = 0; i < num_life; i++) {
		Animation* anim_ptr = AnimationManager::GetInstance().GetAnimationByName("life");
		shared_ptr<Sprite> life_sprite =
			make_shared<Sprite>(anim_ptr->GetWidth(), anim_ptr->GetHeight(), anim_ptr);
		

		shared_ptr<GameObject> life = make_shared<Life>();
		life->SetBoundingShape(make_shared<BoundingSphere>(life->GetThisPtr(), 6));
		life->SetScale(0.1f);
			life->SetSprite(life_sprite);;
		mGameWorld->AddObject(life);
	}
}


void Asteroids::CreateShieldGUI()
{
	// Add a (transparent) border around the edge of the game display
	mGameDisplay->GetContainer()->SetBorder(GLVector2i(10, 10));
	// Create a new GUILabel and wrap it up in a shared_ptr
	  mShieldGUILabel = make_shared<GUILabel>("Shield: OFF");
	// Set the vertical alignment of the label to GUI_VALIGN_TOP
	mShieldGUILabel->SetVerticalAlignment(GUIComponent::GUI_VALIGN_TOP);
	// Add the GUILabel to the GUIComponent
	shared_ptr<GUIComponent> shield_component
		= static_pointer_cast<GUIComponent>(mShieldGUILabel);
	mGameDisplay->GetContainer()->AddComponent(shield_component, GLVector2f(0.67f, 1.0f));
}

void Asteroids::OnShieldChanged(bool shield)
{
	if (shield) {
		// Format the score message using an string-based stream
		std::ostringstream msg_stream;
		msg_stream << "Shield: ON";
		// Get the score message as a string
		std::string shield_msg = msg_stream.str();
		mShieldGUILabel->SetText(shield_msg);
	}
	else {
		// Format the score message using an string-based stream
		std::ostringstream msg_stream;
		msg_stream << "Shield: OFF";
		// Get the score message as a string
		std::string shield_msg = msg_stream.str();
		mShieldGUILabel->SetText(shield_msg);
	}
	
}