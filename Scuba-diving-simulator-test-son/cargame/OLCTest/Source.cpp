#include"olcSimpleEngine.h"
#include<memory>
#include<iostream>
#include<Windows.h>
#include<chrono>
#include<thread>
#include<vector>
/*
	INSTRUCTIONS:

	Controls: W,A,S,D

	Shoot: SPACE

	Game ends if the player collides with an enemy or an asteroid

*/


double depth;
double INIT_depth;
double velocity1;
double velocity;
double INIT_velocity;
double accelaration;
double net_force;
double mass;
double air;
double	INIT_air;
double delayed_inflow2;
double inflow_air;

double delayed_outflow2; 
double outflow_air;
double into;
double INIT_into;
double INIT_dt;
double dt;

	
double delayed_inflow1;
double inflow_delay_time;
double	out_to; 
double	INIT_out_to;
 
double	delayed_outflow1;

double	perceived_depth;
double INIT_perceived_depth;
double	Noname_2;
double	score;
double	INIT_score;

double Noname_4;
double	Air_Adjustment_Decision;
double	Both_Delays_05_sec_05_sec;
double	cross_sectional_area;
double	density;
double	denstity_of_a_human_body;
double	depth_perception_time;
double	desired_height;
double	discrepancy;
double	disturbance;
double	effective_depth;
double	flow_1;
double	flow_3;
double	frictional_force;
double	frictional_force_vector;

double	gravitational_constant;
double	height_of_the_man;

double	Information_Delay_05_sec;
double	lifting_force;
double	lt_m3_donusumu;
double	Material_Delay_05_sec;
double	minus_velocity = -velocity;
double	normal_flow;
double	No_Delay;
double	pressure;
double	radius;
double	radius_of_the_man;
double	RT;
double	volume_in_water;

double vol_bal_in_water;
double vol_man_in_water;
double	volume_lt;
double	volume_m3;
double	volume_of_the_man;
	
double	weight;
double	Your_depth;
double	You_are_at_10_mt_but_there_is_a_disturbance_Stabilize_at_10_mt;
double	You_are_at_20_mt_depth__Stabilize_at_10_mt;
double	You_are_at_the_surface__Stabilize_at_10_mt;
double t;

//const std::chrono::milliseconds interval(1);
//const std::chrono::microseconds interval(7);
//const std::chrono::nanoseconds interval(6200);
double pulse(double air_value, double t_value) 
{
	if (t_value < 5.01 && t_value > 4.99) { return air_value; }

	else { return 0; }
}
class SpaceGame : public olc::PixelGameEngine
{
	//sprites
	olc::Sprite* backgroundSprite;
	

	//variables
	

	


	//game related variables
	bool gameOver = false;
	double totalTime = 0;
	bool gameStarted = false;
	bool gamePaused = false;

public:
	void Initialization() {
		t = 0;
		into = 0;
		//initial konum seçme kodu yaz sadece bir kez input alýnacak
		You_are_at_the_surface__Stabilize_at_10_mt = 0;
		You_are_at_20_mt_depth__Stabilize_at_10_mt = 0;
		You_are_at_10_mt_but_there_is_a_disturbance_Stabilize_at_10_mt = 1;
		depth = You_are_at_10_mt_but_there_is_a_disturbance_Stabilize_at_10_mt * 10 + You_are_at_20_mt_depth__Stabilize_at_10_mt * 20 + You_are_at_the_surface__Stabilize_at_10_mt * 0;

		//effective depth
		if (depth < -0.5) { effective_depth = 0; }
		else if (depth < 0) { effective_depth = (0.5 + depth) / 2; }
		else { effective_depth = depth + 0.5 / 2; }

		pressure = 1 + effective_depth / 10;

		// flow 1 ve 3 için flow decision kodu yap------------------------------------- sadece bir kez input alýnacak
		flow_3 = 1;
		flow_1 = 0;
		//flow 1 ve 3 için flow decision kodu yap-----------------------------------------------

		normal_flow = flow_1 * 1 + flow_3 * 3;
		RT = 25;
		Air_Adjustment_Decision = 0; //onuser update de air adjustment kodu yazDIN input alýncak

		if (Air_Adjustment_Decision == 1) { inflow_air = normal_flow * Air_Adjustment_Decision * pressure / RT; }
		else { inflow_air = 0; }
		inflow_delay_time = 0.5;
		delayed_inflow1 = into / inflow_delay_time;

		No_Delay = 1;
		Material_Delay_05_sec = 0;
		Both_Delays_05_sec_05_sec = 0;
		Information_Delay_05_sec = 0;

		delayed_inflow2 = inflow_air * (1 - (Both_Delays_05_sec_05_sec + Material_Delay_05_sec)) + delayed_inflow1 * (Material_Delay_05_sec + Both_Delays_05_sec_05_sec);

		if (Air_Adjustment_Decision == -1) { outflow_air = normal_flow * Air_Adjustment_Decision * (-1) * pressure / RT; }
		else { outflow_air = 0; }

		mass = 90;
		denstity_of_a_human_body = 1070;
		volume_of_the_man = mass / denstity_of_a_human_body;
		air = pressure * (90 - volume_of_the_man * 1000) / RT;
		out_to = 0;
		delayed_outflow1 = out_to / inflow_delay_time;
		disturbance = You_are_at_10_mt_but_there_is_a_disturbance_Stabilize_at_10_mt;
		delayed_outflow2 = outflow_air * (1 - (Material_Delay_05_sec + Both_Delays_05_sec_05_sec)) + delayed_outflow1 * (Material_Delay_05_sec + Both_Delays_05_sec_05_sec) + disturbance * pulse(air / 4, t) * 100;
		perceived_depth = You_are_at_10_mt_but_there_is_a_disturbance_Stabilize_at_10_mt * 10 + You_are_at_20_mt_depth__Stabilize_at_10_mt * 20 + You_are_at_the_surface__Stabilize_at_10_mt * 0;
		depth_perception_time = 0.5;
		Noname_2 = (depth - perceived_depth) / depth_perception_time;
		Noname_4 = abs(10 - depth);
		velocity = 0;
		score = 0;
		density = 1000;
		gravitational_constant = 9.81;

		if (depth < -1.5) { vol_man_in_water = 0; }
		else
		{
			if (depth < 0) { vol_man_in_water = volume_of_the_man * (1.5 + depth) / 1.5; }
			else { vol_man_in_water = volume_of_the_man; }
		}

		volume_lt = air * RT / pressure;
		lt_m3_donusumu = 1000;
		volume_m3 = volume_lt / lt_m3_donusumu;
		//vol_bal_in_water
		if (depth < -0.5) { vol_bal_in_water = 0; }
		else if (depth < 0) { vol_bal_in_water = volume_m3 * (0.5 + depth) / 0.5; }
		else { vol_bal_in_water = volume_m3; }

		volume_in_water = vol_bal_in_water + vol_man_in_water;

		lifting_force = -volume_in_water * gravitational_constant * density;
		weight = mass * gravitational_constant;
		frictional_force = 27.2 * pow(velocity, 2);
		//frictional_force_vector
		if (velocity == 0) { frictional_force_vector = 0; }
		else { frictional_force_vector = -velocity / abs(velocity) * frictional_force; }

		net_force = weight + lifting_force + frictional_force_vector;
		accelaration = net_force / mass;
		velocity1 = velocity;
		height_of_the_man = 1.50;
		radius_of_the_man = pow((mass / (height_of_the_man * denstity_of_a_human_body * 3.1415)), (1 / 2));
		radius = pow(((volume_m3 / (3.1415 * (height_of_the_man - 1))) + pow(radius_of_the_man, 2)), (1 / 2));
		cross_sectional_area = 3.1415 * pow(radius, 2);
		desired_height = 10;
		discrepancy = desired_height - depth;
		//Information_Delay_05_sec = 0;
		minus_velocity = -velocity;
		//No_Delay = 0;
		Your_depth = depth * (-1) * (1 - (Information_Delay_05_sec + Both_Delays_05_sec_05_sec)) + perceived_depth * (-1) * (Information_Delay_05_sec + Both_Delays_05_sec_05_sec);

	}
	bool OnUserCreate() //override
	{
		const int n = 20; // number of times to run the loop
		double smoothed_time = 0.0; 
		INIT_dt = 0.01;
		t = 0;
		Initialization();
		// start timing
		 auto start = std::chrono::steady_clock::now();
		// perform loop operation here
		while (t < n * INIT_dt) {
			into = into + (inflow_air - delayed_inflow1) * INIT_dt;
			depth = depth + (velocity1)*INIT_dt;
			air = air + (delayed_inflow2 - delayed_outflow2) * INIT_dt;
			out_to = out_to + (outflow_air - delayed_outflow1) * INIT_dt;
			perceived_depth = perceived_depth + (Noname_2)*INIT_dt;
			velocity = velocity + (accelaration)*INIT_dt;
			score = score + (Noname_4)*INIT_dt;

			if (depth < -0.5) { effective_depth = 0; }
			else if (depth < 0) { effective_depth = (0.5 + depth) / 2; }
			else { effective_depth = depth + 0.5 / 2; }

			pressure = 1 + effective_depth / 10;
			normal_flow = flow_1 * 1 + flow_3 * 3;




			inflow_air = normal_flow * pressure / RT;


			delayed_inflow1 = into / inflow_delay_time;
			delayed_inflow2 = inflow_air * (1 - (Both_Delays_05_sec_05_sec + Material_Delay_05_sec)) + delayed_inflow1 * (Material_Delay_05_sec + Both_Delays_05_sec_05_sec);


			outflow_air = 0;
			delayed_outflow1 = out_to / inflow_delay_time;
			disturbance = You_are_at_10_mt_but_there_is_a_disturbance_Stabilize_at_10_mt;
			delayed_outflow2 = outflow_air * (1 - (Material_Delay_05_sec + Both_Delays_05_sec_05_sec)) + delayed_outflow1 * (Material_Delay_05_sec + Both_Delays_05_sec_05_sec) + disturbance * pulse(air / 4, t) * 100;

			volume_of_the_man = mass / denstity_of_a_human_body;
			Noname_2 = (depth - perceived_depth) / depth_perception_time;
			Noname_4 = abs(10 - depth);

			if (depth < -1.5) { vol_man_in_water = 0; }
			else
			{
				if (depth < 0) { vol_man_in_water = volume_of_the_man * (1.5 + depth) / 1.5; }
				else { vol_man_in_water = volume_of_the_man; }
			}

			volume_lt = air * RT / pressure;
			volume_m3 = volume_lt / lt_m3_donusumu;

			if (depth < -0.5) { vol_bal_in_water = 0; }
			else if (depth < 0) { vol_bal_in_water = volume_m3 * (0.5 + depth) / 0.5; }
			else { vol_bal_in_water = volume_m3; }

			volume_in_water = vol_bal_in_water + vol_man_in_water;
			lifting_force = -volume_in_water * gravitational_constant * density;
			weight = mass * gravitational_constant;
			frictional_force = 27.2 * pow(velocity, 2);

			//frictional_force_vector
			if (velocity == 0) { frictional_force_vector = 0; }
			else { frictional_force_vector = -velocity / abs(velocity) * frictional_force; }

			net_force = weight + lifting_force + frictional_force_vector;
			accelaration = net_force / mass;
			velocity1 = velocity;
			radius_of_the_man = pow((mass / (height_of_the_man * denstity_of_a_human_body * 3.1415)), (1 / 2));
			radius = pow(((volume_m3 / (3.1415 * (height_of_the_man - 1))) + pow(radius_of_the_man, 2)), (1 / 2));
			cross_sectional_area = 3.1415 * pow(radius, 2);
			discrepancy = desired_height - depth;
			minus_velocity = -velocity;
			Your_depth = depth * (-1) * (1 - (Information_Delay_05_sec + Both_Delays_05_sec_05_sec)) + perceived_depth * (-1) * (Information_Delay_05_sec + Both_Delays_05_sec_05_sec);

			t = t + INIT_dt;
		}
		// stop timing
		auto end = std::chrono::steady_clock::now();
		// calculate time difference
		double elapsed_time = std::chrono::duration_cast<std::chrono::microseconds>(end - start).count();
		
		if (elapsed_time/(n*pow(10,3))< 0.01) {
			dt = elapsed_time/ (n * pow(10, 3));  //this makes sense if dt is '1/100 of second'
		}
		else { dt = 0.01;}
		
		
		Initialization();
		

		return true;
	}
	bool OnUserUpdate(float fElapsedTime) //override
	{
		
		//Drawing the background
		Clear(olc::BLACK);
		SetPixelMode(olc::Pixel::ALPHA);
		//Drawing the background



		if (gameStarted)
		{
			if (gameOver) // game over screen
			{
				DrawString(50, 50, "GAME OVER" + std::to_string(totalTime));



				DrawString(50, 250, "PRESS E TO EXIT");


				if (GetKey(olc::E).bPressed)
					return false;
				else
					return true;
			}
			else // gameplay
			{
				if (gamePaused)
				{
					DrawString(50, 50, "GAME PAUSED");

					DrawString(50, 75, "PRESS ENTER TO CONTINUE");


					if (GetKey(olc::ENTER).bPressed)
						gamePaused = false;

					return true;
				}
				else // game runs here
				{
					totalTime += fElapsedTime;
					auto initialtime = clock();


					for (int x = 0; x <= ScreenWidth(); x++) {

						Draw(x, 500, olc::RED);
						Draw(x, 500-1, olc::RED);
						Draw(x, 500+1, olc::RED);
						Draw(x, 300, olc::RED);
						Draw(x, 300-1, olc::RED);
						Draw(x, 300+1, olc::RED);
						Draw(x, 100, olc::RED);
						Draw(x, 100+1, olc::RED);
						Draw(x, 100-1, olc::RED);
					}

					for (int y = 0; y <= ScreenHeight(); y++) {

						Draw(30, y, olc::RED);
						Draw(29, y, olc::RED);
						Draw(28, y, olc::RED);

					}

					DrawString(20, 300, "10");
					DrawString(20, 500, "20");
					DrawString(20, 100, "0");

					into = into + (inflow_air - delayed_inflow1) * dt;
					depth = depth + (velocity1)*dt;
					air = air + (delayed_inflow2 - delayed_outflow2) * dt;
					out_to = out_to + (outflow_air - delayed_outflow1) * dt;
					perceived_depth = perceived_depth + (Noname_2)*dt;
					velocity = velocity + (accelaration)*dt;
					score = score + (Noname_4)*dt;
					
					if (depth < -0.5) { effective_depth = 0; }
					else if (depth < 0) { effective_depth = (0.5 + depth) / 2; }
					else { effective_depth = depth + 0.5 / 2; }

					pressure = 1 + effective_depth / 10;
					normal_flow = flow_1 * 1 + flow_3 * 3;

					//air adjustment decision make

					if (GetKey(olc::D).bPressed) { Air_Adjustment_Decision = 1; }
					if (GetKey(olc::S).bPressed) { Air_Adjustment_Decision = 0; }
					if (GetKey(olc::A).bPressed) { Air_Adjustment_Decision = -1; }
					
					
					if (Air_Adjustment_Decision == 1) { inflow_air = normal_flow * Air_Adjustment_Decision * pressure / RT; }
					else { inflow_air = 0; }
					
					delayed_inflow1 = into / inflow_delay_time;
					delayed_inflow2 = inflow_air * (1 - (Both_Delays_05_sec_05_sec + Material_Delay_05_sec)) + delayed_inflow1 * (Material_Delay_05_sec + Both_Delays_05_sec_05_sec);
					
					if (Air_Adjustment_Decision == -1) { outflow_air = normal_flow * Air_Adjustment_Decision * (-1) * pressure / RT; }
					else { outflow_air = 0; }

					volume_of_the_man = mass / denstity_of_a_human_body;
					delayed_outflow1 = out_to / inflow_delay_time;
					disturbance = You_are_at_10_mt_but_there_is_a_disturbance_Stabilize_at_10_mt;
					delayed_outflow2 = outflow_air * (1 - (Material_Delay_05_sec + Both_Delays_05_sec_05_sec)) + delayed_outflow1 * (Material_Delay_05_sec + Both_Delays_05_sec_05_sec) + disturbance * pulse(air / 4, t)*100;
					Noname_2 = (depth - perceived_depth) / depth_perception_time;
					Noname_4 = abs(10 - depth);

					if (depth < -1.5) { vol_man_in_water = 0; }
					else
					{
						if (depth < 0) { vol_man_in_water = volume_of_the_man * (1.5 + depth) / 1.5; }
						else { vol_man_in_water = volume_of_the_man; }
					}

					volume_lt = air * RT / pressure;
					volume_m3 = volume_lt / lt_m3_donusumu;

					if (depth < -0.5) { vol_bal_in_water = 0; }
					else if (depth < 0) { vol_bal_in_water = volume_m3 * (0.5 + depth) / 0.5; }
					else { vol_bal_in_water = volume_m3; }

					volume_in_water = vol_bal_in_water + vol_man_in_water;
					lifting_force = -volume_in_water * gravitational_constant * density;
					weight = mass * gravitational_constant;
					frictional_force = 27.2 * pow(velocity, 2);

					//frictional_force_vector
					if (velocity == 0) { frictional_force_vector = 0; }
					else { frictional_force_vector = -velocity / abs(velocity) * frictional_force; }

					net_force = weight + lifting_force + frictional_force_vector;
					accelaration = net_force / mass;
					velocity1 = velocity;
					radius_of_the_man = pow((mass / (height_of_the_man * denstity_of_a_human_body * 3.1415)) , (1 / 2));
					radius = pow(((volume_m3 / (3.1415 * (height_of_the_man - 1))) + pow(radius_of_the_man , 2)) , (1 / 2));
					cross_sectional_area = 3.1415 * pow(radius,2);
					discrepancy = desired_height - depth;
					minus_velocity = -velocity;
					Your_depth = depth * (-1) * (1 - (Information_Delay_05_sec + Both_Delays_05_sec_05_sec)) + perceived_depth * (-1) * (Information_Delay_05_sec + Both_Delays_05_sec_05_sec);


					//Draw(t * 10, Your_depth * 10);
					
					DrawCircle( 30+t*10 , (Your_depth * -1)*20 + 100 , 2);
					//Draw(30 + t, (Your_depth * -1)*5 + 100);
					t = t + dt;

					
					//std::this_thread::sleep_until(std::chrono::high_resolution_clock::now() + interval);
			
					
					DrawString(400, 400, " " + std::to_string(dt));


					//DrawString(300, 300, " " + std::to_string(Your_depth));
					if (GetKey(olc::Z).bPressed) {gameOver = true; }
				
				}
					

					

					return true;
				
			}
		}
		else
		{

		
			DrawString(150, 100, "DIVING GAME");

			DrawString(125, 125, "PRESS Q FOR FLOW 1, W FOR FLOW 3");
			DrawString(100, 225, "GAMEPLAY");
			DrawString(100, 250, "-USE W-A-S-D TO CONTROL THE SHIP");
			//Clear(olc::BLACK);

			if (GetKey(olc::ENTER).bPressed)
				gameStarted = true;

			return true;
		}
	}
};


int main()
{
	SpaceGame game;
	if (game.Construct(600, 600, 1, 1))
		game.Start();
	
	return 0;
}
