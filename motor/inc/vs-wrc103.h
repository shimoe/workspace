#ifndef __VSWRC103_H__
#define __VSWRC103_H__


#define SW_PORT		PORT0
#define SW_CPORT	LPC_GPIO0
#define SW_BIT		1

unsigned char USB_OutPutData[64];
unsigned char USB_InPutData[64];
unsigned char bSendUSBdata;
unsigned char isUSB_data_sending;


typedef uint8_t BYTE;


//汎用********************************************************************/
/*
CPU初期化
CPU全体の初期化関数。Sync()関数で利用する周波数を与える。
引数：Sync関数の同期周波数（単位はHz、30Hz以上)
戻り値：無し
*/
void Init(unsigned short MainCycle);

/*
ボタン状態取得
CPUボード上の押しボタンの状態を取得する
引数：　無し
戻り値：　0:off　1:on
*/
BYTE getSW();

/*
LED制御
CPUボード上の２つのLEDを制御する関数
引数：　0:消灯　1:緑　2:オレンジ　3：両方
戻り値：　無し
*/
void LED(unsigned char LedOn);

/*
時間待ち
引数：　待ち時間（単位：msec）
戻り値：　無し
*/
void Wait(unsigned int msec);

/*
同期関数
Init()関数で設定した周波数で同期し、処理を戻す関数。
例：Maincycle を60Hzに設定した場合、以下のソースでは、処理Aが60Hzで実行される。
ただし、処理Aが1/60sec以上かかる場合は意味を成さない。
while(1){
	Sync();
	処理A;
}

引数：　無し
戻り値：　内部で待ったカウント（単位無し）。０の場合、設定した周波数で同期できていない。

*/
unsigned long Sync();


/*
Sync関数の同期周波数取得
Initで設定した同期周波数を取得する
引数：　無し
戻り値：　同期周波数（Hz）
*/
unsigned short getMainCycle();


//モータ********************************************************************/

/*
モータの駆動
設定した速度でモータを駆動。呼び出した後はその状態を保持し、制御値に0を与えるまで停止しない。

引数：　モータの制御値
		0		：フリー
		0x8000	：フリー
		CW_MAX	：0x7FFF　(32767)
		CCW_MAX	：0x8001　(-32767)
戻り値：　無し
*/
void Mtr_Run_lv(short m1,short m2,short m3,short m4,short m5,short m6);


//VS-WRC003との互換性確保のため使用
/*
モータの駆動
設定した速度でモータを駆動。呼び出した後はその状態を保持し、制御値に0を与えるまで停止しない。

引数：　モータの制御値
		0		：フリー
		0x80	：フリー
		CW_MAX	：0x7F　(127)
		CCW_MAX	：0x81　(-127)
戻り値：　無し
*/
void Mtr_Run(BYTE mt1,BYTE mt2,BYTE mt3,BYTE mt4);


//AD入力値取得　0～7チャンネル*****************************************************/
//#define AD_SCANMODE

/*
AD入力値取得
A/D変換の入力値を取得。
引数：　チャンネル（0～7　=　1～8)
戻り値：　A/D変換の値(0～1023)
*/
unsigned short ADRead(unsigned char ch );



//ブザー********************************************************************/
/*
音程、ボリュームの設定
ブザーを鳴らす際の音程とボリュームを設定
引数：　pitch：音程の設定（0～255、値が大きいほど低い音）
		vol：ボリュームの設定 。H8版との互換性確保のためのダミーです。設定は反映されません。
戻り値：無し
*/
void BuzzerSet(BYTE pitch , BYTE vol);

/*
開始
ブザーを鳴らし始める。BuzzerStop()関数を呼ぶまでなり続ける。
引数：無し
戻り値：無し
*/
void BuzzerStart();

/*
停止
ブザーを止める。
引数：無し
戻り値：無し
*/
void BuzzerStop();

/*
ブザーが鳴っているかどうか確認
引数：無し
戻り値：	0：鳴っていない
			1：鳴っている
*/
BYTE isBuzzer();



//PSPAD********************************************************************/
/*
PADデータ更新
getPADで取得するPADデータのバッファを最新の情報に更新。
呼び出さない場合、getPADで同じタイミングのデータが取得される。
定期的に呼び出すことが必要。
引数：無し
戻り値：無し
*/
void updatePAD();

/*
データ取得
バッファからパッドの状態データを取得する。
引数には列挙子PADDATAから必要なものを指定

引数：　取得するデータの番号（列挙子からの指定でOK）
戻り値：　パッドの状態データ
*/
short getPAD(uint8_t num);


/*
		b7 | b6 | b5 | b4 | b3 | b2 | b1 | b0
PAD_B1：	左 | 下 | 右 | 上 |ｽﾀｰﾄ| R3 | L3 |ｾﾚｸﾄ
PAD_B2：	□ | × | ○ | △ | R1 | L1 | R2 | L2
PAD_AN_RX：	左:00h,中心:80h,右:FFh
PAD_AN_RY：	上:00h,中心:80h,下:FFh
PAD_AN_LX：	左:00h,中心:80h,右:FFh
PAD_AN_LY：	上:00h,中心:80h,下:FFh
*/

enum PADDATA{
PAD_B1,			//ボタン1バイト目
PAD_B2,			//ボタン2バイト目
PAD_AN_RX,		//右アナログスティック左右
PAD_AN_RY,		//右アナログスティック上下
PAD_AN_LX,		//左アナログスティック左右
PAD_AN_LY		//左アナログスティック上下
};

unsigned char memmap[128];


//エンコーダ**************************************************/
/*
エンコーダ初期化
エンコーダ拡張を使用する際に、Init()後に1度だけ実行する必要がある
エンコーダを使用する際は、VS-WRC004LVのモータ6が利用できなくなります。
戻り値：無し
*/
void InitEncoder();

/*
エンコーダクリア
エンコーダでカウントしている値を0にクリアする
引数：無し
戻り値：無し
*/
void ClearEncoder();

/*
エンコーダ値取得
エンコーダでカウントしている値取得する
引数：	*A： S1,2側のカウントを取得する変数のポインタ
		*B： S3,4側のカウントを取得する変数のポインタ
戻り値：無し
*/
void GetEncoder(long *A,long *B);



//#define USE_LPC_LINK

#endif
