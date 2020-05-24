
#include "stdafx.h"
#include "CommonFunc.h"
#include "BaseObject.h"
#include "game_map.h"
#include "MainObject.h"
#include "ImpTimer.h"
#include "CrateObject.h"
#include "PointObject.h"

BaseObject g_background;

bool InitData(){
	bool success= true;
	int ret = SDL_Init(SDL_INIT_VIDEO);
	if(ret<0){
		return false;
	}
	SDL_SetHint(SDL_HINT_RENDER_SCALE_QUALITY,"1");

	// Create window game
	g_window = SDL_CreateWindow("Game SDL2.0",SDL_WINDOWPOS_UNDEFINED,
											  SDL_WINDOWPOS_UNDEFINED,
											  SCREEN_WIDTH,SCREEN_HEIGHT,
											  SDL_WINDOW_SHOWN);

	if(g_window==NULL){
		success = false;
	}
	else{
		g_screen = SDL_CreateRenderer(g_window,-1,SDL_RENDERER_ACCELERATED);
		if(g_screen==NULL){
			success = false;
		}
		else{
			SDL_SetRenderDrawColor(g_screen, RENDER_DRAW_COLOR , RENDER_DRAW_COLOR , RENDER_DRAW_COLOR , RENDER_DRAW_COLOR);
			int imgFlags = IMG_INIT_PNG;
			if(!(IMG_Init(imgFlags) && imgFlags)){
				success = false;
			}
		}
	}
	

	return success;
}

bool LoadBackground(){
	bool ret = g_background.loadImage("img//background_soukoban.png",g_screen);
	if(ret==false){
		return false;
	}
	return true;
}

void close(){
	g_background.Free();

	SDL_DestroyRenderer(g_screen);
	g_screen = NULL;

	SDL_DestroyWindow(g_window);
	g_window=NULL;

	IMG_Quit();
	SDL_Quit();
}

std::vector<CrateObject*> MakeCrateList(){
	std::vector<CrateObject*> list_crate;
	CrateObject* crate_object_list = new CrateObject[12];
	FILE* fp = NULL;
	char* namefile = "map/Crate01.dat";
	int posList[3][8];
	int temp,index=0;
	
	fopen_s(&fp,namefile,"rb");
	if(fp == NULL){
		return list_crate;
	}
	for(int j=0;j<3;j++){
		for(int i=0;i<8;i++){
			fscanf_s(fp,"%d",&temp);
			posList[j][i] = temp;
		}
	}
	for(int j=0;j<3;j++){
		for(int i=0;i<8;i+=2){
			CrateObject* crate_object = crate_object_list + index;
			crate_object->loadImage("map/5.png",g_screen);
			crate_object->set_clips();
			crate_object->set_x_pos(posList[j][i]);
			crate_object->set_y_pos(posList[j][i+1]);
			list_crate.push_back(crate_object);
			index++;
		}
	}

	return list_crate;
}

std::vector<PointObject*> MakePointList(){
	std::vector<PointObject*> point_list;
	PointObject* point_object_list = new PointObject[12];
	FILE* fp = NULL;
	char*  namefile = "map/Point01.dat";
	int temp,index=0,posList[3][8];
	fopen_s(&fp,namefile,"rb");
	if(fp == NULL){
		return point_list;
	}

	for(int j=0;j<3;j++){
		for(int i=0;i<8;i++){
			fscanf_s(fp,"%d",&temp);
			posList[j][i] = temp;
		}
	}
	for(int j=0;j<3;j++){
		for(int i=0;i<8;i+=2){
			PointObject* point_object = point_object_list + index;
			point_object->loadImage("map/19.png",g_screen);
			point_object->set_clips();
			point_object->set_x_pos(posList[j][i]);
			point_object->set_y_pos(posList[j][i+1]);
			point_list.push_back(point_object);
			index++;
		}
	}
	return point_list;
}

bool  checkLevel(std::vector<CrateObject*> list_crate,int level){
	int dem=0;
	for(int i=0;i<list_crate.size();i++){
		CrateObject* crate_object;
		crate_object = list_crate.at(i);
		int status_crate = crate_object->getCrateStatus();
		if(status_crate==1){
			dem++;
		}
	}
	if((level==0&&dem==4)||(level==1&&dem==8)||(level==2&&dem==12)){
		return true;
	}
	else return false;
}

void start_game(){

}
 
int main(int argc, char* argv[])
{
	// create value time
	ImpTimer fps_timer;

	if(InitData() == false){
		return -1;	
	}
	if(LoadBackground()==false){
		return -1;
	}

	// load map
	GameMap game_map;
	char* name;
	name = "map/map01.dat";
	game_map.LoadMap(name);
	game_map.LoadTiles(g_screen);



	//load player
	MainObject p_player;
	p_player.setPos(64,64);
	p_player.loadImage("img//down.png",g_screen);
	p_player.Set_clip();


	// load crate
	std::vector<CrateObject*> list_crate = MakeCrateList();

	// load point
	std::vector<PointObject*> list_point = MakePointList();
	
	int level=0;
	// loop game 
	bool is_quit = false;
	while(!is_quit){
		//check time
		fps_timer.start();
		
		
		
		// check status game
		while(SDL_PollEvent(&g_event)!=0){
			if(g_event.type == SDL_QUIT){
				is_quit = true;
			}
			p_player.HandelInputAction(g_event,g_screen);
		}
		// clear + load screen
		SDL_SetRenderDrawColor(g_screen,RENDER_DRAW_COLOR,RENDER_DRAW_COLOR,RENDER_DRAW_COLOR,RENDER_DRAW_COLOR);
		SDL_RenderClear(g_screen);

		g_background.Render(g_screen,NULL);

		// draw map && show player
		Map map_data = game_map.getMap();

		p_player.DoPlayer(map_data);
		
		p_player.Show(g_screen);

		game_map.DrawMap(g_screen);

		
		int i_point,i_crate;
		if(level == 0){
			i_crate = 0;
			i_point = 0;
		}
		if(level == 1){
			i_point = 4;
			i_crate = 4;
		}
		if(level == 2){
			i_point = 8;
			i_crate = 8;
		}
		// show point and check collision point && crate
		for(int i=i_point;i<i_point+4;i++){
			PointObject* p_point = list_point.at(i);
			SDL_Rect rect_point = p_point->getRectFrame();
			p_point->Show(g_screen);
			for(int j=i_crate;j<i_crate+4;j++){
				CrateObject* p_crate = list_crate.at(j);
				SDL_Rect rect_crate = p_crate->getRectFrame();
				bool collisionCratePoint = SDLCommonFunc::Crate_Point(rect_crate,rect_point);
				if(collisionCratePoint){
					p_crate->setCrateStatus(1);
				}
				bool win_part = checkLevel(list_crate,level);
				if(win_part && level < 2){
					level++;
					i+=4;
					break;
				}
				if(win_part && level == 2){
					is_quit = true;
				}
				
			}
		}

		//show crate and check collision player && crate
		for(int i = i_crate;i<i_crate+4;i++){
			CrateObject* p_crate = list_crate.at(i);
			p_crate->Show(g_screen);
			p_crate->set_x_val(0);
			p_crate->set_y_val(0);
			

			SDL_Rect rect_player = p_player.getRectFrame();
			SDL_Rect rect_crate = p_crate->getRectFrame();
			bool collision = SDLCommonFunc::CheckCollision(rect_player,rect_crate);
			bool player_move_left = p_player.player_move_left();
			bool player_move_right = p_player.player_move_right();
			bool player_move_up = p_player.player_move_up();
			bool player_move_down = p_player.player_move_down();
			int collisonPos = SDLCommonFunc::CheckCollisonPos(rect_player,rect_crate);

			if(collision){
				if(collisonPos==1 && player_move_left){
					p_crate->set_x_val(-SPEED_PLAYER);
				}
				if(collisonPos==2 && player_move_right){
					p_crate->set_x_val(+SPEED_PLAYER);
				}
				if(collisonPos==3 && player_move_down){
					p_crate->set_y_val(+SPEED_PLAYER);
				}
				if(collisonPos==4 && player_move_up){
					p_crate->set_y_val(-SPEED_PLAYER);
				}
				
				p_crate->CheckToMap(map_data);
			}
			
		}
	

		SDL_RenderPresent(g_screen);

		int real_imp_time = fps_timer.get_ticks();
		int time_one_frame = 1000/FRAME_PER_SECOND; //milisecond

		if(real_imp_time < time_one_frame){
			int delay_time = time_one_frame - real_imp_time;
			if(delay_time>=0){
				SDL_Delay(delay_time);
			}
		}

	}

	close();
    return 0;
}
