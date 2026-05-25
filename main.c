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
char player_name;
int p_hp, p_mp;          
double p_atk, p_def;

int m_hp;
double m_atk, m_def; // 몬스터 능력치는 스테이지별로 변하므로 초기값 제거

int stage = 1;       // 현재 진행 중인 스테이지 번호 (전역 변수)

// 함수 선언
void apply_job_stats(int job_choice);                           
int calculate_damage(double attacker_atk, double defender_def, int mp_used); 
void print_status();                   
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

        //메인 메뉴 루프(전투 종료 후 복귀)
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

            //플레이어가 살아있는 한 스테이지를 계속 진행하는 루프
            while(p_hp>0)
            {
                spawn_monster(); // 현재 스테이지의 몬스터 생성
                
                print_status();
                printf("전투를 시작하려면 엔터를 누르세요...");
                getchar();

                while (p_hp > 0 && m_hp > 0) 
                {
                    int action_choice;
                    int mp_use = 0;
                    int final_damage = 0;

                    printf("\n▶ %c의 턴! 무엇을 하시겠습니까?\n", player_name);
                    printf("1. 공격 | 2. 회복 | 3. 스킬\n선택: ");
                    scanf("%d", &action_choice);

                    if (action_choice == 1) 
                    {
                        final_damage = calculate_damage(p_atk, m_def, 0);
                        m_hp -= final_damage;
                        printf("\n▶ 결과: 몬스터에게 %d의 데미지를 입혔습니다!\n", final_damage);
                    } 
                    else if (action_choice == 2) 
                    {
                        p_hp += 15; 
                        printf("\n▶ 결과: 체력을 15만큼 회복했습니다! (현재 HP: %d)\n", p_hp);
                    } 
                    else if (action_choice == 3) 
                    {
                        printf("사용할 마나의 양을 입력하세요 : ");
                        scanf("%d", &mp_use);

                        if (p_mp >= mp_use) 
                        {
                            p_mp -= mp_use; 
                            final_damage = calculate_damage(p_atk, m_def, mp_use);
                            m_hp -= final_damage;
                            printf("\n▶ 결과: 마나 %d를 소모하여 몬스터에게 %d의 데미지를 입혔습니다!\n", mp_use, final_damage);
                        } 
                        else 
                        {
                            final_damage = calculate_damage(p_atk, m_def, 0);
                            m_hp -= final_damage;
                            printf("\n▶ [발동 실패] 마나가 부족합니다! (현재 MP: %d / 입력 MP: %d)\n", p_mp, mp_use);
                            printf("일반 공격으로 전환하여 %d의 데미지를 입혔습니다!\n", final_damage);
                        }
                    } 
                    else 
                    {
                        printf("\n▶ 당황해서 아무것도 하지 못했습니다...\n");
                    }

                    // 몬스터 사망 체크 -> 전투 종료+보상으로 이동
                    if (m_hp <= 0) break;

                    // 몬스터의 턴
                    printf("\n------------------------------------\n");
                    printf("▶ 몬스터의 턴! %c(을)를 공격합니다!\n", player_name);
                    final_damage = calculate_damage(m_atk, p_def, 0);
                    p_hp -= final_damage;
                    printf("몬스터의 공격! 플레이어가 %d의 데미지를 받았습니다!\n", final_damage);
                    printf("------------------------------------\n");

                    // 플레이어 사망 체크 -> 전투 종료
                    if (p_hp <= 0) break;

                    print_status("현재 턴 종료 상태");
                    printf("엔터를 눌러 다음 턴을 진행합니다...");
                    getchar(); getchar();
                }

                //전투 종료 후 판정
                if (p_hp <= 0) 
                {
                    printf("\n[패배] %c님이 쓰러졌습니다. 총 %d스테이지까지 도달했습니다.\n", player_name, stage);
                    printf("메인화면으로 돌아갑니다. 엔터를 누르세요...");
                    getchar(); getchar();
                    break; // 메인 메뉴로 이동
                }

                if (m_hp <= 0) 
                {
                    give_reward(); // 보상 시스템 /스테이지 증가
                    printf("\n다음 스테이지 진입을 위해 엔터를 누르세요...");
                    getchar(); getchar();
                }              
            }
        }
    }
}



// 함수

//플레이어의 직업에 따른 스탯 부여 
void apply_job_stats(int job_choice) 
{
    switch (job_choice) 
    {
        case 1: p_hp = 120; p_mp = 30; p_atk = 15.0; p_def = 0.2; break;
        case 2: p_hp = 80; p_mp = 80; p_atk = 25.0; p_def = 0.05; break;
        case 3: p_hp = 100; p_mp = 50; p_atk = 20.0; p_def = 0.1; break;
        default:
            p_hp = 100; p_mp = 50; p_atk = 20.0; p_def = 0.1;
            printf("\n잘못된 선택입니다. 기본능력치를 가진 [모험가]가 선택됩니다.\n");
            break;
    }
}

// 데미지 계산식
int calculate_damage(double attacker_atk, double defender_def, int mp_used) 
{
    int calculated_damage;
    if (mp_used > 0) 
    {
        double enhanced_atk = attacker_atk + (attacker_atk * (0.01 * mp_used));
        calculated_damage = (int)(enhanced_atk - (attacker_atk * defender_def));
    }
    else 
    {
        calculated_damage = (int)(attacker_atk - (attacker_atk * defender_def));
    }
    return calculated_damage; 
}

//플레이어 이름과 직업에 따른 스탯 출력
void print_status() 
{
    printf("\n====================================\n");
    printf("[ STAGE %d ]\n", stage);
    printf("플레이어(%c) - HP: %d, MP: %d\n", player_name, p_hp, p_mp);
    printf("몬스터 - HP: %d, 공격력: %.2f\n", (m_hp < 0) ? 0 : m_hp, m_atk);
    printf("====================================\n");
}

// 스테이지에 비례하여 강해지는 몬스터
void spawn_monster() 
{
    // 스테이지가 올라갈 때마다 체력은 10씩, 공격력은 2씩, 방어력은 1%씩 상승
    m_hp = 55 + (stage * 10); 
    m_atk = 11.0 + (stage * 2.0);
    m_def = 0.04 + (stage * 0.01);
    
    // 몬스터 방어력을 최대 40%로 제한
    if (m_def > 0.40) m_def = 0.40; 
}

// 몬스터 처치 보상 및 스테이지 레벨업 처리 (전역변수 stage 변경 포함)
void give_reward() 
{
    printf("\n====================================\n");
    printf("★☆★ 축하합니다! STAGE %d 클리어! ★☆★\n", stage);
    printf("====================================\n");
    printf("[ 전리품 획득 ] 몬스터를 물리치고 강해졌습니다!\n");
    
    // 영구 능력치 상승 보상
    p_atk += 3.0;
    p_def += 0.01;
    printf("-> 공격력 3.0 증가! 현재 공격력: %.1f\n", p_atk);
    printf("-> 방어력 1%% 증가! 현재 방어력: %.1f%%\n", p_def * 100);

    // 승리 보너스 (체력 40, 마나 20 회복)
    p_hp += 40;
    p_mp += 20;
    printf("-> 승리 보너스로 HP 40, MP 30을 회복했습니다.\n");
    
    // 다음 스테이지로 이동
    stage++; 
}
