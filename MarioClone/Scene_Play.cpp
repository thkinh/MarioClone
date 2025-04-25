#include "Scene_Play.h"
#include "GameEngine.h"
#include <fstream>
#include <iostream>
#include "Physics.h"

Scene_Play::Scene_Play(GameEngine* g, const std::string& levelPath):
  Scene(g), m_levelpath(levelPath), m_gridText(m_game->assets().getFont("Arial"))
{
  std::cout << "\n====Scene Play====\n";
  init(m_levelpath);
}
void Scene_Play::init(const std::string& levelPath)
{
  Play_Font = m_game->assets().getFont("Arial");
  registerAction(sf::Keyboard::Scancode::P, "PAUSE");
  registerAction(sf::Keyboard::Scancode::Escape, "QUIT");
  registerAction(sf::Keyboard::Scancode::T, "TOGGLE_TEXTURE");
  registerAction(sf::Keyboard::Scancode::C, "TOGGLE_COLLISION");
  registerAction(sf::Keyboard::Scancode::G, "TOGGLE_GRID"); 
  registerAction(sf::Keyboard::Scancode::F, "TOGGLE_FPS"); 
  registerAction(sf::Keyboard::Scancode::W, "UP"); 
  registerAction(sf::Keyboard::Scancode::B, "SHOOT");
  registerAction(sf::Keyboard::Scancode::A, "LEFT"); 
  registerAction(sf::Keyboard::Scancode::S, "DOWN");
  registerAction(sf::Keyboard::Scancode::D, "RIGHT");
  registerAction(sf::Keyboard::Scancode::LShift, "RUN");
  //registerAction(sf::Keyboard::P, "Pause");
  //registerAction(sf::Keyboard::P, "Pause");
  std::cout << "\nRegistered actions\n";
  m_gridText.setCharacterSize(10);
  LoadLevel(levelPath);
  m_gridText.setFont(Play_Font);
  SpawnPlayer();
  std::cout << "Current player position: (" << m_player->getComponent<CTransform>().pos.x << ", " 
            << m_player->getComponent<CTransform>().pos.y << ")\n";
}

Vec2D Scene_Play::gridToMidPixel(float gridx, float gridy, std::shared_ptr<Entity> entity)
{
  auto& ePos = entity->getComponent<CTransform>().pos;
  auto& eBB = entity->getComponent<CBoundingBox>();
  ePos.y = (height()-(gridy * m_gridsize.y)) - eBB.size.y;
  ePos.x = gridx * m_gridsize.x;
  //ePos.x = gridx * m_gridsize.x ;
  //ePos.y = (gridy * m_gridsize.y) ;
  return Vec2D(ePos.x + eBB.halfsize.x, ePos.y + eBB.halfsize.y);
}


void Scene_Play::LoadLevel(const std::string& filename)
{
  //We reset our EntitiesManager everytime we load our level.
  m_entitiesManger = EntityManager();
  std::fstream file;
  file.open(filename);
  std::string var;
  while (file>>var)
  {
    if (var == "#")
    {
      std::string tmp="";
      std::getline(file, tmp);
      std::cout << "Skipping: " << tmp << "\n";
    }
    if (var == "Tile")
    {
      int x, y;
      file >> x >> y;
      SpawnTile(x, y);
      continue;
    }
    if (var == "Player")
    {
      file >> m_playerConfig.X >> m_playerConfig.Y >> m_playerConfig.CX >> m_playerConfig.CY
        >> m_playerConfig.SPEED >> m_playerConfig.JUMP >> m_playerConfig.MAXSPEED >> m_playerConfig.GRAVITY;

    }
  }
  std::cout << "Loaded player\n";
  std::cout << "Player Jump Speed: "<< m_playerConfig.JUMP << "/" << m_playerConfig.MAXSPEED << "\n";
  std::cout << "Player Gravity: "<< m_playerConfig.GRAVITY << "\n"; 
  file.close();
}

void Scene_Play::SpawnPlayer()
{
  m_player = m_entitiesManger.addEntity("player");
  m_player->addComponent<CBoundingBox>(Vec2D(m_playerConfig.CX, m_playerConfig.CY));
  m_player->addComponent<CTransform>(gridToMidPixel(m_playerConfig.X, m_playerConfig.Y, m_player));
  m_player->addComponent<CInput>();
  m_player->addComponent<CGravity>(m_playerConfig.GRAVITY);
  m_player->addComponent<CState>();
  m_player->addComponent<CAnimation>();
  m_player->getComponent<CTransform>().scale = {1.5f,1.5f}; //Player size = 48
  m_player->getComponent<CAnimation>().anmt = m_game->assets().getAnimation("Stand");
  std::cout << "Spawned player\n";
}

void Scene_Play::SpawnBullet()
{
  auto bullet = m_entitiesManger.addEntity("bullet");
  bullet->addComponent<CTransform>(m_player->getComponent<CTransform>().pos);
  bullet->addComponent<CBoundingBox>(Vec2D(m_playerConfig.CX/6, m_playerConfig.CY/6));
  bullet->addComponent<CAnimation>();
  auto& transform = bullet->getComponent<CTransform>();
  transform.scale = m_player->getComponent<CTransform>().scale;
  transform.velocity.x = 30*((transform.scale.x > 0) - (transform.scale.x < 0));
  transform.pos.x += 24*((transform.scale.x > 0) - (transform.scale.x < 0));
  bullet->getComponent<CAnimation>().anmt = m_game->assets().getAnimation("Bullet");
}

void Scene_Play::SpawnTile(float gridx, float gridy)
{
  auto tile = m_entitiesManger.addEntity("tile");
  tile->addComponent<CBoundingBox>(Vec2D(m_gridsize.x, m_gridsize.y));
  tile->addComponent<CTransform>(gridToMidPixel(gridx,gridy,tile));
  //std::cout << "Spawned a tile at << (" << tile->getComponent<CTransform>().pos.x << "," << tile->getComponent<CTransform>().pos.y << ")\n";
  tile->getComponent<CTransform>().scale = Vec2D(1.34f, 1.34f); //Block size = 64
  tile->addComponent<CAnimation>();
  tile->getComponent<CAnimation>().anmt = m_game->assets().getAnimation("TileDust");
}


void Scene_Play::sDoAction(const Action& action)
{
  if (action.type() == "START")
  {
    if (action.name() == "TOGGLE_TEXTURE") { m_drawTexture = !m_drawTexture; }
    else if (action.name() == "TOGGLE_COLLISION") { m_drawCollision = !m_drawCollision; }
    else if (action.name() == "TOGGLE_GRID") { m_drawGrid = !m_drawGrid; }
    else if (action.name() == "TOGGLE_FPS") { m_drawFPS = !m_drawFPS; }
    else if (action.name() == "QUIT") { onEnd(); }
    //Movements
    else if (action.name() == "SHOOT") { m_player->getComponent<CInput>().action1 = true; }
    else if (action.name() == "UP"){ m_player->getComponent<CInput>().up = true;}
    else if (action.name() == "RIGHT") { m_player->getComponent<CInput>().right = true;  }
    else if (action.name() == "LEFT") { m_player->getComponent<CInput>().left = true; }
    else if (action.name() == "RUN") { m_player->getComponent<CInput>().running = true; }
  }
  else if (action.type() == "END")
  {
    if (action.name() == "SHOOT") { m_player->getComponent<CInput>().action1 = false; }
    if (action.name() == "UP") {m_player->getComponent<CInput>().up = false;}
    else if (action.name() == "RIGHT") { m_player->getComponent<CInput>().right = false; }
    else if (action.name() == "LEFT") { m_player->getComponent<CInput>().left = false; }
    else if (action.name() == "RUN") { m_player->getComponent<CInput>().running = false; }
    else if (action.name() == "DOWN") { m_player->getComponent<CInput>().down = false; }
  }
}

void Scene_Play::sMovement()
{
  Vec2D player_velocity(0, m_player->getComponent<CTransform>().velocity.y);
  int& current_state = m_player->getComponent<CState>().state;
  current_state = 0;
  Vec2D& playerScale = m_player->getComponent<CTransform>().scale;

  int player_jump_speed = 12;
  
  if(m_player->getComponent<CInput>().action1 == true)
  {
    m_player->getComponent<CState>().coolDown -= 1;
    if(m_player->getComponent<CState>().coolDown <= 0)
    {
      SpawnBullet();
      m_player->getComponent<CState>().coolDown = 10;
    }
  }
  if(m_player->getComponent<CInput>().action1 == false)
  {
    m_player->getComponent<CState>().coolDown = 1;
  }

  if (m_player->getComponent<CInput>().up == false )
  {
    //Release the button at mid air won't let them move up the y direction anymore
    if (player_velocity.y < 0)
    {
      current_state = 2;
      m_player->getComponent<CInput>().up = false;
    }
    //If player just landed on the ground, only allow them to jump when they release the button
    else if (player_velocity.y == 0)
    {
    }
  }
  Vec2D& prevVelo = m_player->getComponent<CTransform>().prevVelocity;
  if(player_velocity.y > 0 || player_velocity.y <= -24 || player_velocity.y > prevVelo.y)
  {
    m_player->getComponent<CInput>().up = false; 
    current_state = 2;
  }
  else if (m_player->getComponent<CInput>().up && player_velocity.y <= 0) 
  {
    //std::cout << "\nJump with the speed: " << player_velocity.y << "/" <<player_jump_speed;
    if (player_velocity.y > -24)
    {
      current_state = 2;
      player_jump_speed -= (player_velocity.y < 0)? 6 : 0;
      player_velocity.y -= player_jump_speed;
      //std::cout << "\nMaxSpeed";
    }
  }

  if (m_player->getComponent<CInput>().right == true)
  {
    player_velocity.x = 7;
    current_state = (current_state == 2)? 2:4;
    playerScale.x = std::abs( playerScale.x );
  }
  else if (m_player->getComponent<CInput>().left == true)
  {
    player_velocity.x = -7;
    current_state = (current_state == 2)? 2:4;
    playerScale.x = -std::abs(playerScale.x);
  }
  if (m_player->getComponent<CInput>().running == true)
  {
    player_velocity.x *= 1.55;
  }
  m_player->getComponent<CTransform>().prevVelocity.y = m_player->getComponent<CTransform>().velocity.y;
  m_player->getComponent<CTransform>().velocity = player_velocity;
  for (auto& e : m_entitiesManger.getEntity())
  {		
    if (e->getComponent<CGravity>().has)
    {
      e->getComponent<CTransform>().velocity.y += e->getComponent<CGravity>().gravity;
    }
    e->getComponent<CTransform>().prevPos = e->getComponent<CTransform>().pos;
    e->getComponent<CTransform>().pos += e->getComponent<CTransform>().velocity;
  }
  
}

void Scene_Play::sCollision()
{
  for (auto& t : m_entitiesManger.getEntity("tile"))
  {
    Vec2D overlap = Physics::GetOverLap(t, m_player);
    auto& PlayerPos = m_player->getComponent<CTransform>();
    if (overlap.x < 0 && overlap.y < 0)
    {
      Vec2D Pre_overlap = Physics::GetPreviousOverLap(m_player,t);
      //Overlap right here
      //Check the direction by looking at the frame before
      if (Pre_overlap.y >= 0)
      {
        //This means the overlap caused by the "y" direction
        if (PlayerPos.pos.y >= PlayerPos.prevPos.y)
        {
          //this means the overlap occured by the player moving down_ward
          m_player->getComponent<CTransform>().pos.y += overlap.y;
          //the overlap happened, so overlap.y or overlap,x is always negative
          m_player->getComponent<CTransform>().velocity.y = 0; 
          if (m_player->getComponent<CState>().state == 0)
          {
            m_player->getComponent<CState>().state = 1;
          }
        }
        else if (PlayerPos.pos.y < PlayerPos.prevPos.y)
        {
          //this means the overlap occured by the player moving up_ward
          m_player->getComponent<CTransform>().pos.y -= overlap.y;
          //the overlap happened, so overlap.y or overlap,x is always negative
          m_player->getComponent<CTransform>().velocity.y = 0;
          m_player->getComponent<CState>().state = 2;
        }
      }
      else if (Pre_overlap.x >= 0)
      {
        //This means the overlap caused by the "x" direction
        if (PlayerPos.pos.x > PlayerPos.prevPos.x)
        {
          //this means the overlap occured by the player moving from left to right
          m_player->getComponent<CTransform>().pos.x += overlap.x;
          //the overlap happened, so overlap.y or overlap,x is always negative
        }
        else if (PlayerPos.pos.x < PlayerPos.prevPos.x)
        {			
          //this means the overlap occured by the player moving from right to left
          m_player->getComponent<CTransform>().pos.x -= overlap.x;
          //the overlap happened, so overlap.y or overlap,x is always negative
        }
      }
    }
  }
}

void Scene_Play::sLifeSpan()
{
  if (m_player->getComponent<CTransform>().pos.y > m_game->window().getSize().y)
  {
    m_player->destroy();
    SpawnPlayer();
  }
  for(auto& b : m_entitiesManger.getEntity("bullet"))
  {
    for (auto& t : m_entitiesManger.getEntity("tile"))
    {
      Vec2D overlap = Physics::GetOverLap(b,t);
      if(overlap.x <= 0 && overlap.y <= 0)
      {
        b->destroy();
        break;
      }
    }
    size_t windowSizeRight = m_game->window().getSize().x + m_player->getComponent<CTransform>().pos.x;
    if (b->getComponent<CTransform>().pos.x > windowSizeRight || b->getComponent<CTransform>().pos.x < 0)
    {
      b->destroy();
    }
  }
}

void Scene_Play::sAnimation()
{
  auto& playerstate = m_player->getComponent<CState>();
  Animation& player_animation = m_player->getComponent<CAnimation>().anmt;
  Vec2D& playerVelo = m_player->getComponent<CTransform>().velocity;

  if (playerstate.state == 4)
  {
    if(player_animation.getName() != "Run") 
    {
      player_animation = m_game->assets().getAnimation("Run");
    }
  }
  if ( playerstate.state == 1)  // just land on ground
  {
    if (player_animation.getName() != "Stand")
    {
      player_animation = m_game->assets().getAnimation("Stand");
    }
  }
  else if (playerstate.state == 2)
  {
    if(player_animation.getName() != "Jump")
    {
      player_animation = m_game->assets().getAnimation("Jump");
    }
  }
  for (auto& e : m_entitiesManger.getEntity())
  {
    e->getComponent<CAnimation>().anmt.update();
  }

}


void Scene_Play::update()
{
  m_entitiesManger.update();
  sMovement();
  sLifeSpan();
  sCollision();
  sAnimation();
  sRender();
}

void Scene_Play::sRender () 
{
  m_game->window().clear(sf::Color(6,3,3));
  //Set the camera view port to be the centered on the player if it's far enough right
  Vec2D& pPos = m_player->getComponent<CTransform>().pos;
  float windowCenterX = std::max(m_game->window().getSize().x / 2.0f, pPos.x);
  sf::View view = m_game->window().getView();
  view.setCenter({windowCenterX, m_game->window().getSize().y - view.getCenter().y});
  //float windowCenterY = pPos.y;
  //view.setCenter({windowCenterX, windowCenterY});
  m_game->window().setView(view);

  //drawTexture & animation
  if (m_drawTexture)
  {
    for (auto& e : m_entitiesManger.getEntity()) 
    {
      auto& transform = e->getComponent<CTransform>();
      if (e->hasComponent<CAnimation>())
      {
        auto& animation = e->getComponent<CAnimation>().anmt;
        animation.getSprite().setRotation(sf::degrees(transform.angle));
        animation.getSprite().setPosition({transform.pos.x, transform.pos.y});
        animation.getSprite().setScale({transform.scale.x, transform.scale.y});
        m_game->window().draw(animation.getSprite());
      }
    }
  }

  //Draw grid
  if (m_drawGrid)
  {
    float leftX = m_game->window().getView().getCenter().x - (float)width() / 2;
    float rightX = leftX + width() + m_gridsize.x;
    float nextGridX = leftX - ((int)leftX % (int)m_gridsize.x);
    sf::VertexArray lines(sf::PrimitiveType::Lines);
    for (float x = nextGridX; x < rightX; x+=m_gridsize.x)
    {
      sf::Vertex vertical_start{{x, 0}};
      sf::Vertex vertical_end{sf::Vector2f(x, (float)height())};
      lines.append(vertical_start);
      lines.append(vertical_end);
    }

    for (float y = 0; y < height(); y += m_gridsize.y)
    {
      sf::Vertex vertical_start{sf::Vector2f(0, y)};
      sf::Vertex vertical_end{sf::Vector2f((float)width(),y )};
      lines.append(vertical_start);
      lines.append(vertical_end);
      for (float x = nextGridX; x < rightX; x += m_gridsize.x)
      {
        std::string xCell = std::to_string((int)x / (int)m_gridsize.x);
        std::string yCell = std::to_string((int)y / (int)m_gridsize.y);
        m_gridText.setString(xCell + "," + yCell);
        m_gridText.setPosition({x + 3, height() - y - m_gridsize.y + 2});
        m_game->window().draw(m_gridText);
      }
    }
    m_game->window().draw(lines);
  }

  //draw Collion Bounding Boxes with a rectangle shape
  if (m_drawCollision)
  {
    for (auto& e : m_entitiesManger.getEntity())
    {
      if (e->hasComponent<CBoundingBox>())
      {
        auto& box = e->getComponent<CBoundingBox>();
        auto& transfrom = e->getComponent<CTransform>();
        sf::RectangleShape rect;
        rect.setSize(sf::Vector2f(box.size.x-1, box.size.y-1));
        rect.setOrigin(sf::Vector2f(box.halfsize.x, box.halfsize.y));
        rect.setPosition(sf::Vector2f(transfrom.pos.x, transfrom.pos.y));
        rect.setFillColor(sf::Color(0, 255, 0, 30));
        rect.setOutlineColor(sf::Color(255, 0, 0));
        rect.setOutlineThickness(1);
        m_game->window().draw(rect);
        //m_game->window().draw(m_gridText);
      }
    }
  }
  //if(m_drawFPS)
  //{
  //  m_gridText.setString("FPS: " + std::to_string(m_game->getFPS()));
  //  m_gridText.setPosition({10,10});
  //  m_game->window().draw(m_gridText);
  //}
  m_game->window().display();
}

void Scene_Play::onEnd()
{
  sf::View view = m_game->window().getView();
  view.setCenter({m_game->window().getSize().x / 2.0f, m_game->window().getSize().y/2.0f});
  m_game->window().setView(view);
  m_game->ChangeScene("MENU", nullptr, true);
}
