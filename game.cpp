#define is_down(b) input->buttons[b].is_down
#define pressed(b) (input->buttons[b].is_down && input->buttons[b].changed)
#define released(b) (!input->buttons[b].is_down && input->buttons[b].changed)


float car_p_x, car_dp_x;
float car_p_y = -20, car_dp_y;
float arena_half_size_x = 35, arena_half_size_y = 48;
float car_half_size_x = 3, car_half_size_y = 5;
float ball_p_x, ball_p_y = 30, ball_dp_x, ball_dp_y = -30, ball_half_size_x = 3, ball_half_size_y = 5;
int counter, high_score = 0;



internal void
simulate_player(float *p, float *dp, float ddp, float dt) {
	ddp -= *dp * 10.f;

	*p = *p + *dp * dt + ddp * dt * dt * .5f;
	*dp = *dp + ddp * dt;

	if (*p + car_half_size_x > arena_half_size_x) {
		*p = arena_half_size_x - car_half_size_x;
		*dp = 0;
	}
	else if (*p - car_half_size_x < -arena_half_size_x) {
		*p = -arena_half_size_x + car_half_size_x;
		*dp = 0;
	}
}

internal bool
aabb_vs_aabb(float p1x, float p1y, float hs1x, float hs1y,
	float p2x, float p2y, float hs2x, float hs2y) {
	return(p1x + hs1x > p2x - hs2x &&
		p1x + hs1x < p2x + hs2x &&
		p1y + hs1y > p2y - hs2y &&
		p1y + hs1y < p2y + hs2y);
}

enum Gamemode {
	GM_MENU,
	GM_GAMEPLAY,
};

Gamemode current_gamemode;

internal void
simulate_gray_car() {

	draw_rect(ball_p_x - 3.5f, ball_p_y - 2.7f, 0.5f, 0.5f, 0x000000);
	draw_rect(ball_p_x + 3.5f, ball_p_y - 2.7f, 0.5f, 0.5f, 0x000000);
	draw_rect(ball_p_x + 3.5, ball_p_y - 3.7f, 0.5f, 0.5f, 0x000000); //front tires
	draw_rect(ball_p_x - 3.5f, ball_p_y - 3.7f, 0.5f, 0.5f, 0x000000);

	draw_rect(ball_p_x - 3.5f, ball_p_y + 2.2f, 0.5f, 0.5f, 0x000000);
	draw_rect(ball_p_x + 3.5f, ball_p_y + 2.2f, 0.5f, 0.5f, 0x000000);
	draw_rect(ball_p_x + 3.5, ball_p_y + 3.2f, 0.5f, 0.5f, 0x000000); //back tires
	draw_rect(ball_p_x - 3.5f, ball_p_y + 3.2f, 0.5f, 0.5f, 0x000000);

	draw_rect(ball_p_x, ball_p_y + 1.6f, 2, 0.7f, 0x00FFFF);// windshield

	draw_rect(ball_p_x + 2.5f, ball_p_y + 4.5f, 0.5f, 0.5f, 0xFFFF00);//headlights
	draw_rect(ball_p_x - 2.5f, ball_p_y + 4.5f, 0.5f, 0.5f, 0xFFFF00);

	draw_rect(ball_p_x + 1.5f, ball_p_y - 5.5f, 0.5f, 0.5f, 0xC0C0C0);//exhaust

}

internal void
simulate_ball(int num, float dt) {

	ball_p_x += ball_dp_x * dt;
	ball_p_y += ball_dp_y * dt;
	draw_rect(ball_p_x, ball_p_y, ball_half_size_x, ball_half_size_y, 0xffffff);
	simulate_gray_car();

	if (num == 0) {
		ball_p_x = -30;
	}
	if (num == 1) {
		ball_p_x = -15;
	}
	if (num == 2) {
		ball_p_x = 0;
	}
	if (num == 3) {
		ball_p_x = 15;
	}
	if (num == 4) {
		ball_p_x = 30;
	}


	if (aabb_vs_aabb(ball_p_x, ball_p_y, ball_half_size_x, ball_half_size_x, car_p_x, car_p_y, car_half_size_x, car_half_size_y)) {
		current_gamemode = GM_MENU;
		ball_p_y = 30;
		car_p_x = 0;
		car_p_y = -20;
		if (score > high_score) {
			high_score = score;
		}
	}
	if (aabb_vs_aabb(ball_p_x, ball_p_y, ball_half_size_y - 5, ball_half_size_y - 5, car_p_x, car_p_y, car_half_size_x+5, car_half_size_y+5)) {
		current_gamemode = GM_MENU;
		ball_p_y = 30;
		car_p_x = 0;
		car_p_y = -20;
		if (score > high_score) {
			high_score = score;
			score = 0;
			counter = 1;
		}
		score = 0;
	}
	else if (ball_p_y - ball_half_size_y < -arena_half_size_y) {
		ball_p_y = 30;
		ball_dp_y *= 1;
		score = score + 550;
	}
}


internal void
simulate_car() {

	draw_rect(car_p_x - 3.5f, car_p_y - 2.7f, 0.5f, 0.5f, 0x000000);
	draw_rect(car_p_x + 3.5f, car_p_y - 2.7f, 0.5f, 0.5f, 0x000000);
	draw_rect(car_p_x + 3.5, car_p_y - 3.7f, 0.5f, 0.5f, 0x000000); //back tires
	draw_rect(car_p_x - 3.5f, car_p_y - 3.7f, 0.5f, 0.5f, 0x000000);

	draw_rect(car_p_x - 3.5f, car_p_y + 2.2f, 0.5f, 0.5f, 0x000000);
	draw_rect(car_p_x + 3.5f, car_p_y + 2.2f, 0.5f, 0.5f, 0x000000);
	draw_rect(car_p_x + 3.5, car_p_y + 3.2f, 0.5f, 0.5f, 0x000000); //front tires
	draw_rect(car_p_x - 3.5f, car_p_y + 3.2f, 0.5f, 0.5f, 0x000000);

	draw_rect(car_p_x, car_p_y + 1.6f, 2, 0.7f, 0x00FFFF);// windshield

	draw_rect(car_p_x + 2.5f, car_p_y + 4.5f, 0.5f, 0.5f, 0xFFFF00);//headlights
	draw_rect(car_p_x - 2.5f, car_p_y + 4.5f, 0.5f, 0.5f, 0xFFFF00);

	draw_rect(car_p_x - 1.5f, car_p_y - 5.5f, 0.5f, 0.5f, 0xC0C0C0);//exhaust

}

internal void
simulate_game(Input* input, float dt) {
	draw_rect(0, 0, arena_half_size_x, arena_half_size_y, 0x00D1FF);
	draw_arena_borders(arena_half_size_x, arena_half_size_y, 0x2ECC71);


	if (current_gamemode == GM_GAMEPLAY) {
		counter = 0;
		float car_ddp_x = 0.f;
		if (is_down(BUTTON_RIGHT) || is_down(BUTTON_D)) car_ddp_x += 1000;
		if (is_down(BUTTON_LEFT) || is_down(BUTTON_A)) car_ddp_x -= 1000;
		float car_ddp_y = 0.f;
		if (is_down(BUTTON_UP) || is_down(BUTTON_W)) car_ddp_y += 1000;
		if (is_down(BUTTON_DOWN) || is_down(BUTTON_S)) car_ddp_y -= 1000;

		simulate_player(&car_p_x, &car_dp_x, car_ddp_x, dt);
		simulate_player(&car_p_y, &car_dp_y, car_ddp_y, dt);

		//Simulate Ball
		simulate_ball(number0, dt);
		simulate_ball(number1, dt);
		simulate_ball(number2, dt);

		draw_number(score, -19, 40, 0.6f, 0xbbffbb);
		draw_text("SCORE", -17, 40.5f, 0.2f, 0xFFF0B3);

		draw_number(high_score, 13, 40, 0.6f, 0xbbffbb);
		draw_text("HIGHSCORE", 15, 40.5f, 0.2f, 0xFFF0B3);

		// Rendering
		draw_rect(car_p_x, car_p_y, car_half_size_x, car_half_size_y, 0xFF0000);
		simulate_car();
	}
	else {
		if (pressed(BUTTON_ENTER)) {
			current_gamemode = GM_GAMEPLAY;
		}
		
		draw_rect(0, -12.7f, 20, 10, 0x69ADB1);
		draw_text("START", -13, -10, 1, 0xFFF0B3);
		
		if (counter == 1) {
			draw_text("YOU GOT A NEW HIGH SCORE: ", -29, 10, 0.4f, 0xffffff);
			draw_number(high_score, 30, 4.9f, 0.5f, 0xbbffbb);
			score = 0;
		}
		else {
			draw_text("YOU GOT A NEW HIGH SCORE!", -34, 20, 0.4f, 0x00D1FF);
			score = 0;
		}
		draw_text("HIGHWAY", -29.9f, 40, 1.5f, 0xffffff);
		draw_text("RACING!", -27.1f, 25, 1.5f, 0xffffff);
	}
}