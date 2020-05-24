
#include "stdafx.h"
#include "MainObject.h"
#include "CrateObject.h"

CrateObject crate;

MainObject::MainObject(){
	frame_ = 0;
	x_pos_ = 0;
	y_pos_ = 0;
	x_val_ = 0;
	y_val_ = 0;
	width_frame_ = 0;
	height_frame_ = 0;
	status_ = -1;
	input_type_.left_ = 0;
	input_type_.right_ = 0;
	input_type_.up_ = 0;
	input_type_.down_ = 0;
}
MainObject::~MainObject(){

}

SDL_Rect MainObject::getRectFrame(){
	SDL_Rect rect;
	rect.x = rect_.x;
	rect.y = rect_.y;
	rect.w = width_frame_;
	rect.h = height_frame_;

	return rect;
}

bool MainObject::loadImage(std::string path,SDL_Renderer* screen){
	bool ret = BaseObject::loadImage(path,screen);
	if(ret==true){
		width_frame_ = rect_.w/6;
		height_frame_ = rect_.h;
	}
	 
	return ret;
}

void MainObject::Set_clip(){
	if(width_frame_>0 && height_frame_>0){
		//frame 0
		frame_clip_[0].x = 0;
		frame_clip_[0].y = 0;
		frame_clip_[0].w = width_frame_;
		frame_clip_[0].h = height_frame_;

		// frame 1
		frame_clip_[1].x = width_frame_;
		frame_clip_[1].y = 0;
		frame_clip_[1].w = width_frame_;
		frame_clip_[1].h = height_frame_;

		//frame 2
		frame_clip_[2].x = 2*width_frame_;
		frame_clip_[2].y = 0;
		frame_clip_[2].w = width_frame_;
		frame_clip_[2].h = height_frame_;

		//frame 3
		frame_clip_[3].x = 3*width_frame_;
		frame_clip_[3].y = 0;
		frame_clip_[3].w = width_frame_;
		frame_clip_[3].h = height_frame_;

		//frame 4
		frame_clip_[4].x = 4*width_frame_;
		frame_clip_[4].y = 0;
		frame_clip_[4].w = width_frame_;
		frame_clip_[4].h = height_frame_;

		//frame 5
		frame_clip_[5].x = 5*width_frame_;
		frame_clip_[5].y = 0;
		frame_clip_[5].w = width_frame_;
		frame_clip_[5].h = height_frame_;

		//frame 6
		frame_clip_[6].x = 6*width_frame_;
		frame_clip_[6].y = 0;
		frame_clip_[6].w = width_frame_;
		frame_clip_[6].h = height_frame_;
	}
}

void MainObject::Show(SDL_Renderer* des){
	if(status_==WALK_LEFT){
		loadImage("img//left.png",des);
	}
	else if(status_==WALK_RIGHT){
		loadImage("img//right.png",des);
	}
	else if(status_==WALK_UP){
		loadImage("img//up.png",des);
	}
	else{
		loadImage("img//down.png",des);
	}

	if(input_type_.left_ == 1 ||
		input_type_.right_ ==1 ||
		input_type_.up_ == 1 ||
		input_type_.down_ == 1){

			frame_++;
	}
	else{
		frame_=0;
	}
	if(frame_>=6){
		frame_=0;
	}

	rect_.x = x_pos_;
	rect_.y = y_pos_;

	SDL_Rect* current_clip = &frame_clip_[frame_];
	SDL_Rect renderQuad = {rect_.x,rect_.y,width_frame_,height_frame_};

		SDL_RenderCopy(des,p_object_,current_clip,&renderQuad);
	
}

void MainObject::HandelInputAction(SDL_Event events,SDL_Renderer* screen){
	if(events.type == SDL_KEYDOWN){
		switch (events.key.keysym.sym)
		{
		case SDLK_LEFT:{
			status_ = WALK_LEFT;
			input_type_.left_ =1;
			input_type_.right_ = 0;
			input_type_.up_ = 0;
			input_type_.down_ = 0;
					   }
					   break;
		case SDLK_RIGHT:{
			status_ = WALK_RIGHT;
			input_type_.right_ =1;
			input_type_.left_ = 0;
			input_type_.up_ = 0;
			input_type_.down_ = 0;
					   }
					   break;
		case SDLK_UP:{
			status_ = WALK_UP;
			input_type_.up_ =1;
			input_type_.right_ = 0;
			input_type_.left_ = 0;
			input_type_.down_ = 0;
					   }
					   break;
		case SDLK_DOWN:{
			status_ = WALK_DOWN;
			input_type_.down_ =1;
			input_type_.right_ = 0;
			input_type_.up_ = 0;
			input_type_.left_ = 0;
					   }
					   break;
		}
	}
	else if(events.type == SDL_KEYUP){
		switch (events.key.keysym.sym)
		{
		case SDLK_LEFT:{
			input_type_.left_ =0;
					   }
					   break;
		case SDLK_RIGHT:{
			input_type_.right_ =0;
					   }
					   break;
		case SDLK_UP:{
			input_type_.up_ =0;
					   }
					   break;
		case SDLK_DOWN:{
			input_type_.down_ =0;
					   }
					   break;
		}
	}
}

void MainObject::DoPlayer(Map& map_data){
	x_val_ = 0;
	y_val_ = 0;
	if(input_type_.left_==1){
		x_val_ -= SPEED_PLAYER;
	}
	else if(input_type_.right_==1){
		x_val_ += SPEED_PLAYER;
	}
	else if(input_type_.up_==1){
		y_val_ -= SPEED_PLAYER;
	}
	else if(input_type_.down_==1){
		y_val_ += SPEED_PLAYER;
	}
	
	CheckToMap(map_data);
}

bool MainObject::player_move_left(){
	if(input_type_.left_==1){
		return true;
	}
	return false;
}

bool MainObject::player_move_right(){
	if(input_type_.right_==1){
		return true;
	}
	return false;
}
bool MainObject::player_move_up(){
	if(input_type_.up_==1){
		return true;
	}
	return false;
}
bool MainObject::player_move_down(){
	if(input_type_.down_==1){
		return true;
	}

}

void MainObject::CheckToMap(Map& map_data){
	int x1=0;
	int x2=0;

	int y1=0;
	int y2=0;

	//Check horizontal
	int height_min = height_frame_ < TILE_SIZE ? height_frame_ : TILE_SIZE;
	x1 = (x_pos_ + x_val_)/TILE_SIZE;
	x2 = (x_pos_ + x_val_ + width_frame_ - 1)/TILE_SIZE;

	y1 = (y_pos_)/TILE_SIZE;
	y2 = (y_pos_ + height_min - 1)/TILE_SIZE;

	if(x1>=0 && x2 < MAX_MAP_X && y1>=0 && y2 < MAX_MAP_Y){
		if(x_val_>0){
			if(map_data.tile[y1][x2] != BLANK_TILE || map_data.tile[y2][x2] != BLANK_TILE){
				x_pos_ = x2*TILE_SIZE;
				x_pos_ -= (width_frame_ + 1);
				x_val_= 0;
			}
		}
		else if(x_val_<0){
			if(map_data.tile[y1][x1] != BLANK_TILE || map_data.tile[y2][x1] != BLANK_TILE){
				x_pos_ = (x1+1)*TILE_SIZE;
				x_val_ = 0;
			}
		}
	}

	//Check vertical
	int width_min = width_frame_ < TILE_SIZE ? width_frame_ : TILE_SIZE;
	x1 = (x_pos_)/TILE_SIZE;
	x2 = (x_pos_+width_min)/TILE_SIZE;

	y1 = (y_pos_+y_val_)/TILE_SIZE;
	y2 = (y_pos_+y_val_+height_frame_-1)/TILE_SIZE;

	if(x1>=0 && x2 < MAX_MAP_X && y1>=0 && y2 < MAX_MAP_Y){
		if(y_val_>0){
			if(map_data.tile[y2][x1] != BLANK_TILE || map_data.tile[y2][x2] != BLANK_TILE ){
				y_pos_ = y2*TILE_SIZE;
				y_pos_ -= (height_frame_ + 1);
				y_val_= 0;
			}
		}
		else if(y_val_<0){
			if(map_data.tile[y1][x1] != BLANK_TILE || map_data.tile[y1][x2] != BLANK_TILE){
				y_pos_ = (y1+1)*TILE_SIZE;
				y_val_ = 0;
			}
		}
	}

	x_pos_+=x_val_;
	y_pos_+=y_val_;
}