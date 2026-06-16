/*
	프로그램 : main.c
	하 는 일 : 4단계 과제 - 무한루프 메뉴 유지, 인덱스 방식 배열 순회를 통한 누적 5회 데미지 분석, 포인터 방식 배열 순회, 배열과 함수를 통한 누적 합산 데미지 분석.
	작 성 자 : 60231916 김덕회
	작 성 일 : 26-05-02
*/

#include <stdio.h>
#include <stdlib.h> 


// 전역 변수 선언 구역
char player_name[20];    // [수정] 단일 문자에서 최대 20글자 문자열 배열로 변경
int p_hp, p_mp;          
double p_atk, p_def;

int m_hp;
double m_atk, m_def; 

int stage = 1;       


// 함수 선언
void apply_job_stats(int job_choice);                           
int calculate_damage(double attacker_atk, double defender_def, int mp_used); 
void print_status();                   
void spawn_monster();      
void give_reward();         
void analyze_combat_records(int *arr, int size); 


// 3. main 함수
int main() 
{
    system("chcp 65001"); 
    int menu_choice;
    int job_choice;

    while (1) 
    {
        printf("\n=======================================\n");
        printf("    나만의 C언어 프로그램 - 텍스트 RPG    \n");
        printf("=======================================\n");
        printf(" 1. 게임 시작\n");
        printf(" 2. 게임 종료\n");
        printf("=======================================\n");
        printf("선택: ");
        scanf(" %d", &menu_choice);

        if (menu_choice == 2) 
		{
            printf("\n프로그램을 종료합니다. 플레이해주셔서 감사합니다!\n");
            break; 
        }

        if (menu_choice == 1) 
		{
            int battle_records[5] = { 0 }; 
            int record_idx = 0;            

            
            printf("\n원하시는 닉네임을 입력하세요[20자 미만]: ");
            scanf("19%s", player_name); 

            printf("\n원하는 직업의 번호를 입력하세요.\n");
            printf("1. 전사 (HP: 120, MP: 30, 공격력: 15, 방어력: 0.2)\n");
            printf("2. 마법사 (HP: 80, MP: 80, 공격력: 25, 방어력: 0.05)\n");
            printf("3. 도적 (HP: 100, MP: 50, 공격력: 20, 방어력: 0.1)\n");
            printf("선택: ");
            scanf(" %d", &job_choice);

            apply_job_stats(job_choice);
            stage = 1; 

            printf("\n모험가 %s의 모험이 시작됩니다!\n", player_name);
            printf("계속하려면 엔터를 누르세요...");
            getchar(); getchar();

            while (p_hp > 0) 
            {
                spawn_monster(); 
                print_status();
                printf("전투를 시작하려면 엔터를 누르세요...");
                getchar();

                while (p_hp > 0 && m_hp > 0) 
                {
                    int action_choice;
                    int mp_use = 0;
                    int final_damage = 0;

                    printf("\n▶ %s의 턴! 무엇을 하시겠습니까?\n", player_name);
                    printf("1. 공격 | 2. 회복 | 3. 스킬\n선택: ");
                    scanf("%d", &action_choice);

                    if (action_choice == 1) 
					{
                        final_damage = calculate_damage(p_atk, m_def, 0);
                        m_hp -= final_damage;
                        printf("\n▶ 결과: 몬스터에게 %d의 데미지를 입혔습니다!\n", final_damage);

                        if (record_idx < 5) 
						{
                            battle_records[record_idx] = final_damage;
                            record_idx++;
                        }
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

                        if (record_idx < 5) 
						{
                            battle_records[record_idx] = final_damage;
                            record_idx++;
                        }
                    } 
                    else 
					{
                        printf("\n▶ 당황해서 아무것도 하지 못했습니다...\n");
                    }

                    if (m_hp <= 0) break;


                    printf("\n------------------------------------\n");
                    printf("▶ 몬스터의 턴! %s(을)를 공격합니다!\n", player_name);
                    final_damage = calculate_damage(m_atk, p_def, 0);
                    p_hp -= final_damage;
                    printf("몬스터의 공격! 플레이어가 %d의 데미지를 받았습니다!\n", final_damage);
                    printf("------------------------------------\n");

                    if (p_hp <= 0) break;

                    print_status(); 
                    printf("엔터를 눌러 다음 턴을 진행합니다...");
                    getchar(); getchar();
                }

                if (p_hp <= 0) 
				{
                    printf("\n[패배] %s님이 쓰러졌습니다. 총 %d스테이지까지 도달했습니다.\n", player_name, stage);
                    
                    analyze_combat_records(battle_records, record_idx);

                    printf("메인화면으로 돌아갑니다. 엔터를 누르세요...");
                    getchar(); getchar();
                    break; 
                }

                if (m_hp <= 0) 
				{
                    give_reward(); 
                    printf("\n다음 스테이지 진입을 위해 엔터를 누르세요...");
                    getchar(); getchar();
                }
            }
        }
    }
    return 0;
}


//함수 선언 구역


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

void print_status() 
{
    printf("\n====================================\n");
    printf("[ STAGE %d ]\n", stage);
    // [수정] %c -> %s 변경
    printf("플레이어(%s) - HP: %d, MP: %d\n", player_name, p_hp, p_mp);
    printf("몬스터 - HP: %d, 공격력: %.2f\n", (m_hp < 0) ? 0 : m_hp, m_atk);
    printf("====================================\n");
}

void spawn_monster() 
{
    m_hp = 55 + (stage * 10); 
    m_atk = 11.0 + (stage * 2.0);
    m_def = 0.04 + (stage * 0.01);
    if (m_def > 0.40) m_def = 0.40; 
}

void give_reward() 
{
    printf("\n====================================\n");
    printf("★☆★ 축하합니다! STAGE %d 클리어! ★☆★\n", stage);
    printf("====================================\n");
    printf("[ 전리품 획득 ] 몬스터를 물리치고 강해졌습니다!\n");
    
    p_atk += 3.0;
    p_def += 0.01;
    printf("-> 공격력 3.0 증가! 현재 공격력: %.1f\n", p_atk);
    printf("-> 방어력 1%% 증가! 현재 방어력: %.1f%%\n", p_def * 100);

    p_hp += 40;
    p_mp += 20;
    printf("-> 승리 보너스로 HP 40, MP 20을 회복했습니다.\n");
    
    stage++; 
}

//뱌열, 포인터 순회 - 배열 매개변수
void analyze_combat_records(int *arr, int size) 
{
    printf("\n=======================================\n");
    printf("      [전투 종료] 데미지 분석 리포트     \n");
    printf("=======================================\n");
    
    printf("▶ 기록된 초반 5회 타격 데미지:\n");
    for (int i = 0; i < size; i++) 
	{
        printf("  - %d회차 가한 데미지: %d\n", i + 1, arr[i]); 
    }
    
    int total_damage = 0;
    for (int i = 0; i < size; i++) 
	{
        total_damage += *(arr + i);  
        *(arr + i) += 10;            
    }
    
    printf("---------------------------------------\n");
    printf("▶ [포인터 순회] 누적 공격 데미지 총합: %d\n", total_damage);
    if (size > 0) 
	{
        printf("▶ [포인터 순회] 한 대당 평균 데미지: %.2f\n", (double)total_damage / size);
    }
    printf("=======================================\n");
}
