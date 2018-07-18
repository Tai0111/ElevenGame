#include <iostream>
#include <ios>
#include <iomanip>
#include <random>

using namespace std;


//グローバル変数
int CNT;

//プロトタイプ宣言
bool GetPlayer(int *pos, int END, int array[][32], int times, char name[16], int Pnum);
bool GetAI(int *pos, int END, int array[][32], int times, int dep);
void GetOperate(int pos, int END, int dep);
void GetMemory(int array[][32], int n, char P1[16], char P2[16]);

int main(void){

  int pos = 0;              //現状態
  int endPoint;             //ゲームの終点
  int style;                //プレイ人数選択用
  int level;                //レベル選択用
  int times = 0;            //プレイ数
  int turn = 0;             //ターン数
  int dep;                  //探索の深さ
  int memory[3][32] = {};   //ゲームログ
  char P1[16],P2[16];       //プレイヤー名
  char name[3][16];
  int order;                //プレイ順序
  bool terminal = false;    //終端判定

  std::cout << '\n' << "##GameSetting##" << '\n';

  //プレイ人数選択
  std::cout << "Please select the number of players" << '\n';
  std::cout << "1) Human : 0 | COM : 3 " << '\n';
  std::cout << "2) Human : 1 | COM : 2 " << '\n';
  std::cout << "3) Human : 2 | COM : 1 " << '\n';
  std::cout << "4) Human : 3 | COM : 0 " << '\n';
  while (1) {
    std::cout << ">> ";
    std::cin >> style;
    if (style > 0 && style < 5) {
      std::cout << '\n';
      break;
    }
    std::cout << "!! Please choose a number from 1 to 4 !!" << '\n';
  }

  //プレーヤー名入力
  for (size_t i = 1; i < style; i++) {
    std::cout << "Please enter the name of Player " << i << '\n' << ">> ";
    std::cin >> name[i-1];
    std::cout << name[i-1] << '\n';
  }

  //ゲームの終了点の選択
  std::cout << '\n' << "Please select the end point of the game" << '\n' << ">> ";
  std::cin >> endPoint;

  //COMのレベル選択
  std::cout << '\n' << "Please choose the strength of the computer" << '\n';
  std::cout << "1:weak | 2:mid | 3:strong " << '\n' << ">> ";
  std::cin >> level;

  if (level == 1) {
    //1手先読み
    dep = 2;
  }else if (level == 2) {
    //半手先読み
    dep = endPoint/4;
    //depが偶数になるよう調整
    if (dep % 2 != 0) {
      dep += 1;
    }
  }else if (level == 3) {
    //全手先読み
    dep = endPoint/2;
    //depが偶数になるよう調整
    if (dep % 2 != 0) {
      dep += 1;
    }
  }

  //プレイ順の選択
  std::cout << '\n' << "Decide the order of play at random" << '\n';
  std::random_device rnd;     // 非決定的な乱数生成器を生成
  std::mt19937 mt(rnd());     //  メルセンヌ・ツイスタの32ビット版、引数は初期シード値
  std::uniform_int_distribution<> rand6(0, 5);        // [0, 5] 範囲の一様乱数
  order = rand6(mt);

  switch (order) {
    case 0:
    std::cout << "1st : " << name[1] << '\n';
    std::cout << "2nd : " << name[2] << '\n';
    std::cout << "3rd : " << name[0] << '\n';
    break;

    case 1:
    std::cout << "1st : " << name[2] << '\n';
    std::cout << "2nd : " << name[0] << '\n';
    std::cout << "3rd : " << name[1] << '\n';
    break;

    case 2:
    std::cout << "1st : " << name[0] << '\n';
    std::cout << "2nd : " << name[1] << '\n';
    std::cout << "3rd : " << name[2] << '\n';
    break;

    case 3:
    std::cout << "1st : " << name[1] << '\n';
    std::cout << "2nd : " << name[0] << '\n';
    std::cout << "3rd : " << name[2] << '\n';
    break;

    case 4:
    std::cout << "1st : " << name[0] << '\n';
    std::cout << "2nd : " << name[2] << '\n';
    std::cout << "3rd : " << name[1] << '\n';
    break;

    case 5:
    std::cout << "1st : " << name[2] << '\n';
    std::cout << "2nd : " << name[1] << '\n';
    std::cout << "3rd : " << name[0] << '\n';
    break;
  }

  std::cout << '\n' << "##GameStart##" << '\n';

  while (1){

    if (order < 3) {
      //プレイ順が昇順の場合
      order++;
      if (order > 2) {
        order = 0;
      }
    }else if (order > 2) {
      //プレイ順が降順の場合
      order++;
      if (order > 5) {
        order = 3;
      }
    }

    //プレイ順に関数を呼び出し
    switch (order){
      case 0:
      case 5:
      //プレイヤー1
      if ( GetPlayer(&pos, endPoint, memory, turn, P1, 0) ){
        terminal = true;
      }
      break;

      case 1:
      case 4:
      //プレイヤー2
      if ( GetPlayer(&pos, endPoint, memory, turn, P2, 1) ){
        terminal = true;
      }
      break;

      case 2:
      case 3:
      //AI
      if ( GetAI(&pos, endPoint, memory, turn, dep) ){
        terminal = true;
      }
      break;
    }

    //終端判定がonになっていればゲームを終了
    if (terminal == true) {
      GetMemory(memory, turn, P1, P2);
      break;
    }

    //プレイ数を計測
    times++;
    if (times == 3) {
      //現在状態の表示
      GetMemory(memory, turn, P1, P2);
      //全員が1プレイし終わる度にtimesを初期化し，ターン数をカウント
      times=0;
      turn++;
    }
  }
}

//プレイヤーの入力関数
bool GetPlayer(int *pos, int END, int array[][32], int times, char name[16], int Pnum){

  int choice;

  std::cout << '\n' << "Now number is " << *pos << '\n';

  while (1){
    if (*pos < END-1) {
      std::cout << "Please push number (1 or 2) " << name << '\n';
      std::cout << "-> ";
      std::cin >> choice;
      if (choice == 1 || choice == 2) {
        break;
      }
      std::cout << "!! You can only enter 1 or 2 !!" << '\n';
    }else if(*pos == END-1){
      std::cout << "Please push number 1 " << name << '\n';
      std::cout << "-> ";
      std::cin >> choice;
      if (choice == 1) {
        break;
      }
      std::cout << "!! You can only enter 1 !!" << '\n';
    }
  }

  //現状態を更新しログを保存
  *pos += choice;
  array[Pnum][times] = *pos;

  if (*pos >= END){
    std::cout << name << " Lose..." << '\n';
    return true;
  }

  return false;
}

//AIの選択関数
bool GetAI(int *pos, int END, int array[][32], int times, int dep){

  int one, two;
  int operate;

  std::cout << '\n' << "AI thiking now..." << '\n';

  if (*pos < END-1) {
    //負けが確定していなければ先読み
    for (size_t i = 1; i < 3; i++) {
      //AIが 1or2 を選んだ場合の勝ち数を計測
      CNT = 0;
      GetOperate(*pos+i, END, dep);
      if (i == 1){
        one = CNT;
      }else if(i == 2){
        two = CNT;
      }
    }

    //勝率が高い方を選択同率の場合は2を選択
    if (one < two) {
      operate = 1;
    }else{
      operate = 2;
    }
  }else{
    //負けが確定していたら1を選択
    operate = 1;
  }

  //現状態を更新しログを保存
  *pos += operate;
  array[2][times] = *pos;

  std::cout << "AI selected " << operate << '\n';

  if (*pos >= END){
    std::cout << "AI Lose..." << '\n';
    return true;
  }
  return false;
}

//AIのオペレーター関数
void GetOperate(int pos, int END, int dep){
  //再帰を繰り返し指定の深さまできたら終了
  if (dep == 0) {
    return;
  }

  if (dep % 2 == 0) {
    //depが偶数=人間のターン．
    for (size_t i = 2; i < 5; i++) {
      if (pos + i == END) {
        return;
      }
      GetOperate(pos+i, END, dep-1);
    }
  }else{
    //depが奇数=AIのターン
    for (size_t i = 1; i < 3; i++) {
      if (pos + i == END) {
        //終了点の数値があればグローバル変数CNTをインクリメント
        CNT++;
        return;
      }
      GetOperate(pos+i, END, dep-1);
    }
  }
}

//ゲームログの表示保存関数
void GetMemory(int array[][32], int n, char P1[16], char P2[16]){

  std::cout << '\n' << "##################";
  for (size_t i = 0; i < n; i++) {
    std::cout << "####";
  }
  std::cout << '\n';

  for (size_t i = 0; i < 3; i++) {

    if (i == 0) {
      std::cout << std::left << std::setw(10) << P1 << "...";
    }else if (i == 1){
      std::cout << std::left << std::setw(10) << P2 << "...";
    }else if (i == 2){
      std::cout << std::left << std::setw(10) << "AI" << "...";
    }

    for (size_t j = 0; j < n+1; j++) {
      if (array[i][j] != 0){
        std::cout << std::right << std::setw(3) << array[i][j];
      }else{
        std::cout << std::right << std::setw(3) << "--";
      }
      if(j == n){
        std::cout << " ";
      }else{
        std::cout << ",";
      }
    }
    std::cout << '\n';
  }

  std::cout << "##################";
  for (size_t i = 0; i < n; i++) {
    std::cout << "####";
  }
  std::cout << '\n';
}
