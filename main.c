/*1. 주석

	프로그램 : main.c
	하 는 일 : 기존 코드에서 텍스트 RPG 플레이어의 직업에 따른 능력치 부여, 전투 선택지 부여 내용 추가.
	작 성 자 : 60231916 김덕회
	작 성 일 : 26-05-02
	
*/

//2. 전처리기
#include <stdio.h>
#include <stdlib.h> 

//3. main 함수
int main() 
{
	system("chcp 65001");
		
    // 0. 플레이어 기본 능력치 - 변수 선언 (문자, 정수, 실수)
    char player_name = 'O'; //플레이어의 이름 결정

    // 플레이어 기본 능력치
    int p_hp = 100;
    int p_mp = 50;  // 마나 소비량에 공격력이 강해짐
    double inhanced_atk; //마나 소모에 따른 공격 강화 계산식에 사용되는 변수
    int inhanced_damage; //마나 소모에 따른 공격 강화 계산식에 사용되는 변수
    int mp_use;
    double p_atk = 20.0;
    double p_def = 0.100;  
    
    int job_choice; //직업 선택 변수
    int action_choice; //전투 선택 변수
/*
	방어력 공식 => 플레이어가 받는 피해량 = 몬스터 공격력-(몬스터 공격력*플레이어 방어력)
	결론적으로 방어력 0.100 = 받는 피해량 10% 감소
*/
    
	// 몬스터 능력치(고정)
    int m_hp = 80;
    double m_atk = 15.0;
    double m_def = 0.05;

    // 1. 게임 타이틀 출력
    printf("=======================================\n");
    printf("   나만의 C언어 프로그램 - 텍스트 RPG   \n");
    printf("=======================================\n\n");

    // 2. 플레이어 이름 입력 - 현재는 문자로 대체하였지만, 이름을 입력할 수 있도록 바꿀 예정입니다.
    printf("원하시는 문자를 입력하세요(해당 문자는 플레이어의 이름이 됩니다.): ");
    scanf(" %c", &player_name);


	//3. 직업 선택지 부여. 선택에 따른 특화 능력치를 부여받습니다.
	printf("\n원하는 직업의 번호를 입력하세요.\n");
    printf("1. 전사 (HP: 120, MP: 50, 공격력: 15, 방어력: 0.2)\n");
    printf("2. 마법사 (HP: 100, MP: 100, 공격력: 25, 방어력: 0.05)\n");
    printf("3. 도적 (HP: 80, MP: 50, 공격력: 35, 방어력: 0.05)\n");
    printf("[!] 선택지 외의 다른 번호를 선택하실경우 기본 능력치가 부여됩니다.\n");
    printf("선택: ");
    scanf(" %d", &job_choice);

    switch (job_choice) 
	{
        case 1:
            p_hp = 120; p_mp = 30; p_atk = 15.0; p_def = 0.2;
            printf("\n[전사]를 선택하셨습니다. 높은 방어력과 체력으로 유지력이 좋습니다.\n");
            break;
        case 2:
            p_hp = 80; p_mp = 80; p_atk = 25.0; p_def = 0.05;
            printf("\n[마법사]를 선택하셨습니다. 스킬을 사용하여 더 강력한 공격을 가합니다.\n");
            break;
        case 3:
            p_hp = 100; p_mp = 50; p_atk = 20.0; p_def = 0.1;
            printf("\n[도적]을 선택하셨습니다. 강한 공격력을 가집니다.\n");
            break;
        default:
			printf("\n잘못된 선택입니다.기본능력치를 가진 [모험가]가 선택됩니다.\n");
			break;    
    }

    // 캐릭터 생성 결과를 확인한 후 다음으로 넘어가기 전 일시 정지
    printf("\n계속하려면 엔터를 누르세요...");
    getchar(); getchar(); 

    // 3. 전투 시작 문구 출력, 초기 상태 출력
    printf("\n====================================\n");
    printf("야생의 몬스터가 나타났다! 전투 시작!\n");
    printf("====================================\n");
    printf("[ 전투 초기 상태 ]\n");
    printf("플레이어(%c) - HP: %d, MP: %d\n", player_name, p_hp, p_mp);
    printf("몬스터 - HP: %d, 공격력: %.2f\n\n", m_hp, m_atk);

    // 초기 상태를 확인한 후 전투를 시작하기 전 일시 정지
    printf("전투를 시작하려면 엔터를 누르세요...");
    getchar(); getchar(); 

    // 4. 플레이어가 몬스터를 공격 (플레이어 공격력만큼 몬스터 HP 차감)
    printf("\n▶ %c의 턴!\n", player_name);
    printf("%c(이)가 몬스터를 공격합니다!\n", player_name);
    
    // 실수를 정수형 HP에서 빼기 위해 (int)로 형변환
    // 방어력 공식: 플레이어 공격력 - (플레이어 공격력 * 몬스터 방어력)
    
	printf("\n당신의 턴입니다. 무엇을 하시겠습니까?\n");
    printf("1. 공격 (공격력으로 몬스터 타격)\n");
    printf("2. 회복 (체력을 15 회복)\n");
    printf("3. 스킬 (마나를 소모하여 공격 강화)\n");
    printf("선택: ");
    scanf("%d", &action_choice);

    if (action_choice == 1) 
	{
        // 공격 선택 시 데미지 계산식 적용
        int p_damage = (int)(p_atk - (p_atk * m_def));
        m_hp = m_hp - p_damage;
        printf("\n▶ 결과: 몬스터에게 %d의 데미지를 입혔습니다!\n", p_damage);
    } 
    else if (action_choice == 2) 
	{
        // 회복 선택 시 HP 15 증가
        p_hp = p_hp + 15;
        printf("\n▶ 결과: 체력을 15만큼 회복했습니다! (현재 HP: %d)\n", p_hp);
    } 
    else if (action_choice == 3)
	{
		// 스킬 사용 - 사용할 마나의 양 입력
		printf("사용할 마나의 양을 입력하세요 : ");
		scanf(" %d", &mp_use);
		if(p_mp >= mp_use)
		{
    		p_mp-=mp_use;
    		inhanced_atk = p_atk+(p_atk*(0.01*mp_use));
        	int inhanced_damage = (int)(inhanced_atk - (p_atk * m_def));
       		m_hp-=inhanced_damage;
        	printf("\n▶ 결과: 마나 %d를 소모하여 몬스터에게 %d의 데미지를 입혔습니다!\n", mp_use, inhanced_damage);
		}
		else
		{
			int p_damage = (int)(p_atk - (p_atk * m_def));
        	m_hp = m_hp - p_damage;
			printf("\n▶ [발동 실패] 마나가 부족합니다! (현재 MP: %d / 입력 MP: %d)\n", p_mp, mp_use);
            printf("마나를 사용하지 않고 공격하여 몬스터에게 %d의 데미지를 입혔습니다!\n", p_damage);
		}
    }
    else
    {
    	printf("\n▶ 당황해서 아무것도 하지 못했습니다...\n");
	}
	
	printf("플레이어(%c) - HP: %d, MP: %d\n", player_name, p_hp, p_mp);
    printf("몬스터 - HP: %d, 공격력: %.2f\n\n", m_hp, m_atk);

    // 플레이어의 공격 결과를 확인하기 위해 일시 정지
    printf("엔터를 눌러 턴을 종료합니다...");
    getchar(); getchar(); 

    // 5. 턴 전환
    printf("\n------------------------------------\n");
    printf("턴이 몬스터에게로 넘어갑니다...\n");
    printf("------------------------------------\n\n");

    // 턴이 넘어가는 문구를 확인하기 위해 일시 정지
    getchar(); getchar(); 

    // 6. 몬스터가 플레이어를 공격
    printf("\n▶ 몬스터의 턴!\n");
    printf("몬스터가 %c(을)를 공격합니다!\n", player_name);
    
    // 방어력 공식 적용
    int m_damage = (int)(m_atk - (m_atk * p_def));
    p_hp = p_hp - m_damage;
    
    printf("몬스터의 공격! 플레이어가 %d의 데미지를 받았습니다!\n\n", m_damage);

    // 몬스터의 공격 결과를 확인하기 위해 일시 정지
    printf("엔터를 눌러 결과를 확인하세요...");
    getchar(); getchar(); 

    // 7. 차감된 HP를 반영한 능력치 출력
	printf("\n====================================\n");

    printf("[ 1턴 종료 후 현재 상태 ]\n");

    printf("플레이어(%c) - HP: %d, MP: %d\n", player_name, p_hp, p_mp);

    printf("몬스터 - HP: %d, 공격력: %.2f\n\n", m_hp, m_atk);

    printf("====================================\n");

    return 0;
}
