/*1. 주석

	프로그램 : main.c
	하 는 일 : 기존 코드에서 텍스트 RPG 플레이어의 직업에 따른 능력치 부여, 전투 선택지 부여 내용 추가.
	작 성 자 : 60231916 김덕회
	작 성 일 : 26-05-02
	
*/

//2. 전처리기
#include <stdio.h>
#include <stdlib.h> 

//전역변수 선언
char palyer_name;
int p_hp, p_mp;          
double p_atk, p_def;

int m_hp;
double m_atk, m_def; // 몬스터 능력치는 스테이지별로 변하므로 초기값 제거

int stage = 1;       // 현재 진행 중인 스테이지 번호 (전역 변수)

// 함수 선언
void apply_job_stats(int job_choice);                           
int calculate_damage(double attacker_atk, double defender_def, int mp_used); 
void print_status(const char* status_title);                   
void spawn_monster();      // stage에 맞춰 몬스터를 강화하는 함수
void give_reward();         // 몬스터 처치 시 보상을 지급하는 함수

//3. main 함수
int main() 
{
	system("chcp 65001");
    int menu_choice;
    int job_choice;
	
    // 메인 메뉴 무한루프
    while(1)
    {
        printf("\n=======================================\n");
        printf("    나만의 C언어 프로그램 - 텍스트 RPG    \n");
        printf("=======================================\n");
        printf(" 1. 게임 시작\n");
        printf(" 2. 게임 종료\n");
        printf("=======================================\n");
        printf("선택: ");
        scanf(" %d", &menu_choice);

        if(menu_choice==2)
        {
             printf("\n프로그램을 종료합니다. 플레이해주셔서 감사합니다!\n");
            break; 
        }
        if (menu_choice == 1)
        {
            printf("\n원하시는 문자를 입력하세요(플레이어 이름): ");
            scanf(" %c", &player_name);

            printf("\n원하는 직업의 번호를 입력하세요.\n");
            printf("1. 전사 (HP: 120, MP: 30, 공격력: 15, 방어력: 0.2)\n");
            printf("2. 마법사 (HP: 80, MP: 80, 공격력: 25, 방어력: 0.05)\n");
            printf("3. 도적 (HP: 100, MP: 50, 공격력: 20, 방어력: 0.1)\n");
            printf("선택: ");
            scanf(" %d", &job_choice);

            apply_job_stats(job_choice);
            stage = 1; // 새 게임 시작 시 스테이지를 1로 초기화

            printf("\n모험가 %c의 모험이 시작됩니다!\n", player_name);
            printf("계속하려면 엔터를 누르세요...");
            getchar(); getchar();

            while(p_hp>0)
            {
                spawn_monster(); // 현재 스테이지에 맞는 몬스터 생성
                
                print_status("전투 초기 상태");
                printf("전투를 시작하려면 엔터를 누르세요...");
                getchar();
            }


        }

    }
}
