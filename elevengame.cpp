#include <iostream>
#include <string>
#include <string.h>
#include <ios>
#include <iomanip>
#include <random>

using namespace std;


//グローバル変数
int CNT;

//プロトタイプ宣言
bool GetPlayer(int *pos, int END, int array[][32], int times, char name[][16], int Pnum);
bool GetAI(int *pos, int END, int array[][32], int times, char name[][16], int depth[3], int Pnum);
void GetOperate(int pos, int END, int dep);
void GetMemory(int array[][32], int n, char name[][16]);

int main(void){

  int pos = 0;              //現状態
  int endPoint;             //ゲームの終点
  int style;                //プレイ人数選択用
  int level;                //レベル選択用
  int turn = 0;             //ターン数
  int dep[3];               //探索の深さ
  int memory[3][32] = {};   //ゲームログ
  char name[3][16];         //プレイヤー名(入れ替えように１つ多く用意)
  char tmp[16];             //プレイヤー名入れ替え用
  int order;                //プレイ順序

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
    std::cout << "Please enter the name of Player_" << i << '\n' << ">> ";
    std::cin >> name[i-1];
  }
  //COM名入力
  for (size_t i = style; i < 4; i++) {
    std::cout << "Please enter the name of COM_" << i-(style-1) << '\n' << ">> ";
    std::cin >> name[i-1];
  }

  //ゲームの終了点の選択
  std::cout << '\n' << "Please select the end point of the game" << '\n' << ">> ";
  std::cin >> endPoint;

  //COMのレベル選択
  for (size_t i = style; i < 4; i++) {
    while (1) {
      std::cout << '\n' << "Please choose the strength of the " << name[i-1] << '\n';
      std::cout << "1:weak | 2:mid | 3:strong " << '\n' << ">> ";
      std::cin >> level;
      if (level > 0 && level <4) {
        break;
      }
      std::cout << "!! Please choose a number from 1 to 3 !!" << '\n';
    }
    if (level == 1) {
      //1手先読み
      dep[i-style] = 2;
    }else if (level == 2) {
      //半手先読み
      dep[i-style] = endPoint/4;
      //depが偶数になるよう調整
      if (dep[i-style] % 2 != 0) {
        dep[i-style] += 1;
      }
    }else if (level == 3) {
      //全手先読み
      dep[i-style] = endPoint/2;
      //depが偶数になるよう調整
      if (dep[i-style] % 2 != 0) {
        dep[i-style] += 1;
      }
    }
  }

  //プレイ順の選択
  std::cout << '\n' << "Decide the order of play at random" << '\n';
  std::random_device rnd;     // 非決定的な乱数生成器を生成
  std::mt19937 mt(rnd());     //  メルセンヌ・ツイスタの32ビット版、引数は初期シード値
  std::uniform_int_distribution<> rand6(0, 5);        // [0, 5] 範囲の一様乱数
  order = rand6(mt);

  //プレイ順に配列を入れ替え
  switch (order) {
    case 0:
    //0->1->2
    break;

    case 1:
    //1->2->0
    strcpy(tmp, name[0]);
    strcpy(name[0], name[1]);
    strcpy(name[1], tmp);
    strcpy(tmp, name[1]);
    strcpy(name[1], name[2]);
    strcpy(name[2], tmp);
    break;

    case 2:
    //2->0->1
    strcpy(tmp, name[0]);
    strcpy(name[0], name[1]);
    strcpy(name[1], tmp);
    strcpy(tmp, name[0]);
    strcpy(name[0], name[2]);
    strcpy(name[2], tmp);
    break;

    case 3:
    //1->0->2
    strcpy(tmp, name[0]);
    strcpy(name[0], name[1]);
    strcpy(name[1], tmp);
    break;

    case 4:
    //0->2->1
    strcpy(tmp, name[1]);
    strcpy(name[1], name[2]);
    strcpy(name[2], tmp);
    break;

    case 5:
    //2->1->0
    strcpy(tmp, name[0]);
    strcpy(name[0], name[2]);
    strcpy(name[2], tmp);
    break;

  }
  //プレイ順の表示
  std::cout << "1st : " << name[0] << '\n';
  std::cout << "2nd : " << name[1] << '\n';
  std::cout << "3rd : " << name[2] << '\n';

  std::cout << '\n' << "##GameStart##" << '\n';

  //メインループ開始
  while (1){
    //プレイヤー1
    if (style > 1) {
      if ( GetPlayer(&pos, endPoint, memory, turn, name, 0) ){
        GetMemory(memory, turn, name);
        break;
      }
    }else{
      if ( GetAI(&pos, endPoint, memory, turn, name, dep, 0) ){
        GetMemory(memory, turn, name);
        break;
      }
    }

    //プレイヤー2
    if (style > 2) {
      if ( GetPlayer(&pos, endPoint, memory, turn, name, 1) ){
        GetMemory(memory, turn, name);
        break;
      }
    }else{
      if ( GetAI(&pos, endPoint, memory, turn, name, dep, 1) ){
        GetMemory(memory, turn, name);
        break;
      }
    }

    //プレイヤー3
    if (style > 3) {
      if ( GetPlayer(&pos, endPoint, memory, turn, name, 2) ){
        GetMemory(memory, turn, name);
        break;
      }
    }else{
      if ( GetAI(&pos, endPoint, memory, turn, name, dep, 2) ){
        GetMemory(memory, turn, name);
        break;
      }
    }

    //現在状態の表示
    GetMemory(memory, turn, name);
    turn++;
  }
  return 0;
}

//プレイヤーの入力関数
bool GetPlayer(int *pos, int END, int array[][32], int times, char name[][16], int Pnum){

  int choice;

  std::cout << '\n' << "Now number is " << *pos << '\n';

  while (1){
    if (*pos < END-1) {
      std::cout << "Please push number (1 or 2) " << name[Pnum] << '\n';
      std::cout << "-> ";
      std::cin >> choice;
      if (choice == 1 || choice == 2) {
        break;
      }
      std::cout << "!! You can only enter 1 or 2 !!" << '\n';
    }else if(*pos == END-1){
      std::cout << "Please push number 1 " << name[Pnum] << '\n';
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
    std::cout << name[Pnum] << " Lose..." << '\n';
    return true;
  }

  return false;
}

//AIの選択関数
bool GetAI(int *pos, int END, int array[][32], int times, char name[][16], int depth[3], int Pnum){

  int one, two;
  int operate;
  int dep = depth[Pnum];

  std::cout << '\n' << name[Pnum] << " thiking now..." << '\n';

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
    }else if (one > two){
      operate = 2;
    }else{
      //どちらでも同じならランダムで決定
      std::random_device rnd;     // 非決定的な乱数生成器を生成
      std::mt19937 mt(rnd());     //  メルセンヌ・ツイスタの32ビット版、引数は初期シード値
      std::uniform_int_distribution<> rand2(1, 2);        // [0, 5] 範囲の一様乱数
      operate = rand2(mt);
    }
  }else{
    //負けが確定していたら1を選択
    operate = 1;
  }

  //現状態を更新しログを保存
  *pos += operate;
  array[Pnum][times] = *pos;

  std::cout << name[Pnum] << " selected " << operate << '\n';

  if (*pos >= END){
    std::cout << name[Pnum] << " Lose..." << '\n';
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
      //posが終了点を超えていたら終了
      if (pos + i >= END) {
        return;
      }
      GetOperate(pos+i, END, dep-1);
    }
  }else{
    //depが奇数=AIのターン
    for (size_t i = 1; i < 3; i++) {
      if (pos + i >= END) {
        //終了点の数値があればグローバル変数CNTをインクリメント
        if (pos + i == END) {
          CNT++;
        }
        return;
      }
      GetOperate(pos+i, END, dep-1);
    }
  }
}

//ゲームログの表示保存関数
void GetMemory(int array[][32], int n, char name[][16]){

  std::cout << '\n' << "##################";
  for (size_t i = 0; i < n; i++) {
    std::cout << "####";
  }
  std::cout << '\n';

  for (size_t i = 0; i < 3; i++) {

    if (i == 0) {
      std::cout << std::left << std::setw(10) << name[0] << "...";
    }else if (i == 1){
      std::cout << std::left << std::setw(10) << name[1] << "...";
    }else if (i == 2){
      std::cout << std::left << std::setw(10) << name[2] << "...";
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
