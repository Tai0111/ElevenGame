#include <iostream>
#include <ios>
#include <iomanip>

using namespace std;

#define END     22

//グローバル変数
int CNT;

//プロトタイプ宣言
bool GetPlayer(int *pos, int array[][32], int times, char name[16], int Pnum);
bool GetAI(int *pos, int array[][32], int times, int dep);
void GetOperate(int pos, int dep);
void GetMemory(int array[][32], int n, char P1[16], char P2[16]);

int main(void){

  int pos = 0;           //現状態
  int times = 0;         //ターン数
  int dep = 4;           //探索の深さ
  int memory[3][32] = {};     //ゲームログ
  char P1[16],P2[16];    //プレイヤー名

  std::cout << "##GameSetting##" << '\n';

  std::cout << "Please enter the name of Player 1" << '\n' << ">> ";
  std::cin >> P1;
  std::cout << "Please enter the name of Player 2" << '\n' << ">> ";
  std::cin >> P2;

  std::cout << '\n' << "##GameStart##" << '\n';

  while (1){

    //プレイヤー1
    if ( GetPlayer(&pos, memory, times, P1, 0) ){
      GetMemory(memory, times, P1, P2);
      break;
    }
    //プレイヤー2
    if ( GetPlayer(&pos, memory, times, P2, 1) ){
      GetMemory(memory, times, P1, P2);
      break;
    }
    //AI
    if ( GetAI(&pos, memory, times, dep) ){
      GetMemory(memory, times, P1, P2);
      break;
    }

    GetMemory(memory, times, P1, P2);
    times++;
  }
}

//プレイヤーの入力関数
bool GetPlayer(int *pos, int array[][32], int times, char name[16], int Pnum){

  int choice;

  std::cout << '\n' << "Now number is " << *pos << '\n';

  while (1){
    std::cout << "Please push number (1 or 2) " << name << '\n';
    std::cout << "-> ";
    std::cin >> choice;
    if (choice == 1 || choice == 2) {
      break;
    }
    std::cout << "!! You can only enter 1 or 2 !!" << '\n';
  }

  //現状態を更新しログを保存
  *pos += choice;
  array[Pnum][times] = *pos;

  if (*pos >= END){
    std::cout << name << " win!!" << '\n';
    return true;
  }

  return false;
}

//AIの選択関数
bool GetAI(int *pos, int array[][32], int times, int dep){

  int one, two;
  int operate;

  std::cout << '\n' << "AI thiking now..." << '\n';

  for (size_t i = 1; i < 3; i++) {
    //AIが 1or2 を選んだ場合の勝ち数を計測
    CNT = 0;
    GetOperate(*pos+i, dep);
    if (i == 1){
      one = CNT;
    }else if(i == 2){
      two = CNT;
    }
  }

  //勝率が高い方を選択同率の場合は2を選択
  if (one > two) {
    operate = 1;
  }else{
    operate = 2;
  }

  //現状態を更新しログを保存
  *pos += operate;
  array[2][times] = *pos;

  std::cout << "AI selected " << operate << '\n';

  if (*pos >= END){
    std::cout << "AI win!!" << '\n';
    return true;
  }

  return false;
}

//AIのオペレーター関数
void GetOperate(int pos, int dep){

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
      GetOperate(pos+i,dep-1);
    }
  }else{
    //depが奇数=AIのターン
    for (size_t i = 1; i < 3; i++) {
      if (pos + i == END) {
        //終了点の数値があればグローバル変数CNTをインクリメント
        CNT++;
        return;
      }
      GetOperate(pos+i,dep-1);
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
      std::cout << std::left << std::setw(12) << P1 << "...";
    }else if (i == 1){
      std::cout << std::left << std::setw(12) << P2 << "...";
    }else if (i == 2){
      std::cout << std::left << std::setw(12) << "AI" << "...";
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
