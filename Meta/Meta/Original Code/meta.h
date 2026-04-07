#ifndef _META_H
#define _META_H

void meta_eat(int b);
void meta_timer_become_fish(Boolean start_fish_become, Boolean stop_fish_become);
void meta_timer_jump(Boolean jump_start, Boolean jump_stop);
void meta_timer_snake(Boolean snake_start, Boolean snake_stop);
void meta_timer_beetle(Boolean beetle_start, Boolean beetle_stop);
void meta_timer_jail(Boolean jail_start, Boolean jail_stop);
void meta_timer_bomb(Boolean bomb_start, Boolean bomb_stop);
void meta_beetle_area_die(void);
void meta_beetledie(void);
void meta_smash(void);
void meta_say(char *foo);
void meta_pull(int x);
Boolean meta_bite(void);
Boolean meta_skin(int obj);
Boolean meta_kick(void);
Boolean meta_climb(int obj);
Boolean meta_dig(void);
Boolean meta_move(int n);
Boolean meta_jump(void);
Boolean meta_wield();
Boolean meta_get(int ob);
Boolean meta_fly(void);
Boolean meta_equip(void);
Boolean meta_put(int a, int c);
Boolean meta_drop(int ob);
Boolean meta_close(int obj);
Boolean meta_turn(int obj);
Boolean meta_open(int obj);
Boolean meta_type(void);
Boolean meta_kill(int pl);
Boolean meta_give(int pl, int ob);
Boolean meta_tickle(void);
Boolean meta_smell(void);
Boolean meta_look(int loc);
Boolean meta_enter(int obj);
Boolean meta_wear(int obj);
Boolean meta_wearall(int obj);
Boolean meta_empty(int obj);
Boolean meta_light(void);
Boolean meta_cut(int obj);
Boolean meta_examine(int a);
Boolean meta_cast_spell(int plr);
Boolean meta_peck(int obj);
Boolean meta_wound(int attacker, int victim, int weap);
Boolean meta_exits(void);
Boolean meta_fill(int obj);
Boolean meta_pour(int obj);

#define META_HUMAN 0
#define META_SNAKE 1
#define META_BIRD 2
#define META_BEETLE 3
#define META_FISH_ONLAND 4
#define META_FISH_UNDERWATER 5
#define META_BEETLE_UNDERWATER 6
#define META_SNAKE_UNDERWATER 7

#endif

